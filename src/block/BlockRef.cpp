#include "block/BlockRef.h"

namespace quantum {
    namespace block {

        BlockRef::~BlockRef() {
            if (modified_) {
                commit();
            }
        }

        BlockRef::BlockRef(std::shared_ptr<Chunk> chunk, Chunk::BlockPos pos)

        chunk_(std::move(chunk)), pos_(pos) {
            std::lock_guard<boost::fibers::mutex> lk(chunk_->blocksMutex_);
            original_ = chunk_->getBlockUnlocked(pos_);
        }

        bool BlockRef::commit() {
            std::lock_guard<boost::fibers::mutex> lk(chunk_->blocksMutex_);
            return commitUnlocked();
        }

        bool BlockRef::commitUnlocked() {
            std::shared_ptr<Block> current = chunk_->getBlockUnlocked(pos_);
            if (current == original_) {
                chunk_->setBlockUnlocked(pos_, modified_);
                original_ = std::move(modified_);
                return true;
            } else {
                original_ = std::move(current);
                modified_.reset();
                return false;
            }
        }

        if (current == original_) {
            chunk_->setBlockUnlocked(pos_, modified_);
            original_ = std::move(modified_);
            return true;
        } else {
            original_ = std::move(current);
            modified_.reset();
            return false;
        }

	}
}
