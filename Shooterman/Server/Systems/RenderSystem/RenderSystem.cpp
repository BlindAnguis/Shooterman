#include "RenderSystem.h"
#include "../../../Client/MessageHandler/MessageHandler.h"

RenderSystem::RenderSystem() {}

RenderSystem::RenderSystem(ComponentManager<RenderComponent>* renderComponentManager) : mRenderComponentManager(renderComponentManager) {}

RenderSystem::~RenderSystem() {}

void RenderSystem::render() {
  sf::Packet renderPacket;
  for (auto entityWithRender : mRenderComponentManager->getAllEntitiesWithComponent()) {
    sf::Vector2f currentPosition = entityWithRender.second->sprite.getPosition();
    renderPacket << 33 << currentPosition.x << currentPosition.y;
    MessageHandler::get().pushSpriteListMessage(renderPacket);
    renderPacket.clear();
  }
}
