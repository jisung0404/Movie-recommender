#ifndef RATING_MANAGER_H
#define RATING_MANAGER_H

#include <vector>
#include <string>
#include "Rating.h"
#include "BaseManager.h"

class RatingManager : public BaseManager {
private:
    std::vector<Rating> ratings;
public:
    void loadFromFile(const std::string& filename) override;
    // 뒤에 const를 붙여서 구현부와 상속 구조를 일치시킴
    void saveToFile(const std::string& filename) const override;
    int size() const override;
    
    // .cpp의 void RatingManager::addRating(int, int, double)과 완벽 결합
    void addRating(int userId, int movieId, double score);
    
    // Recommender가 const 상태로 호출할 수 있도록 함수 뒤에 const 장착
    std::vector<Rating> findByUser(int userId) const;
    std::vector<int> getAllUserIds() const;
    
    void printRatingsByMovie(int targetMovieId) const;
    void printAll() const;
};

#endif