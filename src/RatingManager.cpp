#include "../include/RatingManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

void RatingManager::loadFromFile(const std::string& filename) {
    ratings.clear();
    userRatingsMap.clear();
    movieRatingsMap.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string uIdStr, mIdStr, scoreStr;

        if (std::getline(ss, uIdStr, ',') && std::getline(ss, mIdStr, ',') && std::getline(ss, scoreStr, ',')) {
            try {
                int uId = std::stoi(uIdStr);
                int mId = std::stoi(mIdStr);
                double score = std::stod(scoreStr);
                addRating(uId, mId, score);
            } catch (...) {}
        }
    }
    file.close();
}

void RatingManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    for (const auto& r : ratings) {
        file << r.getUserId() << "," << r.getMovieId() << "," << r.getScore() << "\n";
    }
    file.close();
}

int RatingManager::size() const { return ratings.size(); }

void RatingManager::addRating(int userId, int movieId, double score) {
    Rating newRating(userId, movieId, score);
    ratings.push_back(newRating);
    // 다중 캐시 인덱스 테이블에 고속 그룹 매핑 완료
    userRatingsMap[userId].push_back(newRating);
    movieRatingsMap[movieId].push_back(newRating);
}

// 최적화: O(N) 순회 전멸 -> 맵 수색을 통해 해당 유저 배열만 O(1) 즉시 반환
std::vector<Rating> RatingManager::findByUser(int userId) const {
    auto it = userRatingsMap.find(userId);
    if (it != userRatingsMap.end()) return it->second;
    return std::vector<Rating>();
}

std::vector<int> RatingManager::getAllUserIds() const {
    std::vector<int> userIds;
    for (const auto& pair : userRatingsMap) {
        userIds.push_back(pair.first);
    }
    return userIds;
}

// 최적화: 맵 기반 고속 타깃 로그 출력
void RatingManager::printRatingsByMovie(int movieId) const {
    auto it = movieRatingsMap.find(movieId);
    if (it == movieRatingsMap.end() || it->second.empty()) {
        std::cout << "⚠️ 누적된 평점 데이터가 존재하지 않는 영화입니다.\n";
        return;
    }
    for (const auto& r : it->second) {
        std::cout << "👤 유저 ID [" << r.getUserId() << "] ->  평점: " << r.getScore() << "점\n";
    }
}

// 최적화: 특정 영화의 평균 평점을 O(1) 그룹 맵을 통해 고속 계산
double RatingManager::getAverageRating(int movieId) const {
    auto it = movieRatingsMap.find(movieId);
    if (it == movieRatingsMap.end() || it->second.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& r : it->second) sum += r.getScore();
    return sum / it->second.size();
}