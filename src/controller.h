#ifndef URPLUSPLUS_CONTROLLER_H_
#define URPLUSPLUS_CONTROLLER_H_

#include <vector>

class GameViewer;
class Player;
class GameState;

class Controller {
    std::vector<GameViewer*> observers;
    GameState* gameState;
    std::vector<Player*> players;
    void notifyObservers();
  public:
    void registerObserver(GameViewer*);
    void makePlayerTurn();
    // const GameState& getState() const;
    Controller(GameState* gameState, std::vector<Player*> players);
    virtual ~Controller();
};

#endif
