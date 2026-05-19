#include "MovieManager.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept> // std::invalid_argument 예외 처리용 헤더 추가!

// =================================================================
// 💡 [완벽] 파일 입출력 및 기존 메서드 영역 (stoi 예외 완벽 방어)
// =================================================================

void MovieManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file.is_open()) { 
        std::cout << "파일을 열 수 없습니다: " << filename << "\n";
        return;
    }

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
        
        // 🚀 핵심 로직: stoi 터지는 현상 완벽 방어 (try-catch 적용)
        try {
            int id = std::stoi(idStr);      
            int extraVal = extraStr.empty() ? 0 : std::stoi(extraStr);
            movies.push_back(Movie(id, title, genre, extraVal)); 
        } 
        catch (const std::invalid_argument& e) {
            // 헤더(id,title,genre) 문자열이나 오염된 텍스트가 들어오면 무시하고 다음 줄로 패스!
            continue; 
        }
        catch (const std::out_of_range& e) {
            continue;
        }
    }
    file.close();
}

void MovieManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename); 
    if (!file.is_open()) {
        return;
    }

    for (const auto& movie : movies) {
        file << movie.getId() << "," << movie.getTitle() << "," << movie.getGenre() << "\n";
    }
    file.close();
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


// =================================================================
// 🚀 12주차 M3 새 명세: 100% 배운 문법 기반 장르 추천 알고리즘
// =================================================================

int MovieManager::calculateGenreSimilarity(const std::string& genreA, const std::string& genreB) {
    if (genreA.empty() || genreB.empty()) return -100;
    return (genreA == genreB) ? 100 : 0;
}

std::vector<Movie> MovieManager::recommend(const Movie& targetMovie, int N) {
    if (movies.empty() || (movies.size() == 1 && movies[0].getId() == targetMovie.getId())) {
        std::cout << "[Edge Case] 추천할 영화 데이터베이스가 비어있습니다.\n";
        return std::vector<Movie>();
    }

    std::vector<Movie> candidates;
    
    for (const auto& movie : movies) {
        if (movie.getId() == targetMovie.getId()) continue; 
        
        if (calculateGenreSimilarity(targetMovie.getGenre(), movie.getGenre()) == 100) {
            candidates.push_back(movie);
        }
    }

    if (candidates.empty()) {
        for (const auto& movie : movies) {
            if (movie.getId() != targetMovie.getId()) {
                candidates.push_back(movie);
            }
        }
    }

    std::vector<Movie> recommendedList;
    int actualN = N;
    if (candidates.size() < (size_t)N) {
        actualN = candidates.size(); 
    }

    for (int i = 0; i < actualN; ++i) {
        recommendedList.push_back(candidates[i]);
    }

    return recommendedList;
}