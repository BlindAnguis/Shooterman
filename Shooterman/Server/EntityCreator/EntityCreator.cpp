#include "EntityCreator.h"
#include "../Systems/CollisionSystem/Collision.h"
#include "../Systems/EntityCreatorSystem/EntityCreatorSystem.h"
#include "../../Common/Interfaces.h"
#include "../../Common/Messages/SoundMessage.h"

#define MAGE_MAX_VELOCITY 4.5f
#define KNIGHT_MAX_VELOCITY 4.8f
#define SPEARMAN_MAX_VELOCITY 4.65f
#define ARCHER_MAX_VELOCITY 5.1f
#define MAGE_MANA_COST 20
#define MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MIN 15
#define MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MAX 40

EntityCreator::EntityCreator(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<DeleteSystem> deleteSystem, EntityCreatorSystem* entityCreatorSystem, std::shared_ptr<GridSystem> gridSystem, std::shared_ptr<ManaSystem> manaSystem) :
  mEntityManager(&EntityManager::get()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mClockComponentManager(&ComponentManager<ClockComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get()),
  mHealthChangerComponentManager(&ComponentManager<HealthChangerComponent>::get()),
  mGridSystem(gridSystem),
  mDeleteSystem(deleteSystem),
  mEntityCreatorSystem(entityCreatorSystem),
  mMessageHandler(messageHandler),
  mManaSystem(manaSystem)
{
  mName = "SERVER: ENTITY_CREATOR";
  loadTexture(Textures::CharacterBandana, "CharacterBandana1.png");
  loadTexture(Textures::CharacterChainHat, "CharacterChainHat.png");
  loadTexture(Textures::CharacterChainHood, "CharacterChainHood.png");
  loadTexture(Textures::CharacterClothHood, "CharacterClothHood.png");
  loadTexture(Textures::CharacterGoldenHelmet, "CharacterGoldenHelmet.png");
  loadTexture(Textures::CharacterLeatherCap, "CharacterLeatherCap.png");
  loadTexture(Textures::CharacterMetalHelmet, "CharacterMetalHelmet.png");
  loadTexture(Textures::CharacterMage, "mage.png");
  loadTexture(Textures::CharacterKnight, "knight3.png");
  loadTexture(Textures::CharacterSpearman, "spearman.png");
  loadTexture(Textures::CharacterArcher, "archer.png");
  //loadTexture(Textures::Bullet, "waterSpell.png");
  loadTexture(Textures::Bullet, "lightningBall.png");
  loadTexture(Textures::Arrow, "arrow.png");
  loadTexture(Textures::LightningStrike, "lightningBolt.png");
  loadTexture(Textures::SwordSlash, "SwordSlash.png");
  loadTexture(Textures::Tombstone, "Tombstone.png");
  loadTexture(Textures::HealthPotion, "Potions/pt1Small.png");
  loadTexture(Textures::ManaPotion, "Potions/pt2Small.png");
  loadTexture(Textures::Ammo, "Potions/pt4Small.png");
  mMessageHandler->subscribeTo(Interfaces::SERVER_SOUND_LIST, &mSoundSubscriber);
}

EntityCreator::~EntityCreator() {
  mMessageHandler->unsubscribeTo(Interfaces::SERVER_SOUND_LIST, &mSoundSubscriber);
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
    case PlayerClass::Archer:
      createdPlayer = createArcher(position);
      break;
    default:
      TRACE_ERROR("No playerclass for: " << (int)playerClass);
      break;
  }

  return createdPlayer;
}

