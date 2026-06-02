#include "../include/UserManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

void UserManager::loadFromFile(const std::string& filename) {
    users.clear();
    userMap.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, name, email;

        if (std::getline(ss, idStr, ',') && std::getline(ss, name, ',') && std::getline(ss, email, ',')) {
            try {
                int id = std::stoi(idStr);
                addUser(User(id, name, email));
            } catch (...) {}
        }
    }
    file.close();
}

void UserManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    for (const auto& u : users) {
        file << u.getId() << "," << u.getName() << "," << u.getEmail() << "\n";
    }
    file.close();
}

int UserManager::size() const { return users.size(); }

void UserManager::addUser(const User& user) {
    users.push_back(user);
    userMap[user.getId()] = users.size() - 1; // 등록 즉시 인덱스 캐시 확보
}

// O(N) 루프 순회 제거 -> O(1) 해시 주소 반환으로 최적화 완료
User* UserManager::findById(int id) {
    auto it = userMap.find(id);
    if (it != userMap.end()) return &users[it->second];
    return nullptr;
}

void UserManager::printAll() const {
    for (const auto& u : users) std::cout << u << "\n";
}