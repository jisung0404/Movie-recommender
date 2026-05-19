#include <iostream>
#include <vector>
#include <string>
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"

int main() {
    // 1. 모든 매니저 선언 및 데이터 로드
    MovieManager movieMgr;
    UserManager userMgr;
    RatingManager ratingMgr;

    movieMgr.loadFromFile("data/movies.csv");
    userMgr.loadFromFile("data/users.csv");
    ratingMgr.loadFromFile("data/ratings.csv");

    std::cout << "=== [현재 시스템 등록 현황] ===\n";
    std::cout << "▶ 영화 수: " << movieMgr.size() << "개\n";
    std::cout << "▶ 유저 수: " << userMgr.size() << "개\n";
    std::cout << "▶ 평점 데이터 수: " << ratingMgr.size() << "개\n";
    std::cout << "=======================================\n\n";

    std::cout << "=== [1] 새로운 사용자 등록 ===\n";
    int newUserId;
    std::string newUserName, newUserEmail;

    std::cout << "유저 ID 입력 (예: 4): ";
    std::cin >> newUserId;
    std::cin.ignore(); // 버퍼 비우기

    std::cout << "유저 이름 입력: ";
    std::getline(std::cin, newUserName);

    std::cout << "유저 이메일 입력: ";
    std::getline(std::cin, newUserEmail);

    // User 객체 생성 후 매니저에 추가
    User newUser(newUserId, newUserName, newUserEmail);
    userMgr.addUser(newUser);
    std::cout << "사용자 등록 완료!\n\n";

    std::cout << "=== [2] 새로운 영화 등록 ===\n";
    int newMovieId;
    std::string newMovieTitle, newMovieGenre;
    int newMovieYear;

    std::cout << "영화 ID 입력 (예: 4): ";
    std::cin >> newMovieId;
    std::cin.ignore(); // 버퍼 비우기

    std::cout << "영화 제목 입력: ";
    std::getline(std::cin, newMovieTitle);

    std::cout << "영화 장르 입력: ";
    std::getline(std::cin, newMovieGenre);

    std::cout << "영화 개봉 연도 입력: ";
    std::cin >> newMovieYear;

    // Movie 객체 생성 후 매니저에 추가
    Movie newMovie(newMovieId, newMovieTitle, newMovieGenre, newMovieYear);
    movieMgr.addMovie(newMovie);
    std::cout << "영화 등록 완료!\n\n";

    std::cout << "=== [3] 새로운 평점 등록 ===\n";
    int targetMovieId;
    double newScore;

    std::cout << "평점을 매길 영화 ID 입력: ";
    std::cin >> targetMovieId;

    std::cout << "평점 입력 (0.0 ~ 5.0): ";
    std::cin >> newScore;

    // 방금 생성한 유저 ID(newUserId)와 입력한 영화 ID, 점수로 평점 등록
    ratingMgr.addRating(newUserId, targetMovieId, newScore);
    std::cout << "평점 데이터 등록 완료!\n\n";

    std::cout << "=== [4] '" << newMovie.getTitle() << "' 기준 장르 기반 추천 결과 ===\n";
    std::vector<Movie> recommendations = movieMgr.recommend(newMovie, 5);

    if (recommendations.empty()) {
        std::cout << "- 동일한 장르의 다른 추천 영화가 없습니다.\n";
    } else {
        for (const auto& movie : recommendations) {
            std::cout << "- 제목: " << movie.getTitle() << " | 장르: " << movie.getGenre() << "\n";
        }
    }

    // 6. 모든 데이터 매니저의 최신 장부를 각각의 파일에 최종 누적 저장
    movieMgr.saveToFile("data/movies.csv");
    userMgr.saveToFile("data/users.csv");
    ratingMgr.saveToFile("data/ratings.csv");
    
    std::cout << "\n💾 모든 변경 사항이 CSV 파일들에 정상적으로 저장되었습니다.\n";

    return 0;
}