#include "../include/MovieManager.h"
#include "../include/RatingManager.h"
#include "../include/SimilarityCalculator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

void MovieManager::loadFromFile(const std::string& filename) {
    movies.clear();
    idMap.clear();
    titleMap.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, title, genre, yearStr;
        
        if (std::getline(ss, idStr, ',') && std::getline(ss, title, ',') &&
            std::getline(ss, genre, ',') && std::getline(ss, yearStr, ',')) {
            try {
                int id = std::stoi(idStr);
                int year = std::stoi(yearStr);
                addMovie(Movie(id, title, genre, year));
            } catch (...) {}
        }
    }
    file.close();
}

void MovieManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    for (const auto& m : movies) {
        file << m.getId() << "," << m.getTitle() << "," << m.getGenre() << "," << m.getReleaseYear() << "\n";
    }
    file.close();
}

int MovieManager::size() const { return movies.size(); }

void MovieManager::addMovie(const Movie& movie) { 
    movies.push_back(movie); 
    // 추가 즉시 O(1) 캐시 인덱스 테이블에 주소 맵핑
    size_t newIdx = movies.size() - 1;
    idMap[movie.getId()] = newIdx;
    titleMap[movie.getTitle()] = newIdx;
}

// 최적화: 루프 순회 제거, 맵 기반 O(1) 수색 작동
Movie* MovieManager::findByTitle(const std::string& title) {
    auto it = titleMap.find(title);
    if (it != titleMap.end()) return &movies[it->second];
    return nullptr;
}

// 최적화: 루프 순회 제거, 맵 기반 O(1) 수색 작동
Movie* MovieManager::findById(int id) {
    auto it = idMap.find(id);
    if (it != idMap.end()) return &movies[it->second];
    return nullptr;
}

void MovieManager::printAll() const {
    for (const auto& m : movies) std::cout << m << "\n";
}

// 4번 메뉴용: 영화 객체의 평점 기반 내림차순 정렬 출력 기법
void MovieManager::printSortedByRating(const RatingManager& ratingMgr) const {
    std::vector<std::pair<double, Movie>> sortedMovies;
    for (const auto& m : movies) {
        sortedMovies.push_back({ratingMgr.getAverageRating(m.getId()), m});
    }
    // 평점 기준 내림차순 람다 정렬
    std::sort(sortedMovies.begin(), sortedMovies.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });
    for (size_t i = 0; i < sortedMovies.size(); ++i) {
        std::cout << i + 1 << "위 (평점: " << sortedMovies[i].first << "점) -> " << sortedMovies[i].second << "\n";
    }
}

// 최적화가 결합된 9번 사용자 기반 협업 필터링 엔진
std::vector<Movie> MovieManager::recommend(int targetUserId, const RatingManager& ratingMgr, int N) {
    std::vector<Movie> recommendedList;
    
    std::vector<Rating> myRatings = ratingMgr.findByUser(targetUserId);
    if (myRatings.empty()) return recommendedList;

    std::set<int> myWatched;
    for (const auto& r : myRatings) myWatched.insert(r.getMovieId());

    std::vector<int> allUsers = ratingMgr.getAllUserIds();
    int bestNeighborId = -1;
    double maxSim = -1.0;

    for (int otherId : allUsers) {
        if (otherId == targetUserId) continue;

        std::vector<Rating> otherRatings = ratingMgr.findByUser(otherId);
        if (otherRatings.empty()) continue;

        double sim = SimilarityCalculator::calculateUserSimilarity(myRatings, otherRatings);
        if (sim > maxSim) {
            maxSim = sim;
            bestNeighborId = otherId;
        }
    }

    if (bestNeighborId == -1 || maxSim <= 0.0) return recommendedList;

    std::vector<Rating> neighborRatings = ratingMgr.findByUser(bestNeighborId);
    std::sort(neighborRatings.begin(), neighborRatings.end(), [](const Rating& a, const Rating& b) {
        return a.getScore() > b.getScore();
    });

    for (const auto& r : neighborRatings) {
        if (r.getScore() >= 3.5 && myWatched.find(r.getMovieId()) == myWatched.end()) {
            Movie* mPtr = findById(r.getMovieId()); // O(1) 고속 수색 가동
            if (mPtr != nullptr) recommendedList.push_back(*mPtr);
        }
    }

    int actualN = std::min(N, (int)recommendedList.size());
    std::vector<Movie> finalResult;
    for (int i = 0; i < actualN; ++i) finalResult.push_back(recommendedList[i]);

    return finalResult;
}

void MovieManager::syncMovieRatings(const RatingManager& ratingMgr) {
    //  평점 매니저가 들고 있는 모든 유저의 평점 트래픽을 순회
    std::vector<int> allUsers = ratingMgr.getAllUserIds();
    for (int uId : allUsers) {
        std::vector<Rating> userRatings = ratingMgr.findByUser(uId);
        for (const auto& r : userRatings) {
            // 평점이 부여된 대상 영화 객체를 O(1) 해시 테이블로 수색
            Movie* mPtr = findById(r.getMovieId());
            if (mPtr != nullptr) {
                // 영화 객체 내부에 평점 데이터 누적 적재! (totalRating, ratingCount 갱신)
                mPtr->addRating(r.getScore());
            }
        }
    }
}