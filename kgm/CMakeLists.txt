cmake_minimum_required(VERSION 3.10)

project(kgm)
set(CMAKE_CXX_STANDARD 23)


set(SRC_LIST
    src/decryptor.cpp
    src/decryptor.h
    src/kgm.h
    src/kgm.cpp
    src/main.cpp
    src/md5.cpp
    src/md5.h
    src/util.cpp
    src/util.h
)

add_executable(${PROJECT_NAME}  ${SRC_LIST})