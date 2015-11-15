#include "World.hpp"
#include "Game.hpp"
#include <stack>
#include <engine/Entity.hpp>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include "../../../Input.hpp"

namespace glPortal {
typedef std::function<void (Game& game)> HandleEventFunction;
  
class PlayerState
{
private:
  World *world;
  std::stack<HandleEventFunction> stateFunctionStack;
public:
  PlayerState();
  void handleInput(Game& game);
  static void handleInputStanding(Game& game);
  static void handleInputRunning(Game& game);
  static void handleInputJumping(Game& game);
};
} /* namespace glPortal */
