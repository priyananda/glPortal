#ifndef PORTAL_HPP
#define PORTAL_HPP

#include <memory>
#include <bullet/BulletCollision/btBulletCollisionCommon.h>

#include <radix/component/Component.hpp>
#include <radix/core/math/Vector3f.hpp>
#include <radix/core/math/Quaternion.hpp>
#include <radix/material/Material.hpp>
#include <radix/model/Mesh.hpp>

namespace glPortal {

class Portal : public radix::Component {
public:
  static const int PORTAL_RANGE;
  static const radix::Vector3f BLUE_COLOR;
  static const radix::Vector3f ORANGE_COLOR;
  static const double NOISE_FADE_DELAY;
  static const double OPEN_ANIM_DURATION;
  static const float SURFACE_OFFSET;

  Portal(radix::Entity &ent);
  ~Portal();

  const char* getName() const {
    return "Player";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver &archiver);

  radix::Vector3f getDirection() const;

  void placeOnWall(const radix::Vector3f &launchPos, const radix::Vector3f &point, const radix::Vector3f &normal);

  radix::Vector3f getScaleMult() const;

  radix::Material overlayTex, maskTex;
  double openSince;
  radix::Mesh overlayMesh, stencilMesh;
  radix::Vector3f direction, color;
  bool open;
  std::unique_ptr<btDefaultMotionState> motionState;
  std::unique_ptr<btCollisionShape> shape;
  std::unique_ptr<btCollisionObject> object;
  struct Wrapper {
    std::unique_ptr<btCollisionShape> vertShape, horzShape;
    struct Side {
      std::unique_ptr<btRigidBody> body;
      std::unique_ptr<btDefaultMotionState> motionState;
    } top, right, bottom, left;
  } wrapper;

private:
  // TODO: move this to PortalSystem v
  void placeWrapperPiece(const radix::Vector3f &p, const radix::Quaternion &o, const radix::Vector3f &s,
    const std::unique_ptr<btCollisionShape> &shape, Wrapper::Side &side, const radix::Vector3f &offset);
};

} /* namespace glPortal */

#endif /* PORTAL_HPP */
