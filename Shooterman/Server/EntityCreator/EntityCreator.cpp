#include "EntityCreator.h"
#include "../Systems/CollisionSystem/Collision.h"

#define MAGE_MAX_VELOCITY 4.5f
#define KNIGHT_MAX_VELOCITY 4.8f
#define SPEARMAN_MAX_VELOCITY 4.65f

EntityCreator::EntityCreator(
  EntityManager *entityManager,
  ComponentManager<RenderComponent> *renderComponentManager,
  ComponentManager<VelocityComponent> *velocityComponentManager,
  ComponentManager<CollisionComponent> *collisionComponentManager,
  ComponentManager<AnimationComponent> *animationComponentManager,
  ComponentManager<HealthComponent> *healthComponentManager,
  ComponentManager<ClockComponent> *clockComponentManager,
  ComponentManager<PlayerComponent> *playerComponentManager,
  ComponentManager<DamageComponent> *damageComponentManager,
  GridSystem *gridSystem,
  DeleteSystem *deleteSystem
) :
  mEntityManager(entityManager),
  mRenderComponentManager(renderComponentManager),
  mVelocityComponentManager(velocityComponentManager),
  mCollisionComponentManager(collisionComponentManager),
  mAnimationComponentManager(animationComponentManager),
  mHealthComponentManager(healthComponentManager),
  mClockComponentManager(clockComponentManager),
  mPlayerComponentManager(playerComponentManager),
  mDamageComponentManager(damageComponentManager),
  mGridSystem(gridSystem),
  mDeleteSystem(deleteSystem)
{
  mName = "SERVER: ENTITY_CREATOR";
  mTextures[static_cast<int>(Textures::CharacterBandana)] = loadTexture("CharacterBandana1.png");
  mTextures[static_cast<int>(Textures::CharacterChainHat)] = loadTexture("CharacterChainHat.png");
  mTextures[static_cast<int>(Textures::CharacterChainHood)] = loadTexture("CharacterChainHood.png");
  mTextures[static_cast<int>(Textures::CharacterClothHood)] = loadTexture("CharacterClothHood.png");
  mTextures[static_cast<int>(Textures::CharacterGoldenHelmet)] = loadTexture("CharacterGoldenHelmet.png");
  mTextures[static_cast<int>(Textures::CharacterLeatherCap)] = loadTexture("CharacterLeatherCap.png");
  mTextures[static_cast<int>(Textures::CharacterMetalHelmet)] = loadTexture("CharacterMetalHelmet.png");
  mTextures[static_cast<int>(Textures::CharacterMage)] = loadTexture("mage.png");
  mTextures[static_cast<int>(Textures::CharacterKnight)] = loadTexture("knight.png");
  mTextures[static_cast<int>(Textures::CharacterSpearman)] = loadTexture("spearman.png");
  mTextures[static_cast<int>(Textures::Bullet)] = loadTexture("waterSpell.png");
  mTextures[static_cast<int>(Textures::SwordSlash)] = loadTexture("SwordSlash.png");
  mTextures[static_cast<int>(Textures::Tombstone)] = loadTexture("Tombstone.png");
}

EntityCreator::~EntityCreator()
{
}

Entity* EntityCreator::createPlayer(PlayerClass playerClass, sf::Vector2f position) {
  Entity* createdPlayer = nullptr;
  switch (playerClass)
  {
    case PlayerClass::Mage:
      createdPlayer = createMage(position);
      break; //Todo: archer and spearman
    case PlayerClass::Knight:
      createdPlayer = createKnight(position);
      break;
    case PlayerClass::Spearman:
      createdPlayer = createSpearman(position);
      break;
    default:
      TRACE_ERROR("No playerclass for: " << (int)playerClass);
      break;
  }

  return createdPlayer;
}

