CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2
LDLIBS = -lm

TARGET = drawTriangle

all: $(TARGET)

SRCS := $(filter-out drawLine.cpp, $(wildcard *.cpp))

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
