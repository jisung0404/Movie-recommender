#ifndef RATING_MANAGER_H
#define RATING_MANAGER_H

#include "BaseManager.h"
#include "Rating.h"
#include <vector>
#include <string>
#include <unordered_map> // 다중 고속 인덱싱 테이블 맵핑

class RatingManager : public BaseManager {
private:
    std::vector<Rating> ratings;
    // 유저별, 영화별 평점을 그룹화하여 수색 시간을 O(1) 수준으로 압축
    std::unordered_map<int, std::vector<Rating>> userRatingsMap;
    std::unordered_map<int, std::vector<Rating>> movieRatingsMap;

public:
    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;

    void addRating(int userId, int movieId, double score);
    std::vector<Rating> findByUser(int userId) const;
    std::vector<int> getAllUserIds() const;
    void printRatingsByMovie(int movieId) const;
    double getAverageRating(int movieId) const; // 정렬 추천 전용 고속 평균 계산 메서드
};

#endif