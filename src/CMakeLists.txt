file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${EXLANG_SOURCE_DIR}/include/exlang/*hpp")

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(exlang
        cmd_analog.cpp
        cmd_digital.cpp
        cmd_interrupts.cpp
        cmd_log.cpp
	cmd_logic.cpp
	cmd_math.cpp
	cmd_navigate.cpp
	cmd_serial.cpp
	cmd_system.cpp
	cmd_time.cpp
	cmd_variable.cpp
	command.cpp
	constants.cpp
	dev_disk.cpp
	dev_network.cpp
	eeprom_access.cpp
	error.cpp
	helpers.cpp
	interpreter.cpp
	main.cpp
	memory.cpp
	program.cpp
	statement.cpp
        ${HEADER_LIST})

target_include_directories(exlang PUBLIC ../include)
target_link_libraries(exlang m)

target_compile_features(exlang PUBLIC cxx_std_17)
set(CMAKE_CXX_CLANG_TIDY clang-tidy "clang-tidy;-checks=*")
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
