#include <glPortal/renderer/PortalRenderer.hpp>
#include <glPortal/Portal.hpp>
#include <glPortal/Game.hpp>

#include <epoxy/gl.h>

#include <radix/shader/ShaderLoader.hpp>
#include <radix/model/MeshLoader.hpp>
#include <radix/core/math/Matrix3f.hpp>
#include <radix/env/Environment.hpp>

namespace glPortal {

PortalRenderer::PortalRenderer(World &world, radix::Renderer &renderer, Game &game)
  : SubRenderer(world, renderer), game(game) {

}

void PortalRenderer::render() {
  /* pass 1 */
  for (radix::EntityPair entityPair : world.entityPairs.at("portalPairs")) {
    radix::Entity &portalEntity1 = *entityPair.first,
                  &portalEntity2 = *entityPair.second;
    Portal &portal1 = portalEntity1.getComponent<Portal>(),
           &portal2 = portalEntity2.getComponent<Portal>();

    renderPortal(*camera, portalEntity1, portalEntity2);
    renderPortal(*camera, portalEntity2, portalEntity1);

    if (portal1.open and portal2.open) {
      /* bit of an ironic name */
      radix::Shader &unshaded = radix::ShaderLoader::getShader("unshaded.frag");

      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);

      const radix::Mesh &portalStencil = radix::MeshLoader::getMesh("PortalStensil.obj");

      radix::Matrix4f matrix;
      portalEntity1.getComponent<radix::Transform>().getModelMtx(matrix);
      matrix.scale(portal1.getScaleMult());
      renderer.renderMesh(*renderContext, unshaded, matrix, portalStencil);

      portalEntity2.getComponent<radix::Transform>().getModelMtx(matrix);
      matrix.scale(portal2.getScaleMult());
      renderer.renderMesh(*renderContext, unshaded, matrix, portalStencil);

      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
  }

  /* pass 2 */
  for (radix::EntityPair entityPair : world.entityPairs.at("portalPairs")) {
    radix::Entity &portalEntity1 = *entityPair.first,
                  &portalEntity2 = *entityPair.second;
    Portal &portal1 = portalEntity1.getComponent<Portal>(),
           &portal2 = portalEntity2.getComponent<Portal>();

    if (portal1.open and portal2.open) {
      double doubleTimeOpen = world.getTime() - std::max(portal1.openSince, portal2.openSince);
      if (doubleTimeOpen < Portal::NOISE_FADE_DELAY) {
        float fade = 1 - (float)( doubleTimeOpen / Portal::NOISE_FADE_DELAY) * 2;
        renderPortalNoise(*camera, portalEntity1, fade);
        renderPortalNoise(*camera, portalEntity2, fade);
      }
    } else {
      if (portal1.open) {
        renderPortalNoise(*camera, portalEntity1, 1.f);
      }
      if (portal2.open) {
        renderPortalNoise(*camera, portalEntity2, 1.f);
      }
    }

    renderPortalOverlay(*camera, portalEntity1);
    renderPortalOverlay(*camera, portalEntity2);
  }
}

void PortalRenderer::renderPortal(const radix::Camera &camera, const radix::Entity &portal,
                                  const radix::Entity &otherPortal) {
  if (portal.getComponent<Portal>().open and otherPortal.getComponent<Portal>().open) {
    radix::Transform &point      = portal.getComponent<radix::Transform>();
    radix::Transform &otherPoint = otherPortal.getComponent<radix::Transform>();
    radix::Vector3f displacement = point.getPosition() - otherPoint.getPosition();
    int recursionLevel = radix::Environment::getConfig().getRecursionLevel();
    radix::Camera portalCamera;

    radix::Vector3f portalPosition = point.getPosition();
    radix::Quaternion portalOrientation = point.getOrientation();
    radix::Vector3f portalScale = point.getScale();
    radix::Vector3f portalDirection = portal.getComponent<Portal>().getDirection();
    radix::Vector3f otherPortalDirection = otherPortal.getComponent<Portal>().getDirection();
    float dotProduct = dot(portalDirection, otherPortalDirection);

    if (abs(dotProduct) < 0.0001) {
      radix::Quaternion rotationDifference = conjugate(point.getOrientation() *
                                                         conjugate(otherPoint.getOrientation()));
      displacement = radix::toMatrix3f(rotationDifference.toMatrix()).transform(displacement);

      recursionLevel = 1;
    } else if (dotProduct == 1.0f) {
      recursionLevel = 0;
    }

    point.setPosition(point.getPosition() + displacement * (float) recursionLevel);
    for (int i = 0; i < recursionLevel; i++) {
      glEnable(GL_STENCIL_TEST);
      glClear(GL_STENCIL_BUFFER_BIT);
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_FALSE);

      renderPortalStencil(camera, portal);

      glStencilFunc(GL_EQUAL, 1, 0xFF);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

      GameRenderer::setCameraInPortal(camera, portalCamera, portal, otherPortal);
      renderContext->pushCamera(portalCamera);
      game.getGameRenderer().renderPlayer(*renderContext);
      game.getGameRenderer().renderScene(*renderContext);
      renderContext->popCamera();

      radix::Vector3f temporaryPosition = point.getPosition();
      point.setPosition(portalPosition);
      renderPortalOverlay(portalCamera, portal);
      point.setPosition(temporaryPosition);

      point.setPosition(point.getPosition() - displacement);

      point.setOrientation(portalOrientation);
      GameRenderer::setCameraInPortal(camera, portalCamera, portal, otherPortal);
      glDisable(GL_STENCIL_TEST);

      radix::Shader &unshaded = radix::ShaderLoader::getShader("unshaded.frag");
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);
      const radix::Mesh &portalStencil = radix::MeshLoader::getMesh("PortalStencil.obj");

      radix::Matrix4f matrix;
      matrix.translate(portalPosition);
      matrix.rotate(portalOrientation);
      matrix.scale(portalScale);
      matrix.scale(portal.getComponent<Portal>().getScaleMult());
      renderContext->pushCamera(portalCamera);
      renderer.renderMesh(*renderContext, unshaded, matrix, portalStencil);
      renderContext->popCamera();
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    glEnable(GL_STENCIL_TEST);

    renderPortalStencil(camera, portal);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    GameRenderer::setCameraInPortal(camera, portalCamera, portal, otherPortal);
    renderContext->pushCamera(portalCamera);
    game.getGameRenderer().renderPlayer(*renderContext);
    game.getGameRenderer().renderScene(*renderContext);
    renderContext->popCamera();
    renderPortalOverlay(portalCamera, portal);
    glDisable(GL_STENCIL_TEST);
  }
}

