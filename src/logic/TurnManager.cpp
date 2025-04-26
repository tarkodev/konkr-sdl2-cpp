#include "TurnManager.hpp"

TurnManager::TurnManager(const std::vector<Player*>& players)
    : players_(players),
      currentPlayerIndex_(0),
      roundNumber_(1),
      phase_(TurnPhase::GameStart) {}

void TurnManager::update() {
    switch (phase_) {
        case TurnPhase::GameStart:
            phase_ = TurnPhase::PlayerTurnStart;
            break;

        case TurnPhase::PlayerTurnStart:
            phase_ = TurnPhase::PlayerTurnEnd;
            //phase_ = TurnPhase::PlayerTurnAction; à faire
            break;

        case TurnPhase::PlayerTurnEnd:
            if (players_.empty()) {
                phase_ = TurnPhase::GameEnd;
            } else {
                if (currentPlayerIndex_ + 1 >= static_cast<int>(players_.size())) {
                    currentPlayerIndex_ = 0;
                    ++roundNumber_;
                } else {
                    ++currentPlayerIndex_;
                }
                phase_ = TurnPhase::PlayerTurnStart;
            }
            break;

        case TurnPhase::GameEnd:
            // Partie terminée, plus d'avancement
            break;
    }
}

TurnPhase TurnManager::getPhase() const {
    return phase_;
}

int TurnManager::getRoundNumber() const {
    return roundNumber_;
}

int TurnManager::getCurrentPlayerIndex() const {
    return currentPlayerIndex_;
}

Player* TurnManager::getCurrentPlayer() const {
    if (players_.empty()) return nullptr;
    return players_[currentPlayerIndex_];
}

void TurnManager::endGame() {
    phase_ = TurnPhase::GameEnd;
}
