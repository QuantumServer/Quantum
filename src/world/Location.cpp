#include "Location.h"
#include "World.h"

Location::Location(World* world, double x, double y, double z):world{world}, x{x}, y{y}, z{z} {

}

double Location::getX() const {
    return x;
}

double Location::getY() const {
    return y;
}

double Location::getZ() const {
    return z;
}

int Location::getBlockX() const {
    return floor(x);
}

int Location::getBlockY() const {
    return floor(y);
}

int Location::getBlockZ() const {
    return floor(z);
}

World* Location::getWorld() {
    return world;
}

Location& Location::operator*=(Location other) {
    x *= other.x;
    y *= other.y;
    z *= other.y;

    return *this;
}

Location& Location::operator+=(Location other) {
    x += other.x;
    y += other.y;
    z += other.y;

    return *this;
}

Location::~Location() {

}
