option(BUILD_DOC "Build the HTML documentation" ON)
find_package(Sphinx)
find_package(Doxygen)
find_package(Breathe)
if(BUILD_DOC)
    if(SPHINX_FOUND AND DOXYGEN_FOUND AND BREATHE_FOUND)
        set(DOXYXML_DIR "${CMAKE_CURRENT_BINARY_DIR}/xml")
        set(DOC_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/source")
        set(SPHINX_GENERATOR html)
        set(SPHINX_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${SPHINX_GENERATOR}")
        configure_file(Doxyfile.in Doxyfile @ONLY)
        configure_file(conf.py.in conf.py @ONLY)
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/"
            DESTINATION "${DOC_SOURCE_DIR}")
        include(ProcessorCount)
        ProcessorCount(N)
        if(NOT N EQUAL 0)
            set(SPHINX_PARALLEL_FLAGS -j${N})
        endif()
        add_custom_target(doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE}
            "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
            COMMAND ${BREATHE_APIDOC}
            -f -o "${DOC_SOURCE_DIR}"
            "${DOXYXML_DIR}"
            COMMAND ${SPHINX_EXECUTABLE}
            "${SPHINX_PARALLEL_FLAGS}"
            -b ${SPHINX_GENERATOR}
            -c "${CMAKE_CURRENT_BINARY_DIR}"
            "${DOC_SOURCE_DIR}"
            "${SPHINX_OUTPUT_DIR}"
            )
        install(DIRECTORY "${SPHINX_OUTPUT_DIR}"
            DESTINATION "${CMAKE_INSTALL_DOCDIR}"
            PATTERN "*/\.*" EXCLUDE)
    else()
        message(WARNING "Skipping documentation due to missing dependencies.")
    endif()
endif()
