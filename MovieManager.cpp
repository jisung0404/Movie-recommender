#include "MovieManager.h"
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