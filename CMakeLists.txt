cmake_minimum_required(VERSION 3.10...3.27)
project(PROJECT LANGUAGES CXX)

# Spécifier le standard C++ (ici C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Configuration de SDL2 ---
# Le fichier de configuration de SDL2 se trouve dans lib/SDL2/lib/cmake/SDL2
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2/lib/cmake/SDL2")
find_package(SDL2 REQUIRED)
# Ajout des headers de SDL2 (ils se trouvent dans lib/SDL2/include)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("${CMAKE_SOURCE_DIR}/lib/SDL2/include")

# --- Configuration de SDL2_gfx ---
# Recherche de la bibliothèque SDL2_gfx dans lib/SDL2_gfx/lib
find_library(SDL2_GFX_LIBRARY 
    NAMES SDL2_gfx
    PATHS "${CMAKE_SOURCE_DIR}/lib/SDL2_gfx/lib"
    NO_DEFAULT_PATH
)
# Ajout des headers de SDL2_gfx (ils se trouvent dans lib/SDL2_gfx/include)
include_directories("${CMAKE_SOURCE_DIR}/lib/SDL2_gfx/include")

# --- Configuration de SDL2_image ---
# Recherche de la bibliothèque SDL2_image dans lib/SDL2_image/lib
find_library(SDL2_IMAGE_LIBRARY 
    NAMES SDL2_image
    PATHS "${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib"
    NO_DEFAULT_PATH
)
# Ajout des headers de SDL2_image (ils se trouvent dans lib/SDL2_image/include)
include_directories("${CMAKE_SOURCE_DIR}/lib/SDL2_image/include")

# --- Ajout des headers du projet ---
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/src")

# --- Fonction de compilation personnalisée ---
function(compilation)
    cmake_parse_arguments(COMPILATION_PREFIX "USE_SDL" "EXEC" "SRC" ${ARGN})
    add_executable(${COMPILATION_PREFIX_EXEC} ${COMPILATION_PREFIX_SRC})
    target_include_directories(${COMPILATION_PREFIX_EXEC} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(${COMPILATION_PREFIX_EXEC} PROPERTIES CXX_STANDARD 17)
    
    if (COMPILATION_PREFIX_USE_SDL)
        target_link_libraries(${COMPILATION_PREFIX_EXEC} 
            PRIVATE 
            ${SDL2_LIBRARIES} 
            ${SDL2_GFX_LIBRARY}
            ${SDL2_IMAGE_LIBRARY}
        )
    endif()
endfunction()

# --- Définition des fichiers sources ---
# Les fichiers sources se trouvent dans le dossier src
set(SRC_FILES 
    "${CMAKE_SOURCE_DIR}/src/main.cpp" 
    "${CMAKE_SOURCE_DIR}/src/HexagonGrid.tpp"
    "${CMAKE_SOURCE_DIR}/src/Game.cpp"
    "${CMAKE_SOURCE_DIR}/src/GameMap.cpp"
    "${CMAKE_SOURCE_DIR}/src/Window.cpp"
)

# Compilation de l'exécutable en activant SDL
compilation(
    EXEC konkr
    SRC ${SRC_FILES}
    USE_SDL
)
