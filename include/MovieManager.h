#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include "BaseManager.h"
#include "Movie.h"
#include <vector>
#include <string>
#include <unordered_map> // 고속 인덱싱 캐시 맵 레이어

class RatingManager;

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;
    // ID 및 제목 기반 수색 속도를 O(N)에서 O(1)로 줄이는 해시 인덱스 테이블
    std::unordered_map<int, size_t> idMap;
    std::unordered_map<std::string, size_t> titleMap;

public:
    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;

    void addMovie(const Movie& movie);
    Movie* findByTitle(const std::string& title);
    Movie* findById(int id);
    void printAll() const;
    void printSortedByRating(const RatingManager& ratingMgr) const; // 4번 전용 메서드
    
    std::vector<Movie> recommend(int targetUserId, const RatingManager& ratingMgr, int N = 5);
};

#endif