#include "PlayerMotion.hpp"

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/env/Environment.hpp>
#include <engine/component/Health.hpp>
#include <engine/component/SoundSource.hpp>
#include <Input.hpp>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_keyboard.h>
#include <World.hpp>
#include <cmath>

namespace glPortal {

static const float FRICTION = 0.001;
static const float MIN_SPEED_ON_AXIS = FRICTION;

// Movement
void PlayerMotion::mouseLook() {
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);

  // Apply mouse movement to view
  Vector3f &rotation = entity.getComponent<Transform>().rotation;
  rotation.x -= rad(mousedy * Environment::getConfig().getSensitivity());
  rotation.y -= rad(mousedx * Environment::getConfig().getSensitivity());

  // Restrict rotation in horizontal axis
  rotation.x = Math::clamp(rotation.x, rad(-90.0f), rad(90.0f));
}
  
void PlayerMotion::applyGravity(float dtime) {  
  if (not grounded) {
    if (flying) {
      if (velocity.y <= MIN_SPEED_ON_AXIS and velocity.y >= -MIN_SPEED_ON_AXIS) {
        velocity.y = 0;
      } else {
        velocity.y *= std::pow(FRICTION, dtime);
      }
    } else {
      velocity.y -= World::gravity * dtime;
    }
  }
}
void PlayerMotion::applyMinimaMaximaToVelocity(float dtime) {
  if (velocity.x <= MIN_SPEED_ON_AXIS and velocity.x >= -MIN_SPEED_ON_AXIS) {
    velocity.x = 0;
  } else {
    velocity.x *= std::pow(FRICTION, dtime);
  }
  if (velocity.z <= MIN_SPEED_ON_AXIS and velocity.z >= -MIN_SPEED_ON_AXIS) {
    velocity.z = 0;
  } else {
    velocity.z *= std::pow(FRICTION, dtime);
  }
}
  
void PlayerMotion::moveForward(float dtime) {
  Transform &tform = entity.getComponent<Transform>();
  float rot = tform.rotation.y;
  Vector3f tmpVel;
  
  tmpVel.x -= sin(rot);
  tmpVel.z -= cos(rot);
  tmpVel *= (speed * dtime);
  //tmpVel *= speed;

  if (velocity.x + tmpVel.x > 0.1) {
    velocity.x = 0;
  } else {
    velocity.x += tmpVel.x;
  }

  if (velocity.z + tmpVel.z > 0.1) {
    velocity.z = 0;
  } else {
    velocity.z += tmpVel.z;
  }
  // applyMinimaMaximaToVelocity(dtime);
}
  
void PlayerMotion::move(float dtime) {
  bool movingFwd     = Input::isKeyDown(SDL_SCANCODE_W) or Input::isKeyDown(SDL_SCANCODE_UP),
       movingBack    = Input::isKeyDown(SDL_SCANCODE_S) or Input::isKeyDown(SDL_SCANCODE_DOWN),
       strafingLeft  = Input::isKeyDown(SDL_SCANCODE_A) or Input::isKeyDown(SDL_SCANCODE_LEFT),
       strafingRight = Input::isKeyDown(SDL_SCANCODE_D) or Input::isKeyDown(SDL_SCANCODE_RIGHT);
  // applyMinimaMaximaToVelocity(dtime);
  applyGravity(dtime);
  return;
  if (velocity.x <= MIN_SPEED_ON_AXIS and velocity.x >= -MIN_SPEED_ON_AXIS) {
    velocity.x = 0;
  } else {
    velocity.x *= std::pow(FRICTION, dtime);
  }
  if (velocity.z <= MIN_SPEED_ON_AXIS and velocity.z >= -MIN_SPEED_ON_AXIS) {
    velocity.z = 0;
  } else {
    velocity.z *= std::pow(FRICTION, dtime);
  }
  if (not grounded) {
    if (flying) {
      if (velocity.y <= MIN_SPEED_ON_AXIS and velocity.y >= -MIN_SPEED_ON_AXIS) {
        velocity.y = 0;
      } else {
        velocity.y *= std::pow(FRICTION, dtime);
      }
    } else {
      velocity.y -= World::gravity * dtime;
    }
  }

  Transform &tform = entity.getComponent<Transform>();
  float rot = tform.rotation.y;
  if (entity.getComponent<Health>().isAlive()) {
    Vector3f tmpVel;
    double yMult = flying ? cos(tform.rotation.x) : 1;
    if (movingFwd) {
      //      tmpVel.x -= sin(rot) * yMult;
      //      tmpVel.z -= cos(rot) * yMult;
      //      if (flying) {
      //        tmpVel.y += sin(tform.rotation.x);
      //      }
    }
    if (movingBack) {
      tmpVel.x += sin(rot) * yMult;
      tmpVel.z += cos(rot) * yMult;
    }
    if (strafingLeft) {
      tmpVel.x += -cos(rot);
      tmpVel.z += sin(rot);
    }
    if (strafingRight) {
      tmpVel.x += cos(rot);
      tmpVel.z += -sin(rot);
    }
    float tmpVelLen = tmpVel.length();
    if (tmpVelLen > 0) {
      tmpVel.normalise();
    }
    tmpVel *= speed;

  }
}

} /* namespace glPortal */
