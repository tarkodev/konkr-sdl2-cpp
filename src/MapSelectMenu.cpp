#include "MapSelectMenu.hpp"
#include "Game.hpp"
#include <SDL.h>

static const struct { const char* label; const char* file; } MAPS[] = {
    {"Ten Paces",   "../assets/map/Ten Paces.txt"},
    {"Unity",       "../assets/map/Unity.txt"},
    {"Test",        "../assets/map/Test.txt"},
    {"Custom file", ""}              // vide ⇒ à demander plus tard
};


//! faire fonction init pour passer le renderer
MapSelectMenu::MapSelectMenu(Game& app) : app_(app)
{
    int y = 120;
    for(auto m : MAPS){
        Texture* t = new Texture(renderer_, std::string("../assets/map/")+m.label+".png");
        auto* b = new Button(t,nullptr,nullptr, Point{200,y});
        if (m.file[0])   // 3 cartes fournies
            b->setCallback([this,m](){
                app_.requestStartGame(m.file); 
            });
        else             // Custom :
            b->setCallback([](){
                // TODO : ouvrir une boîte de dialogue ou std::cin pour le chemin
                SDL_Log("Custom map selection not implemented yet.");
            });
        buttons_.push_back(b);
        y += 120;
    }

    Texture* backTex = new Texture(renderer_, "../assets/img/back.png");
    auto* backBtn = new Button(backTex, nullptr, nullptr, Point{50, 30});  // Position en haut à gauche
    backBtn->setCallback([this](){
        app_.requestAction(PendingAction::OpenMainMenu);
    });
    buttons_.push_back(backBtn);
}
MapSelectMenu::~MapSelectMenu(){ for(auto* b:buttons_) delete b; }
void MapSelectMenu::handleEvent(const SDL_Event& e){
    for(auto* b:buttons_) b->handleEvent(e);

    // Échap pour revenir au menu principal
    if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_ESCAPE){
        app_.openMapSelect();   // ou recréer un MainMenu si vous préférez
    }
}

void MapSelectMenu::display(const BlitTarget* target) {
    for(auto* b:buttons_) b->display(target);
}
