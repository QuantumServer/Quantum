/*
 * BlockRef
 * Copyright (C) 2016 iTX Technologies
 *
 * This file is part of Cenisys.
 *
 * Cenisys is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Cenisys is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Cenisys.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CENISYS_BLOCK_BLOCK_H
#define CENISYS_BLOCK_BLOCK_H

#include <memory>

namespace Cenisys {

    namespace Block {

        class Block {
            public:
                Block();
                virtual ~Block();
                virtual std::shared_ptr<Block> clone() = 0;
                virtual bool equals(const Block &other);
        };

        // CRTP
        template <typename T>
        class BlockBase : virtual public Block {
            std::shared_ptr<Block> clone() {
                return std::make_shared<T>(*static_cast<T *>(this));
            }
        };

	} // namespace Block

} // namespace Cenisys

#endif // CENISYS_BLOCK_BLOCK_H
