option(BUILD_TEST "Build and install tests" OFF)
if(BUILD_TEST)
    find_package(Boost 1.62
        COMPONENTS
        unit_test_framework
        REQUIRED
        )
    include_directories("${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}" "${PROJECT_SOURCE_DIR}/src")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    add_executable(cenisys-test
        main.cpp
        config.cpp
        )
    target_link_libraries(cenisys-test
        CenisysCore
        Boost::boost
        Boost::unit_test_framework
        )
    if(NOT Boost_USE_STATIC_LIBS)
        target_compile_definitions(cenisys-test PRIVATE BOOST_TEST_DYN_LINK)
    endif()
    # Overrides for boost
    include_directories("${PROJECT_SOURCE_DIR}")
    set_property(TARGET cenisys-test PROPERTY CXX_STANDARD 14)
    set_property(TARGET cenisys-test PROPERTY CXX_STANDARD_REQUIRED YES)
    install(TARGETS cenisys-test
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        )
endif()
