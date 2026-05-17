#include <iostream>
#include <vector>
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"

int main() {
    MovieManager movieMgr;
    UserManager userMgr;
    RatingManager ratingMgr;

    movieMgr.loadFromFile("data/movies.csv");
    userMgr.loadFromFile("data/users.csv");
    ratingMgr.loadFromFile("data/ratings.csv");

    Recommender recommender(movieMgr, ratingMgr);

    int targetUser = 1;
    std::vector<Movie> recommendations = recommender.recommend(targetUser, 5, 3);

    std::cout << "=== User " << targetUser << " 추천 영화 목록 ===\n";
    if (recommendations.empty()) {
        std::cout << "추천할 영화가 없습니다.\n";
    } else {
        for (const auto& movie : recommendations) {
            std::cout << "- ID: " << movie.getId() << " | 제목: " << movie.getTitle() << " | 장르: " << movie.getGenre() << "\n";
        }
    }

    movieMgr.saveToFile("data/movies.csv");
    userMgr.saveToFile("data/users.csv");
    ratingMgr.saveToFile("data/ratings.csv");

    return 0;
}