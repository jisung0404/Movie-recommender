#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include "BaseManager.h"
#include "Movie.h"
#include <vector>
#include <string>
#include <unordered_map>

class RatingManager;

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;
    std::unordered_map<int, size_t> idMap;
    std::unordered_map<std::string, size_t> titleMap;

public:
    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;

    void addMovie(const Movie& movie);
    void printAll() const;

    // 협업 필터링 역추적을 위해 선언 추가
    Movie* findById(int id); 
    Movie* findByTitle(const std::string& title);
    
    // 인자 구성을 (유저 ID, 평점 매니저 대장 참조)로 변경하여 협업 필터링 규격으로 개조
    std::vector<Movie> recommend(int targetUserId, const RatingManager& ratingMgr, int N = 5);

    // 평점 매니저 대장 참조를 활용한 인메모리 동기화 함수
    void syncMovieRatings(const RatingManager& ratingMgr);

    // [확장 기능] 대소문자 무시 및 부분 일치 검색을 위한 함수 정의
    std::vector<Movie> searchMoviesEnhanced(const std::string& query) const;

    // [확장 기능] 단일 책임 원칙에 따라 평점순으로 정렬된 영화 리스트를 반환하는 함수
    std::vector<std::pair<double, Movie>> getMoviesSortedByRating(const RatingManager& ratingMgr) const;

    // [확장 기능] 단일 책임 원칙에 따라 정렬된 데이터를 CSV 파일로 저장하는 함수
    void exportSortedMoviesToCSV(const std::vector<std::pair<double, Movie>>& sortedMovies, const std::string& exportFilename) const;
};

#endif