#include "MovieManager.h"
#include <iostream>

void MovieManager::loadFromFile(const std::string& filename) {
    // 기존 로드 로직 유지
}

void MovieManager::saveToFile(const std::string& filename) const {
    // 기존 저장 로직 뒤에 const 추가
}

int MovieManager::size() const {
    return movies.size();
}

void MovieManager::addMovie(const Movie& movie) {
    movies.push_back(movie);
}

void MovieManager::printAll() const {
    for (const auto& movie : movies) {
        std::cout << movie.getId() << ", " << movie.getTitle() << ", " << movie.getGenre() << "\n";
    }
}

Movie* MovieManager::findByTitle(const std::string& title) {
    for (auto& movie : movies) {
        if (movie.getTitle() == title) return &movie;
    }
    return nullptr;
}

void MovieManager::sortByRating() {
    // 기존 정렬 로직 유지
}