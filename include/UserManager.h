#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <vector>
#include <string>
#include "User.h"
#include "BaseManager.h"

class UserManager : public BaseManager {
private:
    std::vector<User> users;
public:
    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;
    
    void addUser(const User& user);
    void printAll() const;

    // 변경: 유저 ID로 검색하는 함수
    User* findById(int id);
};

#endif