Entity* EntityCreator::createPlayerBase(float maxVelocity, Textures textureType, sf::Vector2f position, int health, int attackSpeed) {
  Entity* player = mEntityManager->createEntity();
  VelocityComponent* vc = mVelocityComponentManager->addComponent(player->id);
  vc->currentVelocity.x = 0;
  vc->currentVelocity.y = 0;
  vc->maxVelocity.x = maxVelocity;
  vc->maxVelocity.y = maxVelocity;
  vc->moveOnce = true;

  auto collisionComponent = mCollisionComponentManager->addComponent(player->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;

  RenderComponent* rc = mRenderComponentManager->addComponent(player->id);
  rc->textureId = textureType;
  rc->texture = *mTextures[static_cast<int>(rc->textureId)];
  rc->visible = true;
  rc->isDynamic = true;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 64 * 6 + 14, 64, 50));
  rc->sprite.setOrigin(32, 25);
  rc->sprite.setPosition(position.x, position.y);

  AnimationComponent* ac = mAnimationComponentManager->addComponent(player->id);
  ac->currentAnimation = AnimationType::IdleUp;

  Animation idleUpAnimation(rc->sprite, false, player->id);
  idleUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(0, 64 * 8 + 14, 64, 50), 140 });
  Animation idleLeftAnimation(rc->sprite, false, player->id);
  idleLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(0, 64 * 9 + 14, 64, 50), 140 });
  Animation idleDownAnimation(rc->sprite, false, player->id);
  idleDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(0, 64 * 10 + 14, 64, 50), 140 });
  Animation idleRightAnimation(rc->sprite, false, player->id);
  idleRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(0, 64 * 11 + 14, 64, 50), 140 });

  Animation runningUpAnimation(rc->sprite, false, player->id);
  for (int i = 1; i < 9; i++) {
    runningUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 8 + 14, 64, 50), 70 });
  }
  Animation runningLeftAnimation(rc->sprite, false, player->id);
  for (int i = 1; i < 9; i++) {
    runningLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 9 + 14, 64, 50), 70 });
  }
  Animation runningDownAnimation(rc->sprite, false, player->id);
  for (int i = 1; i < 9; i++) {
    runningDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 10 + 14, 64, 50), 70 });
  }
  Animation runningRightAnimation(rc->sprite, false, player->id);
  for (int i = 1; i < 9; i++) {
    runningRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 11 + 14, 64, 50), 70 });
  }
  Animation deathAnimation(rc->sprite, true, player->id);
  for (int i = 0; i < 6; i++) {
    deathAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 20, 64, 64), 140 });
  }

  ac->animations.emplace(AnimationType::IdleUp, idleUpAnimation);
  ac->animations.emplace(AnimationType::IdleDown, idleDownAnimation);
  ac->animations.emplace(AnimationType::IdleLeft, idleLeftAnimation);
  ac->animations.emplace(AnimationType::IdleRight, idleRightAnimation);
  ac->animations.emplace(AnimationType::RunningUp, runningUpAnimation);
  ac->animations.emplace(AnimationType::RunningDown, runningDownAnimation);
  ac->animations.emplace(AnimationType::RunningLeft, runningLeftAnimation);
  ac->animations.emplace(AnimationType::RunningRight, runningRightAnimation);
  ac->animations.emplace(AnimationType::Death, deathAnimation);

  ClockComponent* cc = mClockComponentManager->addComponent(player->id);

  HealthComponent* hc = mHealthComponentManager->addComponent(player->id);
  hc->health = health;
  hc->isAlive = true;

  PlayerComponent* pc = mPlayerComponentManager->addComponent(player->id);
  pc->attackSpeed = attackSpeed;
  pc->state = PlayerState::Idle;

  mGridSystem->addEntity(player->id, (sf::Vector2i)rc->sprite.getPosition());

  return player;
}

