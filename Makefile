# ==========================================
# MAKEFILE - MOTOR 3D (LINUX / X11 / OPENGL)
# ==========================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Iinclude -Iecs -Iaudio -Isrc

# Archivos fuente del núcleo de la plataforma y aplicación principal
SRCS = src/Principal.cpp \
       src/ContextoOpenGLLinux.cpp \
       src/VentanaX11.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = motor

# Enlaces a librerías nativas de OpenGL, X11 y Hilos
LIBS = -lGL -lX11 -lpthread

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	@echo "¡Compilación del motor exitosa, Andrés!"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Entorno limpiado correctamente."

.PHONY: all clean
