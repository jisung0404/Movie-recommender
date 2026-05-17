#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include <vector>
#include "Movie.h"
#include "Rating.h"
#include "MovieManager.h"
#include "RatingManager.h"

class Recommender {
private:
    MovieManager& movieMgr;
    RatingManager& ratingMgr;

public:
    Recommender(MovieManager& mNode, RatingManager& rNode);
    int Similaritycalculate(const std::vector<Rating>& ratingsA, const std::vector<Rating>& ratingsB);
    std::vector<Movie> recommend(int targetUserId, int N = 5, int K = 3);
};

#endif