#include "UserManager.h"
#include <iostream>

void UserManager::loadFromFile(const std::string& filename) {
    // 기존 로드 로직 유지
}

void UserManager::saveToFile(const std::string& filename) const {
    // 기존 저장 로직 뒤에 const 추가
}

int UserManager::size() const {
    return users.size();
}

void UserManager::addUser(const User& user) {
    users.push_back(user);
}

void UserManager::printAll() const {
    for (const auto& user : users) {
        std::cout << user.getId() << ", " << user.getName() << "\n";
    }
}

User* UserManager::findByName(const std::string& name) {
    for (auto& user : users) {
        if (user.getName() == name) return &user;
    }
    return nullptr;
}