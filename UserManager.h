#pragma once
#include <vector>
#include <string>
#include "User.h"
#include "BaseManager.h"

class UserManager : public BaseManager {
private:
    std::vector<User> users; // 사용자 목록을 저장

public:
    void addUser(const User& u);
    
    // 이름으로 찾기 (못 찾으면 nullptr 반환) 
    User* findByName(const std::string& name);
    
    // BaseManger의 전체 출력 기능 구현
    void printAll() const override;
};