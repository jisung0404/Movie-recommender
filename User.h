#pragma once
#include <string>

class User {
private:
    int         id;
    std::string name;
    std::string email;
public:
   
    User();

    User(int id, const std::string& name, const std::string& email);

    int         getId()     const;
    std::string getName()   const;
    std::string getEmail()  const;

    void        display()   const;

    bool                    operator==(const User& other) const; // 이름 검색용
    friend std::ostream&    operator<<(std::ostream& os, const User& u); // 출력용
};