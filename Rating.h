#pragma once

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

    bool                    operator==(const User& other) const; // 이름 검색용
    friend std::ostream&    operator<<(std::ostream& os, const User& u); // 출력용
};


