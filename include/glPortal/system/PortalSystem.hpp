#ifndef PORTAL_SYSTEM_HPP
#define PORTAL_SYSTEM_HPP

#include <radix/system/System.hpp>

namespace glPortal {

class PortalSystem : public radix::System {
public:
  PortalSystem(radix::World&);
  ~PortalSystem();

  const char* getName() const {
    return "PhysicsSystem";
  }

  TypeId getTypeId() const {
    return System::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void update(radix::TDelta dtime);
};

} /* namespace glPortal */

#endif /* PORTAL_SYSTEM_HPP */
