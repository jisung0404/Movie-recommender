#include <vector>
#include "Rating.h"
#include "BaseManager.h"

class RatingManager : public BaseManager {
private:
    std::vector<Rating> ratings; // 평점 기록 저장

public:
    // 평점 추가
    void addRating(int userId, int movieId, double score);
    
    // 특정 영화에 달린 평점들만 출력
    void printRatingsByMovie(int targetMovieId) const;
    
    // BaseManager의 전체 출력 기능 구현
    void printAll() const override;
};