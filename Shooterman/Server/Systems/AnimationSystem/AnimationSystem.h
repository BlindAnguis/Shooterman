#pragma once

#include <memory>
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"

class AnimationSystem : Trace
{
public:
  AnimationSystem();
  ~AnimationSystem();
  static AnimationSystem& get();
  void update();
private:
  ComponentManager<AnimationComponent>* mAnimationComponentManager;
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<HealthComponent>* mHealthComponentManager;
  sf::Clock animationTime;
};