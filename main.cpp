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

    int choice;

    while (true) {
        std::cout << "\n=== Movie Recommender ===\n[영화]\n1. 영화 추가\n2. 제목으로 검색\n3. 전체 목록 출력\n4. 평점순 정렬 출력\n[사용자]\n5. 사용자 추가\n6. 사용자 목록 출력\n[평점]\n7. 평점 입력\n8. 영화별 평점 보기\n0. 종료\n선택 > ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "잘못된 입력입니다. 숫자를 입력해주세요.\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "프로그램을 종료합니다.\n";
            break;
        }

    }

    return 0;
}