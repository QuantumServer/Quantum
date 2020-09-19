cmake_minimum_required(VERSION 3.1)

project(RakNet)

file(GLOB_RECURSE RAKNET_SOURCES "*.cpp")

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_library(raknet "${RAKNET_SOURCES}")

IF (WIN32 AND NOT UNIX)
    target_link_libraries(raknet ws2_32)
ENDIF (WIN32 AND NOT UNIX)
    target_include_directories(raknet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

install(TARGETS raknet RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/bin"
			LIBRARY DESTINATION "${CMAKE_SOURCE_DIR}/lib"
			ARCHIVE DESTINATION "${CMAKE_SOURCE_DIR}/lib")
