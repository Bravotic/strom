cmake_minimum_required(VERSION 3.5)
project(strom_file VERSION 0.1.0)

set (CMAKE_C_STANDARD 90)

set(H_FILE include/strom/file.h)

if (UNIX)
    set(SRC_FILES lib/unix/file.c)
endif()

include_directories(include/)
add_library(${PROJECT_NAME} ${SRC_FILES} ${H_FILE})

target_link_libraries(${PROJECT_NAME} PRIVATE strom)
target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${H_FILE})

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