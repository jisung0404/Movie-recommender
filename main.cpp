#include "Movie.h"
#include "User.h"
#include "Rating.h"
#include <iostream>

int main() {
    
    // #1 Movie 생성 및 테스트
    Movie m1(1, "왕과사는남자", "드라마", 2026);
    Movie m2(2, "극한직업", "코미디",    2019);

    m1.addRating(4.4);
    m2.addRating(4.6);

    std::cout << "==== 영화 정보 ====" << std::endl;
    m1.display();
    // 출력: 1. 왕과사는남자 (2026)  평점: 4.4 (1건)
    m2.display(); 
    // 출력: 2. 극한직업 (2019)  평점: 4.6 (1건)
    std::cout << std::endl;


    // #2 User 생성 및 테스트
    User u1(1, "김지성", "skhanesa08@gmail.com");

    std::cout << "=== 사용자 정보 ===" << std::endl;
    u1.display();
    // 출력: [1] 김지성 (skhaensa08@gmail.com)
    std::cout << std::endl;

    
    // #3 Rating 생성 및 테스트
    Rating r1(u1.getId(), m1.getId(), 4.4);
    Rating r2(u1.getId(), m2.getId(), 6.5); // 6.5는 0.0~5.0 범위 초과해서 자동으로 5.0으로 보정

    std::cout << "=== 개별 평점 내역 ===" << std::endl;
    r1.display();
    r2.display();
    std::cout << std::endl;


    // #4 Movie 클래스 추가 기능 테스트
    std::cout << "=== 영화 평점 업데이트 테스트 ===" << std::endl;
    m1.addRating(5.0);
    m1.addRating(6.0);   // 유효성 검사 -> 무시됨

    
    std::cout << m1.getTitle() << ": "
              << m1.getAverageRating() << " (" << m1.getRatingCount() << "건)" << std::endl;
    // 왕과사는남자: 4.7  (4.4 + 5.0) / 2 = 4.7

    return 0;
}