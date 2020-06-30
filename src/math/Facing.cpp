#include <iostream>
#include <string>

class Facing {

    const int AXIS_Y = 0;
    const int AXIS_Z = 1;
    const int AXIS_X = 2;
    const int FLAG_AXIS_POSITIVE = 1;

    const ALL[6] = {
        self::DOWN,
        self::UP,
        self::NORTH,
        self::SOUTH,
        self::WEST,
        self::EAST
    };

    const HORIZONTAL[4] = {
        self::NORTH,
        self::SOUTH,
        self::WEST,
        self::EAST
    };

    const CLOCKWISE[3] = {
        self::AXIS_Y = {
            self::NORTH = self::EAST,
            self::EAST = self::SOUTH,
            self::SOUTH = self::WEST,
            self::WEST = self::NORTH
        },
        self::AXIS_Z {
            self::UP = self::EAST,
            self::EAST = self::DOWN,
            self::DOWN = self::WEST,
            self::WEST = self::UP
        },
        self::AXIS_X = {
            self::UP => self::NORTH,
            self::NORTH => self::DOWN,
            self::DOWN => self::SOUTH,
            self::SOUTH => self::UP
        }
    };

}
