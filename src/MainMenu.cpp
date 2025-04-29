#include "MainMenu.hpp"
#include "Game.hpp"
#include <SDL.h>

MainMenu::MainMenu(SDL_Renderer* r, Game& app) : renderer_(r), app_(app) {


    // Bouton Logo (ne fait rien)
    Texture* logoTex = new Texture(r, "../assets/img/konkr.png");
    auto* logoBtn = new Button(logoTex, nullptr, nullptr, Point{200, 50}); // position en haut
    logoBtn->setCallback([]() {});  // aucun effet
    buttons_.push_back(logoBtn);

    Texture* expedTex = new Texture(r, "../assets/img/expeditions.png");
    auto*    expedBtn = new Button(expedTex,nullptr,nullptr, Point{200,300});
    expedBtn->setCallback([this](){
        app_.requestAction(PendingAction::OpenMapSelect);   // ★ remplace l’ancien appel direct
    });
    buttons_.push_back(expedBtn);

    Texture* howTex   = new Texture(r, "../assets/img/howtoplay.png");
    auto*    howBtn   = new Button(howTex,nullptr,nullptr, Point{200,400});
    howBtn->setCallback([](){
        SDL_OpenURL("https://www.konkr.io/how-to-play/");
    });
    buttons_.push_back(howBtn);

    // bouton Quitter 
    Texture* exitTex = new Texture(r, "../assets/img/exit.png");
    auto* exitBtn = new Button(exitTex, nullptr, nullptr, Point{200, 500});
    exitBtn->setCallback([](){
        // Envoie un SDL_QUIT pour sortir proprement de la boucle de Game::run()
        SDL_Event evt;
        evt.type = SDL_QUIT;
        SDL_PushEvent(&evt);
    });
    buttons_.push_back(exitBtn);
}
MainMenu::~MainMenu() { for(auto* b:buttons_) delete b; }
void MainMenu::handleEvent(const SDL_Event& e){
    for(auto* b:buttons_) b->handleEvent(e);
}
void MainMenu::render(SDL_Renderer* r) const{
    for(auto* b:buttons_) b->render(r);
}
