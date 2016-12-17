#include <glPortal/system/PortalSystem.hpp>
#include <glPortal/Portal.hpp>

#include <radix/component/Player.hpp>
#include <radix/physics/PhysicsHelper.hpp>

using namespace radix;

namespace glPortal {

PortalSystem::PortalSystem(World &w) :
  System(w) {
}

PortalSystem::~PortalSystem() {
}

void PortalSystem::update(radix::TDelta dtime) {
  (void) dtime;
  Entity &e = world.getPlayer();
  Transform &t = e.getComponent<Transform>();
  const Vector3f &pos = t.getPosition();
  Player &plr = e.getComponent<Player>();
  for (const EntityPair &pp : world.entityPairs.at("portalPairs")) {
    btVector3 min, max;
    Portal &pA = pp.first->getComponent<Portal>(),
      &pB = pp.second->getComponent<Portal>();

    // No uncollider? No portal link.
    if (not pA.object or not pB.object) {
      continue;
    }

    pA.shape->getAabb(
      pA.object->getWorldTransform(),
      min, max);
    // Is player within portal A uncollider (AABB then real check) ?
    if (PhysicsHelper::pointInAABB(pos, min, max) and
        PhysicsHelper::pointInVolume(pos, *pA.object)) {
      // Behind portal A
      const Transform
        &pAT = pp.first->getComponent<Transform>(),
        &pBT = pp.second->getComponent<Transform>();
      Vector3f normalA = pA.getDirection();
      Vector3f AP = pos - pAT.getPosition();
      if (dot(AP, normalA) < 0) {
        Matrix4f rotate180;
        rotate180.rotate(rad(180), 0, 1, 0);
        Matrix4f view = inverse(plr.getBaseHeadOrientation().toMatrix());
        view.translate(-t.getPosition());
        Matrix4f iv = inverse(view *
                              Matrix4f(pAT.getPosition(), pAT.getOrientation()) *
                              rotate180 *
                              inverse(Matrix4f(pBT.getPosition(), pBT.getOrientation())));
        t.setPosition(iv.getPosition());
        plr.headAngle = iv.getRotation().toAero();
      }
    }
    if (pB.shape) {
      pB.shape->getAabb(
        pB.object->getWorldTransform(),
        min, max);
      // Is player within portal B uncollider ?
      if (PhysicsHelper::pointInAABB(pos, min, max) and
          PhysicsHelper::pointInVolume(pos, *pB.object)) {
        // Behind portal B
        const Transform
          &pAT = pp.first->getComponent<Transform>(),
          &pBT = pp.second->getComponent<Transform>();
        Vector3f normalB = pB.getDirection();
        Vector3f BP = pos - pBT.getPosition();
        if (dot(BP, normalB) < 0) {
          Matrix4f rotate180;
          rotate180.rotate(rad(180), 0, 1, 0);
          Matrix4f view = inverse(plr.getBaseHeadOrientation().toMatrix());
          view.translate(-t.getPosition());
          Matrix4f iv = inverse(view *
                                Matrix4f(pBT.getPosition(), pBT.getOrientation()) *
                                rotate180 *
                                inverse(Matrix4f(pAT.getPosition(), pAT.getOrientation())));
          t.setPosition(Vector3f(iv[12], iv[13], iv[14]));
          plr.headAngle = iv.getRotation().toAero();
        }
      }
    }
  }
}

} /* namespace glPortal */
