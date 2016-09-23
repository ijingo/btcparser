BUILD_DIR = build
INCLUDE_DIRS = -I ./include -I /usr/local/include

CXX = g++ $(opts)
CPPFLAGS = -O2 -g -Wall -std=c++11 -Wno-unused-result $(INCLUDE_DIRS)

.DEFAULT_GOAL = all
.PHONY: clean

all: clean
	mkdir $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) -c ./src/base58.cc -o $(BUILD_DIR)/base58.o
	$(CXX) $(CPPFLAGS) -c ./src/CRC32.cc -o $(BUILD_DIR)/CRC32.o
	$(CXX) $(CPPFLAGS) -c ./src/address.cc -o $(BUILD_DIR)/address.o
	$(CXX) $(CPPFLAGS) -c ./src/RIPEMD160.cc -o $(BUILD_DIR)/RIPEMD160.o
	$(CXX) $(CPPFLAGS) -c ./src/SHA256.cc -o $(BUILD_DIR)/SHA256.o
	$(CXX) $(CPPFLAGS) -c ./src/RIPEMD160.cc -o $(BUILD_DIR)/RIPEMD160.o
	$(CXX) $(CPPFLAGS) -c ./src/blockchain.cc -o $(BUILD_DIR)/blockchain.o
	$(CXX) $(CPPFLAGS) -c ./src/logging.cc -o $(BUILD_DIR)/logging.o
	mkdir -p $(BUILD_DIR)/bin
	$(CXX) $(CPPFLAGS) ./src/main.cc build/*.o -o $(BUILD_DIR)/bin/btcparser
	rm $(BUILD_DIR)/*.o

clean:
	rm -rf $(BUILD_DIR)
