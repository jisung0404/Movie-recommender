#include "SimilarityCalculator.h"
#include <cmath>
#include <map>

double SimilarityCalculator::calculateUserSimilarity(const std::vector<Rating>& userA, const std::vector<Rating>& userB) {
    std::map<int, double> mapA;
    for (const auto& r : userA) {
        mapA[r.getMovieId()] = r.getScore();
    }

    double dotProduct = 0.0;  // 분자: 벡터 내적 (A · B)
    double normA = 0.0;       // 분모 왼쪽: ||A||^2
    double normB = 0.0;       // 분모 오른쪽: ||B||^2

    // 유저 A 평점 제곱합
    for (const auto& r : userA) normA += r.getScore() * r.getScore();
    // 유저 B 평점 제곱합
    for (const auto& r : userB) normB += r.getScore() * r.getScore();

    bool hasCommon = false;
    // 공통 영화 찾아서 내적 계산
    for (const auto& rB : userB) {
        int mId = rB.getMovieId();
        if (mapA.find(mId) != mapA.end()) {
            hasCommon = true;
            dotProduct += mapA[mId] * rB.getScore();
        }
    }

    // 공통 평점이 없거나 크기가 0이면 유사도 0 반환 (예외 방어)
    if (!hasCommon || normA == 0.0 || normB == 0.0) return 0.0;

    return dotProduct / (std::sqrt(normA) * std::sqrt(normB));
}