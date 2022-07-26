#ifndef URPLUSPLUS_GAMEOBSERVER_H_
#define URPLUSPLUS_GAMEOBSERVER_H_

class GameState;

class GameObserver {
    virtual void doNotify(const GameState&) = 0 ;
  public:
    void notify(const GameState&);

    virtual ~GameObserver();
};

#endif