Entity* EntityCreator::createPlayerBase(float maxVelocity, Textures textureType, sf::Vector2f position, int health, int attackSpeed) {
  Entity* player = mEntityManager->createEntity();
  auto vc = mVelocityComponentManager->addComponent(player->id);
  vc->currentVelocity.x = 0;
  vc->currentVelocity.y = 0;
  vc->maxVelocity.x = maxVelocity;
  vc->maxVelocity.y = maxVelocity;
  vc->moveOnce = true;
  auto collisionComponent = mCollisionComponentManager->addComponent(player->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;

  auto rc = mRenderComponentManager->addComponent(player->id);
  rc->textureId = textureType;
  rc->visible = true;
  rc->isDynamic = true;
  rc->sprite = sf::Sprite(*mTextures[rc->textureId], sf::IntRect(0, 64 * 6 + 14, 64, 50));
  rc->sprite.setOrigin(32, 25);
  rc->sprite.setPosition(position.x, position.y);

  auto ac = mAnimationComponentManager->addComponent(player->id);
  ac->currentAnimation = AnimationType::IdleUp;
  sf::Vector2f originPosition(32, 25);
  Animation idleUpAnimation(rc->sprite, false, player->id);
  idleUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(0, 64 * 8 + 14, 64, 50), 140, originPosition });
  Animation idleLeftAnimation(rc->sprite, false, player->id);
  idleLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(0, 64 * 9 + 14, 64, 50), 140, originPosition });
  Animation idleDownAnimation(rc->sprite, false, player->id);
  idleDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(0, 64 * 10 + 14, 64, 50), 140, originPosition });
  Animation idleRightAnimation(rc->sprite, false, player->id);
  idleRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(0, 64 * 11 + 14, 64, 50), 140, originPosition });

  Animation runningUpAnimation(rc->sprite, false, player->id);
  for (int i = 1; i < 9; i++) {
    runningUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 8 + 14, 64, 50), 70, originPosition });
  }
  Animation runningLeftAnimation(rc->sprite, false, player->id);
  for (int i = 1; i < 9; i++) {
    runningLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 9 + 14, 64, 50), 70, originPosition });
  }
  Animation runningDownAnimation(rc->sprite, false, player->id);
  for (int i = 1; i < 9; i++) {
    runningDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 10 + 14, 64, 50), 70, originPosition });
  }
  Animation runningRightAnimation(rc->sprite, false, player->id);
  for (int i = 1; i < 9; i++) {
    runningRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 11 + 14, 64, 50), 70, originPosition });
  }
  Animation deathAnimation(rc->sprite, true, player->id);
  for (int i = 0; i < 6; i++) {
    deathAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 20, 64, 64), 140, originPosition });
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

  auto cc = mClockComponentManager->addComponent(player->id);

  auto hc = mHealthComponentManager->addComponent(player->id);
  hc->maxHealth = health;
  hc->currentHealth = hc->maxHealth;
  hc->isAlive = true;

  auto pc = mPlayerComponentManager->addComponent(player->id);
  pc->attackSpeed = attackSpeed;
  pc->invinsible = false;
  pc->nrOfDeaths = 0;
  pc->nrOfKills = 0;
  pc->score = 0;
  pc->state = PlayerState::Idle;
  pc->superAttacks = 0;

  mGridSystem->addEntity(player->id, (sf::Vector2i)rc->sprite.getPosition());
  return player;
}

Entity* EntityCreator::createMage(sf::Vector2f position) {
  
  Entity* mage = createPlayerBase(MAGE_MAX_VELOCITY, Textures::CharacterMage, position, MAGE_MAX_HEALTH, MAGE_ATTACK_SPEED);
  
  auto rc = mRenderComponentManager->getComponent(mage->id);
  auto ac = mAnimationComponentManager->getComponent(mage->id);
  auto mc = ComponentManager<ManaComponent>::get().addComponent(mage->id);
  mc->maxMana = MAGE_MAX_MANA;
  mc->currentMana = mc->maxMana;
  auto pc = mPlayerComponentManager->getComponent(mage->id);
  pc->playerClass = PlayerClass::Mage;
  
  auto attackCallback = [this](int entityId) {
    //TRACE_INFO("ATTACKING");
    auto player = mPlayerComponentManager->getComponent(entityId);
    auto mc = ComponentManager<ManaComponent>::get().getComponent(entityId);
    if (mc) {
      if (mc->currentMana >= MAGE_MANA_COST) {
        createBullet(entityId, 0, player->nextAttackMousePosition);
        mManaSystem->changeMana(entityId, -MAGE_MANA_COST);
        auto attackClock = mClockComponentManager->getComponent(entityId);
        //attackClock->clock.restart();
      }
    } else {
      TRACE_ERROR("Mage with id: " << entityId << " has no mana component!");
    }
  };

  auto superAttackCallback = [this](int entityId) {
    int framesUntilAttack = 0;
    int nrOfStrikes = (int)(rand() % (MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MAX - MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MIN) + MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MIN);
    for (int i = 0; i < nrOfStrikes; i++) {
      float posX = (rand() % (1024 - 32)) + 32.0f;
      float posY = (rand() % (1024 - 32)) + 32.0f;
      framesUntilAttack += (int)(rand() % (5 - 2) + 2);
      mEntityCreatorSystem->addEntityToCreate(EntityType::LightningStrike, sf::Vector2f(posX, posY), framesUntilAttack, { entityId });
    }
  };

  auto superAttackFinishedCallback = [this](int entityId) {
    handleFinishedSuperAttack(entityId);
  };
  
  sf::Vector2f originPosition(32, 25);
  Animation castingUpAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    castingUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 14, 64, 50), 70, originPosition });
  }
  castingUpAnimation.setAttackCallback(attackCallback);
  Animation castingLeftAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    castingLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 + 14, 64, 50), 70, originPosition });
  }
  castingLeftAnimation.setAttackCallback(attackCallback);
  Animation castingDownAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    castingDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 2 + 14, 64, 50), 70, originPosition });
  }
  castingDownAnimation.setAttackCallback(attackCallback);
  Animation castingRightAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    castingRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 3 + 14, 64, 50), 70, originPosition });
  }
  castingRightAnimation.setAttackCallback(attackCallback);

  ac->animations.emplace(AnimationType::AttackingUp, castingUpAnimation);
  ac->animations.emplace(AnimationType::AttackingDown, castingDownAnimation);
  ac->animations.emplace(AnimationType::AttackingLeft, castingLeftAnimation);
  ac->animations.emplace(AnimationType::AttackingRight, castingRightAnimation);

  Animation superCastingUpAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    superCastingUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 14, 64, 50), 70, originPosition });
  }
  superCastingUpAnimation.setAttackCallback(superAttackCallback);
  superCastingUpAnimation.setAttackFinishedCallback(superAttackFinishedCallback);
  Animation superCastingLeftAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    superCastingLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 + 14, 64, 50), 70, originPosition });
  }
  superCastingLeftAnimation.setAttackCallback(superAttackCallback);
  superCastingLeftAnimation.setAttackFinishedCallback(superAttackFinishedCallback);
  Animation superCastingDownAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    superCastingDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 2 + 14, 64, 50), 70, originPosition });
  }
  superCastingDownAnimation.setAttackCallback(superAttackCallback);
  superCastingDownAnimation.setAttackFinishedCallback(superAttackFinishedCallback);
  Animation superCastingRightAnimation(rc->sprite, true, mage->id);
  for (int i = 0; i < 7; i++) {
    superCastingRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 3 + 14, 64, 50), 70, originPosition });
  }
  superCastingRightAnimation.setAttackCallback(superAttackCallback);
  superCastingRightAnimation.setAttackFinishedCallback(superAttackFinishedCallback);

  ac->animations.emplace(AnimationType::SuperAttackingUp, superCastingUpAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingDown, superCastingDownAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingLeft, superCastingLeftAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingRight, superCastingRightAnimation);

  return mage;
}

