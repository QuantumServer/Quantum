#include <string>

#include "utils/Zlib.h"

#define BUFLEN 17000
Byte uncompr[BUFLEN];

std::string Zlib::Decompress(unsigned char const& data) {

    uLongf length = BUFLEN;
    uncompress(uncompr, &length, (Bytef*) data, sizeof(data));
    return std::string((char*) uncompr, length);

}
