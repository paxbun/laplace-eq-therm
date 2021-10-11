# Copyright (c) 2021 Chanjung Kim. All rights reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.13)

project(
    laplace-eq-therm-server-core
    VERSION 0.1.0
    DESCRIPTION ""
    HOMEPAGE_URL "https://github.com/paxbun/laplace-eq-therm"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_library(
    laplace-eq-therm-server-core
    ${CMAKE_SOURCE_DIR}/MockSpace.cc
    ${CMAKE_SOURCE_DIR}/Server.cc
)