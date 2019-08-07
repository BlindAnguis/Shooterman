#pragma once
#include <vector>
#include <map>
#include <memory>

template <typename Component>
class ComponentManager {
public:
  static ComponentManager& get() {
    static ComponentManager instance;
    return instance;
  }

  std::vector<std::shared_ptr<Component>> getAllComponents() {
    std::vector<std::shared_ptr<Component>> components;
    for (auto entry : mComponents) {
      components.push_back(entry.second);
    }
    return components;
  }

  std::map<int, std::shared_ptr<Component>>& getAllEntitiesWithComponent() {
    return mComponents;
  }

  std::shared_ptr<Component> getComponent(int entityId) {
    auto it = mComponents.find(entityId);
    return it != mComponents.end() ? it->second : nullptr;
  }

  bool hasComponent(int entityId) {
    return mComponents.count(entityId);
  }

  std::shared_ptr<Component> addComponent(int entityId) {
    std::shared_ptr<Component> newComponent = std::make_shared<Component>();
    mComponents.emplace(entityId, newComponent);
    return newComponent;
  }

  void removeComponent(int entityId) {
    auto it = mComponents.find(entityId);
    if (it != mComponents.end()) {
      mComponents.erase(entityId);
    }
  }

  void clearManager() {
    mComponents.clear();
  }

private:
  std::map<int, std::shared_ptr<Component>> mComponents;

  ComponentManager<Component>() {}
};