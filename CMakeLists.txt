cmake_minimum_required(VERSION 3.1)
project(logi VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DASIO_STANDALONE)
find_package(Threads REQUIRED)

include_directories(lib)
include_directories(src)

set(LIB
    lib/posix/error.hpp
    lib/posix/error.cpp
)
set(MAIN src/main.cpp)
set(FILES LICENSE.md README.md)

########################
# executable
add_executable(${PROJECT_NAME} ${LIB} ${MAIN})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

# install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES ${FILES} DESTINATION share/doc/${PROJECT_NAME})

########################
# package
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_STRIP_FILES ON)
set(CPACK_PACKAGE_FILE_NAME
    ${PROJECT_NAME}_${PROJECT_VERSION}_${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}
)

# binary
#set(CPACK_GENERATOR DEB)
#set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "...")
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dimitry Ishenko <dimitry (dot) ishenko (at) (gee) mail (dot) com>")
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "...")

#include(CPack)
