#include "Player.hpp"
#include "ColorUtils.hpp"
#include "DrawUtils.hpp"

SDL_Texture* Player::defaultHexagonSprite_ = nullptr;
SDL_Rect Player::hexagonSize_ = {0, 0, 0, 0};

Player::Player(SDL_Renderer *renderer, SDL_Color color) : color_(color), renderer_(renderer)
{
    if (!defaultHexagonSprite_)
        throw std::runtime_error("La texture par défaut n'est pas chargée.");

    hexagonSprite_ = DrawUtils::copyTexture(renderer_, defaultHexagonSprite_);
    
    SDL_Texture* currentTarget = SDL_GetRenderTarget(renderer_);
    SDL_SetRenderTarget(renderer_, hexagonSprite_);
    SDL_SetTextureColorMod(hexagonSprite_, color_.r, color_.g, color_.b);
    SDL_SetRenderTarget(renderer_, currentTarget);
}

Player::~Player() = default;


SDL_Texture* Player::getHexagonSprite() const {
    return hexagonSprite_;
}

SDL_Rect Player::getHexagonSize() const {
    return hexagonSize_;
}