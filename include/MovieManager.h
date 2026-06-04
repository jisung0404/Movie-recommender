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

    Movie* findById(int id); 
    Movie* findByTitle(const std::string& title);
    
    std::vector<Movie> recommend(int targetUserId, const RatingManager& ratingMgr, int N = 5);
    void syncMovieRatings(const RatingManager& ratingMgr);

    // [확장 요건 4번] 대소문자 무시 및 부분 일치 검색 함수
    std::vector<Movie> searchMoviesEnhanced(const std::string& query) const;

    // [확장 요건 2번] 실시간 전체 평점 평균 및 인기 지표 통계를 계산하고 화면에 출력하는 함수
    std::vector<std::pair<double, Movie>> printAdvancedStatistics(const RatingManager& ratingMgr) const;

    // [확장 요건 6번] 오직 통계 결과 데이터를 받아 외부 CSV 파일로 내보내는 저장 전담 함수
    void exportStatisticsToCSV(const std::vector<std::pair<double, Movie>>& sortedMovies, const std::string& exportFilename) const;
};

#endif