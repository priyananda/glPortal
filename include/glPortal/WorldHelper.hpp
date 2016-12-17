#ifndef WORLD_HELPER_HPP
#define WORLD_HELPER_HPP

#include <utility>

#include <radix/World.hpp>

namespace glPortal {

class World;

class WorldHelper {
public:
  static void shootPortal(int button, World &world);
  static radix::EntityPair& getPortalPair(int pair, World &world);
  static radix::EntityPair& initPortalPair(int pair, World &world);
  static void closePortals(World &world);
};

} /* namespace glPortal */

#endif /* WORLD_HELPER_HPP */
