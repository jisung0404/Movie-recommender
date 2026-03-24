#include <iostream>
#include "Rating.h"

using namespace std;

Rating::Rating() {
    userId = 0;
    movieId = 0;
    score = 0.0;
}

Rating::Rating(int uId, int mId, double s) {
    userId = uId;
    movieId = mId;

    if (s < 0.0) {
        score = 0.0;
    } else if (s > 5.0) {
        score = 5.0;
    } else { 
        score = s;
    }
}

int Rating::getUserId() const {
    return userId;
}

int Rating::getMovieId() const {
    return movieId;
}

double Rating::getScore() const {
    return score;
}

void Rating::display() const {
    cout << "User [" << userId << "] -> Movie [" << movieId << "] : 평점 " << score << endl;        
}


