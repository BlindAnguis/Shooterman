#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"

class RenderSystem
{
public:
  RenderSystem();
  RenderSystem(ComponentManager<RenderComponent>* renderComponentManager);
  ~RenderSystem();
  void render();

private:
  ComponentManager<RenderComponent>* mRenderComponentManager;
};

