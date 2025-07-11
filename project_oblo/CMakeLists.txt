cmake_minimum_required(VERSION 3.10)
project(ProjetMAX11627)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Inclure les headers dans /include
include_directories(include)

# Charger CURL
find_package(CURL REQUIRED)

# Créer la bibliothèque commune (sans main.cpp)
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
list(REMOVE_ITEM CORE_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
add_library(corelib ${CORE_SOURCES})
target_link_libraries(corelib PUBLIC CURL::libcurl)

# Exécutable principal (optionnel, si main.cpp est utilisé plus tard)
add_executable(prj_exec src/main.cpp)
target_link_libraries(prj_exec PRIVATE corelib)

# === Tests unitaires ===
add_executable(testADC tests/testADC.cpp)
target_link_libraries(testADC PRIVATE corelib)

add_executable(testAnalogMultiplexer tests/testAnalogMultiplexer.cpp)
target_link_libraries(testAnalogMultiplexer PRIVATE corelib)

add_executable(testNTPClient tests/testNTPClient.cpp)
target_link_libraries(testNTPClient PRIVATE corelib)

add_executable(testObloAPI tests/testObloAPI.cpp)
target_link_libraries(testObloAPI PRIVATE corelib)

add_executable(testSimulator tests/testSimulator.cpp)
target_link_libraries(testSimulator PRIVATE corelib)

add_executable(testMainController tests/testMainController.cpp)
target_link_libraries(testMainController PRIVATE corelib)

add_executable(testMainControllerStateMachine tests/testMainControllerStateMachine.cpp)
target_link_libraries(testMainControllerStateMachine PRIVATE corelib)

add_executable(testADCStateMachine tests/testADCStateMachine.cpp)
target_link_libraries(testADCStateMachine PRIVATE corelib)

add_executable(testMuxStateMachine tests/testMuxStateMachine.cpp)
target_link_libraries(testMuxStateMachine PRIVATE corelib)