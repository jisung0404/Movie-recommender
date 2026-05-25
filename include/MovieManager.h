#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include "BaseManager.h"
#include "Movie.h"
#include <vector>
#include <string>

class RatingManager;

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;

public:
    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;

    void addMovie(const Movie& movie);
    Movie* findByTitle(const std::string& title);
    Movie* findById(int id); // 협업 필터링 역추적을 위해 선언 추가
    void printAll() const;
    
    // 인자 구성을 (유저 ID, 평점 매니저 대장 참조)로 변경하여 협업 필터링 규격으로 개조
    std::vector<Movie> recommend(int targetUserId, const RatingManager& ratingMgr, int N = 5);
};

#endif