Entity* EntityCreator::createLightningStrike(sf::Vector2f position, std::set<int> immuneEntityIds) {
  Entity* lightningStrike = mEntityManager->createEntity();
  auto rc = mRenderComponentManager->addComponent(lightningStrike->id);
  rc->visible = true;
  rc->isDynamic = true;
  rc->sprite = sf::Sprite(*mTextures[Textures::LightningStrike], sf::IntRect(0, 0, 98, 203));
  rc->sprite.setOrigin(49, 101);
  rc->sprite.setPosition(position.x, position.y);
  rc->textureId = Textures::LightningStrike;

  auto hcc = mHealthChangerComponentManager->addComponent(lightningStrike->id);
  hcc->healthChange = -40;
  hcc->immuneEntityIds = immuneEntityIds;

  auto cc = mCollisionComponentManager->addComponent(lightningStrike->id);
  cc->collided = false;
  cc->destroyOnCollision = true;

  auto attackFinishedCallback = [this](int entityId) {
    mDeleteSystem->addEntity(entityId);
  };

  auto ac = mAnimationComponentManager->addComponent(lightningStrike->id);
  Animation attackAnimation(rc->sprite, true, lightningStrike->id);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 11; j++) {
      attackAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(j * 98, i * 203, 98, 203), 40, sf::Vector2f(49, 101) });
    }
  }
  attackAnimation.setAttackFinishedCallback(attackFinishedCallback);
  ac->animations.emplace(AnimationType::Attacking, attackAnimation);
  ac->currentAnimation = AnimationType::Attacking;
  mGridSystem->addEntity(lightningStrike->id, (sf::Vector2i)rc->sprite.getPosition());

  return lightningStrike;
}

void EntityCreator::createRandomLightningBolts() {
  for (int i = 0; i < MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MAX; i++) {
    int sleepTime = (int)(rand() % (100 - 40) + 40);
    sf::sleep(sf::milliseconds(sleepTime));
    Entity* lightningBolt = mEntityManager->createEntity();
    auto rc = mRenderComponentManager->addComponent(lightningBolt->id);
    rc->visible = true;
    rc->isDynamic = true;
    rc->sprite = sf::Sprite(*mTextures[Textures::LightningStrike], sf::IntRect(0, 0, 98, 203));
    rc->sprite.setOrigin(49, 101);
    float posX = (rand() % (1024 - 32)) + 32.0f;
    float posY = (rand() % (1024 - 32)) + 32.0f;
    rc->sprite.setPosition(posX, posY);
    rc->textureId = Textures::LightningStrike;

    auto hcc = mHealthChangerComponentManager->addComponent(lightningBolt->id);
    hcc->healthChange = -40;

    auto cc = mCollisionComponentManager->addComponent(lightningBolt->id);
    cc->collided = false;
    cc->destroyOnCollision = true;

    auto attackFinishedCallback = [this](int entityId) {
      mDeleteSystem->addEntity(entityId);
    };

    auto ac = mAnimationComponentManager->addComponent(lightningBolt->id);
    Animation attackAnimation(rc->sprite, true, lightningBolt->id);
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 11; j++) {
        attackAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(j * 98, i * 203, 98, 203), 40, sf::Vector2f(49, 101) });
      }
    }
    attackAnimation.setAttackFinishedCallback(attackFinishedCallback);
    ac->animations.emplace(AnimationType::Attacking, attackAnimation);
    ac->currentAnimation = AnimationType::Attacking;
    mGridSystem->addEntity(lightningBolt->id, (sf::Vector2i)rc->sprite.getPosition());
  }
}

