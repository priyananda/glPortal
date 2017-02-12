#include <glPortal/Game.hpp>
#include <glPortal/GameState.hpp>

#include <iostream>
#include <radix/component/Trigger.hpp>
#include <radix/component/Player.hpp>
#include <SDL_opengl.h>
#include <radix/system/ParticleSystem.hpp>

using namespace radix;

namespace glPortal {

Game::Game() {
  windowTitle = "GlPortal";
  defaultMap = "/maps/n1.xml";
}

void Game::initHook() {
  initFunctionStack();
  gameController = std::make_unique<GameController>(this);
  initRenderers();
  addRenderers();

  emitter = &world.entityManager.create();
  emitter->addComponent<Emitter>("hand.png", Vector3f(2.5, 1.0, 5.0), Vector3f(0, 1, 0), Vector3f(0, 2, 0), Vector4f(1, 0, 1, 1), 100, 200, 1, 30);
  world.systems.get<ParticleSystem>().addEmitter(emitter->getComponent<Emitter>());
}

void Game::initFunctionStack() {
  world.stateFunctionStack.push(&GameState::handleRunning);
  world.stateFunctionStack.push(&GameState::handleSplash);
}

void Game::processInput() {
  gameController->processInput();
}

void Game::update() {
  BaseGame::update();
  dtime = (currentTime-lastRender)/1000.;
}

void Game::initRenderers() {
  World& worldReference = static_cast<glPortal::World&>(world);
  radix::Renderer& rendererReference = *renderer.get();
  gameRenderer =
    std::make_unique<GameRenderer>(worldReference, rendererReference, camera.get(), &dtime);
  uiRenderer =
    std::make_unique<UiRenderer>(worldReference, rendererReference);
 }


void Game::addRenderers() {
  renderer->addRenderer(*gameRenderer);
  renderer->addRenderer(*uiRenderer);
  renderer->addRenderer(*particleRenderer);
}

} /* namespace glPortal */
