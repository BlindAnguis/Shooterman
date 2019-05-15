#include "RenderSystem.h"

RenderSystem::RenderSystem()
  : mRenderComponentManager(&ComponentManager<RenderComponent>::get()), mNetworkSystem(&NetworkSystem::get()) {
  mName = "SERVER: RENDER_SYSTEM";
}

RenderSystem::~RenderSystem() { }

RenderSystem& RenderSystem::get() {
  static RenderSystem instance;
  return instance;
}

void RenderSystem::render(std::shared_ptr<std::map<int, Player*>> connectedClients) {

  if (!mSentCachedSpriteList) {
    renderCached(connectedClients);
    mSentCachedSpriteList = true;
  }
  sf::Clock c;
  std::shared_ptr<SpriteMessage> sm = std::make_shared<SpriteMessage>();
  for (auto entityWithRender : mRenderComponentManager->getAllEntitiesWithComponent()) {
    if (entityWithRender.second->isDynamic && entityWithRender.second->visible) {
      sf::Vector2f currentPosition = entityWithRender.second->sprite.getPosition();

      SpriteData data;
      data.textureId = entityWithRender.second->textureId;
      data.position = entityWithRender.second->sprite.getPosition();
      data.texturePosition = entityWithRender.second->sprite.getTextureRect();
      data.rotation = entityWithRender.second->sprite.getRotation();

      sm->addSpriteData(data);
    }
  }
  auto first = c.getElapsedTime().asMicroseconds();
  c.restart();
  mNetworkSystem->setRenderData(sm);
  auto second = c.getElapsedTime().asMicroseconds();
  //TRACE_INFO("Render f: " << first << "us, s: " << second << "us");
}

void RenderSystem::renderCached(std::shared_ptr<std::map<int, Player*>> connectedClients) {
  SpriteCacheMessage sm;
  for (auto entityWithRender : mRenderComponentManager->getAllEntitiesWithComponent()) {
    if (!entityWithRender.second->isDynamic) {
      sf::Vector2f currentPosition = entityWithRender.second->sprite.getPosition();

      SpriteData data;
      data.textureId = entityWithRender.second->textureId;
      data.position = entityWithRender.second->sprite.getPosition();
      data.texturePosition = entityWithRender.second->sprite.getTextureRect();
      data.rotation = entityWithRender.second->sprite.getRotation();

      sm.addSpriteData(data);
    }
  }

  for (auto client : *connectedClients) {
    if (client.second->getSocket()) {
      sf::Packet tempPacket = sm.pack();
      client.second->getSocket()->send(tempPacket);
      tempPacket.clear();
    }
  }
}