#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <map>

#include <SFML/Graphics.hpp>

#define SPRITE_FILE_PATH "Client/Resources/Sprites/"

class SpriteManager {
public:
  SpriteManager();
  ~SpriteManager();

  void loadSprites();
  sf::Texture* loadTexture(std::string fileName);
  sf::Sprite get(int id) { return mSpriteMap.at(id).first; }

private:
  std::map<int, std::pair<sf::Sprite, sf::Texture*>> mSpriteMap;
};

#endif // SPRITE_MANAGER_H