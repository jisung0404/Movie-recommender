#include <iostream>
#include <vector>
#include <string>
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"

int main() {
    MovieManager movieMgr;
    UserManager userMgr;
    RatingManager ratingMgr;

    movieMgr.loadFromFile("data/movies.csv");
    userMgr.loadFromFile("data/users.csv");
    ratingMgr.loadFromFile("data/ratings.csv");

    int menuChoice = 0;

    while (true) {
        std::cout << "\n=======================================\n";
        std::cout << "▶ 현재 로드 현황 - 영화: " << movieMgr.size() 
                  << "개 | 유저: " << userMgr.size() 
                  << "개 | 평점: " << ratingMgr.size() << "개\n";
        std::cout << "=======================================\n";
        std::cout << " 1. 새로운 사용자(User) 등록\n";
        std::cout << " 2. 새로운 영화(Movie) 등록\n";
        std::cout << " 3. 새로운 평점(Rating) 등록\n";
        std::cout << " 4. 등록된 영화 기준 장르 추천 실행\n";
        std::cout << " 5. 등록된 사용자 ID 검색\n"; // 💡 메뉴 텍스트 변경!
        std::cout << " 6. 시스템 종료 및 변경 사항 저장 (Save & Exit)\n";
        std::cout << "=======================================\n";
        std::cout << "원하는 작업 번호를 선택하세요: ";
        std::cin >> menuChoice;
        std::cin.ignore(); 

        if (menuChoice == 6) { // 종료 번호를 6번으로 조정
            std::cout << "\n데이터 변경 사항을 CSV 파일들에 영구 저장 중...\n";
            movieMgr.saveToFile("data/movies.csv");
            userMgr.saveToFile("data/users.csv");
            ratingMgr.saveToFile("data/ratings.csv");
            std::cout << "안전하게 종료되었습니다. 수고하셨습니다!\n";
            break; 
        }

        switch (menuChoice) {
            case 1: {
                std::cout << "\n--- [1] 새로운 사용자 등록 ---\n";
                int newUserId; std::string newUserName, newUserEmail;
                std::cout << "유저 ID 입력: "; std::cin >> newUserId; std::cin.ignore();
                std::cout << "유저 이름 입력: "; std::getline(std::cin, newUserName);
                std::cout << "유저 이메일 입력: "; std::getline(std::cin, newUserEmail);
                
                User newUser(newUserId, newUserName, newUserEmail);
                userMgr.addUser(newUser);
                
                std::cout << "사용자 등록 완료 -> " << newUser << "\n";
                break;
            }
            case 2: {
                std::cout << "\n--- [2] 새로운 영화 등록 ---\n";
                int newMovieId; std::string newMovieTitle, newMovieGenre; int newMovieYear;
                std::cout << "영화 ID 입력: "; std::cin >> newMovieId; std::cin.ignore();
                std::cout << "영화 제목 입력: "; std::getline(std::cin, newMovieTitle);
                std::cout << "영화 장르 입력: "; std::getline(std::cin, newMovieGenre);
                std::cout << "영화 개봉 연도 입력: "; std::cin >> newMovieYear;
                
                Movie newMovie(newMovieId, newMovieTitle, newMovieGenre, newMovieYear);
                movieMgr.addMovie(newMovie);
                
                std::cout << "영화 등록 완료 -> " << newMovie << "\n";
                break;
            }
            case 3: {
                std::cout << "\n--- [3] 새로운 평점 등록 ---\n";
                int uId, mId; double score;
                std::cout << "평점을 매길 유저 ID 입력: "; std::cin >> uId;
                std::cout << "평점을 매길 영화 ID 입력: "; std::cin >> mId;
                std::cout << "평점 입력 (0.0 ~ 5.0): "; std::cin >> score;
                
                ratingMgr.addRating(uId, mId, score);
                std::cout << "평점 데이터 등록 완료!\n";
                break;
            }
            case 4: {
                std::cout << "\n--- [4] 영화 기준 장르 기반 추천 ---\n";
                if (movieMgr.size() == 0) {
                    std::cout << "등록된 영화 데이터가 없습니다!\n";
                    break;
                }
                
                std::cout << "현재 등록된 영화 목록:\n";
                movieMgr.printAll();
                
                std::string targetTitle;
                std::cout << "\n추천의 기준이 될 영화 제목을 입력하세요: ";
                std::getline(std::cin, targetTitle);
                
                Movie* targetPtr = movieMgr.findByTitle(targetTitle);
                if (targetPtr == nullptr) {
                    std::cout << "입력한 제목의 영화를 찾을 수 없습니다.\n";
                } else {
                    std::cout << "\n=== '" << targetPtr->getTitle() << "' 기준 장르 기반 추천 결과 ===\n";
                    std::vector<Movie> recommendations = movieMgr.recommend(*targetPtr, 5);
                    
                    if (recommendations.empty()) {
                        std::cout << "- 동일한 장르의 다른 추천 영화가 없습니다.\n";
                    } else {
                        for (const auto& movie : recommendations) {
                            std::cout << "- " << movie << "\n";
                        }
                    }
                }
                break;
            }
            case 5: { 
                std::cout << "\n--- [5] 등록된 사용자 ID 검색 ---\n";
                int searchId;
                std::cout << "검색할 유저의 ID(숫자)를 입력하세요: ";
                std::cin >> searchId;
                std::cin.ignore(); // 숫자 입력 후 버퍼 엔터값 삭제

                User* foundUser = userMgr.findById(searchId);
                if (foundUser == nullptr) {
                    std::cout << "ID [" << searchId << "] 사용자를 시스템에서 찾을 수 없습니다.\n";
                } else {
                    std::cout << "검색 결과 발견! -> " << *foundUser << "\n";
                }
                break;
            }
            default:
                std::cout << "잘못된 번호입니다. 1번부터 6번 사이의 숫자를 입력해 주세요.\n";
                break;
        }
    }

    return 0;
}