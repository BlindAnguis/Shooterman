#pragma once
#include <vector>
#include <map>

template <typename Component>
class ComponentManager {
public:
  static ComponentManager& get() {
    static ComponentManager instance;
    return instance;
  }

  std::vector<Component*> getAllComponents() {
    std::vector<Component*> components;
    for (auto entry : mComponents) {
      components.push_back(entry.second);
    }
    return components;
  }

  std::map<int, Component*> getAllEntitiesWithComponent() {
    return mComponents;
  }

  Component* getComponent(int entityId) {
    auto it = mComponents.find(entityId);
    return it != mComponents.end() ? it->second : nullptr;
  }

  bool hasComponent(int entityId) {
    return mComponents.count(entityId);
  }

  Component* addComponent(int entityId) {
    Component* newComponent = new Component;
    mComponents.emplace(entityId, newComponent);
    return newComponent;
  }

  void removeComponent(int entityId) {
    auto it = mComponents.find(entityId);
    if (it != mComponents.end()) {
      delete it->second;
      mComponents.erase(entityId);
    }
  }

private:
  std::map<int, Component*> mComponents;

  ComponentManager<Component>() {}
};