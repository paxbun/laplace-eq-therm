# Copyright (c) 2021 Chanjung Kim. All rights reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.12)

project(
    laplace-eq-therm-server-core
    VERSION 0.1.0
    DESCRIPTION ""
    HOMEPAGE_URL "https://github.com/paxbun/laplace-eq-therm"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
    laplace-eq-therm-server-core
    ${CMAKE_SOURCE_DIR}/Source/Config.cc
    ${CMAKE_SOURCE_DIR}/Source/Server.cc

    # Spaces
    ${CMAKE_SOURCE_DIR}/Source/FiniteElementMethodSpace.cc
    ${CMAKE_SOURCE_DIR}/Source/MatrixSpace.cc
    ${CMAKE_SOURCE_DIR}/Source/MonteCarloSpace.cc
    ${CMAKE_SOURCE_DIR}/Source/SuccessiveOverRelaxationSpace.cc
)

target_include_directories(
    laplace-eq-therm-server-core
    PUBLIC ${CMAKE_SOURCE_DIR}/Public
)

install(
    TARGETS laplace-eq-therm-server-core
    CONFIGURATIONS Debug Release
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION lib
)

option(ENABLE_LAPLACE_EQ_THERM_SERVER_CORE_TEST "Enable unit tests" OFF)
if (ENABLE_LAPLACE_EQ_THERM_SERVER_CORE_TEST)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    function(add_laplace_eq_therm_server_core_test TEST_NAME)
        set(FILE_NAME ${TEST_NAME})
        set(EXE_NAME ${TEST_NAME})
        
        string(REGEX REPLACE "([^A-Z\-])([A-Z][A-Z]+)([A-Z][a-z])" "\\1-\\2-\\3" EXE_NAME "${EXE_NAME}")
        string(REGEX REPLACE "([A-Z]+)$" "-\\1" EXE_NAME "${EXE_NAME}")
        string(REGEX REPLACE "([^-])([A-Z][a-z]+)" "\\1-\\2" EXE_NAME "${EXE_NAME}")
        string(TOLOWER "${EXE_NAME}" EXE_NAME)
        
        add_executable(${EXE_NAME} "${CMAKE_SOURCE_DIR}/Tests/${FILE_NAME}.cc")
        target_link_libraries(${EXE_NAME} GTest::gtest_main laplace-eq-therm-server-core)
        add_test(NAME ${TEST_NAME} COMMAND ${EXE_NAME})
        
        unset(FILE_NAME)
        unset(EXE_NAME)
        unset(TEST_NAME)
    endfunction()

    add_laplace_eq_therm_server_core_test(FooTest)
endif()