void PortalRenderer::renderPortalStencil(const radix::Camera &camera,
                                         const radix::Entity &portal) {
  glClear(GL_STENCIL_BUFFER_BIT);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glStencilFunc(GL_NEVER, 1, 0xFF);
  glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

  const Portal &portalComponent = portal.getComponent<Portal>();
  radix::Matrix4f matrix;
  portal.getComponent<radix::Transform>().applyModelMtx(matrix);
  matrix.scale(portalComponent.getScaleMult());

  renderContext->pushCamera(camera);
  renderer.renderMesh(*renderContext, radix::ShaderLoader::getShader("unshaded.frag"), matrix,
                      portalComponent.stencilMesh);
  renderContext->popCamera();

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
}

void PortalRenderer::renderPortalOverlay(const radix::Camera &camera,
                                         const radix::Entity &portal) {
  const Portal &portalComponent = portal.getComponent<Portal>();
  if (portalComponent.open) {
    radix::Matrix4f matrix;
    portal.getComponent<radix::Transform>().getModelMtx(matrix);
    matrix.scale(portalComponent.getScaleMult());

    renderContext->pushCamera(camera);
    renderer.renderMesh(*renderContext, radix::ShaderLoader::getShader("unshaded.frag"), matrix,
                        portalComponent.overlayMesh, portalComponent.overlayTex);
    renderContext->popCamera();
  }
}

void PortalRenderer::renderPortalNoise(const radix::Camera &camera, const radix::Entity &portal,
                                       float fade) {
  radix::Matrix4f matrix;
  portal.getComponent<radix::Transform>().getModelMtx(matrix);

  const Portal &portalComponent = portal.getComponent<Portal>();
  matrix.scale(portalComponent.getScaleMult());

  radix::Shader &simplexTime = radix::ShaderLoader::getShader("simplexTime.frag");
  simplexTime.bind();
  int timeLocation = simplexTime.uni("time");
  int colorLocation = simplexTime.uni("color");
  int noiseAlphaLocation = simplexTime.uni("noiseAlpha");
  glUniform1f(timeLocation, world.getTime());
  glUniform3f(colorLocation, portalComponent.color.x, portalComponent.color. y,
              portalComponent.color.z);
  glUniform1f(noiseAlphaLocation, fade);

  renderContext->pushCamera(camera);
  renderer.renderMesh(*renderContext, simplexTime, matrix, portalComponent.overlayMesh,
                      portalComponent.maskTex);
  renderContext->popCamera();
}

} /* namespace glPortal */