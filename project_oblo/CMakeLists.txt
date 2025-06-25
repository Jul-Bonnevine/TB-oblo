cmake_minimum_required(VERSION 3.10)
project(ProjetMAX11627)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Inclure les headers dans /include
include_directories(include)

# Créer la bibliothèque commune
file(GLOB SOURCES "src/*.cpp")
add_library(corelib ${SOURCES})

# Exécutable principal
add_executable(prj_exec ${SOURCES})
find_package(CURL REQUIRED)
target_link_libraries(prj_exec PRIVATE CURL::libcurl)

# === Test ADC ===
add_executable(testADC tests/testADC.cpp)
target_link_libraries(testADC PRIVATE corelib)
