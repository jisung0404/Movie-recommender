#include <iostream>
#include "User.h"
using namespace std;

User::User()
    : id(0), name(""), email("") {}

User::User(int i, const string& n, const string& e)
    : id(i), name(n), email(e) {}

int         User::getId()          const { return id; }
string      User::getName()        const { return name; }
string      User::getEmail()       const { return email; }

void        User::display()        const {
    std::cout << "[" << id 
              << "] " << name 
              << " (" << email 
              << ")" << endl;
}

// --- [M2 추가] 연산자 오버로딩 구현부 ---

// == 연산자: 이름이 같으면 같은 사용자로 취급 (검색용)
bool        User::operator==(const User& other) const {
    return this->name == other.name;
}
