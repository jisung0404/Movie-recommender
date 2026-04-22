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

        switch (choice) {
           case 1: {
                int id, year;
                std::string title, genre;
                std::cout << "영화 ID, 제목, 장르, 개봉연도를 입력하세요 (띄어쓰기로 구분):\n> ";
                std::cin >> id >> title >> genre >> year;
                
                Movie newMovie(id, title, genre, year);
                movieMgr.addMovie(newMovie);
                std::cout << "영화가 성공적으로 추가되었습니다!\n";
                break;
            }
            case 2: {
                std::string searchTitle;
                std::cout << "검색할 영화 제목을 입력하세요:\n> ";
                std::cin >> searchTitle;
                
                Movie* found = movieMgr.findByTitle(searchTitle);
                if (found != nullptr) {
                    std::cout << "검색 결과: " << *found << "\n";
                } else {
                    std::cout << "해당 제목의 영화를 찾을 수 없습니다.\n";
                }
                break;
            }
            case 3:
                std::cout << "\n--- 등록된 전체 영화 ---\n";
                movieMgr.printAll();
                break;
            case 4: {
                // 매니저한테 정렬하라고 시키고 바로 출력
                movieMgr.sortByRating();
                std::cout << "\n평점순으로 정렬되었습니다!\n--- 정렬된 영화 목록 ---\n";
                movieMgr.printAll();
                break;
            }
            case 5: {
                int id;
                std::string name, email;
                std::cout << "사용자 ID, 이름, 이메일을 입력하세요 (띄어쓰기로 구분):\n> ";
                std::cin >> id >> name >> email;
                
                User newUser(id, name, email);
                userMgr.addUser(newUser);
                std::cout << "사용자가 성공적으로 추가되었습니다!\n";
                break;
            }
            case 6:
                std::cout << "\n--- 등록된 전체 사용자 ---\n";
                userMgr.printAll();
                break;
            case 7: {
                int userId, movieId;
                double score;
                std::cout << "사용자 ID, 영화 ID, 평점(0.0~5.0)을 입력하세요 (띄어쓰기로 구분):\n> ";
                std::cin >> userId >> movieId >> score;
                
                ratingMgr.addRating(userId, movieId, score);
                std::cout << "평점이 성공적으로 등록되었습니다!\n";
                break;
            }
            case 8: {
                int targetMovieId;
                std::cout << "평점을 조회할 영화 ID를 입력하세요:\n> ";
                std::cin >> targetMovieId;
                
                std::cout << "\n--- 해당 영화의 평점 내역 ---\n";
                
                ratingMgr.printRatingsByMovie(targetMovieId);
                break;
            }
            default:
                std::cout << "0~8 사이의 번호를 선택해주세요.\n";
        }
    }

    return 0;
}