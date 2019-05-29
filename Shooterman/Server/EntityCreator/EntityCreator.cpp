#include "EntityCreator.h"
#include "../Systems/CollisionSystem/Collision.h"
#include "../Systems/ManaSystem/ManaSystem.h"
#include "../Systems/EntityCreatorSystem/EntityCreatorSystem.h"
#include "../../Common/Messages/SoundMessage.h"

#define MAGE_MAX_VELOCITY 4.5f
#define KNIGHT_MAX_VELOCITY 4.8f
#define SPEARMAN_MAX_VELOCITY 4.65f
#define MAGE_MANA_COST 20
#define MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MIN 15
#define MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MAX 40

EntityCreator::EntityCreator() :
  mEntityManager(&EntityManager::get()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mClockComponentManager(&ComponentManager<ClockComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get()),
  mHealthChangerComponentManager(&ComponentManager<HealthChangerComponent>::get()),
  mGridSystem(&GridSystem::get()),
  mDeleteSystem(&DeleteSystem::get())
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
  //loadTexture(Textures::Bullet, "waterSpell.png");
  loadTexture(Textures::Bullet, "lightningBall.png");
  loadTexture(Textures::LightningStrike, "lightningBolt.png");
  loadTexture(Textures::SwordSlash, "SwordSlash.png");
  loadTexture(Textures::Tombstone, "Tombstone.png");
  loadTexture(Textures::HealthPotion, "Potions/pt1Small.png");
  loadTexture(Textures::ManaPotion, "Potions/pt2Small.png");
  loadTexture(Textures::Ammo, "Potions/pt4Small.png");
  MessageHandler::get().subscribeTo("ServerSoundList", &mSoundSubscriber);
}

EntityCreator::~EntityCreator() {
  MessageHandler::get().unsubscribeTo("ServerSoundList", &mSoundSubscriber);
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

  auto pc = mPlayerComponentManager->addComponent(player->id);
  pc->attackSpeed = attackSpeed;
  pc->state = PlayerState::Idle;

  mGridSystem->addEntity(player->id, (sf::Vector2i)rc->sprite.getPosition());

  return player;
}

