cmake_minimum_required(VERSION 3.10...3.27)
project(PROJECT LANGUAGES CXX)

# Spécifier le standard C++ (ici C++23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    # Sur macOS, on utilise les bibliothèques locales dans lib_macos/
    set(SDL2_DIR "${CMAKE_SOURCE_DIR}/lib_macos/SDL2/lib/cmake/SDL2")
    find_package(SDL2 REQUIRED)
    set(SDL2_LIBRARIES ${SDL2_LIBRARIES})
    include_directories(${SDL2_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/lib_macos/SDL2/include")
    
    # SDL2_gfx
    find_library(SDL2_GFX_LIBRARY 
        NAMES SDL2_gfx
        PATHS "${CMAKE_SOURCE_DIR}/lib_macos/SDL2_gfx/lib"
        NO_DEFAULT_PATH
    )
    include_directories("${CMAKE_SOURCE_DIR}/lib_macos/SDL2_gfx/include")
    
    # SDL2_image
    find_library(SDL2_IMAGE_LIBRARY 
        NAMES SDL2_image
        PATHS "${CMAKE_SOURCE_DIR}/lib_macos/SDL2_image/lib"
        NO_DEFAULT_PATH
    )
    include_directories("${CMAKE_SOURCE_DIR}/lib_macos/SDL2_image/include")
else()
    # Sur Linux (Ubuntu), on utilise les bibliothèques système via pkg-config
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2GFX REQUIRED SDL2_gfx)
    pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image)
    
    include_directories(${SDL2GFX_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
    
    # Les bibliothèques seront celles fournies par pkg-config
    set(SDL2_LIBRARIES SDL2::SDL2)
    set(SDL2_GFX_LIBRARY ${SDL2GFX_LIBRARIES})
    set(SDL2_IMAGE_LIBRARY ${SDL2IMAGE_LIBRARIES})
endif()

# Ajout des headers du projet
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
set(SRC_FILES 
    "${CMAKE_SOURCE_DIR}/src/Texture.cpp"
    "${CMAKE_SOURCE_DIR}/src/HexagonDisplayer.cpp"
    "${CMAKE_SOURCE_DIR}/src/FenceDisplayer.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/logic/GameElement.cpp"
    "${CMAKE_SOURCE_DIR}/src/logic/Troop.cpp"

    "${CMAKE_SOURCE_DIR}/src/logic/units/Bandit.cpp"
    "${CMAKE_SOURCE_DIR}/src/logic/units/Town.cpp"
    "${CMAKE_SOURCE_DIR}/src/logic/units/Castle.cpp"
    "${CMAKE_SOURCE_DIR}/src/logic/units/Camp.cpp"
    "${CMAKE_SOURCE_DIR}/src/logic/units/Villager.cpp"
    "${CMAKE_SOURCE_DIR}/src/logic/units/Pikeman.cpp"
    "${CMAKE_SOURCE_DIR}/src/logic/units/Knight.cpp"
    "${CMAKE_SOURCE_DIR}/src/logic/units/Hero.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/Cell.cpp"
    "${CMAKE_SOURCE_DIR}/src/Water.cpp"
    "${CMAKE_SOURCE_DIR}/src/Ground.cpp"
    "${CMAKE_SOURCE_DIR}/src/PlayableGround.cpp"
    "${CMAKE_SOURCE_DIR}/src/Forest.cpp"

    "${CMAKE_SOURCE_DIR}/src/Player.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/HexagonGrid.tpp"
    "${CMAKE_SOURCE_DIR}/src/GameMap.cpp"
    "${CMAKE_SOURCE_DIR}/src/Window.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/Game.cpp"
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/Button.cpp"
    "${CMAKE_SOURCE_DIR}/src/Overlay.cpp"
    "${CMAKE_SOURCE_DIR}/src/MainMenu.cpp"
    "${CMAKE_SOURCE_DIR}/src/MapSelectMenu.cpp"
)

add_compile_options(-std=c++23)

# Compilation de l'exécutable en activant SDL
compilation(
    EXEC konkr
    SRC ${SRC_FILES}
    USE_SDL
)
