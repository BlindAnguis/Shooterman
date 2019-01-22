#include "AnimationSystem.h"

AnimationSystem::AnimationSystem() {}

AnimationSystem::AnimationSystem(
  ComponentManager<AnimationComponent>* animationComponentManager,
  ComponentManager<VelocityComponent>* velocityComponentManager,
  ComponentManager<RenderComponent>* renderComponentManager,
  ComponentManager<HealthComponent>* healthComponent
) :
  mAnimationComponentManager(animationComponentManager),
  mVelocityComponentManager(velocityComponentManager),
  mRenderComponentManager(renderComponentManager),
  mHealthComponent(healthComponent) {
  mName = "SERVER: ANIMATION_SYSTEM";
}

AnimationSystem::~AnimationSystem() {}
// TODO: Add a way to create animations....i.e. x.addAnimation("someNameOrEnum"). x.addFrameToAnimation("theAnimationNameOrEnum", sf::IntRect("The positions needed to get the right sprite from a sprite sheet")).
// TODO: An animation is just a vector with frames that contains the sprite position (a sf::IntRect). The vector should exist in the AnimationComponent I think...as a map of animations or something.
void AnimationSystem::update()
{
  for (auto entity : mAnimationComponentManager->getAllEntitiesWithComponent()) {
    if (mHealthComponent->getComponent(entity.first)->isAlive) {
      //TRACE_INFO(static_cast<int>(entity.second->animation));
      if (mRenderComponentManager->hasComponent(entity.first)) {
        auto render = mRenderComponentManager->getComponent(entity.first);
        if (entity.second->animation == Animations::RunningRight) {
          //TRACE_INFO("Running right");
          sf::IntRect textureRect = render->sprite.getTextureRect();
          textureRect.left = 64 * entity.second->animationFrame + 14;
          textureRect.top = 206;
          textureRect.width = 36;
          textureRect.height = 50;
          render->sprite.setTextureRect(textureRect);
        }
        else if (entity.second->animation == Animations::RunningLeft) {
          //TRACE_INFO("Running left");
          sf::IntRect textureRect = render->sprite.getTextureRect();
          textureRect.left = 64 * entity.second->animationFrame + 14;
          textureRect.top = 78;
          textureRect.width = 36;
          textureRect.height = 50;
          render->sprite.setTextureRect(textureRect);
        }
        else if (entity.second->animation == Animations::RunningDown) {
          //TRACE_INFO("Running down");
          sf::IntRect textureRect = render->sprite.getTextureRect();
          textureRect.left = 64 * entity.second->animationFrame + 14;
          textureRect.top = 142;
          textureRect.width = 36;
          textureRect.height = 50;
          render->sprite.setTextureRect(textureRect);
        }
        else if (entity.second->animation == Animations::RunningUp) {
          //TRACE_INFO("Running up");
          sf::IntRect textureRect = render->sprite.getTextureRect();
          textureRect.left = 64 * entity.second->animationFrame + 14;
          textureRect.top = 14;
          textureRect.width = 36;
          textureRect.height = 50;
          render->sprite.setTextureRect(textureRect);
        }
        else {
          sf::IntRect textureRect = render->sprite.getTextureRect();
          textureRect.left = 0;
          render->sprite.setTextureRect(textureRect);
        }
      }
      if (animationTime.getElapsedTime().asMilliseconds() > 35) {
        entity.second->animationFrame == 8 ? entity.second->animationFrame = 0 : entity.second->animationFrame++;
        animationTime.restart();
      }
    } else {
      auto render = mRenderComponentManager->getComponent(entity.first);
      auto pos = render->sprite.getPosition();
      render->sprite = sf::Sprite(render->deathTexture, sf::IntRect(0, 0, 64, 64));
      render->textureId = Textures::Tombstone;
      render->sprite.setPosition(pos.x, pos.y);
      render->sprite.setOrigin(32, 32);
    }
  }
}