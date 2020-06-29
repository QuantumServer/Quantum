#include <iostream>

using namespace std;

#if defined(_WIN32) || (_WIN64)
#define PLATFORM_NAME "windows"
#elif defined (__linux__)
#define PLATFORM_NAME "linux"
#elif defined (__APPLE__)
#define PLATFORM_NAME "osx"
#endif

const char getOS() {
    return PLATFORM_NAME;
}

string getPrivateIpAddress() {
    
}

string getPublicIpAddress() {
    
}
