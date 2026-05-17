CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -Iinclude

# 실행파일 이름
TARGET = movie-app

# 컴파일할 객체 파일들이 obj/ 폴더에 생성되도록 지정
OBJS = obj/main.o obj/Movie.o obj/User.o obj/Rating.o \
       obj/MovieManager.o obj/UserManager.o obj/RatingManager.o obj/FileHandler.o

# 빌드 규칙 (최종 실행파일 생성)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# --- 개별 객체 파일 빌드 규칙 (src/ 에서 읽어서 obj/ 에 생성) ---

obj/FileHandler.o: src/FileHandler.cpp include/FileHandler.h include/Movie.h include/User.h include/Rating.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/FileHandler.cpp -o obj/FileHandler.o

obj/Movie.o: src/Movie.cpp include/Movie.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/Movie.cpp -o obj/Movie.o

obj/User.o: src/User.cpp include/User.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/User.cpp -o obj/User.o

obj/Rating.o: src/Rating.cpp include/Rating.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/Rating.cpp -o obj/Rating.o

obj/MovieManager.o: src/MovieManager.cpp include/MovieManager.h include/Movie.h include/BaseManager.h include/FileHandler.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/MovieManager.cpp -o obj/MovieManager.o

obj/UserManager.o: src/UserManager.cpp include/UserManager.h include/User.h include/BaseManager.h include/FileHandler.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/UserManager.cpp -o obj/UserManager.o

obj/RatingManager.o: src/RatingManager.cpp include/RatingManager.h include/Rating.h include/BaseManager.h include/FileHandler.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/RatingManager.cpp -o obj/RatingManager.o

obj/main.o: src/main.cpp include/Movie.h include/User.h include/Rating.h include/MovieManager.h include/UserManager.h include/RatingManager.h
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o obj/main.o

# 파일 이름이 아님을 명시
.PHONY: clean run

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf obj $(TARGET)