# ######################################################################### #
#                                                                           #
# Copyright (C) 2015 Jiří Fatka <ntsfka@gmail.com>                          #
#                                                                           #
# This program is free software: you can redistribute it and/or modify      #
# it under the terms of the GNU Lesser General Public License as published  #
# by the Free Software Foundation, either version 3 of the License, or      #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU Lesser General Public License  #
# along with this program. If not, see <http://www.gnu.org/licenses/>.      #
#                                                                           #
# ######################################################################### #

# Required version
cmake_minimum_required(VERSION 3.0)

# ######################################################################### #

# Template Regex Library
project(template_regex)

# ######################################################################### #

if (NOT DEFINED BUILD_TESTING)
    option(BUILD_TESTING "Enables library testing" Off)
endif ()

# ######################################################################### #

# Header only library
add_library(${PROJECT_NAME} INTERFACE)

# Compiler options
target_compile_options(${PROJECT_NAME}
    INTERFACE -std=c++14
)

# Subprojects
add_subdirectory(perf)

# ######################################################################### #

if (BUILD_TESTING)
    # Enable testing
    enable_testing()

    # Gtest from parent's project

    # Try to find GTest in OS
    if (NOT GTEST_FOUND)
        find_package(GTest)
    endif ()

    # Download from GitHub
    if (NOT GTEST_FOUND)
        include(ExternalProject)
        ExternalProject_Add(GTest
            GIT_REPOSITORY "https://github.com/google/googletest.git"
            GIT_TAG "release 1.7.0"
        )
    endif ()

    # Test name
    set(PROJECT_TEST_NAME "${PROJECT_NAME}_test")

    # Create test
    add_test(${PROJECT_TEST_NAME}
        ${PROJECT_TEST_NAME}
    )

    # Files
    add_executable(${PROJECT_TEST_NAME}
        test/string_test.cpp
        test/rules_test.cpp
        test/regex_test.cpp
    )

    target_compile_options(${PROJECT_TEST_NAME}
        PRIVATE -std=c++14
    )

    # GTest
    target_include_directories(${PROJECT_TEST_NAME}
        PRIVATE ${GTEST_INCLUDE_DIRS}
    )

    # Libraries
    target_link_libraries(${PROJECT_TEST_NAME}
        ${GTEST_BOTH_LIBRARIES}
    )

    if (UNIX AND NOT APPLE)
        target_link_libraries(${PROJECT_TEST_NAME}
            -pthread
        )
    endif ()

endif ()

# ########################################################################## #
