#ifndef LOGIC_TROOP_HPP
#define LOGIC_TROOP_HPP

#include "SDL.h"
#include "GameElements/GameElement.hpp"
#include "Texture.hpp"

class Troop: public GameElement {
public:
    /** Charge le sprite commun à la classe (à appeler une seule fois). */
    static void init();
    static void quit();
    static std::shared_ptr<Troop> cast(const std::weak_ptr<GameElement>& obj);
    static bool is(const std::weak_ptr<GameElement>& obj);

    virtual ~Troop() = default;

    bool isFree() const;
    void setFree(const bool& free);

    bool isMovable() const;
    void setMovable(const bool& movable);

protected:
    static std::shared_ptr<Texture> shadow_;
    static std::shared_ptr<Texture> lostSprite_;

    bool free_ = false;
    bool movable_ = false;

    Troop(const Point& pos, const Size& size);

    void displaySprite(const std::weak_ptr<BlitTarget>& target, const std::weak_ptr<Texture>& sprite);
};

#endif
