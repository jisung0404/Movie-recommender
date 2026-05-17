#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>

void FileHandler::loadMovies(const std::string& filename, std::vector<Movie>& movies) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "파일 오픈 실패: " << filename << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line); // 헤더 스킵
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, title, genre, year;
        std::getline(ss, id, ',');
        std::getline(ss, title, ',');
        std::getline(ss, genre, ',');
        std::getline(ss, year, ',');
        movies.push_back(Movie(std::stoi(id), title, genre, std::stoi(year)));
    }
    file.close();
}

void FileHandler::loadUsers(const std::string& filename, std::vector<User>& users) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "파일 오픈 실패: " << filename << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line); // 헤더 스킵
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, name, email;
        
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, email, ','); // 콤마 기준으로 이메일 문자열 파싱
        
        // 3번째 인자에 stoi() 없이 문자열 그대로 User 객체 생성 후 추가
        users.push_back(User(std::stoi(id), name, email));
    }
    file.close();
}

void FileHandler::loadRatings(const std::string& filename, std::vector<Rating>& ratings) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "파일 오픈 실패: " << filename << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line); // 헤더 스킵
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string uId, mId, score;
        std::getline(ss, uId, ',');
        std::getline(ss, mId, ',');
        std::getline(ss, score, ',');
        ratings.push_back(Rating(std::stoi(uId), std::stoi(mId), std::stod(score)));
    }
    file.close();
}