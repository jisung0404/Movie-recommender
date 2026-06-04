#include "../include/MovieManager.h"
#include "../include/RatingManager.h"
#include "../include/SimilarityCalculator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>

static std::string toLowerCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void MovieManager::loadFromFile(const std::string& filename) {
    movies.clear();
    idMap.clear();
    titleMap.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, title, genre, yearStr;
        
        if (std::getline(ss, idStr, ',') && std::getline(ss, title, ',') &&
            std::getline(ss, genre, ',') && std::getline(ss, yearStr, ',')) {
            try {
                int id = std::stoi(idStr);
                int year = std::stoi(yearStr);
                addMovie(Movie(id, title, genre, year));
            } catch (...) {}
        }
    }
    file.close();
}

void MovieManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    for (const auto& m : movies) {
        file << m.getId() << "," << m.getTitle() << "," << m.getGenre() << "," << m.getReleaseYear() << "\n";
    }
    file.close();
}

int MovieManager::size() const { return movies.size(); }

void MovieManager::addMovie(const Movie& movie) { 
    movies.push_back(movie); 
    size_t newIdx = movies.size() - 1;
    idMap[movie.getId()] = newIdx;
    titleMap[movie.getTitle()] = newIdx;
}

Movie* MovieManager::findByTitle(const std::string& title) {
    auto it = titleMap.find(title);
    if (it != titleMap.end()) return &movies[it->second];
    return nullptr;
}

Movie* MovieManager::findById(int id) {
    auto it = idMap.find(id);
    if (it != idMap.end()) return &movies[it->second];
    return nullptr;
}

void MovieManager::printAll() const {
    for (const auto& m : movies) std::cout << m << "\n";
}

std::vector<Movie> MovieManager::searchMoviesEnhanced(const std::string& query) const {
    std::vector<Movie> foundMovies;
    std::string lowerQuery = toLowerCase(query);

    for (const auto& m : movies) {
        std::string lowerTitle = toLowerCase(m.getTitle());
        if (lowerTitle.find(lowerQuery) != std::string::npos) {
            foundMovies.push_back(m);
        }
    }
    return foundMovies;
}

