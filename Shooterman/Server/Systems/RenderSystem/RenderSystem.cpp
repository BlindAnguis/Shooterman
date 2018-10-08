#include "RenderSystem.h"
#include "../../../Client/MessageHandler/MessageHandler.h"
#include "../../../Common/Messages/SpriteMessage.h"

RenderSystem::RenderSystem() {
  mName = "SERVER: RENDER_SYSTEM";
}

RenderSystem::RenderSystem(ComponentManager<RenderComponent>* renderComponentManager) : mRenderComponentManager(renderComponentManager) {}

RenderSystem::~RenderSystem() {}

void RenderSystem::render(std::list<sf::TcpSocket*> connectedClients) {
  SpriteMessage sm;
  for (auto entityWithRender : mRenderComponentManager->getAllEntitiesWithComponent()) {
    sf::Vector2f currentPosition = entityWithRender.second->sprite.getPosition();
    sm.addSpriteData(33, currentPosition);
  }
  MessageHandler::get().pushSpriteListMessage(sm.pack());
  for (auto client : connectedClients) {
    sf::Packet tempPacket = sm.pack();
    int id;
    int spriteId;
    sf::Vector2f currentPos;
    
    tempPacket >> id;
    tempPacket >> spriteId;
    tempPacket >> currentPos.x;
    tempPacket >> currentPos.y;
    TRACE_INFO("Sending packet with id:" << id << ", spriteId: " << spriteId << ", x: " << currentPos.x << ", y: " << currentPos.y);
    
    client->send(tempPacket);
    TRACE_INFO("Packet sent to client: " << client->getRemoteAddress());
    tempPacket.clear();
  }
}
