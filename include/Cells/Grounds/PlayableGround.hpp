#ifndef PLAYABLEGROUND_HPP
#define PLAYABLEGROUND_HPP

//------------------------------
// SDL2 Core
//------------------------------
#include "SDL.h"  // SDL_Point, SDL_Texture, etc.

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for render targets
#include "SDLWrappers/Renderers/Texture.hpp"    // RAII wrapper around SDL_Texture

//------------------------------
// Game Logic
//------------------------------
#include "GameElements/Player.hpp"              // Player ownership logic
#include "Cells/Grounds/Ground.hpp"             // Base class for all ground cells
#include "GameElements/GameElement.hpp"         // Represents elements placed on grounds
#include "Displayers/FenceDisplayer.hpp"        // Utility to render fences

#include <unordered_set>
#include <queue>
#include <memory>

/**
 * @brief A ground cell that can be owned and interacted with.
 *
 * Extends Ground with ownership by a Player, elements placement,
 * selectable state, fence/link mechanics, and income generation.
 *
 * Inherits enable_shared_from_this to allow safe weak/shared pointers.
 */
class PlayableGround : public Ground,
                       public std::enable_shared_from_this<PlayableGround> {
public:
    /**
     * @brief Cast a generic Cell to PlayableGround if possible.
     * @param obj Weak pointer to a Cell.
     * @return Shared pointer to PlayableGround or nullptr.
     */
    static std::shared_ptr<PlayableGround> cast(const std::weak_ptr<Cell>& obj);

    /**
     * @brief Check whether a Cell is a PlayableGround.
     * @param obj Weak pointer to a Cell.
     * @return true if obj is a PlayableGround.
     */
    static const bool is(const std::weak_ptr<Cell>& obj);
    
    /**
     * @brief Make a deep copy of object.
     * @return deep copy of this.
     */
    std::shared_ptr<Cell> deepCopy() const override;

    /**
     * @brief Load static resources (textures, sprites, etc.).
     * Must be called once before creating instances.
     */
    static void init();

    /**
     * @brief Release static resources.
     * Call at program shutdown to free textures.
     */
    static void quit();

    /**
     * @brief Construct with position and owner.
     * @param pos Cell center in pixel coordinates.
     * @param owner Player who owns this ground.
     */
    PlayableGround(const Point& pos, const std::shared_ptr<Player>& owner);

    /**
     * @brief Construct with position only (unowned).
     * @param pos Cell center in pixel coordinates.
     */
    PlayableGround(const Point& pos);

    /**
     * @brief Retrieve current owner.
     * @return Shared pointer to Player.
     */
    std::shared_ptr<Player> getOwner();

    /**
     * @brief Retrieve previous owner.
     * @return Shared pointer to old Player.
     */
    std::shared_ptr<Player> getOldOwner();

    /**
     * @brief Change the owner of this ground.
     * @param owner New owning Player.
     */
    void setOwner(std::shared_ptr<Player> owner);

    /**
     * @brief Render the ground base and any overlay.
     * @param target Weak pointer to the BlitTarget.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

    /**
     * @brief Check if any fences are present.
     * @return true if fences exist.
     */
    const bool hasFences() const;

    /**
     * @brief Render fences around this ground.
     * @param target Fence texture.
     */
    void displayFences(const std::weak_ptr<Texture>& target);

    /**
     * @brief Render the GameElement on this ground.
     * @param target Element texture.
     */
    void displayElement(const std::weak_ptr<Texture>& target);

    /**
     * @brief Render the shield sprite showing defense value.
     * @param target Shield texture.
     */
    void displayShield(const std::weak_ptr<Texture>& target);

    /**
     * @brief Render a cross overlay (e.g., for blocked).
     * @param target Cross texture.
     */
    void displayCross(const std::weak_ptr<Texture>& target);

    /**
     * @brief Render selection highlight.
     * @param target Selection texture.
     * @param selected Whether it's actively selected.
     */
    void displaySelectable(const std::weak_ptr<Texture>& target,
                           const bool& selected = false);

    /**
     * @brief Check if this ground is linked to others of same owner.
     * @return true if linked.
     */
    const bool isLinked();

    /**
     * @brief Recompute linked status across connected grounds.
     */
    void updateLinked();

    /**
     * @brief Link this ground and its neighbors to an owner.
     * @param owner Player to link with.
     */
    void link(const std::weak_ptr<Player>& owner);

    /**
     * @brief Release all troops/elements when ownership changes.
     */
    void freeTroops();

    /**
     * @brief Get all towns reachable via links.
     * @return Vector of weak pointers to Towns.
     */
    std::vector<std::weak_ptr<Town>> getTowns();

    /**
     * @brief Find the nearest town in range.
     * @return Shared pointer to the nearest Town.
     */
    std::shared_ptr<Town> getNearestTown();

    /**
     * @brief Update income for the owner based on this ground.
     */
    void updateIncome();

    /**
     * @brief Get the element placed on this ground.
     * @return Shared pointer to GameElement or nullptr.
     */
    std::shared_ptr<GameElement> getElement();

    /**
     * @brief Set a GameElement on this ground.
     * @param elt Element to place.
     */
    void setElement(const std::shared_ptr<GameElement>& elt);

    /**
     * @brief Get current shield/defense value.
     * @return Integer shield strength.
     */
    const int getShield() const;

    /**
     * @brief Update whether this ground is selectable based on strength.
     * @param strength Value to compare.
     */
    void updateSelectable(int strength);

    /**
     * @brief Check if ground is marked selectable.
     * @return true if selectable.
     */
    const bool isSelectable() const { return selectable_; }

private:
    // Static display helpers
    static FenceDisplayer fenceDisplayer_;
    static std::vector<std::shared_ptr<Texture>> shieldSprites_;
    static std::shared_ptr<Texture> smallSelectableSprite_;
    static std::shared_ptr<Texture> selectableSprite_;
    static std::shared_ptr<Texture> crossSprite_;

    // Ownership state
    std::shared_ptr<Player> oldOwner_;
    std::shared_ptr<Player> owner_;

    // Visual plates for linking/highlighting
    HexagonDisplayer plate_     = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};
    HexagonDisplayer lostPlate_ = HexagonDisplayer{-1, nullptr, nullptr, nullptr, nullptr, nullptr};

    // Placed game element
    std::shared_ptr<GameElement> element = nullptr;

    // Flags
    bool hasPlate_    = false;
    bool selectable_  = false;

    // Recursive helpers for linking and selection
    const bool isLinked(std::unordered_set<std::shared_ptr<PlayableGround>>& visited);
    void unlink(std::unordered_set<std::shared_ptr<PlayableGround>>& visited);
    void link(const std::weak_ptr<Player>& owner,
              std::unordered_set<std::shared_ptr<PlayableGround>>& visited);
    void updateSelectable(int strength,
                          std::unordered_set<std::shared_ptr<PlayableGround>>& visited);

    // Helpers for town retrieval
    void getTowns(std::queue<std::weak_ptr<PlayableGround>>& toVisit,
                  std::unordered_set<std::shared_ptr<PlayableGround>>& visited,
                  std::vector<std::weak_ptr<Town>>& towns);
    std::shared_ptr<Town> getNearestTown(std::queue<std::weak_ptr<PlayableGround>>& toVisit,
                                         std::unordered_set<std::shared_ptr<PlayableGround>>& visited);

    // Recursive troop freeing
    void freeTroops(std::unordered_set<std::shared_ptr<PlayableGround>>& visited);

    /**
     * @brief Internal setter for selectable flag.
     * @param selectable New selectable state.
     */
    void setSelectable(bool selectable);
};

#endif // PLAYABLEGROUND_HPP