Entity* EntityCreator::createMage(sf::Vector2f position) {
  
  Entity* mage = createPlayerBase(MAGE_MAX_VELOCITY, Textures::CharacterMage, position, 100, 500);
  
  auto rc = mRenderComponentManager->getComponent(mage->id);
  auto ac = mAnimationComponentManager->getComponent(mage->id);
  
  auto attackCallback = [this](int entityId) {
    //TRACE_INFO("ATTACKING");
    auto player = mPlayerComponentManager->getComponent(entityId);
    createBullet(entityId, 0, player->nextAttackMousePosition);
  };
  
  Animation castingUpAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    castingUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 14, 64, 50), 70 });
  }
  castingUpAnimation.setAttackCallback(attackCallback);
  Animation castingLeftAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    castingLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 + 14, 64, 50), 70 });
  }
  castingLeftAnimation.setAttackCallback(attackCallback);
  Animation castingDownAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    castingDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 2 + 14, 64, 50), 70 });
  }
  castingDownAnimation.setAttackCallback(attackCallback);
  Animation castingRightAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    castingRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 3 + 14, 64, 50), 70 });
  }
  castingRightAnimation.setAttackCallback(attackCallback);

  ac->animations.emplace(AnimationType::AttackingUp, castingUpAnimation);
  ac->animations.emplace(AnimationType::AttackingDown, castingDownAnimation);
  ac->animations.emplace(AnimationType::AttackingLeft, castingLeftAnimation);
  ac->animations.emplace(AnimationType::AttackingRight, castingRightAnimation);

  return mage;
}

Entity* EntityCreator::createBullet(int entityId, std::uint32_t input, sf::Vector2i mousePosition, bool visible) {
  //std::cout << "Creating bullet" << std::endl;
  auto playerShootClockComponent = mClockComponentManager->getComponent(entityId);
  auto player = mPlayerComponentManager->getComponent(entityId);
  auto animation = mAnimationComponentManager->getComponent(entityId);
  //animation->animations.at(animation->currentAnimation);
  auto currentFrame = animation->animations.at(animation->currentAnimation).getCurrentAnimationFrame();
  if (playerShootClockComponent->clock.getElapsedTime() >= sf::milliseconds(player->attackSpeed)) {
    auto playerPositionComponent = mRenderComponentManager->getComponent(entityId);
    float originXPos = playerPositionComponent->sprite.getPosition().x;
    float originYPos = playerPositionComponent->sprite.getPosition().y;

    sf::Vector2f bulletVelocity((float)(mousePosition.x - originXPos), (float)(mousePosition.y - originYPos));

    // Normalize velocity to avoid huge speeds, and multiply with 10 for extra speed
    float divider = sqrt(bulletVelocity.x*bulletVelocity.x + bulletVelocity.y*bulletVelocity.y);
    bulletVelocity.x = (bulletVelocity.x / divider) * 10;
    bulletVelocity.y = (bulletVelocity.y / divider) * 10;

    float angle = atan2(originYPos - mousePosition.y, originXPos - mousePosition.x) * (180 / 3.1415f) - 90; // Remove 90 degrees to compensate for rotation...

    // Move origin position to avoid colliding with the player
    if ((angle <= -125 && angle >= -235) || (angle >= -55 && angle <= 55)) {
      originXPos += bulletVelocity.x * 5.5f;
      originYPos += bulletVelocity.y * 5.5f;
    }
    else {
      originXPos += bulletVelocity.x * 4.0f;
      originYPos += bulletVelocity.y * 4.0f;
    }

    Entity* bullet = mEntityManager->createEntity();

    VelocityComponent* vc = mVelocityComponentManager->addComponent(bullet->id);
    vc->currentVelocity.x = bulletVelocity.x;
    vc->currentVelocity.y = bulletVelocity.y;
    vc->maxVelocity.x = 15;
    vc->maxVelocity.y = 15;
    vc->moveOnce = false;

    RenderComponent* rc = mRenderComponentManager->addComponent(bullet->id);
    rc->texture = *mTextures[static_cast<int>(Textures::Bullet)];
    rc->visible = visible;
    rc->isDynamic = true;
    rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, 28, 20));
    rc->sprite.setOrigin(14, 10);
    rc->sprite.setPosition(originXPos, originYPos);
    rc->sprite.setRotation(angle);
    rc->textureId = Textures::Bullet;

    DamageComponent* dc = mDamageComponentManager->addComponent(bullet->id);
    dc->damage = 10;

    CollisionComponent* cc = mCollisionComponentManager->addComponent(bullet->id);
    cc->collided = false;
    cc->destroyOnCollision = true;

    Animation attackAnimation(rc->sprite, false, bullet->id);
    for (int i = 0; i < 5; i++) {
      attackAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 28, 0, 28, 20), 400 });
    }

    mGridSystem->addEntity(bullet->id, (sf::Vector2i)rc->sprite.getPosition());

    return bullet;
  }
  return nullptr;
}

