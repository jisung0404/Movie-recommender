#include "MovieManager.h"
#include "FileHandler.h"
#include <iostream>
#include <algorithm>

using namespace std;

// 1. 영화 추가
void MovieManager::addMovie(const Movie& m) {
    movies.push_back(m); 
}

// 2. 제목으로 검색
Movie* MovieManager::findByTitle(const string& title) {
    for (Movie& m : movies) {
        if (m.getTitle() == title) { // 제목이 일치하는 영화를 찾으면
            return &m;               // 그 영화의 메모리 주소(&)를 반환
        }
    }
    return nullptr;                  // 찾아보고 없으면 nullptr 반환 (방어적 프로그래밍) 
}

// 3. 평점순 정렬
void MovieManager::sortByRating() {
    sort(movies.begin(), movies.end()); 
}

// 4. 전체 출력
void MovieManager::printAll() const {
    for (const Movie& m : movies) {
        cout << m << endl; 
    }
}

// 5. 파일에서 데이터 로드
void MovieManager::loadFromFile(const string& filename) {
    // FileHandler의 static 메서드에 내 'movies' 벡터의 주소를 넘겨서 채워달라고 함
    FileHandler::loadMovies(filename, this->movies);
}