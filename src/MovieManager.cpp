#include "../include/MovieManager.h"
#include "../include/RatingManager.h"
#include "../include/SimilarityCalculator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>

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

Movie* MovieManager::findById(int id) {
    auto it = idMap.find(id);
    if (it != idMap.end()) return &movies[it->second];
    return nullptr;
}

void MovieManager::printAll() const {
    for (const auto& m : movies) std::cout << m << "\n";
}

std::vector<Movie> MovieManager::searchMoviesEnhanced(const std::string& query) const {
    std::vector<Movie> foundMovies;
    std::string lowerQuery = toLowerCase(query);

    for (const auto& m : movies) {
        std::string lowerTitle = toLowerCase(m.getTitle());
        if (lowerTitle.find(lowerQuery) != std::string::npos) {
            foundMovies.push_back(m);
        }
    }
    return foundMovies;
}

// [확장 요건 2번 통계 기능 구현부]: 실시간 수치 연산 및 화면 대시보드 리포팅 출력 책임 완수
std::vector<std::pair<double, Movie>> MovieManager::printAdvancedStatistics(const RatingManager& ratingMgr) const {
    std::vector<std::pair<double, Movie>> sortedMovies;
    
    double totalScoreSum = 0.0;
    int totalTrafficCount = 0;
    std::map<std::string, int> genreFrequencyMap; // 장르 빈도수 카운팅 장부

    for (const auto& m : movies) {
        double avg = ratingMgr.getAverageRating(m.getId());
        sortedMovies.push_back({avg, m});
        
        totalScoreSum += (avg * m.getRatingCount());
        totalTrafficCount += m.getRatingCount();
        genreFrequencyMap[m.getGenre()]++;
    }

    // 람다 함수 기반 평점 높은 순 정렬
    std::sort(sortedMovies.begin(), sortedMovies.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    // 최다 보유 장르 스캔
    std::string mostPopularGenre = "None";
    int maxGenreCount = -1;
    for (const auto& pair : genreFrequencyMap) {
        if (pair.second > maxGenreCount) {
            maxGenreCount = pair.second;
            mostPopularGenre = pair.first;
        }
    }

    double systemGlobalAverage = (totalTrafficCount == 0) ? 0.0 : totalScoreSum / totalTrafficCount;

    // 정돈된 텍스트 양식으로 콘솔창에 통계 리포트를 출력합니다
    std::cout << "\n===================================================\n";
    std::cout << "[-] 시스템 통합 통계 분석 데이터 요약\n";
    std::cout << "===================================================\n";
    std::cout << " * 전체 플랫폼 누적 평점 평균: " << systemGlobalAverage << " 점\n";
    std::cout << " * 데이터베이스 총 평점 등록 건수: " << totalTrafficCount << " 건\n";
    std::cout << " * 플랫폼 내 최다 보유 강세 장르: " << mostPopularGenre << " (총 " << maxGenreCount << "개 보유)\n";
    std::cout << "===================================================\n";

    std::cout << "\n[+] 실시간 평점순 랭킹 차트 현황\n";
    for (size_t i = 0; i < sortedMovies.size(); ++i) {
        std::cout << "  " << i + 1 << "위 -> [평점: " << sortedMovies[i].first << "점] " << sortedMovies[i].second << "\n";
    }

    return sortedMovies;
}

// [확장 요건 6번 CSV 강화 구현부]: 화면 출력 간섭 없이 순수하게 파일 스트림 저장 책임을 완수
void MovieManager::exportStatisticsToCSV(const std::vector<std::pair<double, Movie>>& sortedMovies, const std::string& exportFilename) const {
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
    
    // [개선 포인트 1] 단 한 명이 아니라, 유사도가 있는 '모든 이웃'을 리스트에 수집
    std::vector<std::pair<double, int>> neighbors;

    for (int otherId : allUsers) {
        if (otherId == targetUserId) continue;
        std::vector<Rating> otherRatings = ratingMgr.findByUser(otherId);
        if (otherRatings.empty()) continue;

        double sim = SimilarityCalculator::calculateUserSimilarity(myRatings, otherRatings);
        if (sim > 0.0) { 
            neighbors.push_back({sim, otherId}); // 유사도 0 이상인 이웃 전원 등록
        }
    }

    // [개선 포인트 2] 유사도가 높은 순(취향이 가장 비슷한 순)으로 이웃들을 내림차순 정렬
    std::sort(neighbors.begin(), neighbors.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    std::set<int> recommendedMovieIds; // 중복 추천 방지용 셋(Set)

    // [개선 포인트 3] 1위 이웃부터 순차적으로 물어보면서 N개의 영화를 채울 때까지 반복
    for (const auto& neighbor : neighbors) {
        int neighborId = neighbor.second;
        std::vector<Rating> neighborRatings = ratingMgr.findByUser(neighborId);
        
        // 해당 이웃이 높게 평가한 순으로 정렬
        std::sort(neighborRatings.begin(), neighborRatings.end(), [](const Rating& a, const Rating& b) {
            return a.getScore() > b.getScore();
        });

        for (const auto& r : neighborRatings) {
            // 이웃이 3.5점 이상 준 영화 중, 내가 아직 안 봤고, 아직 추천 리스트에 없는 영화라면 추가
            if (r.getScore() >= 3.5 && 
                myWatched.find(r.getMovieId()) == myWatched.end() && 
                recommendedMovieIds.find(r.getMovieId()) == recommendedMovieIds.end()) {
                
                Movie* mPtr = findById(r.getMovieId()); 
                if (mPtr != nullptr) {
                    recommendedList.push_back(*mPtr);
                    recommendedMovieIds.insert(r.getMovieId());
                    
                    // 목표 개수(N개)를 꽉 채우면 그 즉시 수색 종료
                    if (recommendedList.size() == static_cast<size_t>(N)) { 
                        return recommendedList;
                    }
                }
            }
        }
    }

    // N개를 다 못 채우더라도 지금까지 찾은 목록 반환
    return recommendedList;
}

void MovieManager::syncMovieRatings(const RatingManager& ratingMgr) {
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