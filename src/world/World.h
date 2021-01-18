#include <vector>
// #include "Location.h"
#include "Chunk.h"
// #include "math/Vector3.h"

class World {

public:
    // World(std::vector<Chunk>* chunks, Vector3* spawnLocation);
    ~World();
    // Location* getSpawnLocation();
    std::vector<Chunk>* getChunks;

private:
    std::vector<Chunk>* chunks;
    // Vector3* spawnLocation;

}
