#include <glPortal/Game.hpp>
#include <glPortal/GameState.hpp>

#include <iostream>
#include <radix/component/Trigger.hpp>
#include <radix/component/Player.hpp>
#include <glPortal/system/PortalSystem.hpp>

using namespace radix;

namespace glPortal {

Game::Game() {
  windowTitle = "GlPortal";
  defaultMap = "/maps/n1.xml";
}

void Game::initHook() {
  world.stateFunctionStack.push(&GameState::handleRunning);
  world.stateFunctionStack.push(&GameState::handleSplash);
  gameController = std::make_unique<GameController>(this);
  World& worldReference = *(getWorld());
  worldReference.entityPairs.insert(std::make_pair("portalPairs", std::vector<EntityPair>()));
  worldReference.systems.add<PortalSystem>();
  radix::Renderer& rendererReference = *renderer.get();
  gameRenderer =
    std::make_unique<GameRenderer>(worldReference, rendererReference, camera.get(), &dtime);
  portalRenderer =
    std::make_unique<PortalRenderer>(worldReference, rendererReference, *this);
  uiRenderer =
    std::make_unique<UiRenderer>(worldReference, rendererReference);

  renderer->addRenderer(*gameRenderer);
  renderer->addRenderer(*portalRenderer);
  renderer->addRenderer(*uiRenderer);
}

void Game::processInput() {
  gameController->processInput();
}

void Game::update() {
  BaseGame::update();
  dtime = (currentTime-lastRender)/1000.;
}

} /* namespace glPortal */
