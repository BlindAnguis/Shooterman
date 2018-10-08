#pragma once

#include <list>
#include <SFML\Network.hpp>
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"

class RenderSystem : Trace {
public:
  RenderSystem();
  RenderSystem(ComponentManager<RenderComponent>* renderComponentManager);
  ~RenderSystem();
  void render(std::list<sf::TcpSocket*> connectedClients);

private:
  ComponentManager<RenderComponent>* mRenderComponentManager;
};

