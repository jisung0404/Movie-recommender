#include "../include/MovieManager.h"
#include "../include/RatingManager.h"
#include "../include/SimilarityCalculator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

// 문자열을 소문자로 변환하기 위한 헬퍼 함수
static std::string toLowerCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

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
    size_t newIdx = movies.size() - 1;
    idMap[movie.getId()] = newIdx;
    titleMap[movie.getTitle()] = newIdx;
}

Movie* MovieManager::findByTitle(const std::string& title) {
    auto it = titleMap.find(title);
    if (it != titleMap.end()) return &movies[it->second];
    return nullptr;
}

// O(1) 고속 수색 가동
Movie* MovieManager::findById(int id) {
    auto it = idMap.find(id);
    if (it != idMap.end()) return &movies[it->second];
    return nullptr;
}

void MovieManager::printAll() const {
    for (const auto& m : movies) std::cout << m << "\n";
}

// [확장 기능] 부분 일치 및 대소문자 무시 기반의 영화 검색 로직 구현
std::vector<Movie> MovieManager::searchMoviesEnhanced(const std::string& query) const {
    std::vector<Movie> foundMovies;
    std::string lowerQuery = toLowerCase(query);

    for (const auto& m : movies) {
        std::string lowerTitle = toLowerCase(m.getTitle());
        // 대상 영화 제목에 검색 키워드가 포함되어 있는지 검증합니다
        if (lowerTitle.find(lowerQuery) != std::string::npos) {
            foundMovies.push_back(m);
        }
    }
    return foundMovies;
}

// [확장 기능] 평점순 정렬 책임을 전담하는 함수 구현
std::vector<std::pair<double, Movie>> MovieManager::getMoviesSortedByRating(const RatingManager& ratingMgr) const {
    std::vector<std::pair<double, Movie>> sortedMovies;
    for (const auto& m : movies) {
        sortedMovies.push_back({ratingMgr.getAverageRating(m.getId()), m});
    }

    // 람다 함수 기반 평점 높은 순 정렬
    std::sort(sortedMovies.begin(), sortedMovies.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    return sortedMovies;
}

// [확장 기능] 정렬된 데이터를 받아 외부 CSV 파일로 출력하는 책임만 전담하는 함수 구현
void MovieManager::exportSortedMoviesToCSV(const std::vector<std::pair<double, Movie>>& sortedMovies, const std::string& exportFilename) const {
    std::ofstream exportFile(exportFilename);
    if (!exportFile.is_open()) {
        std::cout << "[!] 통계 리포트 파일 생성에 실패하였습니다.\n";
        return;
    }

    exportFile << "rank,movieId,title,averageRating,ratingCount\n";
    for (size_t i = 0; i < sortedMovies.size(); ++i) {
        const Movie& m = sortedMovies[i].second;
        exportFile << (i + 1) << ","
                   << m.getId() << ","
                   << m.getTitle() << ","
                   << sortedMovies[i].first << ","
                   << m.getRatingCount() << "\n";
    }
    exportFile.close();
}

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

        // 코사인 유사도 연산 가동
        double sim = SimilarityCalculator::calculateUserSimilarity(myRatings, otherRatings);
        if (sim > maxSim) {
            maxSim = sim;
            bestNeighborId = otherId;
        }
    }

    // 취향이 겹치는 이웃이 전멸한 경우 예외 처리 빈 배열 리턴
    if (bestNeighborId == -1 || maxSim <= 0.0) return recommendedList;

    // 3. 선출된 이웃이 높은 점수를 준 영화 리스트를 1회용 익명(람다) 함수 정렬로 수집
    std::vector<Rating> neighborRatings = ratingMgr.findByUser(bestNeighborId);
    
    // 람다 함수 기반 평점 높은 순 정렬
    std::sort(neighborRatings.begin(), neighborRatings.end(), [](const Rating& a, const Rating& b) {
        return a.getScore() > b.getScore();
    });

    for (const auto& r : neighborRatings) {
        // 이웃이 호평(3.5점 이상)했고, 내가 보지 않은 영화만 선출
        if (r.getScore() >= 3.5 && myWatched.find(r.getMovieId()) == myWatched.end()) {
            Movie* mPtr = findById(r.getMovieId()); 
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
            Movie* mPtr = findById(r.getMovieId());
            if (mPtr != nullptr) {
                mPtr->addRating(r.getScore());
            }
        }
    }
}