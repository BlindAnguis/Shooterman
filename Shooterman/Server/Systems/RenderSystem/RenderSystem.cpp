#include "RenderSystem.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common/Messages/SpriteMessage.h"
#include "../../EntityManager/EntityManager.h"

RenderSystem::RenderSystem() {
  mName = "SERVER: RENDER_SYSTEM";
}

RenderSystem::RenderSystem(ComponentManager<RenderComponent>* renderComponentManager) : mRenderComponentManager(renderComponentManager) {}

RenderSystem::~RenderSystem() {}

void RenderSystem::render(std::shared_ptr<std::map<int, std::pair<sf::TcpSocket*, Entity*>>> connectedClients) {
  SpriteMessage sm;
  for (auto entityWithRender : mRenderComponentManager->getAllEntitiesWithComponent()) {
    sf::Vector2f currentPosition = entityWithRender.second->sprite.getPosition();

    SpriteData data;
    data.textureId = entityWithRender.second->textureId;
    data.position = entityWithRender.second->sprite.getPosition();
    data.texturePosition = entityWithRender.second->sprite.getTextureRect();
    data.rotation = entityWithRender.second->sprite.getRotation();

    sm.addSpriteData(data);
  }

  for (auto client : *connectedClients) {
    if (client.second.first) {
      sf::Packet tempPacket = sm.pack();
      int id;
      int textureId;
      sf::Vector2f currentPos;
      sf::IntRect currentTexturePos;
    
      tempPacket >> id;
      tempPacket >> textureId;
      tempPacket >> currentPos.x >> currentPos.y;
      tempPacket >> currentTexturePos.left >> currentTexturePos.top >> currentTexturePos.width >> currentTexturePos.height;
      //TRACE_INFO("Sending packet with id:" << id << ", textureId: " << textureId << ", x: " << currentPos.x << ", y: " << currentPos.y << "left: " << currentTexturePos.left << "top: " << currentTexturePos.top << "width: " << currentTexturePos.width << "height: " << currentTexturePos.height);

      client.second.first->send(tempPacket);
      //TRACE_INFO("Packet sent to client: " << client.second.first->getRemoteAddress());
   
      tempPacket.clear();
    } else {
      //Host
    }
  }
}