Entity* EntityCreator::createMelee(int entityId, std::uint32_t input, sf::Vector2i mousePosition) {
  auto playerAttackClockComponent = mClockComponentManager->getComponent(entityId);
  auto player = mPlayerComponentManager->getComponent(entityId);

  if (playerAttackClockComponent->clock.getElapsedTime() >= sf::milliseconds(player->attackSpeed)) {
    auto playerPositionComponent = mRenderComponentManager->getComponent(entityId);
    float originXPos = playerPositionComponent->sprite.getPosition().x;
    float originYPos = playerPositionComponent->sprite.getPosition().y;

    sf::Vector2f bulletVelocity((float)(mousePosition.x - originXPos), (float)(mousePosition.y - originYPos));

    // Normalize velocity to avoid huge speeds, and multiply with 10 for extra speed
    float divider = sqrt(bulletVelocity.x*bulletVelocity.x + bulletVelocity.y*bulletVelocity.y);
    bulletVelocity.x = (bulletVelocity.x / divider) * 10;
    bulletVelocity.y = (bulletVelocity.y / divider) * 10;

    float angle = atan2(originYPos - mousePosition.y, originXPos - mousePosition.x) * (180 / 3.1415f) - 90; // Remove 90 degrees to compensate for rotation...

    // Move origin position to avoid colliding with the player
    originXPos += bulletVelocity.x * 4.5f;
    originYPos += bulletVelocity.y * 4.5f;

    Entity* bullet = mEntityManager->createEntity();

    RenderComponent* rc = mRenderComponentManager->addComponent(bullet->id);
    rc->texture = *mTextures[static_cast<int>(Textures::SwordSlash)];
    rc->visible = true;
    rc->isDynamic = true;
    rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, 28, 20));
    rc->sprite.setOrigin(14, 10);
    rc->sprite.setPosition(originXPos, originYPos);
    rc->sprite.setRotation(angle);
    rc->textureId = Textures::SwordSlash;

    DamageComponent* dc = mDamageComponentManager->addComponent(bullet->id);
    dc->damage = 50;

    VelocityComponent* vc = mVelocityComponentManager->addComponent(bullet->id);
    vc->currentVelocity.x = 0.0001f;
    vc->currentVelocity.y = 0.0001f;
    vc->maxVelocity.x = 0.0001f;
    vc->maxVelocity.y = 0.0001f;
    vc->moveOnce = false;

    CollisionComponent* cc = mCollisionComponentManager->addComponent(bullet->id);
    cc->collided = false;
    cc->destroyOnCollision = false;

    ClockComponent* clockComponent = mClockComponentManager->addComponent(bullet->id);
    clockComponent->timeout = 100;
    clockComponent->timeoutCallback = [this, bullet]() {
      mDeleteSystem->addEntity(bullet->id);
    };

    mGridSystem->addEntity(bullet->id, (sf::Vector2i)rc->sprite.getPosition());

    return bullet;
  }
  return nullptr;
}

