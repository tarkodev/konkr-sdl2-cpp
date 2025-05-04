#ifndef TREASURYDISPLAYER_HPP
#define TREASURYDISPLAYER_HPP

//------------------------------
// STL & Core SDL2
//------------------------------
#include <vector>      // For internal collections (not used here but included for future extensions)
#include "SDL.h"       // Provides SDL_Point, SDL_Rect, SDL_Color, etc.

//------------------------------
// Rendering Abstractions
//------------------------------
#include "SDLWrappers/Renderers/Texture.hpp"    // Encapsulates SDL_Texture with RAII and blitting helpers
#include "SDLWrappers/Renderers/BlitTarget.hpp" // Interface for targets that can be drawn onto

//------------------------------
// Base Classes & Text Rendering
//------------------------------
#include "GenericDisplayer.hpp" // Provides Displayer base class with position/size
#include "SDLWrappers/Font.hpp" // Wraps TTF_Font for text rendering

/**
 * @brief Displays a treasury UI element: background + text showing current treasury,
 *        income delta, and projected next treasury.
 *
 * Inherits from Displayer, so it has a position and size and implements display().
 */
class TreasuryDisplayer : public Displayer {
public:
    /**
     * @brief Load shared resources: background texture and font.
     * Call once before creating any TreasuryDisplayer instances.
     */
    static void init();

    /**
     * @brief Release shared resources.
     * Call at program shutdown to free textures and fonts.
     */
    static void quit();
    
    /**
     * @brief Construct a TreasuryDisplayer at a screen position.
     * @param pos Screen coordinates center of the UI element.
     * @param treasury Initial treasury amount (default 0).
     * @param income   Initial income delta (default 0).
     */
    TreasuryDisplayer(const Point& pos, int treasury = 0, int income = 0);

    /// Default destructor cleans up instance-only data.
    ~TreasuryDisplayer() = default;

    /** 
     * @brief Update the displayed treasury amount.
     * @param treasury New treasury value.
     */
    void setTreasury(int treasury);

    /**
     * @brief Update the displayed income delta.
     * @param income New income value.
     */
    void setIncome(int income);

    /**
     * @brief If true, suppress income display when refreshing text.
     * @param noIncome Disable income display.
     */
    void setNoIncome(bool noIncome);

    /**
     * @brief Render the treasury UI onto a BlitTarget (e.g., the game window).
     * Overrides Displayer::display().
     * @param target Weak pointer to the rendering target.
     */
    void display(const std::weak_ptr<BlitTarget>& target) const override;

private:
    /// Shared background image for all TreasuryDisplayer instances.
    static std::shared_ptr<Texture> bg_;

    /// Shared font used to render the treasury text.
    static std::shared_ptr<Font> font_;

    /// Texture containing the current textual representation ("X + Y -> Z").
    std::shared_ptr<Texture> treasuryTex_;

    int treasury_;  // Current treasury amount
    int income_;    // Current income delta
    bool noIncome_ = false; // Flag to hide income info

    /**
     * @brief Recreate treasuryTex_ whenever treasury_, income_, or noIncome_ changes.
     * Called internally by setTreasury(), setIncome(), and setNoIncome().
     */
    void refreshTexture();
};

#endif // TREASURYDISPLAYER_HPP
