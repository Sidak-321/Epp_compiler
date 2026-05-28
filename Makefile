CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include/
TARGET   = epp
SRC_DIR  = src
SRC      = $(wildcard $(SRC_DIR)/*.cpp)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean