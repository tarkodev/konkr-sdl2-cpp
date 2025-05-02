#ifndef MAPSELECTMENU_HPP
#define MAPSELECTMENU_HPP

#include "MenuBase.hpp"

class MainMenu;

class MapSelectMenu : public MenuBase {
public:
    MapSelectMenu(const std::shared_ptr<Window>& window);

    ~MapSelectMenu() = default;

    std::shared_ptr<MenuBase> run() override;

protected:
    void handleEvents() override;
    void draw() override;

private:
    std::vector<Button> buttons_;
};

#endif
