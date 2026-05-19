#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include <vector>
#include <string>
#include "Movie.h"
#include "BaseManager.h"

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;

public:
    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;
    
    void addMovie(const Movie& movie);
    void printAll() const;
    const std::vector<Movie>& getMovies() const { return movies; }

    Movie* findByTitle(const std::string& title);
    void sortByRating();

    // MovieManager 자체에 위치하면서 N개 목록을 반환하는 recommend 함수
    std::vector<Movie> recommend(const Movie& targetMovie, int N = 5);
};

#endif