#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"

class AnimationSystem : Trace
{
public:
  AnimationSystem();
  AnimationSystem(ComponentManager<AnimationComponent>* animationComponentManager, 
                  ComponentManager<VelocityComponent>* velocityComponentManager,
                  ComponentManager<RenderComponent>* renderComponentManager);
  ~AnimationSystem();
  void update();
private:
  ComponentManager<AnimationComponent>* mAnimationComponentManager;
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<RenderComponent>* mRenderComponentManager;
};