Entity* EntityCreator::createMage(sf::Vector2f position) {
  
  Entity* mage = createPlayerBase(MAGE_MAX_VELOCITY, Textures::CharacterMage, position, 100, 500);
  
  auto rc = mRenderComponentManager->getComponent(mage->id);
  auto ac = mAnimationComponentManager->getComponent(mage->id);
  auto hc = mHealthComponentManager->getComponent(mage->id);
  hc->maxHealth = 100;
  hc->currentHealth = hc->maxHealth;
  hc->isAlive = true;
  auto mc = ComponentManager<ManaComponent>::get().addComponent(mage->id);
  mc->maxMana = 100;
  mc->currentMana = mc->maxMana;
  
  auto attackCallback = [this](int entityId) {
    //TRACE_INFO("ATTACKING");
    auto player = mPlayerComponentManager->getComponent(entityId);
    auto mc = ComponentManager<ManaComponent>::get().getComponent(entityId);
    if (mc) {
      if (mc->currentMana >= MAGE_MANA_COST) {
        createBullet(entityId, 0, player->nextAttackMousePosition);
        ManaSystem::get().changeMana(entityId, -MAGE_MANA_COST);
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
      EntityCreatorSystem::get().addEntityToCreate(EntityType::LightningStrike, sf::Vector2f(posX, posY), framesUntilAttack, { entityId });
    }
  };

  auto superAttackFinishedCallback = [this](int entityId) {
    TRACE_INFO("Super attack finished");
    auto player = mPlayerComponentManager->getComponent(entityId);
    player->invinsible = false;
    player->superAttacks--;
    player->state = PlayerState::Idle;
    ComponentManager<HealthChangerComponent>::get().removeComponent(entityId);
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

  Entity* knight = createPlayerBase(KNIGHT_MAX_VELOCITY, Textures::CharacterKnight, position, 100, 400);

  auto rc = mRenderComponentManager->getComponent(knight->id);
  auto ac = mAnimationComponentManager->getComponent(knight->id);
  auto hc = mHealthComponentManager->getComponent(knight->id);
  hc->maxHealth = 200;
  hc->currentHealth = hc->maxHealth;
  hc->isAlive = true;
  auto sc = ComponentManager<StaminaComponent>::get().addComponent(knight->id);
  sc->maxStamina = 100;
  sc->currentStamina = sc->maxStamina;

  auto attackCallback = [this](int entityId) {
    //TRACE_INFO("ATTACKING");
    auto player = mPlayerComponentManager->getComponent(entityId);
    createMelee(entityId, 0, player->nextAttackMousePosition);
  };

  auto attackFinishedCallback = [this](int entityId) {
    TRACE_INFO("Attack finished");
    auto player = mPlayerComponentManager->getComponent(entityId);
    player->invinsible = false;
    player->superAttacks--;
    player->state = PlayerState::Idle;
    ComponentManager<HealthChangerComponent>::get().removeComponent(entityId);
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
  superAttackUpAnimation.setAttackFinishedCallback(attackFinishedCallback);

  Animation superAttackRightAnimation(rc->sprite, true, knight->id);
  superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1038, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Right no slash
  for (int i = 0; i < 2; i++) {
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
  }
  //superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1038, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Right no slash
  superAttackRightAnimation.setAttackFinishedCallback(attackFinishedCallback);

  Animation superAttackDownAnimation(rc->sprite, true, knight->id);
  superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1042, 1805, 56, 98), superAttackSpeed, sf::Vector2f(28, 49) });  // Down no slash
  for (int i = 0; i < 2; i++) {
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
  }
  //superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1042, 1805, 56, 98), superAttackSpeed, sf::Vector2f(28, 49) });  // Down no slash
  superAttackDownAnimation.setAttackFinishedCallback(attackFinishedCallback);

  Animation superAttackLeftAnimation(rc->sprite, true, knight->id);
  superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(977, 1613, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Left no slash
  for (int i = 0; i < 2; i++) {
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash        
  }
  //superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(977, 1613, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Left no slash
  superAttackLeftAnimation.setAttackFinishedCallback(attackFinishedCallback);

  ac->animations.emplace(AnimationType::SuperAttackingUp, superAttackUpAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingDown, superAttackDownAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingLeft, superAttackLeftAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingRight, superAttackRightAnimation);

  return knight;
}

Entity* EntityCreator::createSpearman(sf::Vector2f position) {

  Entity* spearman = createPlayerBase(SPEARMAN_MAX_VELOCITY, Textures::CharacterSpearman, position, 100, 450);

  auto rc = mRenderComponentManager->getComponent(spearman->id);
  auto ac = mAnimationComponentManager->getComponent(spearman->id);
  auto hc = mHealthComponentManager->getComponent(spearman->id);
  hc->maxHealth = 150;
  hc->currentHealth = hc->maxHealth;
  hc->isAlive = true;
  auto sc = ComponentManager<StaminaComponent>::get().addComponent(spearman->id);
  sc->maxStamina = 100;
  sc->currentStamina = sc->maxStamina;

  auto attackCallback = [this](int entityId) {
    //TRACE_INFO("ATTACKING");
    auto player = mPlayerComponentManager->getComponent(entityId);
    createMelee(entityId, 0, player->nextAttackMousePosition);
  };

  auto attackFinishedCallback = [this](int entityId) {
    TRACE_INFO("Attack finished");
    auto player = mPlayerComponentManager->getComponent(entityId);
    player->invinsible = false;
    player->superAttacks--;
    player->state = PlayerState::Idle;
    ComponentManager<HealthChangerComponent>::get().removeComponent(entityId);
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
  superAttackUpAnimation.setAttackFinishedCallback(attackFinishedCallback);

  Animation superAttackRightAnimation(rc->sprite, true, spearman->id);
  superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1038, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Right no slash
  for (int i = 0; i < 2; i++) {
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
  }
  //superAttackRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1038, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Right no slash
  superAttackRightAnimation.setAttackFinishedCallback(attackFinishedCallback);

  Animation superAttackDownAnimation(rc->sprite, true, spearman->id);
  superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1042, 1805, 56, 98), superAttackSpeed, sf::Vector2f(28, 49) });  // Down no slash
  for (int i = 0; i < 2; i++) {
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
  }
  //superAttackDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1042, 1805, 56, 98), superAttackSpeed, sf::Vector2f(28, 49) });  // Down no slash
  superAttackDownAnimation.setAttackFinishedCallback(attackFinishedCallback);

  Animation superAttackLeftAnimation(rc->sprite, true, spearman->id);
  superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(977, 1613, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Left no slash
  for (int i = 0; i < 2; i++) {
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1173, 1579, 97, 86), superAttackSpeed, sf::Vector2f(48, 43) }); // Left slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1237, 1371, 80, 102), superAttackSpeed, sf::Vector2f(40, 51) }); // Up slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1226, 1997, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) }); // Right slash
    superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(1185, 1798, 75, 98), superAttackSpeed, sf::Vector2f(37, 49) }); // Down slash
  }
  //superAttackLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(977, 1613, 97, 52), superAttackSpeed, sf::Vector2f(48, 26) });  // Left no slash
  superAttackLeftAnimation.setAttackFinishedCallback(attackFinishedCallback);

  ac->animations.emplace(AnimationType::SuperAttackingUp, superAttackUpAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingDown, superAttackDownAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingLeft, superAttackLeftAnimation);
  ac->animations.emplace(AnimationType::SuperAttackingRight, superAttackRightAnimation);

  return spearman;
}

