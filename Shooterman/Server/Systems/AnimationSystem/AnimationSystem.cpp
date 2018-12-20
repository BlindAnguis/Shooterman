#include "AnimationSystem.h"

AnimationSystem::AnimationSystem() {}

AnimationSystem::AnimationSystem(
  ComponentManager<AnimationComponent>* animationComponentManager,
  ComponentManager<VelocityComponent>* velocityComponentManager
) :
  mAnimationComponentManager(animationComponentManager),
  mVelocityComponentManager(velocityComponentManager)
{}

AnimationSystem::~AnimationSystem() {}

void AnimationSystem::update()
{
}