#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"

class DeleteSystem : Trace {
public:
  DeleteSystem();
  ~DeleteSystem();

  static DeleteSystem& get();
  void addEntity(int entityId);
  std::vector<int> getEntities();

private:
  std::vector<int> mEntitiesToRemove;
};
