#include <glPortal/GameController.hpp>
#include <glPortal/Game.hpp>

#include <radix/env/Environment.hpp>
#include <radix/component/Player.hpp>
#include <glPortal/GameState.hpp>

namespace glPortal {

GameController::GameController(Game *game) {
  this->game = game;
  this->world = static_cast<World*>(game->getWorld());
  this->gameState = std::make_unique<radix::GameState>();
  GameState::init(*game);
  initObservers();
}

void GameController::initObservers() {
  this->closeWindowHolder = game->getWorld()->event.addObserver(
    radix::InputSource::WindowCloseEvent::Type, [this](const radix::Event& event) {
      this->game->close();
    });
  this->mouseButtonDownHolder = game->getWorld()->event.addObserver(
    radix::InputSource::MouseButtonPressedEvent::Type, [this](const radix::Event& event) {
      radix::InputSource::MouseButtonPressedEvent& mouseEvent =
        (radix::InputSource::MouseButtonPressedEvent&) event;
      if(mouseEvent.button == radix::InputSource::MouseButton::Left) {
        game->getWorld()->shootPortal(1);
      }
      if(mouseEvent.button == radix::InputSource::MouseButton::Right) {
        game->getWorld()->shootPortal(2);
      }
    });
}

void GameController::processInput() {
  game->getWindow().processEvents();
  if (game->getWindow().isKeyDown(SDL_SCANCODE_Q)) {
    game->close();
  }

  gameState->handleInput(*game);
}

} /* namespace glPortal */
