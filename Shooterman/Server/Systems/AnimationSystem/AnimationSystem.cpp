#include "AnimationSystem.h"

AnimationSystem::AnimationSystem() {}

AnimationSystem::AnimationSystem(
  ComponentManager<AnimationComponent>* animationComponentManager,
  ComponentManager<VelocityComponent>* velocityComponentManager,
  ComponentManager<RenderComponent>* renderComponentManager,
  ComponentManager<HealthComponent>* healthComponentManager
) :
  mAnimationComponentManager(animationComponentManager),
  mVelocityComponentManager(velocityComponentManager),
  mRenderComponentManager(renderComponentManager),
  mHealthComponentManager(healthComponentManager)
{
  mName = "SERVER: ANIMATION_SYSTEM";
}

AnimationSystem::~AnimationSystem() {}

void AnimationSystem::update()
{
  for (auto entity : mAnimationComponentManager->getAllEntitiesWithComponent()) {
    auto entityHealth = mHealthComponentManager->getComponent(entity.first);
    if (entityHealth && !entityHealth->isAlive) {
      entity.second->currentAnimation = AnimationType::Death;
    }

    //TRACE_INFO("CurrentAnimation: " << (int)entity.second->currentAnimation);
    auto animation = entity.second->animations.find(entity.second->currentAnimation);
    if (animation != entity.second->animations.end()) {
      animation->second.play();
    }
  }
}