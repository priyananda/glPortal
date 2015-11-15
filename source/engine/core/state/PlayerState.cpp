#include "PlayerState.hpp"
#include <engine/env/Environment.hpp>
#include <PlayerMotion.hpp>

namespace glPortal {

PlayerState::PlayerState() {
  stateFunctionStack.push(&PlayerState::handleInputStanding);
}
void PlayerState::handleInput(Game& game){
  HandleEventFunction stateFunction = stateFunctionStack.top();
  stateFunction(game);
}
void PlayerState::handleInputStanding(Game& game){
  uint32_t updateTime = SDL_GetTicks();
  float dtime = (updateTime-game.getWorld()->lastUpdateTime)/1000.f;
  //  HandleEventFunction stateFunction = stateFunctionStack.top();
  PlayerMotion &playerMotion = game.getWorld()->getPlayer().getComponent<PlayerMotion>();
  bool movingForward = Input::isKeyDown(SDL_SCANCODE_W) or Input::isKeyDown(SDL_SCANCODE_UP);
  if (movingForward){
     playerMotion.moveForward(dtime);
  }
  //  playerMotion.
}
void PlayerState::handleInputRunning(Game& game){}
void PlayerState::handleInputJumping(Game& game){}
}
