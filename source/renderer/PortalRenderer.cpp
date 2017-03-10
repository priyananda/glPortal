#include <glPortal/renderer/PortalRenderer.hpp>
#include <glPortal/World.hpp>

#include <radix/core/gl/OpenGL.hpp>

#include <radix/renderer/Renderer.hpp>
#include <radix/Viewport.hpp>
#include <radix/component/ViewFrame.hpp>
#include <radix/component/MeshDrawable.hpp>
#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/data/model/MeshLoader.hpp>
#include <radix/data/material/MaterialLoader.hpp>

using namespace radix;

namespace glPortal {

PortalRenderer::PortalRenderer(glPortal::World& w, radix::Renderer& ren, radix::Camera* cam, double* ptime) :
  SubRenderer(w, ren),
  camera(cam),
  dtime(ptime){
}

void PortalRenderer::render() {
}
} /* namespace glPortal */
