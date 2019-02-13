cmake_minimum_required(VERSION 3.1)
project(logi VERSION 1.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DASIO_STANDALONE)
find_package(Threads REQUIRED)

include_directories(lib src)

set(LIB
    lib/posix/error.hpp
    lib/posix/error.cpp
    lib/util/logging.hpp
    lib/util/logging.cpp
)
set(SRC src/main.cpp)
set(DOC LICENSE.md README.md)

########################
# executable
add_executable(${PROJECT_NAME} ${LIB} ${SRC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

# install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES ${DOC} DESTINATION share/doc/${PROJECT_NAME})

install(FILES udev/50-logi.rules DESTINATION /lib/udev/rules.d)

configure_file(systemd/logi@.service.in logi@.service @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/logi@.service DESTINATION /lib/systemd/system)

########################
# package
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_STRIP_FILES ON)
set(CPACK_PACKAGE_FILE_NAME
    ${PROJECT_NAME}_${PROJECT_VERSION}_${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}
)

# source
set(CPACK_SOURCE_GENERATOR TBZ2)
set(CPACK_SOURCE_IGNORE_FILES /\.git.*;/build/;\.user$)

# binary
set(CPACK_GENERATOR DEB)
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Logitech R800 remote control")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dimitry Ishenko <dimitry (dot) ishenko (at) (gee) mail (dot) com>")
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libasio-dev (>= 1.10.10)")

include(CPack)
