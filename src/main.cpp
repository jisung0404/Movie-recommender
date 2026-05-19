#include <iostream>
#include <vector>
#include "MovieManager.h"

int main() {
    MovieManager movieMgr;
    movieMgr.loadFromFile("data/movies.csv");

    if (movieMgr.size() > 0) {
        // 첫 번째 영화를 기준으로 장르 기반 추천 실행
        Movie target = movieMgr.getMovies()[0]; 
        std::vector<Movie> recommendations = movieMgr.recommend(target, 5);

        std::cout << "=== [" << target.getTitle() << "] 장르 기반 추천 결과 ===\n";
        for (const auto& movie : recommendations) {
            std::cout << "- 제목: " << movie.getTitle() << " | 장르: " << movie.getGenre() << "\n";
        }
    } else {
        std::cout << "영화 데이터가 로드되지 않았습니다.\n";
    }

    movieMgr.saveToFile("data/movies.csv");
    return 0;
}