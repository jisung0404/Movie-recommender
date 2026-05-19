#include "SimilarityCalculator.h"

int SimilarityCalculator::calculateGenreSimilarity(const std::string& genreA, const std::string& genreB) {
    if (genreA.empty() || genreB.empty()) return -100; // 예외 방어
    return (genreA == genreB) ? 100 : 0;
}