#ifndef SIMILARITY_CALCULATOR_H
#define SIMILARITY_CALCULATOR_H

#include <vector>
#include "Rating.h"

class SimilarityCalculator {
public:
    // 장르 문자열 비교를 제거하고, 유저 평점 기반 함수로 변경
    static double calculateUserSimilarity(const std::vector<Rating>& userA, const std::vector<Rating>& userB);
};

#endif