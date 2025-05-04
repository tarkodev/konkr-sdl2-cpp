#ifndef CHECKER_HPP
#define CHECKER_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"             // Provides SDL_GetError()

//------------------------------
// C++ Standard Library
//------------------------------
#include <stdexcept>         // std::runtime_error
#include <string>            // std::string
#include <iostream>          // std::cerr (for optional logging)

/**
 * @brief Verify the return status of an SDL call.
 *
 * Throws a std::runtime_error if the status indicates failure (non-zero),
 * appending the SDL error message from SDL_GetError().
 *
 * Known SDL2 bugs can produce spurious errors; the commented-out block
 * below shows how to log non-critical messages instead of throwing.
 *
 * @param status   Return value from an SDL function (0 on success).
 * @param errmsg   Description of the operation being checked.
 */
inline void SDL_Check(int status, std::string errmsg) {
    if (status != 0)
        throw std::runtime_error(errmsg + " failed: " + SDL_GetError());
    
    /*
    // Optional logging of SDL errors without throwing:
    // if (SDL_GetError()[0] != '\0'
    //     && std::string(SDL_GetError()) != "Parameter 'texture' is invalid"
    //     && std::string(SDL_GetError()) != "Unknown touch device id -1, cannot reset")
    // {
    //     SDL_Log("(%s): %s", errmsg.c_str(), SDL_GetError());
    // }
    */
}

#endif // CHECKER_HPP
