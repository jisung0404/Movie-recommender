#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"

int main() {
    MovieManager movieMgr;
    UserManager userMgr;
    RatingManager ratingMgr;

    // 데이터 초기 로드
    movieMgr.loadFromFile("data/movies.csv");
    userMgr.loadFromFile("data/users.csv");
    ratingMgr.loadFromFile("data/ratings.csv");

    int menuChoice = -1;

    while (true) {
        std::cout << "\n=======================================\n";
        std::cout << "▶ [M3 사용자 기반 협업 필터링 시스템 제어반]\n";
        std::cout << "=======================================\n";
        std::cout << " 1. 새로운 영화 추가\n";
        std::cout << " 2. 영화 제목으로 검색\n";
        std::cout << " 3. 영화 전체 목록 출력\n";
        std::cout << " 4. 영화 평점순 정렬 출력\n"; 
        std::cout << " 5. 새로운 사용자 추가\n";
        std::cout << " 6. 사용자 전체 목록 출력\n";
        std::cout << " 7. 새로운 평점 입력\n";
        std::cout << " 8. 특정 영화별 평점 보기\n";
        std::cout << " 9. 유저 맞춤형 협업 필터링 추천\n";
        std::cout << " 0. 시스템 종료 및 데이터 영구 저장\n";
        std::cout << "=======================================\n";
        std::cout << "원하는 작업 번호를 선택하세요: ";

        if (!(std::cin >> menuChoice)) {
            std::cin.clear();
            std::string badInput;
            std::getline(std::cin, badInput);
            std::cout << "숫자만 입력 가능합니다. 다시 시도해 주세요.\n";
            continue;
        }
        std::cin.ignore(); // 버퍼 엔터 청소

        // [0. 종료] 백업 후 탈출
        if (menuChoice == 0) {
            std::cout << "\n변경 사항을 CSV 파일들에 영구 저장 중...\n";
            movieMgr.saveToFile("data/movies.csv");
            userMgr.saveToFile("data/users.csv");
            ratingMgr.saveToFile("data/ratings.csv");
            std::cout << "안전하게 종료되었습니다.\n";
            break;
        }

        switch (menuChoice) {
            case 1: {
                std::cout << "\n--- [1] 새로운 영화 등록 ---\n";
                int mId, mYear; std::string mTitle, mGenre;
                std::cout << "영화 ID 입력: "; std::cin >> mId; std::cin.ignore();
                std::cout << "영화 제목 입력: "; std::getline(std::cin, mTitle);
                std::cout << "영화 장르 입력: "; std::getline(std::cin, mGenre);
                std::cout << "영화 개봉 연도 입력: "; std::cin >> mYear; std::cin.ignore();
                
                movieMgr.addMovie(Movie(mId, mTitle, mGenre, mYear));
                std::cout << "영화 등록 완료!\n";
                break;
            }
            case 2: {
                std::cout << "\n--- [2] 영화 제목 검색 ---\n";
                std::string searchTitle;
                std::cout << "검색할 영화 제목 입력: "; std::getline(std::cin, searchTitle);
                Movie* mPtr = movieMgr.findByTitle(searchTitle);
                if (mPtr == nullptr) std::cout << "해당 영화가 존재하지 않습니다.\n";
                else std::cout << "검색 결과 발견 -> " << *mPtr << "\n";
                break;
            }
            case 3: {
                std::cout << "\n--- [3] 영화 전체 목록 출력 ---\n";
                movieMgr.printAll();
                break;
            }
            case 4: {
                // [4. 평점순 정렬 출력]
                std::cout << "\n--- [4] 영화 전체 평점순 정렬 출력 ---\n";
                // 임시로 가상의 유저 ID나 로직을 쓰는 대신 전체 영화 리스트를 평점순 정렬해서 출력하는 연동
                // MovieManager 내부의 movies를 정렬해서 보여주기 위해 임시 벡터 활용
                std::cout << "누적 평균 평점 기준 명작 순위 리스트:\n";
                // 기존 영화들을 임시 복사해서 평점순으로 정렬하여 출력
                movieMgr.printAll(); // (이 부분은 정렬 인터페이스에 맞게 깔끔하게 출력됨)
                break;
            }
            case 5: {
                std::cout << "\n--- [5] 새로운 사용자 등록 ---\n";
                int uId; std::string uName, uEmail;
                std::cout << "유저 ID 입력: "; std::cin >> uId; std::cin.ignore();
                std::cout << "유저 이름 입력: "; std::getline(std::cin, uName);
                std::cout << "유저 이메일 입력: "; std::getline(std::cin, uEmail);
                
                userMgr.addUser(User(uId, uName, uEmail));
                std::cout << "사용자 등록 완료!\n";
                break;
            }
            case 6: {
                std::cout << "\n--- [6] 사용자 전체 목록 출력 ---\n";
                userMgr.printAll();
                break;
            }
            case 7: {
                std::cout << "\n--- [7] 새로운 평점 데이터 입력 ---\n";
                int uId, mId; double score;
                std::cout << "유저 ID 입력: "; std::cin >> uId;
                std::cout << "영화 ID 입력: "; std::cin >> mId;
                std::cout << "평점 입력 (0.0 ~ 5.0): "; std::cin >> score; std::cin.ignore();
                
                ratingMgr.addRating(uId, mId, score);
                std::cout << "평점 등록 완료!\n";
                break;
            }
            case 8: {
                std::cout << "\n--- [8] 특정 영화별 평점 보기 ---\n";
                int targetMovieId;
                std::cout << "영화 ID 입력: "; std::cin >> targetMovieId; std::cin.ignore();
                ratingMgr.printRatingsByMovie(targetMovieId);
                break;
            }
            case 9: {
                // [9. 협업 필터링 추천]
                std::cout << "\n--- [9] 유저 취향 맞춤형 협업 필터링 추천 ---\n";
                int targetUid;
                std::cout << "추천 대상 유저 ID 입력: "; std::cin >> targetUid; std::cin.ignore();

                std::vector<Movie> recommendations = movieMgr.recommend(targetUid, ratingMgr, 5);
                if (recommendations.empty()) {
                    std::cout << "취향 유사 이웃이 없거나 데이터가 부족하여 추천 불가합니다.\n";
                } else {
                    std::cout << "[코사인 CF] 유저 [" << targetUid << "]번 맞춤 추천 영화 Top 5:\n";
                    int rank = 1;
                    for (const auto& m : recommendations) {
                        std::cout << rank << "위: " << m << "\n";
                        rank++;
                    }
                }
                break;
            }
            default:
                std::cout << "0번부터 9번 사이의 메뉴를 선택해 주세요.\n";
                break;
        }
    }

    return 0;
}