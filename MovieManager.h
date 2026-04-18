#pragma once
#include <vector>
#include <string>
#include "Movie.h"
#include "BaseManager.h" 

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;

public:
    void addMovie(const Movie& m);
    
    // 못 찾으면 nullptr을 뱉어야 해서 포인터(*) 타입으로 반환
    Movie* findByTitle(const std::string& title);
    
    void sortByRating();
    
    void printAll() const override; 
};