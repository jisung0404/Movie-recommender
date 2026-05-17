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
    // 뒤에 const를 붙여서 구현부와 상속 구조를 일치시킴
    void saveToFile(const std::string& filename) const override;
    int size() const override;
    
    void addMovie(const Movie& movie);
    void printAll() const;
    const std::vector<Movie>& getMovies() const { return movies; }

    Movie* findByTitle(const std::string& title);
    void sortByRating();
};

#endif