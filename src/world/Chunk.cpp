#include "world/Chunk.h"
#include <boost/assert.hpp>
#include "block/Air.h"
#include "block/Block.h"
#include "block/BlockRef.h"

namespace quantum {

    Chunk::Chunk() : blocks_({{BlockPos{0, 0, 0}, std::make_shared<Block::Air>()}}) {

    }

    Chunk::~Chunk() {

    }

    Block::BlockRef Chunk::getBlock(Chunk::BlockPos pos) {
        return {shared_from_this(), pos};
    }

    void Chunk::setBlock(Chunk::BlockPos pos, std::shared_ptr<Block::Block> block) {
        std::lock_guard<boost::fibers::mutex> lk(blocksMutex_);
        return setBlockUnlocked(pos, std::move(block));
    }

    std::shared_ptr<Block::Block> Chunk::getBlockUnlocked(Chunk::BlockPos pos) {
        return blocks_.upper_bound(pos)->second;
    }

    void Chunk::setBlockUnlocked(Chunk::BlockPos pos, std::shared_ptr<Block::Block> block) {
        auto after = blocks_.upper_bound(pos);
        BOOST_ASSERT(after != blocks_.begin()); // upper bound
        auto before = std::prev(after);
        auto cur = blocks_.end();
        if (before->first == pos) {
            cur = before;
            before == blocks_.begin() ? before = blocks_.end() : --before;
        }
        if (before != blocks_.end() && before->second->equals(*block)) {
            if(cur != blocks_.end()) {
                blocks_.erase(cur);
            }
        } else {
            if (cur == blocks_.end()) {
                blocks_.insert(after, {pos, block});
            }
        }
        if (after != blocks_.end() && after->second->equals(*block)) {
            blocks_.erase(after);
        }
    }

}
