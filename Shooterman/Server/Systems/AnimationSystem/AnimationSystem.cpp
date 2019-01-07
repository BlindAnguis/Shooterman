#include "AnimationSystem.h"

AnimationSystem::AnimationSystem() {}

AnimationSystem::AnimationSystem(
  ComponentManager<AnimationComponent>* animationComponentManager,
  ComponentManager<VelocityComponent>* velocityComponentManager,
  ComponentManager<RenderComponent>* renderComponentManager
) :
  mAnimationComponentManager(animationComponentManager),
  mVelocityComponentManager(velocityComponentManager),
  mRenderComponentManager(renderComponentManager)
{
  mName = "SERVER: ANIMATION_SYSTEM";
}

AnimationSystem::~AnimationSystem() {}
// TODO: Add a way to create animations....i.e. x.addAnimation("someNameOrEnum"). x.addFrameToAnimation("theAnimationNameOrEnum", sf::IntRect("The positions needed to get the right sprite from a sprite sheet")).
// TODO: An animation is just a vector with frames that contains the sprite position (a sf::IntRect). The vector should exist in the AnimationComponent I think...as a map of animations or something.
void AnimationSystem::update()
{
  for (auto entity : mAnimationComponentManager->getAllEntitiesWithComponent()) {
    //TRACE_INFO(static_cast<int>(entity.second->animation));
    if (mRenderComponentManager->hasComponent(entity.first)) {
      auto render = mRenderComponentManager->getComponent(entity.first);
      if (entity.second->animation == Animations::RunningRight) {
        //TRACE_INFO("Running right");
        sf::IntRect textureRect = render->sprite.getTextureRect();
        textureRect.left = 95 * entity.second->animationFrame;
        textureRect.top = 0;
        textureRect.width = 95;
        textureRect.height = 95;
        render->sprite.setTextureRect(textureRect);
      }
      else if (entity.second->animation == Animations::RunningLeft) {
        //TRACE_INFO("Running left");
        sf::IntRect textureRect = render->sprite.getTextureRect();
        textureRect.left = 95 * entity.second->animationFrame;
        textureRect.top = 0;
        textureRect.width = 95;
        textureRect.height = 95;
        render->sprite.setTextureRect(textureRect);
      }
      else if (entity.second->animation == Animations::RunningDown) {
        //TRACE_INFO("Running down");
        sf::IntRect textureRect = render->sprite.getTextureRect();
        textureRect.left = 95 * entity.second->animationFrame;
        textureRect.top = 0;
        textureRect.width = 95;
        textureRect.height = 95;
        render->sprite.setTextureRect(textureRect);
      }
      else if (entity.second->animation == Animations::RunningUp) {
        //TRACE_INFO("Running up");
        sf::IntRect textureRect = render->sprite.getTextureRect();
        textureRect.left = 95 * entity.second->animationFrame;
        textureRect.top = 0;
        textureRect.width = 95;
        textureRect.height = 95;
        render->sprite.setTextureRect(textureRect);
      } else {
        sf::IntRect textureRect = render->sprite.getTextureRect();
        //textureRect.left = 0 * entity.second->animationFrame;
        //textureRect.top = 305;
        //textureRect.width = 95;
        //textureRect.height = 75;
        render->sprite.setTextureRect(textureRect);
      }
    }
    entity.second->animationFrame == 2 ? entity.second->animationFrame = 0 : entity.second->animationFrame++;
  }
}