Entity* EntityCreator::createBullet(int entityId, std::uint32_t input, sf::Vector2i mousePosition, bool visible) {
  TRACE_DEBUG2("Creating a lightningbolt for entity: " << entityId);
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

    // Normalize velocity to avoid huge speeds, and multiply for extra speed
    float divider = sqrt(bulletVelocity.x*bulletVelocity.x + bulletVelocity.y*bulletVelocity.y);
    bulletVelocity.x = (bulletVelocity.x / divider) * 20;
    bulletVelocity.y = (bulletVelocity.y / divider) * 20;

    float angle = atan2(originYPos - mousePosition.y, originXPos - mousePosition.x) * (180 / 3.1415f) - 90; // Remove 90 degrees to compensate for rotation...

    // Move origin position to avoid colliding with the player
    if ((angle <= -125 && angle >= -235) || (angle >= -55 && angle <= 55)) {
      originXPos += bulletVelocity.x * 2.5f;
      originYPos += bulletVelocity.y * 2.5f;
    }
    else {
      originXPos += bulletVelocity.x * 2.0f;
      originYPos += bulletVelocity.y * 2.0f;
    }

    Entity* bullet = mEntityManager->createEntity();

    auto vc = mVelocityComponentManager->addComponent(bullet->id);
    vc->currentVelocity.x = bulletVelocity.x;
    vc->currentVelocity.y = bulletVelocity.y;
    vc->maxVelocity.x = 20;
    vc->maxVelocity.y = 20;
    vc->moveOnce = false;

    auto rc = mRenderComponentManager->addComponent(bullet->id);
    rc->visible = visible;
    rc->isDynamic = true;
    //rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, 28, 20));
    rc->sprite = sf::Sprite(*mTextures[Textures::Bullet], sf::IntRect(0, 0, 56, 65));
    rc->sprite.setOrigin(28, 32);
    rc->sprite.setPosition(originXPos, originYPos);
    rc->sprite.setRotation(angle);
    rc->textureId = Textures::Bullet;

    auto hcc = mHealthChangerComponentManager->addComponent(bullet->id);
    hcc->healthChange = -10;

    auto cc = mCollisionComponentManager->addComponent(bullet->id);
    cc->collided = false;
    cc->destroyOnCollision = true;

    auto ac = mAnimationComponentManager->addComponent(bullet->id);
    Animation attackAnimation(rc->sprite, false, bullet->id);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 5; j++) {
        attackAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(j * 56, i*65, 56, 65), 40, sf::Vector2f(28, 32) });
      }
    }
    ac->animations.emplace(AnimationType::Attacking, attackAnimation);
    ac->currentAnimation = AnimationType::Attacking;
    mGridSystem->addEntity(bullet->id, (sf::Vector2i)rc->sprite.getPosition());
    return bullet;
  }
  return nullptr;
}

