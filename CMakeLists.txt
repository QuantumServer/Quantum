cmake_minimum_required(VERSION 3.1)
project(cenisys VERSION 0.1)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(GNUInstallDirs)
# Configuration
set(CONFIG_DIR "${CMAKE_INSTALL_FULL_SYSCONFDIR}")
# Gettext
set(GETTEXT_PACKAGE "${PROJECT_NAME}")
set(PACKAGE_LOCALE_DIR "${CMAKE_INSTALL_FULL_LOCALEDIR}")

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(doc)
add_subdirectory(po)
add_subdirectory(test)
