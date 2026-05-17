#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <vector>
#include <string>
#include "Movie.h"
#include "User.h"
#include "Rating.h"

class FileHandler {
public:
    static void loadMovies(const std::string& filename, std::vector<Movie>& movies);
    static void loadUsers(const std::string& filename, std::vector<User>& users);
    static void loadRatings(const std::string& filename, std::vector<Rating>& ratings);
};

#endif