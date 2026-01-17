# Copyright (c) 2026 Yao Zeran, Zhang Chenzhi, Zhang Senyao

cmake_minimum_required(VERSION 4.1.1)

project(distbank)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} ./src/server/main.cc)
