#include "RenderSystem.h"

RenderSystem::RenderSystem(std::shared_ptr<MessageHandler> messageHandler)
  : mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mMessageHandler(messageHandler) {
  mName = "SERVER: RENDER_SYSTEM";
}

RenderSystem::~RenderSystem() { TRACE_DEBUG1("Enter Destructor"); }

RenderSystem& RenderSystem::get(std::shared_ptr<MessageHandler> messageHandler) {
  static RenderSystem instance(messageHandler);
  return instance;
}

void RenderSystem::resetSystem() {
  mSentCachedSpriteList = false;
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
  NetworkSystem::get(mMessageHandler).setRenderData(sm);
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