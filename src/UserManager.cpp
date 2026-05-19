#include "UserManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

void UserManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "유저 파일을 열 수 없습니다: " << filename << "\n";
        return;
    }

    users.clear();
    std::string line;
    
    std::getline(file, line); 

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string idStr, name, email;
        
        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, email, ',');
        
        try {
            int id = std::stoi(idStr);
            users.push_back(User(id, name, email));
        }
        catch (const std::exception& e) {
            continue; // 데이터 파싱 에러 방어
        }
    }
    file.close();
}

void UserManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    file << "id,name,email\n";
    for (const auto& user : users) {
        file << user.getId() << "," << user.getName() << "," << user.getEmail() << "\n";
    }
    file.close();
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

User* UserManager::findById(int id) {
    for (auto& user : users) {
        if (user.getId() == id) {
            return &user; // 일치하는 유저를 찾으면 주소 반환
        }
    }
    return nullptr; // 찾지 못하면 nullptr 반환
}