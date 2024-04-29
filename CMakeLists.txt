cmake_minimum_required(VERSION 3.27)
project(strom)

set (CMAKE_C_STANDARD 90)

# Select the default backends if none were specified
if(NOT DEFINED ${STROM_BACKENDS})
	set(STROM_BACKENDS file string)
	message("No backends specified, using defaults")
endif(NOT DEFINED ${STROM_BACKENDS})

# The generic stream backend is required for all modules.
set(SRC_FILES "lib/xp/stream.c")
set(H_FILES "include/strom/stream.h")
set(TEST_FILES "${TEST_FILES}"
	test/stream/dummyStream.cc
	test/stream/test_readLine.cc
	test/stream/test_getch.cc
	test/stream/test_read.cc
	test/stream/test_readUntil.cc
	test/stream/test_readWord.cc
	test/stream/test_ungetch.cc)

# UNIX specific backends
if (UNIX)

	if (file IN_LIST STROM_BACKENDS)
		message("[UNIX] File backend added")
		set(SRC_FILES "${SRC_FILES}" lib/unix/file.c)
		set(H_FILES "include/strom/file.h")
		set(TEST_FILES "${TEST_FILES}" test/file/test_file.cc)
	endif (file IN_LIST STROM_BACKENDS)

endif(UNIX)

# Cross platform backends

if (string IN_LIST STROM_BACKENDS)
	message("[Cross platform] String backend added")
	set(SRC_FILES "${SRC_FILES}" lib/xp/string.c)
	set(H_FILES "include/strom/string.h")
	set(TEST_FILES "${TEST_FILES}" test/string/test_string.cc)
endif (string IN_LIST STROM_BACKENDS)

include_directories(include/)
add_library(strom ${SRC_FILES} ${H_FILES})

# Testing stuff here

if (${STROM_TESTS})

message("Building libstrom with tests")

find_package(Catch2 3 REQUIRED)

add_executable(strom_tests ${TEST_FILES})
target_compile_definitions(strom_tests PUBLIC S_BUFFERSIZE=5)
target_compile_definitions(strom_tests PUBLIC ROOT="${CMAKE_SOURCE_DIR}")
target_link_libraries(strom_tests PRIVATE Catch2::Catch2WithMain strom)

include(Catch)
catch_discover_tests(strom_tests)

endif(${STROM_TESTS})
