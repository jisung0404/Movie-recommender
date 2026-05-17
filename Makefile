TARGET = movie-app
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -Iinclude

SRCS = src/main.cpp src/Movie.cpp src/User.cpp src/Rating.cpp src/MovieManager.cpp src/UserManager.cpp src/RatingManager.cpp src/FileHandler.cpp src/Recommender.cpp
OBJS = obj/main.o obj/Movie.o obj/User.o obj/Rating.o obj/MovieManager.o obj/UserManager.o obj/RatingManager.o obj/FileHandler.o obj/Recommender.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

run: $(TARGET)
	./$(TARGET)