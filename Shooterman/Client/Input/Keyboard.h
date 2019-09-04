#pragma once

namespace {
  enum KEY { LEFT, RIGHT, UP, DOWN, SPACE, A, D, S, W, ESC, LEFT_MOUSE, RIGHT_MOUSE };
}

class Keyboard {
public:
  virtual ~Keyboard() {}

  virtual bool isKeyPressed(KEY) = 0;
};