Entity* EntityCreator::createArrow(int entityId, std::uint32_t input, sf::Vector2i mousePosition, bool visible) {
  TRACE_DEBUG2("Creating an arrow for entity: " << entityId);
  auto playerShootClockComponent = mClockComponentManager->getComponent(entityId);
  auto player = mPlayerComponentManager->getComponent(entityId);
  auto animation = mAnimationComponentManager->getComponent(entityId);
  //animation->animations.at(animation->currentAnimation);
  auto currentFrame = animation->animations.at(animation->currentAnimation).getCurrentAnimationFrame();
  if (playerShootClockComponent->clock.getElapsedTime() >= sf::milliseconds(player->attackSpeed)) {
    auto playerPositionComponent = mRenderComponentManager->getComponent(entityId);
    float originXPos = playerPositionComponent->sprite.getPosition().x;
    float originYPos = playerPositionComponent->sprite.getPosition().y;
    sf::Vector2f arrowVelocity((float)(mousePosition.x - originXPos), (float)(mousePosition.y - originYPos));

    // Normalize velocity to avoid huge speeds, and multiply for extra speed
    float divider = sqrt(arrowVelocity.x*arrowVelocity.x + arrowVelocity.y*arrowVelocity.y);
    arrowVelocity.x = (arrowVelocity.x / divider) * 25;
    arrowVelocity.y = (arrowVelocity.y / divider) * 25;

    float angle = atan2(originYPos - mousePosition.y, originXPos - mousePosition.x) * (180 / 3.1415f) - 90; // Remove 90 degrees to compensate for rotation...

    // Move origin position to avoid colliding with the player
    if ((angle <= -125 && angle >= -235) || (angle >= -55 && angle <= 55)) {
      originXPos += arrowVelocity.x * 2.5f;
      originYPos += arrowVelocity.y * 2.5f;
    }
    else {
      originXPos += arrowVelocity.x * 2.0f;
      originYPos += arrowVelocity.y * 2.0f;
    }

    Entity* arrow = mEntityManager->createEntity();

    auto vc = mVelocityComponentManager->addComponent(arrow->id);
    vc->currentVelocity.x = arrowVelocity.x;
    vc->currentVelocity.y = arrowVelocity.y;
    vc->maxVelocity.x = 25;
    vc->maxVelocity.y = 25;
    vc->moveOnce = false;

    auto rc = mRenderComponentManager->addComponent(arrow->id);
    rc->visible = visible;
    rc->isDynamic = true;
    //rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, 28, 20));
    rc->sprite = sf::Sprite(*mTextures[Textures::Arrow], sf::IntRect(0, 0, 32, 32));
    rc->sprite.setOrigin(0, 0);
    rc->sprite.setPosition(originXPos, originYPos);
    rc->sprite.setRotation(angle);
    rc->textureId = Textures::Arrow;

    auto hcc = mHealthChangerComponentManager->addComponent(arrow->id);
    hcc->healthChange = -10;

    auto cc = mCollisionComponentManager->addComponent(arrow->id);
    cc->collided = false;
    cc->destroyOnCollision = true;

    auto ac = mAnimationComponentManager->addComponent(arrow->id);
    Animation attackAnimation(rc->sprite, false, arrow->id);

    attackAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(0, 0, 32, 32), 40, sf::Vector2f(0, 0) });

    ac->animations.emplace(AnimationType::Attacking, attackAnimation);
    ac->currentAnimation = AnimationType::Attacking;
    mGridSystem->addEntity(arrow->id, (sf::Vector2i)rc->sprite.getPosition());

    return arrow;
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

    auto rc = mRenderComponentManager->addComponent(bullet->id);
    rc->visible = true;
    rc->isDynamic = true;
    rc->sprite = sf::Sprite(*mTextures[Textures::SwordSlash], sf::IntRect(0, 0, 28, 20));
    rc->sprite.setOrigin(14, 10);
    rc->sprite.setPosition(originXPos, originYPos);
    rc->sprite.setRotation(angle);
    rc->textureId = Textures::SwordSlash;

    auto hcc = mHealthChangerComponentManager->addComponent(bullet->id);
    hcc->healthChange = -50;

    auto vc = mVelocityComponentManager->addComponent(bullet->id);
    vc->currentVelocity.x = 0.0001f;
    vc->currentVelocity.y = 0.0001f;
    vc->maxVelocity.x = 0.0001f;
    vc->maxVelocity.y = 0.0001f;
    vc->moveOnce = false;

    auto cc = mCollisionComponentManager->addComponent(bullet->id);
    cc->collided = false;
    cc->destroyOnCollision = false;

    auto clockComponent = mClockComponentManager->addComponent(bullet->id);
    clockComponent->timeout = 100;
    clockComponent->timeoutCallback = [this, bullet]() {
      mDeleteSystem->addEntity(bullet->id);
    };

    SoundMessage sm;
    int random = (int)rand() % 10;
    if (random < 7) {
      sm.addSound(Sounds::SlashLong1);
    } else {
      sm.addSound(Sounds::SlashLong2);
    }
    sm.addSound(Sounds::Hit1);
    mSoundSubscriber.reverseSendMessage(sm.pack());
    mGridSystem->addEntity(bullet->id, (sf::Vector2i)rc->sprite.getPosition());
    return bullet;
  }
  return nullptr;
}

