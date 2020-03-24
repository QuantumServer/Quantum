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
#include "block/blockref.h"

namespace Cenisys {
	namespace Block {
		BlockRef::~BlockRef() {
			if(modified_)
				commit();
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
			if(current == original_) {
				chunk_->setBlockUnlocked(pos_, modified_);
				original_ = std::move(modified_);
				return true;
			} else {
				original_ = std::move(current);
				modified_.reset();
				return false;
			}
		}
		
		if(current == original_) {
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
/**
 * namespace Block
 *
 * namespace Cenisys
 */