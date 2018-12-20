#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"

class AnimationSystem
{
public:
  AnimationSystem();
  AnimationSystem(ComponentManager<AnimationComponent>* animationComponentManager, ComponentManager<VelocityComponent>* velocityComponentManager);
  ~AnimationSystem();
  void update();
private:
  ComponentManager<AnimationComponent>* mAnimationComponentManager;
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
};