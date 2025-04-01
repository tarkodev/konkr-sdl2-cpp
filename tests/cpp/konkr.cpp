/**
 * Pour compiler:
 *  - clang++ konkr.cpp $(sdl2-config --cflags --libs) -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2_gfx -g -std=gnu++17 -arch arm64 -o konkr && ./konkr
 */
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <cmath>
#include <vector>
#include <iostream>

// Constantes
const int N = 6;
double radius = 40.0;
int start_x = 50;
int start_y = 50;

const double PI = 3.14159265358979323846;
const double PI2 = 2 * PI;
const double PI05 = PI / 2.0;

// Couleurs (RGBA)
const SDL_Color BLACK  = {0, 0, 0, 255};
const SDL_Color BLUE   = {50, 125, 160, 255};
const SDL_Color GREEN  = {0, 255, 0, 255};
const SDL_Color RED    = {255, 0, 0, 255};
const SDL_Color BROWN  = {81, 62, 62, 255};
const SDL_Color YELLOW = {252, 242, 0, 255};

// Fonctions de conversion

// Arrondi en coordonnées hexagonales
std::pair<int, int> hex_round(double q, double r) {
    double s = -q - r;
    int q_round = static_cast<int>(std::round(q));
    int r_round = static_cast<int>(std::round(r));
    int s_round = static_cast<int>(std::round(s));

    double q_diff = std::abs(q_round - q);
    double r_diff = std::abs(r_round - r);
    double s_diff = std::abs(s_round - s);

    if (q_diff > r_diff && q_diff > s_diff) {
        q_round = -r_round - s_round;
    } else if (r_diff > s_diff) {
        r_round = -q_round - s_round;
    } // Sinon, s_round est recalculé, mais nous ne l'utilisons pas ensuite.

    return {q_round, r_round};
}

// Conversion d'une position en pixels vers des coordonnées hexagonales
std::pair<int, int> pixel_to_hex(double x, double y, double hex_size) {
    double q = ((std::sqrt(3) / 3.0 * x) - (1.0/3.0 * y)) / hex_size;
    double r = (2.0/3.0 * y) / hex_size;
    return hex_round(q, r);
}

// Conversion d'une coordonnée hexagonale en position en pixels
std::pair<double, double> hex_to_pixel(double q, double r, double hex_size) {
    double x = hex_size * std::sqrt(3) * (q + r/2.0);
    double y = hex_size * (3.0/2.0) * r;
    return {x, y};
}

// Conversion de coordonnées offset (col, row) en coordonnées axiales (q, r)
std::pair<int, int> offset_to_axial(int col, int row) {
    int q = col - ((row - (row & 1)) / 2);
    int r = row;
    return {q, r};
}

// Dessine un polygone régulier (n-gone) centré en "position"
void draw_ngon(SDL_Renderer* renderer, const SDL_Color& color, int n, double rad, std::pair<double,double> position, int width = 0) {
    std::vector<Sint16> vx(n), vy(n);
    for (int i = 0; i < n; ++i) {
        double angle = PI05 + (static_cast<double>(i) / n) * PI2;
        vx[i] = static_cast<Sint16>(std::round(std::cos(angle) * rad + position.first));
        vy[i] = static_cast<Sint16>(std::round(std::sin(angle) * rad + position.second));
    }
    if (width == 0) {
        // Rempli
        filledPolygonRGBA(renderer, vx.data(), vy.data(), n, color.r, color.g, color.b, color.a);
    } else {
        // Seul le contour
        polygonRGBA(renderer, vx.data(), vy.data(), n, color.r, color.g, color.b, color.a);
        // Pour simuler l'épaisseur, on peut répéter le tracé
        for (int w = 1; w < width; ++w) {
            polygonRGBA(renderer, vx.data(), vy.data(), n, color.r, color.g, color.b, color.a);
        }
    }
}

// Dessine la grille et le hexagone survolé (si sel n'est pas nullptr)
void draw_grid(SDL_Renderer* renderer, const std::pair<int,int>* sel = nullptr) {
    // Parcours des lignes et colonnes
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            auto [q, r] = offset_to_axial(col, row);
            auto [x_pixel, y_pixel] = hex_to_pixel(q, r, radius);
            x_pixel += start_x;
            y_pixel += start_y;
            draw_ngon(renderer, BROWN, 6, radius, {x_pixel, y_pixel});
        }
    }
    // Si une sélection est active, dessiner un contour en jaune
    if (sel) {
        int q = sel->first;
        int r = sel->second;
        auto [x_pixel, y_pixel] = hex_to_pixel(q, r, radius);
        x_pixel += start_x;
        y_pixel += start_y;
        draw_ngon(renderer, YELLOW, 6, radius, {x_pixel, y_pixel}, 3);
    }
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur d'initialisation de SDL: %s", SDL_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Grille Hexagonale",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          500, 500, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Erreur de création de la fenêtre: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Erreur de création du renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    bool loop = true;
    SDL_Event event;
    // Option de sélection : contient les coordonnées axiales du hexagone survolé
    std::pair<int,int> sel;
    bool hasSel = false;
    
    while (loop) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                loop = false;
            }
            if (event.type == SDL_MOUSEMOTION) {
                int mouse_x = event.motion.x;
                int mouse_y = event.motion.y;
                // Coordonnées relatives en tenant compte du décalage de départ
                double rel_x = mouse_x - start_x;
                double rel_y = mouse_y - start_y;
                sel = pixel_to_hex(rel_x, rel_y, radius);
                hasSel = true;
            } else if (event.type == SDL_MOUSEWHEEL) {
                radius *=  (event.wheel.preciseY > 0) ? 1.1 : 0.9;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT)
                    start_x -= 10;
                else if (event.key.keysym.sym == SDLK_RIGHT)
                    start_x += 10;
                else if (event.key.keysym.sym == SDLK_UP)
                    start_y -= 10;
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    start_y += 10;
                    
                }
            }
        }
        
        // Remplir le fond en bleu
        SDL_SetRenderDrawColor(renderer, BLUE.r, BLUE.g, BLUE.b, BLUE.a);
        SDL_RenderClear(renderer);
        
        // Dessiner la grille et, si présente, la sélection
        if (hasSel)
            draw_grid(renderer, &sel);
        else
            draw_grid(renderer);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Limite à environ 60 FPS
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
