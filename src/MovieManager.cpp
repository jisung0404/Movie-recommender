#include "MovieManager.h"
#include "RatingManager.h"
#include "SimilarityCalculator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

void MovieManager::loadFromFile(const std::string& filename) {
    movies.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, title, genre, yearStr;
        
        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, title, ',') &&
            std::getline(ss, genre, ',') &&
            std::getline(ss, yearStr, ',')) {
            try {
                int id = std::stoi(idStr);
                int year = std::stoi(yearStr);
                movies.push_back(Movie(id, title, genre, year));
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

void MovieManager::addMovie(const Movie& movie) { movies.push_back(movie); }

Movie* MovieManager::findByTitle(const std::string& title) {
    for (auto& m : movies) {
        if (m.getTitle() == title) return &m;
    }
    return nullptr;
}

// ID 기반 검색 함수 구현체 연결
Movie* MovieManager::findById(int id) {
    for (auto& m : movies) {
        if (m.getId() == id) return &m;
    }
    return nullptr;
}

void MovieManager::printAll() const {
    for (const auto& m : movies) {
        std::cout << m << "\n";
    }
}

// 정통 코사인 유사도 기반 사용자 협업 필터링 알고리즘
std::vector<Movie> MovieManager::recommend(int targetUserId, const RatingManager& ratingMgr, int N) {
    std::vector<Movie> recommendedList;
    
    // 1. 타깃 유저의 평점 기록 및 시청 목록 분석
    std::vector<Rating> myRatings = ratingMgr.findByUser(targetUserId);
    if (myRatings.empty()) return recommendedList;

    std::set<int> myWatched;
    for (const auto& r : myRatings) {
        myWatched.insert(r.getMovieId());
    }

    // 2. 전체 유저 중에서 코사인 취향 동조율이 가장 높은 최고의 이웃 수색
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
    std::sort(neighborRatings.begin(), neighborRatings.end(), [](const Rating& a, const Rating& b) {
        return a.getScore() > b.getScore();
    });

    for (const auto& r : neighborRatings) {
        // 이웃이 호평(3.5점 이상)했고, 내가 보지 않은 영화만 선출
        if (r.getScore() >= 3.5 && myWatched.find(r.getMovieId()) == myWatched.end()) {
            Movie* mPtr = findById(r.getMovieId());
            if (mPtr != nullptr) {
                recommendedList.push_back(*mPtr);
            }
        }
    }

    // 4. 요청 수 만큼 슬라이싱 처리 안전장치
    int actualN = std::min(N, (int)recommendedList.size());
    std::vector<Movie> finalResult;
    for (int i = 0; i < actualN; ++i) {
        finalResult.push_back(recommendedList[i]);
    }

    return finalResult;
}