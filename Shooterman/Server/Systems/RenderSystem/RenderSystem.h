#pragma once

#include <list>
#include <SFML\Network.hpp>
#include "../NetworkSystem/NetworkSystem.h"
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../EntityManager/EntityManager.h"
#include "../../Player.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common/Messages/SpriteMessage.h"
#include "../../../Common/Messages/SpriteCacheMessage.h"
#include "../../../Common/Trace.h"

class RenderSystem : Trace {
public:
  RenderSystem(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<NetworkSystem> networkSystem);
  ~RenderSystem();
  void render(std::shared_ptr<std::map<int, Player*>> connectedClients);

private:
  bool mSentCachedSpriteList = false;
  ComponentManager<RenderComponent>* mRenderComponentManager;
  std::shared_ptr<MessageHandler> mMessageHandler;
  std::shared_ptr<NetworkSystem> mNetworkSystem;

  void renderCached(std::shared_ptr<std::map<int, Player*>> connectedClients);
};

