cmake_minimum_required(VERSION 3.10)
project(ProjetMAX11627)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Inclure les headers dans /include
include_directories(include)

# Créer la liste des sources
file(GLOB SOURCES "src/*.cpp")

# Créer l'exécutable
add_executable(prj_exec ${SOURCES})

# Ajouter la bibliothèque CURL
find_package(CURL REQUIRED)
target_link_libraries(prj_exec PRIVATE CURL::libcurl)