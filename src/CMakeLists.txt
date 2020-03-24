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
include_directories("${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
configure_file(config.h.in config.h)
add_library(CenisysCore SHARED
    block/block.cpp
    block/blockref.cpp
    block/air.cpp
    world/chunk.cpp
    server/servermanager.cpp
    server/server.cpp
    )
target_link_libraries(CenisysCore PRIVATE
    Threads::Threads
    Boost::boost
    Boost::fiber
    Boost::filesystem
    Boost::iostreams
    Boost::locale
    Boost::system
    )
add_executable(cenisys main.cpp)
target_link_libraries(cenisys
    CenisysCore
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
set_property(TARGET cenisys CenisysCore PROPERTY CXX_STANDARD 14)
set_property(TARGET cenisys CenisysCore PROPERTY CXX_STANDARD_REQUIRED YES)
install(TARGETS cenisys CenisysCore
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    )
