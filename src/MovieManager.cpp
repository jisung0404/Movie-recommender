#include "MovieManager.h"
#include "SimilarityCalculator.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

void MovieManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file.is_open()) return;
    movies.clear(); 
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line); 
        std::string idStr, title, genre, extraStr;
        std::getline(ss, idStr, ',');   
        std::getline(ss, title, ',');   
        std::getline(ss, genre, ',');   
        std::getline(ss, extraStr, ','); 
        try {
            int id = std::stoi(idStr);      
            int extraVal = extraStr.empty() ? 0 : std::stoi(extraStr);
            movies.push_back(Movie(id, title, genre, extraVal)); 
        } 
        catch (const std::exception& e) { continue; }
    }
    file.close();
}

void MovieManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    for (const auto& movie : movies) {
        file << movie.getId() << "," << movie.getTitle() << "," 
             << movie.getGenre() << "," << movie.getReleaseYear() << "\n";
    }
    file.close();
}

int MovieManager::size() const { return movies.size(); }
void MovieManager::addMovie(const Movie& movie) { movies.push_back(movie); }
void MovieManager::printAll() const {
    for (const auto& m : movies) { std::cout << m.getId() << ", " << m.getTitle() << ", " << m.getGenre() << "\n"; }
}
Movie* MovieManager::findByTitle(const std::string& title) {
    for (auto& m : movies) { if (m.getTitle() == title) return &m; }
    return nullptr;
}
void MovieManager::sortByRating() {
    std::sort(movies.begin(), movies.end());
}

std::vector<Movie> MovieManager::recommend(const Movie& targetMovie, int N) {
    if (movies.empty()) return std::vector<Movie>();

    std::vector<Movie> candidates;
    
    for (const auto& movie : movies) {
        if (movie.getId() == targetMovie.getId()) continue; // 자기 자신 제외
        
        // 분리한 SimilarityCalculator 클래스의 정적 메서드를 활용해 장르 점수 계산
        int score = SimilarityCalculator::calculateGenreSimilarity(targetMovie.getGenre(), movie.getGenre());
        if (score == 100) {
            candidates.push_back(movie);
        }
    }

    // 만약 동일 장르가 전혀 없다면 예외 케이스로 다른 영화 전체를 후보로 지정
    if (candidates.empty()) {
        for (const auto& movie : movies) {
            if (movie.getId() != targetMovie.getId()) candidates.push_back(movie);
        }
    }

    // 후보군 평점 순 정렬 (Movie 클래스의 operator< 연산자 활용)
    std::sort(candidates.begin(), candidates.end());

    // 상위 N개만 리스트에 바인딩
    std::vector<Movie> recommendedList;
    int actualN = std::min(N, (int)candidates.size());
    for (int i = 0; i < actualN; ++i) {
        recommendedList.push_back(candidates[i]);
    }

    return recommendedList;
}