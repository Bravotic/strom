cmake_minimum_required(VERSION 3.5)
project(strom VERSION 0.1.0 DESCRIPTION "Cross platform Java-like input/output streams for C")

set (CMAKE_C_STANDARD 90)

# Select the default backends if none were specified
if(NOT DEFINED ${STROM_BACKENDS})
	set(STROM_BACKENDS file string)
	message("No backends specified, using defaults")
endif(NOT DEFINED ${STROM_BACKENDS})

# The generic stream backend is required for all modules.
set(SRC_FILES 	${CMAKE_CURRENT_SOURCE_DIR}/lib/xp/core.c
				${CMAKE_CURRENT_SOURCE_DIR}/lib/xp/input.c
				${CMAKE_CURRENT_SOURCE_DIR}/lib/xp/output.c)

set(H_FILES "include/strom/stream.h")
set(TEST_FILES "${TEST_FILES}"
	test/stream/dummyStream.cc
	test/stream/test_readLine.cc
	test/stream/test_getch.cc
	test/stream/test_read.cc
	test/stream/test_readUntil.cc
	test/stream/test_readWord.cc
	test/stream/test_ungetch.cc)

if (file IN_LIST STROM_BACKENDS)
	add_subdirectory(modules/file)
	set(TEST_FILES "${TEST_FILES}" test/file/test_file.cc)
endif (file IN_LIST STROM_BACKENDS)

if (string IN_LIST STROM_BACKENDS)
	add_subdirectory(modules/string)
	set(TEST_FILES "${TEST_FILES}" test/string/test_string.cc)
endif (string IN_LIST STROM_BACKENDS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(strom ${SRC_FILES} ${H_FILES})

target_include_directories(strom PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(strom PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(strom PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(strom PROPERTIES PUBLIC_HEADER "${H_FILES}")

if(UNIX)
	include(GNUInstallDirs)
	install(TARGETS ${PROJECT_NAME}
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
			PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/strom)
elseif(WIN32)
	install(TARGETS ${PROJECT_NAME}
			RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
			PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/strom)
endif()

# Testing stuff here

if (${STROM_TESTS})
	message("Building libstrom with tests")

	find_package(Catch2 3 REQUIRED)

	add_executable(strom_tests ${TEST_FILES})
	target_compile_definitions(strom_tests PUBLIC S_BUFFERSIZE=5)
	target_compile_definitions(strom_tests PUBLIC ROOT="${CMAKE_SOURCE_DIR}")
	target_link_libraries(strom_tests PRIVATE Catch2::Catch2WithMain strom strom_file strom_string)

	include(Catch)
	catch_discover_tests(strom_tests)
endif(${STROM_TESTS})
