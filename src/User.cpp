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

// --- [M2 추가] 연산자 오버로딩 구현부 ---

// == 연산자: 이름이 같으면 같은 사용자로 취급 (검색용)
bool        User::operator==(const User& other) const {
    return this->name == other.name;
}

// << 연산자: 사용자 정보 연쇄 출력
std::ostream&     operator<<(std::ostream& os, const User& u) {
    os << "[" << u.getId() << "] " << u.getName() << " (" << u.getEmail() << ")";
    return os;
}