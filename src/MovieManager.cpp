#include "MovieManager.h"
#include <iostream>
#include <algorithm>

// 기존 loadFromFile, saveToFile, size, addMovie, printAll, findByTitle, sortByRating 구현부는 지성이가 만들어둔 본문 코드를 유지하되, 아래의 두 핵심 추천 메서드를 하단에 그대로 추가해줘!

// 장르 문자열을 비교하는 단순 유사도 알고리즘 (동일 장르 시 가중치 부여 패턴)
int MovieManager::calculateGenreSimilarity(const std::string& genreA, const std::string& genreB) {
    if (genreA.empty() || genreB.empty()) return -100; // 엣지 케이스 방어
    return (genreA == genreB) ? 100 : 0;
}

// 교안 12p 명세: 상위 N개 영화 반환하는 장르 기반 추천 시스템 핵심 구현
std::vector<Movie> MovieManager::recommend(const Movie& targetMovie, int N) {
    // 엣지 케이스: 영화 목록이 비어있거나 target 본인 제외하고 없을 때 방어
    if (movies.empty() || (movies.size() == 1 && movies Christy[0].getId() == targetMovie.getId())) {
        std::cout << "[Edge Case] 추천할 영화 데이터베이스가 비어있습니다.\n";
        return std::vector<Movie>();
    }

    // (movieId, 유사도 점수) 쌍을 저장하여 정렬하기 위한 pair 활용 패턴
    std::vector<std::pair<int, int>> similarities;
    for (const auto& movie : movies) {
        if (movie.getId() == targetMovie.getId()) continue; // 자기 자신은 추천에서 제외
        
        int sim = calculateGenreSimilarity(targetMovie.getGenre(), movie.getGenre());
        similarities.push_back({movie.getId(), sim});
    }

    // 교안 9p 문법 적용: 람다 식을 활용한 유사도 기준 내림차순 정렬
    std::sort(similarities.begin(), similarities.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });

    // 엣지 케이스: 추천할 영화가 N개 미만이면 있는 만큼만 반환
    int actualN = N;
    if (similarities.size() < (size_t)N) {
        actualN = similarities.size();
    }

    std::vector<Movie> recommendedList;
    for (int i = 0; i < actualN; ++i) {
        int targetId = similarities[i].first;
        for (const auto& movie : movies) {
            if (movie.getId() == targetId) {
                recommendedList.push_back(movie);
                break;
            }
        }
    }

    return recommendedList;
}