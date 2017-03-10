#ifndef PORTAL_RENDERER_HPP
#define PORTAL_RENDERER_HPP

#include <memory>
#include <string>

#include <radix/core/math/Vector3f.hpp>
#include <radix/renderer/SubRenderer.hpp>

namespace radix {
  struct RenderContext;
  class Camera;
  class Renderer;
  class Entity;
}

namespace glPortal {

class World;

class PortalRenderer : public radix::SubRenderer{
public:
  PortalRenderer(World& w, radix::Renderer& ren, radix::Camera* cam, double* ptime);

  void render();

private:
  radix::Camera* camera;
  double* dtime;
  double time;
};


} /* namespace glPortal */

#endif /* PORTAL_RENDERER_HPP */
