#include "Movie.h"
#include "User.h"
#include "Rating.h"
#include <iostream>
#include <vector>
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include <string>

int main() {
    
    MovieManager movieMgr;
    UserManager userMgr;
    RatingManager ratingMgr;

    // #1 Movie 생성 및 테스트
    Movie m1(1, "왕과사는남자", "드라마", 2026);
    Movie m2(2, "극한직업", "코미디",    2019);

    m1.addRating(4.4);
    m2.addRating(4.6);
    m1.addRating(5.0);
    m1.addRating(6.0);   // 유효성 검사

    movieMgr.addMovie(m1);
    movieMgr.addMovie(m2);

    // #2 User 생성
    User u1(1, "김지성", "skhanesa08@gmail.com");
    
    userMgr.addUser(u1);

    // #3 Rating 추가
    ratingMgr.addRating(u1.getId(), m1.getId(), 4.4);
    ratingMgr.addRating(u1.getId(), m2.getId(), 6.5);

    return 0;
}