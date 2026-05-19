#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include <vector>
#include <string>
#include "Movie.h"
#include "BaseManager.h"

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;

    // 교안 12p: 장르 기반 유사도 계산 헬퍼 함수
    int calculateGenreSimilarity(const std::string& genreA, const std::string& genreB);

public:
    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;
    
    void addMovie(const Movie& movie);
    void printAll() const;
    const std::vector<Movie>& getMovies() const { return movies; }

    Movie* findByTitle(const std::string& title);
    void sortByRating();

    // 교안 12p: MovieManager 자체에 위치해야 하는 필수 recommend 메서드
    std::vector<Movie> recommend(const Movie& targetMovie, int N = 5);
};

#endif