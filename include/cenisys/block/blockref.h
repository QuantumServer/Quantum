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
#ifndef CENISYS_BLOCK_BLOCKREF_H
#define CENISYS_BLOCK_BLOCKREF_H

#include <memory>
#include "block/block.h"
#include "world/chunk.h"

namespace Cenisys {
	class Chunk;
	namespace Block {
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
	} // namespace Block
} // namespace Cenisys

#endif // CENISYS_BLOCK_BLOCKREF_H