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

    # SDL2_ttf
    find_library(SDL2_TTF_LIBRARY 
        NAMES SDL2_ttf
        PATHS "${CMAKE_SOURCE_DIR}/lib_macos/SDL2_ttf/lib"
        NO_DEFAULT_PATH
    )
    include_directories("${CMAKE_SOURCE_DIR}/lib_macos/SDL2_ttf/include")
else()
    # Sur Linux (Ubuntu), on utilise les bibliothèques système via pkg-config
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2GFX REQUIRED SDL2_gfx)
    pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2TTF REQUIRED SDL2_ttf)
    
    include_directories(${SDL2GFX_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
    
    # Les bibliothèques seront celles fournies par pkg-config
    set(SDL2_LIBRARIES SDL2::SDL2)
    set(SDL2_GFX_LIBRARY ${SDL2GFX_LIBRARIES})
    set(SDL2_IMAGE_LIBRARY ${SDL2IMAGE_LIBRARIES})
    set(SDL2_TTF_LIBRARY ${SDL2TTF_LIBRARIES})
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
            ${SDL2_TTF_LIBRARY}
        )
    endif()
endfunction()

# --- Définition des fichiers sources ---
set(SRC_FILES 
    "${CMAKE_SOURCE_DIR}/src/SDLWrappers/Cursor.cpp"
    "${CMAKE_SOURCE_DIR}/src/SDLWrappers/Font.cpp"

    "${CMAKE_SOURCE_DIR}/src/SDLWrappers/Coords/Point.cpp"
    "${CMAKE_SOURCE_DIR}/src/SDLWrappers/Coords/Size.cpp"
    "${CMAKE_SOURCE_DIR}/src/SDLWrappers/Coords/Rect.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/SDLWrappers/Renderers/Texture.cpp"
    "${CMAKE_SOURCE_DIR}/src/SDLWrappers/Renderers/RenderTargetGuard.cpp"
    "${CMAKE_SOURCE_DIR}/src/SDLWrappers/Renderers/Window.cpp"

    "${CMAKE_SOURCE_DIR}/src/Displayers/Displayer.cpp"
    "${CMAKE_SOURCE_DIR}/src/Displayers/HexagonDisplayer.cpp"
    "${CMAKE_SOURCE_DIR}/src/Displayers/FenceDisplayer.cpp"
    "${CMAKE_SOURCE_DIR}/src/Displayers/TreasuryDisplayer.cpp"

    "${CMAKE_SOURCE_DIR}/src/GameElements/Player.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/GameElements/GameElement.cpp"
    "${CMAKE_SOURCE_DIR}/src/GameElements/Town.cpp"
    "${CMAKE_SOURCE_DIR}/src/GameElements/Castle.cpp"
    "${CMAKE_SOURCE_DIR}/src/GameElements/Camp.cpp"

    "${CMAKE_SOURCE_DIR}/src/GameElements/Troops/Troop.cpp"
    "${CMAKE_SOURCE_DIR}/src/GameElements/Troops/Bandit.cpp"
    "${CMAKE_SOURCE_DIR}/src/GameElements/Troops/Villager.cpp"
    "${CMAKE_SOURCE_DIR}/src/GameElements/Troops/Pikeman.cpp"
    "${CMAKE_SOURCE_DIR}/src/GameElements/Troops/Knight.cpp"
    "${CMAKE_SOURCE_DIR}/src/GameElements/Troops/Hero.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/Cells/Cell.cpp"
    "${CMAKE_SOURCE_DIR}/src/Cells/Water.cpp"
    "${CMAKE_SOURCE_DIR}/src/Cells/Grounds/Ground.cpp"
    "${CMAKE_SOURCE_DIR}/src/Cells/Grounds/PlayableGround.cpp"
    "${CMAKE_SOURCE_DIR}/src/Cells/Grounds/Forest.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/Widgets/Button.cpp"
    "${CMAKE_SOURCE_DIR}/src/Widgets/HexagonGrid.tpp"
    "${CMAKE_SOURCE_DIR}/src/Widgets/GameMap.cpp"
    "${CMAKE_SOURCE_DIR}/src/Widgets/Overlay.cpp"

    "${CMAKE_SOURCE_DIR}/src/Menus/MenuBase.cpp"
    "${CMAKE_SOURCE_DIR}/src/Menus/MainMenu.cpp"
    "${CMAKE_SOURCE_DIR}/src/Menus/MapsMenu.cpp"
    "${CMAKE_SOURCE_DIR}/src/Menus/GameMenu.cpp"
    "${CMAKE_SOURCE_DIR}/src/Menus/MakeMenu.cpp"
    
    "${CMAKE_SOURCE_DIR}/src/Game.cpp"
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

add_compile_options(-std=c++23)

# Compilation de l'exécutable en activant SDL
compilation(
    EXEC konkr
    SRC ${SRC_FILES}
    USE_SDL
)
