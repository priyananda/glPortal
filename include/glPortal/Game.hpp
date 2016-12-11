#ifndef GAME_HPP
#define GAME_HPP

#include <glPortal/World.hpp>
#include <glPortal/GameController.hpp>
#include <glPortal/renderer/GameRenderer.hpp>
#include <glPortal/renderer/UiRenderer.hpp>
#include <glPortal/renderer/PortalRenderer.hpp>

#include <radix/Window.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/env/Config.hpp>
#include <radix/BaseGame.hpp>

namespace glPortal {

class Game : public radix::BaseGame {
public:
  Game();
  void processInput();
  void update();

  inline World* getWorld() {
    return static_cast<World*>(&world);
  }

  inline GameController& getGameController() {
    return *gameController;
  }

  inline GameRenderer& getGameRenderer() {
    return *gameRenderer;
  }
private:
  std::unique_ptr<GameController> gameController;
  std::unique_ptr<PortalRenderer> portalRenderer;
  std::unique_ptr<GameRenderer> gameRenderer;
  std::unique_ptr<UiRenderer> uiRenderer;
  void initHook();
  double dtime;
};

} /* namespace glPortal */

#endif /* GAME_HPP */
