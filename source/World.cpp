#include <glPortal/World.hpp>
#include <glPortal/WorldHelper.hpp>

using namespace radix;

namespace glPortal {

World::World(InputSource &input) :
  radix::World(input) {
}

World::~World() = default;

void World::shootPortal(int portal) {
  WorldHelper::shootPortal(portal, *this);
}

void World::hidePortals() {
  WorldHelper::closePortals(*this);
}

} /* namespace glPortal */
