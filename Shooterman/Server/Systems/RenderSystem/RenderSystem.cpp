#include "RenderSystem.h"

RenderSystem::RenderSystem() {
  mName = "SERVER: RENDER_SYSTEM";
}

RenderSystem::RenderSystem(ComponentManager<RenderComponent>* renderComponentManager)
  : mRenderComponentManager(renderComponentManager) {}

RenderSystem::~RenderSystem() {}

void RenderSystem::render(std::shared_ptr<std::map<int, Player*>> connectedClients) {

  if (!mSentCachedSpriteList) {
    renderCached(connectedClients);
    mSentCachedSpriteList = true;
  } else {
    SpriteMessage sm;
    for (auto entityWithRender : mRenderComponentManager->getAllEntitiesWithComponent()) {
      if (entityWithRender.second->isDynamic) {
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