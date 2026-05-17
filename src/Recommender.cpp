#include "Recommender.h"
#include <cstdlib>
#include <algorithm>
#include <set>
#include <map>
#include <iostream>

Recommender::Recommender(MovieManager& mNode, RatingManager& rNode) 
    : movieMgr(mNode), ratingMgr(rNode) {}

int Recommender::Similaritycalculate(const std::vector<Rating>& ratingsA, const std::vector<Rating>& ratingsB) {
    int commonCount = 0;
    int scoreDiffSum = 0;

    for (const auto& r1 : ratingsA) {
        for (const auto& r2 : ratingsB) {
            if (r1.getMovieId() == r2.getMovieId()) {
                commonCount++;
                scoreDiffSum += std::abs(r1.getScore() - r2.getScore());
            }
        }
    }

    if (commonCount == 0) {
        return -100;
    }

    return (commonCount * 10) - scoreDiffSum;
}

std::vector<Movie> Recommender::recommend(int targetUserId, int N, int K) {
    std::vector<Rating> myRatings = ratingMgr.findByUser(targetUserId); 
    
    if (myRatings.empty()) {
        std::cout << "[Edge Case] 대상 사용자의 평점 데이터가 없습니다.\n";
        return std::vector<Movie>();
    }

    std::vector<std::pair<int, int>> similarities;
    for (int otherId : ratingMgr.getAllUserIds()) {
        if (otherId == targetUserId) continue;
        
        std::vector<Rating> otherRatings = ratingMgr.findByUser(otherId);
        int sim = Similaritycalculate(myRatings, otherRatings);
        similarities.push_back({otherId, sim});
    }

    std::sort(similarities.begin(), similarities.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });

    int actualK = K;
    if (similarities.size() < (size_t)K) {
        actualK = similarities.size();
    }

    std::set<int> myMovieIds;
    for (const auto& r : myRatings) {
        myMovieIds.insert(r.getMovieId());
    }

    std::map<int, int> movieScores;
    for (int i = 0; i < actualK; ++i) {
        int userId = similarities[i].first;
        std::vector<Rating> similarUserRatings = ratingMgr.findByUser(userId);

        for (const auto& r : similarUserRatings) {
            if (myMovieIds.find(r.getMovieId()) == myMovieIds.end()) {
                movieScores[r.getMovieId()] += r.getScore();
            }
        }
    }

    if (movieScores.empty()) {
        std::cout << "[Edge Case] 추천할 수 있는 새로운 영화 후보가 없습니다.\n";
        return std::vector<Movie>();
    }

    std::vector<std::pair<int, int>> sortedScores(movieScores.begin(), movieScores.end());
    std::sort(sortedScores.begin(), sortedScores.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });

    std::vector<Movie> recommendedMovies;
    int actualN = N;
    if (sortedScores.size() < (size_t)N) {
        actualN = sortedScores.size();
    }

    for (int i = 0; i < actualN; ++i) {
        int movieId = sortedScores[i].first;
        for (const auto& m : movieMgr.getMovies()) {
            if (m.getId() == movieId) {
                recommendedMovies.push_back(m);
                break;
            }
        }
    }

    return recommendedMovies;
}