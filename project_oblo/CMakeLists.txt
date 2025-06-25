cmake_minimum_required(VERSION 3.10)
project(ProjetMAX11627)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Inclure les headers dans /include
include_directories(include)

# Charger CURL
find_package(CURL REQUIRED)

# Créer la bibliothèque commune
file(GLOB SOURCES "src/*.cpp")
add_library(corelib ${SOURCES})
target_link_libraries(corelib PUBLIC CURL::libcurl)

# Exécutable principal
add_executable(prj_exec ${SOURCES})
target_link_libraries(prj_exec PRIVATE corelib)

# === Test ADC ===
add_executable(testADC tests/testADC.cpp)
target_link_libraries(testADC PRIVATE corelib)

# === Test AnalogMultiplexer ===
add_executable(testAnalogMultiplexer tests/testAnalogMultiplexer.cpp)
target_link_libraries(testAnalogMultiplexer PRIVATE corelib)

# === Test NTPClient ===
add_executable(testNTPClient tests/testNTPClient.cpp)
target_link_libraries(testNTPClient PRIVATE corelib)

# === Test ObloAPI ===
add_executable(testObloAPI tests/testObloAPI.cpp)
target_link_libraries(testObloAPI PRIVATE corelib)

# === Test Simulator ===
add_executable(testSimulator tests/testSimulator.cpp)
target_link_libraries(testSimulator PRIVATE corelib)

# === Test MainController ===
add_executable(testMainController tests/testMainController.cpp)
target_link_libraries(testMainController PRIVATE corelib)
