CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

#실행파일 이름
TARGET = movie-app
#컴파일 할 갹체 파일들
OBJS = Movie.o User.o Rating.o main.o

#빌드 규칙
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

Movie.o: Movie.cpp Movie.h
	$(CXX) $(CXXFLAGS) -c Movie.cpp

User.o: User.cpp User.h
	$(CXX) $(CXXFLAGS) -c User.cpp

Rating.o: Rating.cpp Rating.h
	$(CXX) $(CXXFLAGS) -c Rating.cpp

main.o: main.cpp Movie.h User.h Rating.h
	$(CXX) $(CXXFLAGS) -c main.cpp

#파일 이름이 아님을 명시
.PHONY: clean run

run: $(TARGET)
	./$(TARGET)
clean:
	rm -f $(OBJS) $(TARGET)
