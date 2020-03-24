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
#include "world/chunk.h"
#include <boost/assert.hpp>
#include "block/air.h"
#include "block/block.h"
#include "block/blockref.h"

namespace Cenisys {
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
		if(before->first == pos) {
			cur = before;
			before == blocks_.begin() ? before = blocks_.end() : --before;
		}
		if(before != blocks_.end() && before->second->equals(*block)) {
			if(cur != blocks_.end())
				blocks_.erase(cur);
		} else {
			if(cur == blocks_.end())
				blocks_.insert(after, {pos, block});
		}
		if(after != blocks_.end() && after->second->equals(*block)) {
			blocks_.erase(after);
		}
	}
} // namespace Cenisys