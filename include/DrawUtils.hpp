#ifndef DRAWUTILS_HPP
#define DRAWUTILS_HPP

#include <SDL.h>
#include <array>
#include "RenderTargetGuard.hpp"

namespace DrawUtils
{
    inline void SetRenderDrawColor(SDL_Renderer* renderer, const SDL_Color& color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    /**
     * @brief Dessine le contour d'un triangle.
     */
    inline void drawTriangleOutline(SDL_Renderer* renderer,
                                    const SDL_Point& p1,
                                    const SDL_Point& p2,
                                    const SDL_Point& p3,
                                    const SDL_Color& color)
    {
        // On garde l'ancien blend mode
        SDL_BlendMode oldBm;
        SDL_GetRenderDrawBlendMode(renderer, &oldBm);

        // On utilise le mode none pour écraser si besoin
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SetRenderDrawColor(renderer, color);

        // On dessine les 4 segments (retour à p1 pour fermer)
        std::array<SDL_Point,4> pts = {{p1, p2, p3, p1}};
        SDL_RenderDrawLines(renderer, pts.data(), static_cast<int>(pts.size()));

        // On restaure le blend mode
        SDL_SetRenderDrawBlendMode(renderer, oldBm);
    }

#if SDL_VERSION_ATLEAST(2,0,18)
    /**
     * @brief Remplit un triangle avec SDL_RenderGeometry (SDL2 ≥ 2.0.18).
     */
    inline void fillTriangle(SDL_Renderer* renderer,
                             const SDL_Point& p1,
                             const SDL_Point& p2,
                             const SDL_Point& p3,
                             const SDL_Color& color)
    {
        // On sauve et met à jour le blend mode
        SDL_BlendMode oldBm;
        SDL_GetRenderDrawBlendMode(renderer, &oldBm);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        // Préparation des sommets
        SDL_Vertex verts[3] = {
            {{(float)p1.x,(float)p1.y}, color, {0,0}},
            {{(float)p2.x,(float)p2.y}, color, {0,0}},
            {{(float)p3.x,(float)p3.y}, color, {0,0}}
        };

        // Remplissage
        SDL_RenderGeometry(renderer, nullptr, verts, 3, nullptr, 0);

        // Restauration
        SDL_SetRenderDrawBlendMode(renderer, oldBm);
    }
#else
    /**
     * @brief Fallback : dessine le triangle plein via scanline.
     */
    inline void fillTriangle(SDL_Renderer* renderer,
                             const SDL_Point& p1,
                             const SDL_Point& p2,
                             const SDL_Point& p3,
                             const SDL_Color& color)
    {
        // Blend mode none pour écraser
        SDL_BlendMode oldBm;
        SDL_GetRenderDrawBlendMode(renderer, &oldBm);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SetRenderDrawColor(renderer, color);

        // Sort points par y
        const SDL_Point* pts[3] = {&p1,&p2,&p3};
        std::sort(pts, pts+3, [](auto a, auto b){ return a->y < b->y; });

        auto interp = [](int y, const SDL_Point& a, const SDL_Point& b)->int {
            if (b.y==a.y) return a.x;
            return a.x + (b.x - a.x)*(float)(y - a.y)/(b.y - a.y);
        };

        int y0 = pts[0]->y, y1 = pts[1]->y, y2 = pts[2]->y;
        for (int y=y0; y<=y2; ++y) {
            if (y <= y1) {
                int xa = interp(y, *pts[0], *pts[1]);
                int xb = interp(y, *pts[0], *pts[2]);
                SDL_RenderDrawLine(renderer, xa, y, xb, y);
            } else {
                int xa = interp(y, *pts[1], *pts[2]);
                int xb = interp(y, *pts[0], *pts[2]);
                SDL_RenderDrawLine(renderer, xa, y, xb, y);
            }
        }

        // Restauration
        SDL_SetRenderDrawBlendMode(renderer, oldBm);
    }
#endif

} // namespace DrawUtils

#endif
