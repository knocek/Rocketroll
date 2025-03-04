cmake_minimum_required(VERSION 3.10)
project(Rocketroll)

cmake_policy(SET CMP0074 NEW)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the SFML root directory
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/extern/SFML-2.4.0")
set(CMAKE_MODULE_PATH ${SFML_ROOT}/cmake/Modules/)

# Find SFML
# include()
find_package(SFML 2.4.0 COMPONENTS system window graphics audio REQUIRED)

# Add the executable
file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp ${HEADERS})
add_executable(${PROJECT_NAME} 
    # ${SOURCES}
    src/main.cpp
    src/SoundManager.cpp
    src/Menu.cpp
    src/Game.cpp
    src/OptionsMenu.cpp
    src/Rocket.cpp
    src/Droneship.cpp
)

# Link SFML libraries 
target_include_directories(${PROJECT_NAME} 
    PUBLIC ${SFML_INCLUDE_DIR}
    PUBLIC include
    )

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})


