#pragma once

#include <list>
#include <SFML\Network.hpp>
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"
#include "../../EntityManager/EntityManager.h"

class RenderSystem : Trace {
public:
  RenderSystem();
  RenderSystem(ComponentManager<RenderComponent>* renderComponentManager);
  ~RenderSystem();
  void render(std::map<int, std::pair<sf::TcpSocket*, Entity*>> connectedClients);

private:
  ComponentManager<RenderComponent>* mRenderComponentManager;
};

