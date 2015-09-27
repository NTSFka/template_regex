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

# Performance measure project
project(perf)

# ######################################################################### #

# Options
option(USE_POSIX  "Use POSIX regex" Off)
option(USE_BOOST  "Use BOOST regex" Off)
option(USE_BOOSTX "Use BOOST xregex" Off)
option(USE_JAVA   "Use Java regex" Off)

# ######################################################################### #

# Compiler options
add_compile_options(-std=c++14)

# ######################################################################### #

# Create program
add_executable(perf_template_regex
    functions.hpp
    functions.cpp
    template_regex.cpp
)

# Create program
add_executable(perf_handwrittern
    functions.hpp
    functions.cpp
    handwritten.cpp
)

# Create program
add_executable(perf_std_regex
    functions.hpp
    functions.cpp
    std_regex.cpp
)

# ######################################################################### #

# POSIX Regex
if (USE_POSIX)
    add_executable(perf_posix
        functions.hpp
        functions.cpp
        posix.cpp
    )
endif ()

# Boost Regex
if (USE_BOOST)
    find_package(Boost REQUIRED COMPONENTS regex)
    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(perf_boost_regex
        functions.hpp
        functions.cpp
        boost_regex.cpp
    )

    target_link_libraries(perf_boost_regex
        ${Boost_LIBRARIES}
    )
endif ()

# Boost Xpression
if (USE_BOOSTX)
    find_package(Boost REQUIRED)
    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(perf_boost_xregex
        functions.hpp
        functions.cpp
        boost_xregex.cpp
    )
endif ()

# Java Regex
if (USE_JAVA)
    find_package(Java)
    include(UseJava)

    add_jar(perf_java_regex
        ENTRY_POINT JavaRegex
        JavaRegex.java
    )
endif ()

# ########################################################################## #
