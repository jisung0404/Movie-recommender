#include <iostream>
#include <vector>
#include <string>
#include "../include/MovieManager.h"
#include "../include/UserManager.h"
#include "../include/RatingManager.h"

int main() {
    MovieManager movieMgr;
    UserManager userMgr;
    RatingManager ratingMgr;

    // 초기 CSV 로드 단행
    movieMgr.loadFromFile("data/movies.csv");
    userMgr.loadFromFile("data/users.csv");
    ratingMgr.loadFromFile("data/ratings.csv");

    movieMgr.syncMovieRatings(ratingMgr);
    
    int menuChoice = -1;

    while (true) {
        std::cout << "\n===================================================\n";
        std::cout << "▶ 최적화 추천 엔진\n";
        std::cout << "===================================================\n";
        std::cout << " 1. 새로운 영화 추가\n";
        std::cout << " 2. 영화 제목으로 검색\n";
        std::cout << " 3. 영화 전체 목록 출력\n";
        std::cout << " 4. 영화 평점순 랭킹 정렬 출력\n";
        std::cout << " 5. 새로운 사용자 추가\n";
        std::cout << " 6. 사용자 전체 목록 출력\n";
        std::cout << " 7. 새로운 평점 입력\n";
        std::cout << " 8. 특정 영화별 누적 평점 보기\n";
        std::cout << " 9. 유저 맞춤형 협업 필터링 추천\n";
        std::cout << " 0. 시스템 종료 및 데이터 안전 저장\n";
        std::cout << "===================================================\n";
        std::cout << "실행할 메뉴 번호를 입력해 주세요: ";

        if (!(std::cin >> menuChoice)) {
            std::cin.clear();
            std::string badInput;
            std::getline(std::cin, badInput);
            std::cout << "[!] 올바른 숫자를 입력해 주세요.\n";
            continue;
        }
        std::cin.ignore();

        if (menuChoice == 0) {
            std::cout << "\n데이터 백업 및 가상 영구 데이터 저장을 가동합니다...\n";
            movieMgr.saveToFile("data/movies.csv");
            userMgr.saveToFile("data/users.csv");
            ratingMgr.saveToFile("data/ratings.csv");
            std::cout << "시스템 데이터 영구 보존 완료.\n";
            break;
        }

        switch (menuChoice) {
            case 1: {
                std::cout << "\n--- [1] 신규 영화 데이터 등록 ---\n";
                int id, year; std::string title, genre;
                std::cout << "영화 고유 ID 입력: "; std::cin >> id; std::cin.ignore();
                std::cout << "영화 제목 입력: "; std::getline(std::cin, title);
                std::cout << "영화 장르 입력: "; std::getline(std::cin, genre);
                std::cout << "영화 개봉 연도 입력: "; std::cin >> year; std::cin.ignore();
                movieMgr.addMovie(Movie(id, title, genre, year));
                std::cout << "🎬 신규 영화 마스터 데이터 등록 완료.\n";
                break;
            }
            case 2: {
                std::cout << "\n--- [2] O(1) 고속 영화 제목 검색 ---\n";
                std::string searchTitle;
                std::cout << "검색할 영화의 정확한 제목 입력: "; std::getline(std::cin, searchTitle);
                Movie* mPtr = movieMgr.findByTitle(searchTitle);
                if (mPtr == nullptr) std::cout << "[X] 해당 영화를 장부에서 찾을 수 없습니다.\n";
                else std::cout << "색 결과 발견 ->  " << *mPtr << "\n";
                break;
            }
            case 3: {
                std::cout << "\n--- [3] 영화 전체 데이터베이스 출력 ---\n";
                movieMgr.printAll();
                break;
            }
            case 4: {
                std::cout << "\n--- [4] 대중 평균 평점순 랭킹 오더 출력 ---\n";
                movieMgr.printSortedByRating(ratingMgr);
                break;
            }
            case 5: {
                std::cout << "\n--- [5] 신규 유저 계정 등록 ---\n";
                int id; std::string name, email;
                std::cout << "유저 고유 ID 입력: "; std::cin >> id; std::cin.ignore();
                std::cout << "유저 이름 입력: "; std::getline(std::cin, name);
                std::cout << "유저 이메일 입력: "; std::getline(std::cin, email);
                userMgr.addUser(User(id, name, email));
                std::cout << "👤 신규 사용자 계정이 인덱싱 캐시에 추가되었습니다.\n";
                break;
            }
            case 6: {
                std::cout << "\n--- [6] 마스터 유저 목록 출력 ---\n";
                userMgr.printAll();
                break;
            }
            case 7: {
                std::cout << "\n--- [7] 실시간 영화 평점 트래픽 입력 ---\n";
                int uId, mId; double score;
                std::cout << "평점 유저 ID 입력: "; std::cin >> uId;
                std::cout << "대상 영화 ID 입력: "; std::cin >> mId;
                std::cout << "평점 입력 (0.0 ~ 5.0): "; std::cin >> score; std::cin.ignore();
                ratingMgr.addRating(uId, mId, score);
                std::cout << "평점 트래픽 매핑이 완료되었습니다.\n";
                break;
            }
            case 8: {
                std::cout << "\n--- [8] O(1) 고속 특정 영화별 평점 리포트 조회 ---\n";
                int mId;
                std::cout << "조회 타깃 영화 ID 입력: "; std::cin >> mId; std::cin.ignore();
                ratingMgr.printRatingsByMovie(mId);
                break;
            }
            case 9: {
                std::cout << "\n--- [9] 유저 취향 맞춤형 협업 필터링 추천 가동 ---\n";
                int targetUid;
                std::cout << "추천 스캔을 돌릴 타깃 유저 ID 입력: "; std::cin >> targetUid; std::cin.ignore();
                
                User* uPtr = userMgr.findById(targetUid);
                if (uPtr == nullptr) {
                    std::cout << "[X] 시스템에 등록되지 않은 사용자 ID입니다.\n";
                } else {
                    std::cout << uPtr->getName() << "' 유저의 코사인 동조율 기반 추천 가동...\n";
                    std::vector<Movie> res = movieMgr.recommend(targetUid, ratingMgr, 5);
                    if (res.empty()) {
                        std::cout << "[!] 유사 취향을 지닌 이웃이 부재하거나 미시청 영화가 없어 목록을 생성하지 못했습니다.\n";
                    } else {
                        for (size_t i = 0; i < res.size(); ++i) {
                            std::cout << "[추천 " << i + 1 << "순위] " << res[i] << "\n";
                        }
                    }
                }
                break;
            }
            default:
                std::cout << "[!] 잘못된 번호 선택입니다. 0번부터 9번 사이를 선택해 주세요.\n";
                break;
        }
    }
    return 0;
}