# Author: Wanjare Samuel
# Date: Tuesday 12th November 2024
# LICENSE: MIT
# Description: A simple media player
# Special Thanks To: C++, wxWidgets-3.2.6, libvlcpp and I

cmake_minimum_required(VERSION 3.30.3)
project(WanjPlayer VERSION 0.1.0 LANGUAGES CXX)

# Project metadata
set(PROJECT_AUTHOR "Wanjare Samuel")
set(PROJECT_EMAIL "samuelwanjare@protonmail.com")
set(PROJECT_DESCRIPTION "A C++ music player using wxWidgets and libvlcpp")

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project directories
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set(SOURCE_DIR ${PROJECT_ROOT}/src)
set(INCLUDE_DIR ${PROJECT_ROOT}/include)
set(LIBS_DIR ${PROJECT_ROOT}/libs)
set(TEST_DIR  ${PROJECT_ROOT}/tests)
set(ASSETS_DIR ${PROJECT_ROOT}/assets)
set(BIN_DIR ${PROJECT_ROOT}/build)

include_directories(${INCLUDE_DIR})
include_directories(${LIBS_DIR}/libvlcpp)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS base core media aui net adv)

include(${wxWidgets_USE_FILE})

# Add executable
add_executable(WanjPlayer 
${SOURCE_DIR}/wanjplayer.cpp
${SOURCE_DIR}/menubar.cpp
${SOURCE_DIR}/statusbar.cpp
${SOURCE_DIR}/about.cpp
${SOURCE_DIR}/license_dialogbx.cpp
${SOURCE_DIR}/media_ctrls.cpp
${SOURCE_DIR}/media_events.cpp
${SOURCE_DIR}/playlist.cpp
)

# Link libraries
target_link_libraries(WanjPlayer ${wxWidgets_LIBRARIES})


# target_include_directories(WanjPlayerTests PRIVATE ${LIBS_DIR}/wxWidgets/include)

# set output directory for the binary
set_target_properties(WanjPlayer PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

# COPY Assets dir to binary dir
file(COPY assets DESTINATION ${BIN_DIR})

# Add Tests

# enable_testing()

# Add test executable
# add_executable(WanjPlayerTests
    # $(TESTS_DIR)/test_main.cpp
#)
# Link test libraries #target_link_libraries(WanjPlayerTests ${wxWidgets_LIBRARIES})
#target_link_libraries(WanjPlayerTests ${LIBS_DIR}/wxWidgets/lib/libwx_baseu-3.1.so)  # Adjust path if necessary
#target_include_directories(WanjPlayerTests PRIVATE ${LIBS_DIR}/wxWidgets/include)

## Add test
#add_test(NAME WanjPlayerTests COMMAND WanjPlayerTests)


# Project metadata variables
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")
add_definitions(-DPROJECT_AUTHOR="${PROJECT_AUTHOR}")
add_definitions(-DPROJECT_EMAIL="${PROJECT_EMAIL}")
add_definitions(-DPROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}")
