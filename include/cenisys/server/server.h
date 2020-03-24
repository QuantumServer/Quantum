/*
 * Server
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
#ifndef CENISYS_SERVER_H
#define CENISYS_SERVER_H

#include <utility>
#include "config/configparser.h"

namespace Cenisys {
	class Server {
		public:
			struct Config;
			Server(Config &config);
			
		private:
			std::reference_wrapper<Config> config_;
	};
	
} // namespace Cenisys

CENISYS_DEFINE_CONFIG_PARSER((Cenisys), Server::Config, (int, max_players, 64))

#endif // CENISYS_SERVER_H