CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -Iecs -Iaudio -Isrc

SRCS = src/Principal.cpp \
       src/ContextoOpenGLLinux.cpp \
       src/VentanaX11.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = motor

LIBS = -lGL -lX11 -lpthread

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