Entity* EntityCreator::createKnight(sf::Vector2f position) {

  Entity* knight = createPlayerBase(KNIGHT_MAX_VELOCITY, Textures::CharacterKnight, position, KNIGHT_MAX_HEALTH, KNIGHT_ATTACK_SPEED);

  auto rc = mRenderComponentManager->getComponent(knight->id);
  auto ac = mAnimationComponentManager->getComponent(knight->id);
  auto sc = ComponentManager<StaminaComponent>::get().addComponent(knight->id);
  sc->maxStamina = KNIGHT_MAX_STAMINA;
  sc->currentStamina = sc->maxStamina;
  auto pc = mPlayerComponentManager->getComponent(knight->id);
  pc->playerClass = PlayerClass::Knight;

  auto attackCallback = [this](int entityId) {
    //TRACE_INFO("ATTACKING");
    auto player = mPlayerComponentManager->getComponent(entityId);
    createMelee(entityId, 0, player->nextAttackMousePosition);
    auto attackClock = mClockComponentManager->getComponent(entityId);
    //attackClock->clock.restart();
  };

  auto superAttackFinishedCallback = [this](int entityId) {
    handleFinishedSuperAttack(entityId);
  };

  sf::Vector2f originPosition(32, 25);
  Animation slashUpAnimation(rc->sprite, true, knight->id);
  for (int i = 0; i < 6; i++) {
    slashUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 12 + 14, 64, 50), 70, originPosition });
  }
  slashUpAnimation.setAttackCallback(attackCallback);

  Animation slashLeftAnimation(rc->sprite, true, knight->id);
  for (int i = 0; i < 6; i++) {
    slashLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 13 + 14, 64, 50), 70, originPosition });
  }
  slashLeftAnimation.setAttackCallback(attackCallback);

  Animation slashDownAnimation(rc->sprite, true, knight->id);
  for (int i = 0; i < 6; i++) {
    slashDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 14 + 14, 64, 50), 70, originPosition });
  }
  slashDownAnimation.setAttackCallback(attackCallback);

  Animation slashRightAnimation(rc->sprite, true, knight->id);
  for (int i = 0; i < 6; i++) {
    slashRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 15 + 14, 64, 50), 70, originPosition });
  }
  slashRightAnimation.setAttackCallback(attackCallback);

  ac->animations.emplace(AnimationType::AttackingUp, slashUpAnimation);
  ac->animations.emplace(AnimationType::AttackingDown, slashDownAnimation);
  ac->animations.emplace(AnimationType::AttackingLeft, slashLeftAnimation);
  ac->animations.emplace(AnimationType::AttackingRight, slashRightAnimation);

  Animation superAttackUpAnimation(rc->sprite, true, knight->id);
  int superAttackSpeed = 60;
  superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1045, 1363, 30, 110), superAttackSpeed, sf::Vector2f(15, 55) }); // Up no slash
  for (int i = 0; i < 2; i++) {
    superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
  }
  //superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1045, 1363, 30, 110), superAttackSpeed, sf::Vector2f(15, 55) }); // Up no slash
  //superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1045, 1363, 30, 110), superAttackSpeed, sf::Vector2f(11, 87) }); // Up no slash
  superAttackUpAnimation.setAttackFinishedCallback(superAttackFinishedCallback);

  Animation superAttackRightAnimation(rc->sprite, true, knight->id);
  superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1038, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Right no slash
  for (int i = 0; i < 2; i++) {
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
  }
  //superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1038, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Right no slash
  superAttackRightAnimation.setAttackFinishedCallback(superAttackFinishedCallback);

  Animation superAttackDownAnimation(rc->sprite, true, knight->id);
  superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1042, 1805, 56, 98), superAttackSpeed, sf::Vector2f(28, 49) });  // Down no slash
  for (int i = 0; i < 2; i++) {
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
  }
  //superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1042, 1805, 56, 98), superAttackSpeed, sf::Vector2f(28, 49) });  // Down no slash
  superAttackDownAnimation.setAttackFinishedCallback(superAttackFinishedCallback);

  Animation superAttackLeftAnimation(rc->sprite, true, knight->id);
  superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(977, 1613, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Left no slash
  for (int i = 0; i < 2; i++) {
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash        
  }
  //superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(977, 1613, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Left no slash
  superAttackLeftAnimation.setAttackFinishedCallback(superAttackFinishedCallback);

  ac->animations.emplace(AnimationType::SuperAttackingUp, superAttackUpAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingDown, superAttackDownAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingLeft, superAttackLeftAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingRight, superAttackRightAnimation);

  return knight;
}

Entity* EntityCreator::createSpearman(sf::Vector2f position) {
  Entity* spearman = createPlayerBase(SPEARMAN_MAX_VELOCITY, Textures::CharacterSpearman, position, SPEARMAN_MAX_HEALTH, SPEARMAN_ATTACK_SPEED);


  auto rc = mRenderComponentManager->getComponent(spearman->id);
  auto ac = mAnimationComponentManager->getComponent(spearman->id);
  auto sc = ComponentManager<StaminaComponent>::get().addComponent(spearman->id);
  sc->maxStamina = SPEARMAN_MAX_STAMINA;
  sc->currentStamina = sc->maxStamina;
  auto pc = mPlayerComponentManager->getComponent(spearman->id);
  pc->playerClass = PlayerClass::Spearman;

  auto attackCallback = [this](int entityId) {
    //TRACE_INFO("ATTACKING");
    auto player = mPlayerComponentManager->getComponent(entityId);
    createMelee(entityId, 0, player->nextAttackMousePosition);
    auto attackClock = mClockComponentManager->getComponent(entityId);
    //attackClock->clock.restart();
  };

  auto superAttackFinishedCallback = [this](int entityId) {
    handleFinishedSuperAttack(entityId);
  };

  sf::Vector2f originPosition(32, 25);
  Animation thrustUpAnimation(rc->sprite, true, spearman->id);
  for (int i = 0; i < 6; i++) {
    thrustUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 4 + 14, 64, 50), 90, originPosition });
  }
  Animation thrustLeftAnimation(rc->sprite, true, spearman->id);
  for (int i = 0; i < 6; i++) {
    thrustLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 5 + 14, 64, 50), 90, originPosition });
  }
  Animation thrustDownAnimation(rc->sprite, true, spearman->id);
  for (int i = 0; i < 6; i++) {
    thrustDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 6 + 14, 64, 50), 90, originPosition });
  }
  Animation thrustRightAnimation(rc->sprite, true, spearman->id);
  for (int i = 0; i < 6; i++) {
    thrustRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 7 + 14, 64, 50), 90, originPosition });
  }

  ac->animations.emplace(AnimationType::AttackingUp, thrustUpAnimation);
  ac->animations.emplace(AnimationType::AttackingDown, thrustDownAnimation);
  ac->animations.emplace(AnimationType::AttackingLeft, thrustLeftAnimation);
  ac->animations.emplace(AnimationType::AttackingRight, thrustRightAnimation);

  Animation superAttackUpAnimation(rc->sprite, true, spearman->id);
  int superAttackSpeed = 60;
  superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1045, 1363, 30, 110), superAttackSpeed, sf::Vector2f(15, 55) }); // Up no slash
  for (int i = 0; i < 2; i++) {
    superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
  }
  //superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1045, 1363, 30, 110), superAttackSpeed, sf::Vector2f(15, 55) }); // Up no slash
  //superAttackUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1045, 1363, 30, 110), superAttackSpeed, sf::Vector2f(11, 87) }); // Up no slash
  superAttackUpAnimation.setAttackFinishedCallback(superAttackFinishedCallback);

  Animation superAttackRightAnimation(rc->sprite, true, spearman->id);
  superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1038, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Right no slash
  for (int i = 0; i < 2; i++) {
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
  }
  //superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1038, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Right no slash
  superAttackRightAnimation.setAttackFinishedCallback(superAttackFinishedCallback);

  Animation superAttackDownAnimation(rc->sprite, true, spearman->id);
  superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1042, 1805, 56, 98), superAttackSpeed, sf::Vector2f(28, 49) });  // Down no slash
  for (int i = 0; i < 2; i++) {
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
  }
  //superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1042, 1805, 56, 98), superAttackSpeed, sf::Vector2f(28, 49) });  // Down no slash
  superAttackDownAnimation.setAttackFinishedCallback(superAttackFinishedCallback);

  Animation superAttackLeftAnimation(rc->sprite, true, spearman->id);
  superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(977, 1613, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Left no slash
  for (int i = 0; i < 2; i++) {
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
  }
  //superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(977, 1613, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Left no slash
  superAttackLeftAnimation.setAttackFinishedCallback(superAttackFinishedCallback);

  ac->animations.emplace(AnimationType::SuperAttackingUp, superAttackUpAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingDown, superAttackDownAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingLeft, superAttackLeftAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingRight, superAttackRightAnimation);
  
  return spearman;
}

