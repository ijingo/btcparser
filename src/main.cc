#include <cstdio>
#include "blockchain.h"

int main(int argc, const char** argv) {
  if (argc < 2) {
    printf("usage:  btcparser root-path-of-blockchain-data\n");
    return 0;
  }
  const char* data_path = argv[1];
  uint32_t max_blocks = 220020;
  BlockChain* blockchain = BlockChain::createBlockChain(data_path, max_blocks);
  if (blockchain) {
    printf("Scanning the Bitcoin blockchain raw data  for blocks.\n");
    while (true) {
      uint32_t last_block_read;
      bool done = blockchain->scanBlockChain(last_block_read);
      if (done) {
        break;
      } else {
        if ((last_block_read % 1000) == 0)
          printf("Scanned block header: %d\n", last_block_read);
      }
    }
    printf("Finished scanning the avaiable blocks.\n");
    printf("Building the active chain (leave out the orphaned blocks)\n");
    uint32_t ret = blockchain->buildBlockChain();
    printf("The active chain containing %d blocks.\n", ret);
    while (true) {
      int32_t idx;
      printf("Input a block number to read: ");
      scanf("%d", &idx);
      if (idx < 0) {
        blockchain->release();
        break;
      } if (static_cast<uint32_t>(idx) >= ret) {
        printf("invalid index, try again.\n");
      } else {
        const BlockChain::Block* block = blockchain->readBlock(idx);
        blockchain->printBlock(block);
      }
    }
  }
  return 0;
}
