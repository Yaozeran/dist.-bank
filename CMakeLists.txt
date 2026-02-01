# Copyright (c) 2026 Yao Zeran, Zhang Chenzhi, Zhang Senyao

cmake_minimum_required(VERSION 3.20)

project(distbank)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/Users/yaozeran/CodeBase/qt/6.10.2/macos")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()


qt_add_executable(main
  ./src/server/main.cc
  ./src/server/include.h

  ./src/server/qt/mainwindow.cc
  ./src/server/qt/rpcpanel.cc
  ./src/server/qt/datapanel.cc
  
  ./src/server/server/controller.cc
  ./src/server/server/server.cc
)
target_link_libraries(main
    PRIVATE Qt6::Widgets
)