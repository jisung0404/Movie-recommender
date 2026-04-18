#include <iostream>
#include "Rating.h"

using namespace std;

Rating::Rating()
    : userId(0), movieId(0), score(0.0) {}

Rating::Rating(int uId, int mId, double s) {
    userId = uId;
    movieId = mId;

    if (s < 0.0) {
        std::cout << "~입력 평점 0.0으로 재조정~\n";
        score = 0.0;
    } else if (s > 5.0) {
        std::cout << "~입력 평점 5.0으로 재조정~\n";
        score = 5.0;
    } else { 
        score = s;
    }
}

int    Rating::getUserId()      const { return userId; }
int    Rating::getMovieId()     const { return movieId; }
double Rating::getScore()       const { return score; }

void   Rating::display()        const {
    std::cout << "User [" << userId 
              << "] -> Movie [" 
              << movieId << "] : 평점 " 
              << score << "\n";        
}

// --- [M2 추가] 연산자 오버로딩 구현부 ---

// << 연산자: 평점 정보 연쇄 출력
std::ostream& operator<<(std::ostream& os, const Rating& r) {
    os << "User [" << r.getUserId() << "] -> Movie [" 
       << r.getMovieId() << "] : 평점 " << r.getScore();
    return os;
}