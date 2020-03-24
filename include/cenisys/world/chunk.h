/*
 * Chunk
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
#ifndef CENISYS_CHUNK_H
#define CENISYS_CHUNK_H

#include <map>
#include <memory>
#include <boost/fiber/mutex.hpp>

namespace Cenisys {
	namespace Block {
		class Block;
		class BlockRef;
	}
	
	class Chunk : public std::enable_shared_from_this<Chunk> {
		public:
			struct BlockPos {
				unsigned char y : 8;
				unsigned char x : 4;
				unsigned char z : 4;
				friend bool operator==(BlockPos lhs, BlockPos rhs) {
					return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
				}
				friend bool operator!=(BlockPos lhs, BlockPos rhs) {
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
} // namespace Cenisys

#endif // CENISYS_CHUNK_H