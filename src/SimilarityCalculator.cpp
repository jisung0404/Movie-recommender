#include "../include/SimilarityCalculator.h"
#include <cmath>
#include <unordered_map> // O(1) 고속 탐색을 위한 해시맵 장착

double SimilarityCalculator::calculateUserSimilarity(const std::vector<Rating>& userA, const std::vector<Rating>& userB) {
    // 최적화: 유저 A의 평점 데이터를 해시맵에 O(N)으로 한 번만 적재
    std::unordered_map<int, double> mapA;
    for (const auto& r : userA) {
        mapA[r.getMovieId()] = r.getScore();
    }

    double dotProduct = 0.0;
    double normA = 0.0;
    double normB = 0.0;

    for (const auto& r : userA) normA += r.getScore() * r.getScore();
    for (const auto& r : userB) normB += r.getScore() * r.getScore();

    bool hasCommon = false;
    // 최적화: 이중 루프 O(N^2)를 해시 수색을 통해 O(N) 선형 시간으로 단축
    for (const auto& rB : userB) {
        int mId = rB.getMovieId();
        auto it = mapA.find(mId);
        if (it != mapA.end()) {
            hasCommon = true;
            dotProduct += it->second * rB.getScore();
        }
    }

    if (!hasCommon || normA == 0.0 || normB == 0.0) return 0.0;

    return dotProduct / (std::sqrt(normA) * std::sqrt(normB));
}