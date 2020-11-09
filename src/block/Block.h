#ifndef CENISYS_BLOCK_BLOCK_H
#define CENISYS_BLOCK_BLOCK_H

#include <memory>

namespace quantum {
    namespace block {

        class Block {
            public:
                Block();
                virtual ~Block();
                virtual std::shared_ptr<Block> clone() = 0;
                virtual bool equals(const Block &other);
        };

        template <typename T>
        class BlockBase : virtual public Block {
            std::shared_ptr<Block> clone() {
                return std::make_shared<T>(*static_cast<T *>(this));
            }
        };

	}
}

#endif
