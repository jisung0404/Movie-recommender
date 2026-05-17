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
    // 뒤에 const를 붙여서 구현부와 상속 구조를 일치시킴
    void saveToFile(const std::string& filename) const override;
    int size() const override;
    
    void addUser(const User& user);
    void printAll() const;

    User* findByName(const std::string& name);
};

#endif