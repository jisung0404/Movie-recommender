#include "RatingManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

void RatingManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "평점 파일을 열 수 없습니다: " << filename << "\n";
        return;
    }

    ratings.clear();
    std::string line;
    
    // 첫 줄 헤더(userId,movieId,score) 스킵
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string uIdStr, mIdStr, scoreStr;
        
        std::getline(ss, uIdStr, ',');
        std::getline(ss, mIdStr, ',');
        std::getline(ss, scoreStr, ',');
        
        try {
            int uId = std::stoi(uIdStr);
            int mId = std::stoi(mIdStr);
            double score = std::stod(scoreStr); // double 파싱은 stod
            ratings.push_back(Rating(uId, mId, score));
        }
        catch (const std::exception& e) {
            continue; // 에러 행 예외 방어
        }
    }
    file.close();
}

void RatingManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    file << "userId,movieId,score\n";
    for (const auto& r : ratings) {
        file << r.getUserId() << "," << r.getMovieId() << "," << r.getScore() << "\n";
    }
    file.close();
}

int RatingManager::size() const {
    return ratings.size();
}

void RatingManager::addRating(int userId, int movieId, double score) {
    ratings.push_back(Rating(userId, movieId, score));
}

std::vector<Rating> RatingManager::findByUser(int userId) const {
    std::vector<Rating> userRatings;
    for (const auto& r : ratings) {
        if (r.getUserId() == userId) {
            userRatings.push_back(r);
        }
    }
    return userRatings;
}

std::vector<int> RatingManager::getAllUserIds() const {
    std::set<int> uniqueIds;
    for (const auto& r : ratings) {
        uniqueIds.insert(r.getUserId());
    }
    return std::vector<int>(uniqueIds.begin(), uniqueIds.end());
}

void RatingManager::printRatingsByMovie(int targetMovieId) const {
    for (const auto& r : ratings) {
        if (r.getMovieId() == targetMovieId) {
            std::cout << r.getUserId() << ": " << r.getScore() << "\n";
        }
    }
}

void RatingManager::printAll() const {
    for (const auto& r : ratings) {
        std::cout << "User " << r.getUserId() << " -> Movie " << r.getMovieId() << ": " << r.getScore() << "\n";
    }
}