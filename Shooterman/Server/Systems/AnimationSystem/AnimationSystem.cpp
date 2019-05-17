#include "AnimationSystem.h"

AnimationSystem::AnimationSystem() :
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()) {
  mName = "SERVER: ANIMATION_SYSTEM";
}

AnimationSystem::~AnimationSystem() { TRACE_DEBUG("Enter Destructor"); }

AnimationSystem& AnimationSystem::get() {
  static AnimationSystem instance;
  return instance;
}

void AnimationSystem::update() {
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