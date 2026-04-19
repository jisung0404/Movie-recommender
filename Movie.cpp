#include "Movie.h"
#include <iostream>

// 기본 생성자
Movie::Movie()
    : id(0), releaseYear(0),
      totalRating(0.0), ratingCount(0) {}

// 4인자 생성자
Movie::Movie(int id, const std::string& title,
             const std::string& genre, int year)
    : id(id), title(title), genre(genre),
      releaseYear(year),
      totalRating(0.0), ratingCount(0) {}

int         Movie::getId()           const { return id; }
std::string Movie::getTitle()        const { return title; }
std::string Movie::getGenre()        const { return genre; }
int         Movie::getReleaseYear()  const { return releaseYear; }
int         Movie::getRatingCount()  const { return ratingCount; }

double Movie::getAverageRating() const {
    if (ratingCount == 0) return 0.0;   // 0 나눗셈 방어
    return totalRating / ratingCount;
}

void Movie::addRating(double r) {
    if (r < 0.0) {
        std::cout << "~입력 평점 0.0으로 재조정~\n";
        r = 0.0;
    } else if (r > 5.0) {
        std::cout << "~입력 평점 5.0으로 재조정~\n";
        r = 5.0;
    }
    totalRating += r;
    ratingCount++;
}

void Movie::display() const {           // 중복 제거 — 하나만 유지
    std::cout << "["  << id << "] "   << title
              << " (" << releaseYear << ")"
              << "  평점: " << getAverageRating()
              << " (" << ratingCount << "건)"
              << std::endl;
}

// --- [M2 추가] 연산자 오버로딩 구현부 ---
// == 연산자: 제목과 개봉 연도가 같으면 같은 영화로 취급
bool Movie::operator==(const Movie& other) const {
    return this->title == other.title && this->releaseYear == other.releaseYear;
}

// < 연산자: 평점 오름차순 정렬, 평점이 같으면 제목 가나다순
bool Movie::operator<(const Movie& other) const {
    if (this->getAverageRating() != other.getAverageRating()) {
        return this->getAverageRating() < other.getAverageRating();
    }
    return this->title < other.title; 
}

// << 연산자: 영화 정보 연쇄 출력
std::ostream& operator<<(std::ostream& os, const Movie& m) { 
    os << "[" << m.getId() << "] " << m.getTitle() 
       << " (" << m.getReleaseYear() << ")  평점: " 
       << m.getAverageRating() << " (" << m.getRatingCount() << "건)";
    return os;
}