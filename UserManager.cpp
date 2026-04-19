#include "UserManager.h"
#include <iostream>

using namespace std;

// 1. 사용자 추가
void UserManager::addUser(const User& u) {
    users.push_back(u);
}

// 2. 이름으로 사용자 검색
User* UserManager::findByName(const string& name) {
    for (User& u : users) {
        if (u.getName() == name) {
            return &u;  // 해당 사용자 찾으면 그 사용자의 메모리 주소 반환
        }
    }
    return nullptr;     // 해당 사용자 못찾으면 nullptr 반환
}

// 3. 전체 사용자 출력
void UserManager::printAll() const {
    for (const User& u : users) {
        cout << u << endl;
    }
}