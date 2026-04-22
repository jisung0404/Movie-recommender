CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

# 실행파일 이름
TARGET = movie-app
# 컴파일 할 객체 파일들
OBJS = main.o Movie.o User.o Rating.o MovieManager.o UserManager.o RatingManager.o

# 빌드 규칙 (최종 실행파일)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# --- 개별 객체 파일 빌드 규칙 ---
Movie.o: Movie.cpp Movie.h
	$(CXX) $(CXXFLAGS) -c Movie.cpp

User.o: User.cpp User.h
	$(CXX) $(CXXFLAGS) -c User.cpp

Rating.o: Rating.cpp Rating.h
	$(CXX) $(CXXFLAGS) -c Rating.cpp

MovieManager.o: MovieManager.cpp MovieManager.h Movie.h BaseManager.h
	$(CXX) $(CXXFLAGS) -c MovieManager.cpp

UserManager.o: UserManager.cpp UserManager.h User.h BaseManager.h
	$(CXX) $(CXXFLAGS) -c UserManager.cpp

RatingManager.o: RatingManager.cpp RatingManager.h Rating.h BaseManager.h
	$(CXX) $(CXXFLAGS) -c RatingManager.cpp

main.o: main.cpp Movie.h User.h Rating.h MovieManager.h UserManager.h RatingManager.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# 파일 이름이 아님을 명시
.PHONY: clean run

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)