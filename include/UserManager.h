#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "BaseManager.h"
#include "User.h"
#include <vector>
#include <string>
#include <unordered_map> // 고속 수색 인덱싱용 해시맵 장착

class UserManager : public BaseManager {
private:
    std::vector<User> users;
    std::unordered_map<int, size_t> userMap; // O(1) 수색용 캐시 레이어

public:
    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;

    void addUser(const User& user);
    User* findById(int id);
    void printAll() const;
};

#endif