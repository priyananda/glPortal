#ifndef WORLD_HPP
#define WORLD_HPP

#include <radix/World.hpp>
#include <fstream>

namespace glPortal {

class Screen;

class World : public radix::World {
public:
  World(radix::InputSource &input);
  ~World();

  void shootPortal(int portal);
  void hidePortals();
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
