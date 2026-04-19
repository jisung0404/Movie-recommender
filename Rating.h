#pragma once
#include <iostream>

class Rating {
private:
    int     userId;
    int     movieId;
    double  score;
public:
    
    Rating();

    Rating(int uId, int mId, double s);

    int         getUserId()     const;
    int         getMovieId()    const;
    double      getScore()      const;
    void        display()       const;

    friend      std::ostream& operator<<(std::ostream& os, const Rating& r);
};


