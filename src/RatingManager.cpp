#include "RatingManager.h"
#include <iostream>
#include <set>

void RatingManager::loadFromFile(const std::string& filename) {
    // 기존 로드 로직 유지
}

void RatingManager::saveToFile(const std::string& filename) const {
    // 기존 저장 로직 뒤에 const 추가
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
        std::cout << r.getUserId() << ", " << r.getMovieId() << ", " << r.getScore() << "\n";
    }
}