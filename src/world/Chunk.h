#ifndef CENISYS_CHUNK_H
#define CENISYS_CHUNK_H

#include <map>
#include <memory>
#include <boost/fiber/mutex.hpp>

namespace quantum {

    namespace block {
        class Block;
        class BlockRef;
    }

    class Chunk : public std::enable_shared_from_this<Chunk> {
        public:
            struct BlockPos {
                unsigned char y : 8;
                unsigned char x : 4;
                unsigned char z : 4;
                friend bool operator == (BlockPos lhs, BlockPos rhs) {
                    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
                }
                friend bool operator != (BlockPos lhs, BlockPos rhs) {
                    return !(lhs == rhs);
                }
                friend bool operator<(BlockPos lhs, BlockPos rhs) {
                    return lhs.y == rhs.y ? (lhs.x == rhs.x ? lhs.z < rhs.z : lhs.x < rhs.x) : lhs.y < rhs.y;
                }
            };

            Chunk();
            ~Chunk();

            Block::BlockRef getBlock(BlockPos pos);
            void setBlock(BlockPos pos, std::shared_ptr<Block::Block> block);

        private:
            std::shared_ptr<Block::Block> getBlockUnlocked(BlockPos pos);
            void setBlockUnlocked(BlockPos pos, std::shared_ptr<Block::Block> block);

            friend class Block::BlockRef;

            std::map<BlockPos, std::shared_ptr<Block::Block>> blocks_;
            boost::fibers::mutex blocksMutex_;
	};

}

#endif
