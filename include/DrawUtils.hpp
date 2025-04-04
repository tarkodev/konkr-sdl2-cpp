#ifndef DRAWUTILS_HPP
#define DRAWUTILS_HPP

#include <SDL.h>
#include <stdexcept>

namespace DrawUtils
{
    inline void SetRenderDrawColor(SDL_Renderer* renderer, const SDL_Color& color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    //! A deplacer dans un util sur les textures
    inline SDL_Rect getSize(SDL_Texture *texture) {
        SDL_Rect size = {0, 0, 0, 0};
        SDL_QueryTexture(texture, NULL, NULL, &size.w, &size.h);
        return size;
    }

    //! A deplacer dans un util sur les textures (et utiliser pointeur qui libère la mémoire)
    inline SDL_Texture *copyTexture(SDL_Renderer* renderer, SDL_Texture *texture) {
        SDL_Rect size = getSize(texture);
        SDL_Texture *copy = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.w, size.h);
        if (!copy)
            throw std::runtime_error("Échec de la duplication de la texture par défaut: " + std::string(SDL_GetError()));

        // Copier le contenu de defaultHexagonSprite_ dans hexagonSprite_
        SDL_Texture* currentTarget = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, copy);

        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        SDL_SetRenderTarget(renderer, currentTarget);
        SDL_SetTextureBlendMode(copy, SDL_BLENDMODE_BLEND);

        return copy;
    }
}

#endif