// [확장 요건 2번 통계 기능 구현부]: 실시간 수치 연산 및 화면 대시보드 리포팅 출력 책임 완수
std::vector<std::pair<double, Movie>> MovieManager::printAdvancedStatistics(const RatingManager& ratingMgr) const {
    std::vector<std::pair<double, Movie>> sortedMovies;
    
    double totalScoreSum = 0.0;
    int totalTrafficCount = 0;
    std::map<std::string, int> genreFrequencyMap; // 장르 빈도수 카운팅 장부

    for (const auto& m : movies) {
        double avg = ratingMgr.getAverageRating(m.getId());
        sortedMovies.push_back({avg, m});
        
        totalScoreSum += (avg * m.getRatingCount());
        totalTrafficCount += m.getRatingCount();
        genreFrequencyMap[m.getGenre()]++;
    }

    // 람다 함수 기반 평점 높은 순 정렬
    std::sort(sortedMovies.begin(), sortedMovies.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    // 최다 보유 장르 스캔
    std::string mostPopularGenre = "None";
    int maxGenreCount = -1;
    for (const auto& pair : genreFrequencyMap) {
        if (pair.second > maxGenreCount) {
            maxGenreCount = pair.second;
            mostPopularGenre = pair.first;
        }
    }

    double systemGlobalAverage = (totalTrafficCount == 0) ? 0.0 : totalScoreSum / totalTrafficCount;

    // 정돈된 텍스트 양식으로 콘솔창에 통계 리포트를 출력합니다
    std::cout << "\n===================================================\n";
    std::cout << "[-] 시스템 통합 통계 분석 데이터 요약\n";
    std::cout << "===================================================\n";
    std::cout << " * 전체 플랫폼 누적 평점 평균: " << systemGlobalAverage << " 점\n";
    std::cout << " * 데이터베이스 총 평점 등록 건수: " << totalTrafficCount << " 건\n";
    std::cout << " * 플랫폼 내 최다 보유 강세 장르: " << mostPopularGenre << " (총 " << maxGenreCount << "개 보유)\n";
    std::cout << "===================================================\n";

    std::cout << "\n[+] 실시간 평점순 랭킹 차트 현황\n";
    for (size_t i = 0; i < sortedMovies.size(); ++i) {
        std::cout << "  " << i + 1 << "위 -> [평점: " << sortedMovies[i].first << "점] " << sortedMovies[i].second << "\n";
    }

    return sortedMovies;
}

// [확장 요건 6번 CSV 강화 구현부]: 화면 출력 간섭 없이 순수하게 파일 스트림 저장 책임을 완수
void MovieManager::exportStatisticsToCSV(const std::vector<std::pair<double, Movie>>& sortedMovies, const std::string& exportFilename) const {
    std::ofstream exportFile(exportFilename);
    if (!exportFile.is_open()) {
        std::cout << "[!] 통계 리포트 파일 생성에 실패하였습니다.\n";
        return;
    }

    exportFile << "rank,movieId,title,averageRating,ratingCount\n";
    for (size_t i = 0; i < sortedMovies.size(); ++i) {
        const Movie& m = sortedMovies[i].second;
        exportFile << (i + 1) << ","
                   << m.getId() << ","
                   << m.getTitle() << ","
                   << sortedMovies[i].first << ","
                   << m.getRatingCount() << "\n";
    }
    exportFile.close();
}

std::vector<Movie> MovieManager::recommend(int targetUserId, const RatingManager& ratingMgr, int N) {
    std::vector<Movie> recommendedList;
    std::vector<Rating> myRatings = ratingMgr.findByUser(targetUserId);
    if (myRatings.empty()) return recommendedList;

    std::set<int> myWatched;
    for (const auto& r : myRatings) myWatched.insert(r.getMovieId());

    std::vector<int> allUsers = ratingMgr.getAllUserIds();
    int bestNeighborId = -1;
    double maxSim = -1.0;

    for (int otherId : allUsers) {
        if (otherId == targetUserId) continue;
        std::vector<Rating> otherRatings = ratingMgr.findByUser(otherId);
        if (otherRatings.empty()) continue;

        double sim = SimilarityCalculator::calculateUserSimilarity(myRatings, otherRatings);
        if (sim > maxSim) {
            maxSim = sim;
            bestNeighborId = otherId;
        }
    }

    if (bestNeighborId == -1 || maxSim <= 0.0) return recommendedList;

    std::vector<Rating> neighborRatings = ratingMgr.findByUser(bestNeighborId);
    std::sort(neighborRatings.begin(), neighborRatings.end(), [](const Rating& a, const Rating& b) {
        return a.getScore() > b.getScore();
    });

    for (const auto& r : neighborRatings) {
        if (r.getScore() >= 3.5 && myWatched.find(r.getMovieId()) == myWatched.end()) {
            Movie* mPtr = findById(r.getMovieId()); 
            if (mPtr != nullptr) recommendedList.push_back(*mPtr);
        }
    }

    int actualN = std::min(N, (int)recommendedList.size());
    std::vector<Movie> finalResult;
    for (int i = 0; i < actualN; ++i) finalResult.push_back(recommendedList[i]);

    return finalResult;
}

void MovieManager::syncMovieRatings(const RatingManager& ratingMgr) {
    std::vector<int> allUsers = ratingMgr.getAllUserIds();
    for (int uId : allUsers) {
        std::vector<Rating> userRatings = ratingMgr.findByUser(uId);
        for (const auto& r : userRatings) {
            Movie* mPtr = findById(r.getMovieId());
            if (mPtr != nullptr) {
                mPtr->addRating(r.getScore());
            }
        }
    }
}