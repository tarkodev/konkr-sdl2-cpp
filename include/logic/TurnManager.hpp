#pragma once

#include <vector>
#include "Player.hpp"

// Phases de gestion de tour : début de partie, début/fin de chaque tour de joueur, fin de partie
enum class TurnPhase {
    GameStart,
    PlayerTurnStart,
    PlayerTurnAction,
    PlayerTurnEnd,
    GameEnd
};

class TurnManager {
public:
    explicit TurnManager(const std::vector<Player*>& players);

    // Fait avancer la phase du tour
    void update();

    // Accesseurs
    TurnPhase getPhase() const;
    int getRoundNumber() const;
    int getCurrentPlayerIndex() const;
    Player* getCurrentPlayer() const;

    // Forcer la fin de partie
    void endGame();

private:
    std::vector<Player*> players_;
    int currentPlayerIndex_;
    int roundNumber_;
    TurnPhase phase_;
};