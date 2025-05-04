#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "SDLWrappers/Coords/Point.hpp"
#include "Menus/MenuBase.hpp"
#include "SDLWrappers/Renderers/Window.hpp"
#include "SDLWrappers/Renderers/Texture.hpp"
#include "Widgets/Button.hpp"
#include <memory>

class MapsMenu;

class MainMenu : public MenuBase {
public:
    MainMenu(const std::shared_ptr<Window>& window);

    ~MainMenu() = default;

    std::shared_ptr<MenuBase> run() override;

protected:
    void handleEvents() override;
    void draw() override;

private:
    std::shared_ptr<Texture> bg_;
    std::shared_ptr<Texture> logo_;

    std::unique_ptr<Button> expeditionBtn_;
    std::unique_ptr<Button> howToPlayBtn_;
    std::unique_ptr<Button> exitBtn_;

    Point logoPos_;
};

#endif