Entity* EntityCreator::createArcher(sf::Vector2f position) {
  Entity* archer = createPlayerBase(ARCHER_MAX_VELOCITY, Textures::CharacterArcher, position, ARCHER_MAX_HEALTH, ARCHER_ATTACK_SPEED);

  auto rc = mRenderComponentManager->getComponent(archer->id);
  auto ac = mAnimationComponentManager->getComponent(archer->id);
  auto sc = ComponentManager<StaminaComponent>::get().addComponent(archer->id);
  sc->maxStamina = ARCHER_MAX_STAMINA;
  sc->currentStamina = sc->maxStamina;

  auto attackCallback = [this](int entityId) {
    //TRACE_INFO("ATTACKING");
    auto player = mPlayerComponentManager->getComponent(entityId);
    createArrow(entityId, 0, player->nextAttackMousePosition);
    auto attackClock = mClockComponentManager->getComponent(entityId);
    //attackClock->clock.restart();
  };

  auto superAttackFinishedCallback = [this](int entityId) {
    handleFinishedSuperAttack(entityId);
    auto player = mPlayerComponentManager->getComponent(entityId);
    sf::Vector2i randomMousePosition;
    randomMousePosition.x = (int)rand() % 1024;
    randomMousePosition.y = (int)rand() % 1024;
    createArrow(entityId, 0, randomMousePosition);
  };
  auto superAttackRandomShootCallback = [this](int entityId) {
    auto player = mPlayerComponentManager->getComponent(entityId);
    sf::Vector2i randomMousePosition;
    randomMousePosition.x = (int)rand() % 1024;
    randomMousePosition.y = (int)rand() % 1024;
    createArrow(entityId, 0, randomMousePosition);
  };
  sf::Vector2f originPosition(32, 25);
  Animation shootUpAnimation(rc->sprite, true, archer->id);
  for (int i = 0; i < 13; i++) {
    shootUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 16 + 14, 64, 50), 25, originPosition });
  }
  shootUpAnimation.setAttackCallback(attackCallback, 10);
  Animation shootLeftAnimation(rc->sprite, true, archer->id);
  for (int i = 0; i < 13; i++) {
    shootLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 17 + 14, 64, 50), 25, originPosition });

  }
  shootLeftAnimation.setAttackCallback(attackCallback, 10);
  Animation shootDownAnimation(rc->sprite, true, archer->id);
  for (int i = 0; i < 13; i++) {
    shootDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 18 + 14, 64, 50), 25, originPosition });
  }
  shootDownAnimation.setAttackCallback(attackCallback, 10);
  Animation shootRightAnimation(rc->sprite, true, archer->id);
  for (int i = 0; i < 13; i++) {
    shootRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, 64 * 19 + 14, 64, 50), 25, originPosition });
  }
  shootRightAnimation.setAttackCallback(attackCallback, 10);
  ac->animations.emplace(AnimationType::AttackingUp, shootUpAnimation);
  ac->animations.emplace(AnimationType::AttackingDown, shootDownAnimation);
  ac->animations.emplace(AnimationType::AttackingLeft, shootLeftAnimation);
  ac->animations.emplace(AnimationType::AttackingRight, shootRightAnimation);

  
  Animation superAttackAnimation(rc->sprite, true, archer->id);

  int superAttackSpeed = 60;
  for (int i = 0; i < 13; i++) {
    int randomDirection = (int)rand() % 4;
    superAttackAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, (64 * (16 + randomDirection)) + 14, 64, 50), 50, originPosition });
    superAttackAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, (64 * (16 + randomDirection)) + 14, 64, 50), 50, originPosition });
    superAttackAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, (64 * (16 + randomDirection)) + 14, 64, 50), 50, originPosition });
    superAttackAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64, (64 * (16 + randomDirection)) + 14, 64, 50), 50, originPosition });
  }
  superAttackAnimation.setAttackFinishedCallback(superAttackFinishedCallback);
  superAttackAnimation.setSuperAttackIntervallCallback(superAttackRandomShootCallback, 1);
  ac->animations.emplace(AnimationType::SuperAttackingUp, superAttackAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingDown, superAttackAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingLeft, superAttackAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingRight, superAttackAnimation);
  
  return archer;
}

