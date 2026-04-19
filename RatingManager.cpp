#include "RatingManager.h"
#include <iostream>

using namespace std;

// 1. 평점 추가 로직
void RatingManager::addRating(int userId, int movieId, double score) {
    Rating newRating(userId, movieId, score);
    ratings.push_back(newRating);
}

// 2. 특정 영화의 평점만 모아서 보기
void RatingManager::printRatingsByMovie(int targetMovieId) const {
    bool found = false;
    for (const Rating& r : ratings) {
        if (r.getMovieId() == targetMovieId) {
            cout << r << endl;
            found = true;
        }
    }
    if (found == false) {
        cout << "해당 영화에 등록된 평점이 없습니다." << endl;
    }
}

// 3. 전체 평점 목록 출력
void RatingManager::printAll() const {
    for (const Rating& r : ratings) {
        cout << r << endl;
    }
}