#ifndef MAPSMENU_HPP
#define MAPSMENU_HPP

#include "MenuBase.hpp"

class MainMenu;

class MapsMenu : public MenuBase {
public:
    MapsMenu(const std::shared_ptr<Window>& window);

    ~MapsMenu() = default;

    std::shared_ptr<MenuBase> run() override;

protected:
    void handleEvents() override;
    void draw() override;

private:
    std::vector<Button> buttons_;
};

#endif
