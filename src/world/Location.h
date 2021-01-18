#pragma once

#include "World.h"

class Location {

public:
    Location(World* worldName, double x, double y, double z);
    ~Location();
    World* getWorld();
    double getX() const;
    double getY() const;
    double getZ() const;
    int getblockX() const;
    int getblockY() const;
    int getBlockZ() const;
    Location& operator+(Location other);
    Location& operator-(Location other);
    Location& operator*=(Location other);
    Location& operator+=(Location other);

private:
    World* world;
    double x;
    double y;
    double z;

}
