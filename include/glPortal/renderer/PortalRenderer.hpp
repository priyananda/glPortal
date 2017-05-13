#ifndef GLPORTAL_PORTAL_RENDERER_HPP
#define GLPORTAL_PORTAL_RENDERER_HPP

#include <glPortal/World.hpp>

#include <radix/renderer/SubRenderer.hpp>

namespace radix {

class Camera;
class Entity;

} /* namespace radix */

namespace glPortal {

class Game;

class PortalRenderer : public radix::SubRenderer {
public:
  PortalRenderer(World &world, radix::Renderer &renderer, Game &game);

  void render();
private:
  Game& game;

  void renderPortal (const radix::Camera &camera, const radix::Entity &portal,
                     const radix::Entity &otherPortal);
  void renderPortalStencil (const radix::Camera &camera, const radix::Entity &portal);
  void renderPortalOverlay(const radix::Camera &camera, const radix::Entity &portal);
  void renderPortalNoise(const radix::Camera &camera, const radix::Entity &portal, float fade);
};

} /* namsepace glPortal */
#endif /* GLPORTAL_PORTAL_RENDERER_HPP */
