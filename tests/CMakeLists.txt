file(GLOB TEST_CASES_FILE *.ss)

set(LINK_LIBRARY_PATH ${PROJECT_BINARY_DIR}/lib)

foreach(filepath ${TEST_CASES_FILE})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" filename ${filepath})
    add_test(
            NAME test_${filename}
            COMMAND staticscript ${filepath} -L ${LINK_LIBRARY_PATH}
    )
endforeach()