Entity* EntityCreator::createKnight(sf::Vector2f position) {

  Entity* knight = createPlayerBase(KNIGHT_MAX_VELOCITY, Textures::CharacterKnight, position, 100, 400);

  auto rc = mRenderComponentManager->getComponent(knight->id);
  auto ac = mAnimationComponentManager->getComponent(knight->id);

  auto attackCallback = [this](int entityId) {
    //TRACE_INFO("ATTACKING");
    auto player = mPlayerComponentManager->getComponent(entityId);
    createMelee(entityId, 0, player->nextAttackMousePosition);
  };

  Animation slashUpAnimation(rc->sprite, true, knight->id);
  for (int i = 0; i < 6; i++) {
    slashUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 12 + 14, 64, 50), 70 });
  }
  slashUpAnimation.setAttackCallback(attackCallback);

  Animation slashLeftAnimation(rc->sprite, true, knight->id);
  for (int i = 0; i < 6; i++) {
    slashLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 13 + 14, 64, 50), 70 });
  }
  slashLeftAnimation.setAttackCallback(attackCallback);

  Animation slashDownAnimation(rc->sprite, true, knight->id);
  for (int i = 0; i < 6; i++) {
    slashDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 14 + 14, 64, 50), 70 });
  }
  slashDownAnimation.setAttackCallback(attackCallback);

  Animation slashRightAnimation(rc->sprite, true, knight->id);
  for (int i = 0; i < 6; i++) {
    slashRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 15 + 14, 64, 50), 70 });
  }
  slashRightAnimation.setAttackCallback(attackCallback);

  ac->animations.emplace(AnimationType::AttackingUp, slashUpAnimation);
  ac->animations.emplace(AnimationType::AttackingDown, slashDownAnimation);
  ac->animations.emplace(AnimationType::AttackingLeft, slashLeftAnimation);
  ac->animations.emplace(AnimationType::AttackingRight, slashRightAnimation);

  return knight;
}

Entity* EntityCreator::createSpearman(sf::Vector2f position) {

  Entity* spearman = createPlayerBase(SPEARMAN_MAX_VELOCITY, Textures::CharacterSpearman, position, 100, 450);

  auto rc = mRenderComponentManager->getComponent(spearman->id);
  auto ac = mAnimationComponentManager->getComponent(spearman->id);

  Animation thrustUpAnimation(rc->sprite, true, spearman->id);
  for (int i = 0; i < 6; i++) {
    thrustUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 4 + 14, 64, 50), 90 });
  }
  Animation thrustLeftAnimation(rc->sprite, true, spearman->id);
  for (int i = 0; i < 6; i++) {
    thrustLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 5 + 14, 64, 50), 90 });
  }
  Animation thrustDownAnimation(rc->sprite, true, spearman->id);
  for (int i = 0; i < 6; i++) {
    thrustDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 6 + 14, 64, 50), 90 });
  }
  Animation thrustRightAnimation(rc->sprite, true, spearman->id);
  for (int i = 0; i < 6; i++) {
    thrustRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 7 + 14, 64, 50), 90 });
  }

  ac->animations.emplace(AnimationType::AttackingUp, thrustUpAnimation);
  ac->animations.emplace(AnimationType::AttackingDown, thrustDownAnimation);
  ac->animations.emplace(AnimationType::AttackingLeft, thrustLeftAnimation);
  ac->animations.emplace(AnimationType::AttackingRight, thrustRightAnimation);

  return spearman;
}

sf::Texture* EntityCreator::loadTexture(std::string fileName) {
  sf::Texture* texture = new sf::Texture();
  if (!Collision::CreateTextureAndBitmask(*texture, "Client/Resources/Sprites/" + fileName)) {
    std::cout << "[GUI] ERROR could not load file " << "Client/Resources/Sprites/" << fileName << std::endl;
  }
  return texture;
}
