cmake_minimum_required(VERSION 3.12)
project(walipkg)

find_package(PkgConfig)
pkg_check_modules(PackageKit REQUIRED IMPORTED_TARGET packagekit-glib2)
include_directories(${PackageKit_INCLUDE_DIRS})

add_subdirectory(src)

set(EXEC_SOURCE
        example/main.cpp)

add_executable(${PROJECT_NAME}_example ${EXEC_SOURCE})

target_link_libraries(${PROJECT_NAME}_example
        walipkg)