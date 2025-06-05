CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
FTXUIFLAGS = -lftxui-component -lftxui-dom -lftxui-screen

./bin/seeker.out: ./src/main.cpp
	$(CXX) ./src/main.cpp ./src/utils.cpp  -o ./bin/seeker.out $(FTXUIFLAGS) $(CXXFLAGS) 
