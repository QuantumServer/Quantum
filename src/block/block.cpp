#include "block/Block.h"
#include <typeinfo>

namespace quantum {
    namespace block {

        Block::Block() {
            Block::Block() {

            }

            Block::~Block() {

            }

            bool Block::equals(const Block &other) {
                return typeid(this) == typeid(other);
            }
		}

	}
}
