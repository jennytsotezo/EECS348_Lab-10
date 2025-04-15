CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -g

TARGET := Assignment10.exe
OBJS := main.o calculator.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main\ \(2\).cpp calculator.hpp
	$(CXX) $(CXXFLAGS) -c main\ \(2\).cpp

calculator.o: calculator.cpp calculator.hpp
	$(CXX) $(CXXFLAGS) -c calculator.cpp

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean