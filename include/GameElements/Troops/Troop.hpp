#ifndef LOGIC_TROOP_HPP
#define LOGIC_TROOP_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"                              // Provides SDL_Point, SDL_Renderer, timing, etc.

//------------------------------
// Game Element Base
//------------------------------
#include "GameElements/GameElement.hpp"      // Defines the abstract GameElement base class

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/Texture.hpp"  // Encapsulates SDL_Texture with RAII

/**
 * @brief Base class for all movable units (troops) in the game.
 *
 * Inherits from GameElement and adds:
 *  - Shared assets (shadow and lost overlays)
 *  - State flags for whether the troop is free or can still move
 *  - Helper for rendering the combined sprite
 */
class Troop : public GameElement {
public:
    /**
     * @brief Load the shared sprites for all Troop instances.
     * Must be called once before creating any Troop objects.
     */
    static void init();

    /**
     * @brief Release the shared sprites.
     * Call at shutdown to free resources.
     */
    static void quit();

    /**
     * @brief Attempt to cast a generic GameElement to a Troop.
     * @param obj Weak pointer to the GameElement
     * @return Shared pointer to Troop, or nullptr if not a Troop
     */
    static std::shared_ptr<Troop> cast(const std::weak_ptr<GameElement>& obj);

    /**
     * @brief Check if a GameElement is actually a Troop.
     * @param obj Weak pointer to the GameElement
     * @return true if obj wraps a Troop instance
     */
    static const bool is(const std::weak_ptr<GameElement>& obj);

    virtual ~Troop() = default;

    /**
     * @brief Query whether this troop is marked as "free" (no longer bound).
     * @return true if free
     */
    const bool isFree() const;

    /**
     * @brief Mark this troop as free or not.
     * @param free New free status
     */
    void setFree(const bool& free);

    /**
     * @brief Query whether this troop can still move this turn.
     * @return true if movable
     */
    const bool isMovable() const;

    /**
     * @brief Enable or disable movement for this troop.
     * @param movable New movable status
     */
    void setMovable(const bool& movable);

protected:
    /// Shared shadow sprite displayed beneath all troops
    static std::shared_ptr<Texture> shadow_;
    /// Shared overlay sprite used when a troop is lost or inactive
    static std::shared_ptr<Texture> lostSprite_;

    bool free_    = false;  ///< Indicates if the troop is free (unbound)
    bool movable_ = false;  ///< Indicates if the troop can still move

    /**
     * @brief Construct a Troop at a given position and size.
     * @param pos  Center point in pixel coordinates
     * @param size Dimensions of the troop sprite
     */
    Troop(const Point& pos, const Size& size);

    /**
     * @brief Internal helper to render shadow, lost overlay, and main sprite.
     * @param target Weak pointer to the render target
     * @param sprite The troop’s main texture
     */
    void displaySprite(const std::weak_ptr<BlitTarget>& target,
                       const std::weak_ptr<Texture>& sprite) const;
};

#endif // LOGIC_TROOP_HPP
