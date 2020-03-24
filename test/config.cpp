/*
 * Unit test for the config parser
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

#include <boost/test/unit_test.hpp>
#include "config/configparser.h"

CENISYS_DEFINE_CONFIG_PARSER((ConfigTest), Section, (std::string, string))
using SectionTable = std::unordered_map<std::string, ConfigTest::Section>;
using NumberArray = std::vector<double>;
CENISYS_DEFINE_CONFIG_PARSER((ConfigTest), Main, (int, defaultValue, 0xdead), (int, defaultZero), (NumberArray, floats), (int, integer), (SectionTable, sections))

BOOST_AUTO_TEST_SUITE(config)

BOOST_AUTO_TEST_CASE(parse) {
	std::stringstream ifs("integer = 4\n", "floats = [1.0,2.3,5.5]\n", "[sections.a]\n", "string = \"str\"\n", "");
	toml::ParseResult pr = toml::parse(ifs);
	if(!pr.valid()) {
		throw std::runtime_error(pr.errorReason);
	}
	const toml::Value &v = pr.value;
	
	
	
	
	
	
    const toml::Value &v = pr.value;
    ConfigTest::Main conf;
    Cenisys::ConfigParser<ConfigTest::Main>()(v, conf);
    BOOST_TEST(conf.defaultValue == 0xdead);
    BOOST_TEST(conf.defaultZero == 0);
    BOOST_TEST(conf.integer == 4);
    BOOST_TEST_REQUIRE(conf.floats.size() == 3);
    BOOST_TEST(conf.floats[0] == 1.0);
    BOOST_TEST(conf.floats[1] == 2.3);
    BOOST_TEST(conf.floats[2] == 5.5);
    BOOST_TEST(conf.sections["a"].string == "str");
}

BOOST_AUTO_TEST_SUITE_END()