Entity* EntityCreator::createRandomPickup() {
  int randomPercentage = (int)rand() % 100;
  PickupType type;
  if (randomPercentage >= 0 && randomPercentage <= 20) {
    type = PickupType::Ammo;
  } else if (randomPercentage >= 21 && randomPercentage <= 75) {
    type = PickupType::HealthPotion;
  } else {
    type = PickupType::ManaPotion;
  }

  Entity* pickup = mEntityManager->createEntity();
  TRACE_DEBUG1("Creating a pickup of type: " << static_cast<int>(type) << " with id: " << pickup->id);
  auto pc = ComponentManager<PickupComponent>::get().addComponent(pickup->id);
  pc->type = type;

  auto cc = ComponentManager<CollisionComponent>::get().addComponent(pickup->id);
  cc->collided = false;
  cc->destroyOnCollision = true;

  auto rc = ComponentManager<RenderComponent>::get().addComponent(pickup->id);
  rc->visible = true;
  rc->isDynamic = true;

  switch (pc->type)
  {
  case PickupType::HealthPotion: {
    TRACE_DEBUG1("Creating health potion! Type: " << static_cast<int>(pc->type) << " id: " << pickup->id);
    rc->textureId = Textures::HealthPotion;
    rc->sprite = sf::Sprite(*mTextures[rc->textureId]);
    auto hcc = ComponentManager<HealthChangerComponent>::get().addComponent(pickup->id);
    pc->addedEffect = HEALTH_POTION_EFFECT;
    hcc->healthChange = pc->addedEffect;
    break;
  }
  case PickupType::ManaPotion: {
    TRACE_DEBUG1("Creating mana potion created! Type: " << static_cast<int>(pc->type) << " id: " << pickup->id);
    rc->textureId = Textures::ManaPotion;
    rc->sprite = sf::Sprite(*mTextures[rc->textureId]);
    pc->addedEffect = MANA_POTION_EFFECT;
    break;
  }
  case PickupType::Ammo: {
    TRACE_DEBUG1("Creating ammo created! Type: " << static_cast<int>(pc->type) << " id: " << pickup->id);
    rc->textureId = Textures::Ammo;
    rc->sprite = sf::Sprite(*mTextures[rc->textureId]);
    break;
  }
  default:
    TRACE_ERROR("Created a pickup of unknown type: " << (int)pc->type);
    break;
  }

  auto gridPositions = mGridSystem->getEmptyGridPositions();
  if (gridPositions.size() == 0) {
    TRACE_WARNING("No empty positions in the grid!");
    return nullptr;
  }

  int randomGridPos = (int)rand() % gridPositions.size();
  auto pos = mGridSystem->gridPositionToFirstPixelPosition(gridPositions.at(randomGridPos));

  rc->sprite.setOrigin(16, 16);
  rc->sprite.setPosition(pos.x + 16.0f, pos.y + 16.0f);

  mGridSystem->addEntity(pickup->id, (sf::Vector2i)rc->sprite.getPosition());

  return pickup;
}

void EntityCreator::loadTexture(Textures textureId, std::string fileName) {
  sf::Texture* texture = new sf::Texture();
  if (!Collision::CreateTextureAndBitmask(*texture, "Client/Resources/Sprites/" + fileName)) {
    TRACE_ERROR("Could not load file: Client/Resource/Sprites/" << fileName);
  }
  mTextures[textureId] = texture;
}

void EntityCreator::handleFinishedSuperAttack(int entityId)
{
  TRACE_DEBUG1("Super attack finished");
  auto player = mPlayerComponentManager->getComponent(entityId);
  player->invinsible = false;
  if (player->superAttacks >= 0) {
    player->superAttacks--;
  }
  auto attackClock = mClockComponentManager->getComponent(entityId);
  //attackClock->clock.restart();
  ComponentManager<HealthChangerComponent>::get().removeComponent(entityId);
}
