cmake_minimum_required(VERSION 3.5.0)
project(exlang
        VERSION 0.1.0
        LANGUAGES CXX)

find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${EXLANG_SOURCE_DIR}/src/*.[ch]pp
        ${EXLANG_SOURCE_DIR}/include/exlang/*.[h]pp
    )

    add_custom_target(
        lint
        COMMAND clang-tidy
        ${ALL_CXX_SOURCE_FILES}
        -format-style=file
        --
        -std=gnu++17
        -I ./include
        -I /usr/include/c++/11
        -I /usr/include/x86_64-linux-gnu/c++/11
        -I{EXLANG_ROOT_DIR}/include
    )
endif()


find_program(PLATFORMIO "platformio")
if (PLATFORMIO)
  add_custom_target(
    mega
    WORKING_DIRECTORY ${EXLANG_SOURCE_DIR}
    COMMAND platformio run -t upload -e mega
  )
  add_custom_target(
    esp32
    WORKING_DIRECTORY ${EXLANG_SOURCE_DIR}
    COMMAND platformio run -t upload -e esp32
  )
  add_custom_target(
    clean-embedded
    WORKING_DIRECTORY ${EXLANG_SOURCE_DIR}
    COMMAND platformio run --target clean
  )
  add_custom_target(
    build-only
    WORKING_DIRECTORY ${EXLANG_SOURCE_DIR}
    COMMAND platformio run
  )
endif()

add_custom_target(clean-all
	COMMAND git clean -fXd
)

add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
