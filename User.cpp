#include <iostream>
#include "User.h"
using namespace std;

User::User() {
    id = 0;
    name = "";
    email = "";
}

User::User(int i, const string& n, const string& e) {
    id = i;
    name = n;
    email = e;
}

int User::getId() const {
    return id; 
}

string User::getName() const {
    return name;
}

string User::getEmail() const {
    return email;
}

void User::display() const {
    cout << "[" << id << "] " << name << " (" << email << ")" << endl;
}