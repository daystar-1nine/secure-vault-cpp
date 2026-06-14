# Makefile for Password Manager

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude

# Source files
SRCS = src/main.cpp \
       src/auth/AuthManager.cpp \
       src/encryption/Encryption.cpp \
       src/storage/StorageManager.cpp \
       src/vault/VaultManager.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable name
TARGET = password_manager.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /q /f src\*.o src\auth\*.o src\encryption\*.o src\storage\*.o src\vault\*.o $(TARGET) 2>nul || rm -f $(OBJS) $(TARGET)

.PHONY: all clean
