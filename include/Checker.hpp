#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "SDL.h"
#include <stdexcept>
#include <string>
#include <iostream>

/**
 * Vérifie le retour des fonctions de la SDL
 */
inline void SDL_Check(int status, std::string errmsg) {
    if (status != 0)
        throw std::runtime_error(errmsg + " failed : " + SDL_GetError());
    
    /*
    // Faux positifs renvoyés par la SDL2 (bug connu)
    if (SDL_GetError()[0] != '\0' && std::string(SDL_GetError()) != "Parameter 'texture' is invalid" && std::string(SDL_GetError()) != "Unknown touch device id -1, cannot reset")
        SDL_Log("(%s): %s", errmsg.c_str(), SDL_GetError());
    */
}

#endif