Entity* EntityCreator::createRandomPickup() {
  int randomPercentage = (int)rand() % 101;
  PickupType type;
  if (randomPercentage >= 0 && randomPercentage <= 99) {
    type = PickupType::Ammo;
  } else if (randomPercentage >= 100 && randomPercentage <= 101) {
    type = PickupType::HealthPotion;
  } else {
    type = PickupType::ManaPotion;
  }

  Entity* pickup = mEntityManager->createEntity();
  TRACE_DEBUG1("Creating a pickup of type: " << static_cast<int>(type) << " with id: " << pickup->id);
  auto pc = ComponentManager<PickupComponent>::get().addComponent(pickup->id);
  pc->type = type;
  pc->addedEffect = 50;

  auto cc = ComponentManager<CollisionComponent>::get().addComponent(pickup->id);
  cc->collided = false;
  cc->destroyOnCollision = true;

  auto rc = ComponentManager<RenderComponent>::get().addComponent(pickup->id);
  switch (pc->type)
  {
  case PickupType::HealthPotion: {
    TRACE_DEBUG1("Creating health potion! Type: " << static_cast<int>(pc->type) << " id: " << pickup->id);
    rc->textureId = Textures::HealthPotion;
    rc->sprite = sf::Sprite(*mTextures[rc->textureId]);
    auto hcc = ComponentManager<HealthChangerComponent>::get().addComponent(pickup->id);
    hcc->healthChange = pc->addedEffect;
    break;
  }
  case PickupType::ManaPotion: {
    TRACE_DEBUG1("Creating mana potion created! Type: " << static_cast<int>(pc->type) << " id: " << pickup->id);
    rc->textureId = Textures::ManaPotion;
    rc->sprite = sf::Sprite(*mTextures[rc->textureId]);
    break;
  }
  case PickupType::Ammo: {
    TRACE_DEBUG1("Creating ammo created! Type: " << static_cast<int>(pc->type) << " id: " << pickup->id);
    rc->textureId = Textures::Ammo;
    rc->sprite = sf::Sprite(*mTextures[rc->textureId]);
    break;
  }
  default:
    break;
  }
  rc->visible = true;
  rc->isDynamic = true;

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
