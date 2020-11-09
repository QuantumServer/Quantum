find_package(Boost 1.62
    COMPONENTS filesystem
    iostreams
    fiber
    locale
    program_options
    system
    REQUIRED
)
find_package(Threads REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
configure_file(config.h.in config.h)
add_library(QuantumCore SHARED
    block/Block.cpp
    block/BlockRef.cpp
    block/Air.cpp
    world/Chunk.cpp
    server/ServerManager.cpp
    Server.cpp
)
target_link_libraries(QuantumCore PRIVATE
    Threads::Threads
    Boost::boost
    Boost::fiber
    Boost::filesystem
    Boost::iostreams
    Boost::locale
    Boost::system
)
add_executable(quantum main.cpp)
target_link_libraries(quantum
    QuantumCore
    Threads::Threads
    Boost::boost
    Boost::fiber
    Boost::filesystem
    Boost::locale
    Boost::program_options
    Boost::system
)
# Overrides for boost
include_directories("${PROJECT_SOURCE_DIR}")
set_property(TARGET quantum QuantumCore PROPERTY CXX_STANDARD 14)
set_property(TARGET quantum QuantumCore PROPERTY CXX_STANDARD_REQUIRED YES)
install(TARGETS quantum QuantumCore
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
