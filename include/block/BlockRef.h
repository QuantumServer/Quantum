#ifndef CENISYS_BLOCK_BLOCKREF_H
#define CENISYS_BLOCK_BLOCKREF_H

#include <memory>
#include "block/block.h"
#include "world/chunk.h"

namespace quantum {
    class Chunk;
    namespace block {
        class BlockRef {

            public:
                ~BlockRef();
                BlockRef(const BlockRef &) = delete;
                BlockRef(BlockRef &&) = default;
                const Block &block() const {
                    return modified_ ? *modified_ : *original_;
                }
                Block &block() {
                    return modified_ ? *modified_ : *(modified_ = original_->clone());
                }

            private:
                friend class ::Cenisys::Chunk;
                BlockRef(std::shared_ptr<Chunk> chunk, Chunk::BlockPos pos);

                bool commit();
                bool commitUnlocked();

                std::shared_ptr<Chunk> chunk_;
                Chunk::BlockPos pos_;

                std::shared_ptr<Block> original_;
                std::shared_ptr<Block> modified_;

		};
	}
}

#endif
