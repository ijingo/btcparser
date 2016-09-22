BUILD_DIR = build
INCLUDE_DIRS = -I ./include -I /usr/local/include

CXX = g++ $(opts)
CPPFLAGS = -O2 -g -Wall -std=c++11 $(INCLUDE_DIRS)


.DEFAULT_GOAL = all
.PHONY: clean

all:
	mkdir $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) -c ./src/base58.cc -o $(BUILD_DIR)/base58.o
	$(CXX) $(CPPFLAGS) -c ./src/CRC32.cc -o $(BUILD_DIR)/CRC32.o
	$(CXX) $(CPPFLAGS) -c ./src/RIPEMD160.cc -o $(BUILD_DIR)/RIPEMD160.o
	$(CXX) $(CPPFLAGS) -c ./src/SHA256.cc -o $(BUILD_DIR)/SHA256.o
	$(CXX) $(CPPFLAGS) -c ./src/RIPEMD160.cc -o $(BUILD_DIR)/RIPEMD160.o

clean:
	rm -rf $(BUILD_DIR)
