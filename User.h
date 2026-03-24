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

    int         getId() const;
    std::string getName() const;
    std::string getEmail() const;

    void display() const;

};