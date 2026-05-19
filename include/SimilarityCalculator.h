#ifndef SIMILARITY_CALCULATOR_H
#define SIMILARITY_CALCULATOR_H

#include <string>

class SimilarityCalculator {
public:
    // 두 영화의 장르를 비교해 동일 장르면 100점, 다르면 0점을 반환
    static int calculateGenreSimilarity(const std::string& genreA, const std::string& genreB);
};

#endif