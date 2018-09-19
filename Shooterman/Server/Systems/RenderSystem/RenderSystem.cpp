#include "RenderSystem.h"
#include "../../../Client/MessageHandler/MessageHandler.h"
#include "../../../Common/Messages/SpriteMessage.h"

RenderSystem::RenderSystem() {}

RenderSystem::RenderSystem(ComponentManager<RenderComponent>* renderComponentManager) : mRenderComponentManager(renderComponentManager) {}

RenderSystem::~RenderSystem() {}

void RenderSystem::render() {
  sf::Packet renderPacket;
  SpriteMessage sm;
  for (auto entityWithRender : mRenderComponentManager->getAllEntitiesWithComponent()) {
    sf::Vector2f currentPosition = entityWithRender.second->sprite.getPosition();
    sm.addSpriteData(33, currentPosition);
  }
  MessageHandler::get().pushSpriteListMessage(sm.pack());
}
