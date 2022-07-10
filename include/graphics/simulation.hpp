#pragma once
#include <random>
#include <ctime>

#include <SFML/Graphics.hpp>
#include "graphics/engine.hpp"
#include "graphics/tile_manager.hpp"


class Simulation : public Engine
{
  public:
    Simulation() = default;

  private:
    float m_counter = 0.0f;
    float m_delay = 0.2f;
    bool m_paused = true;

    TileManager m_tile_manager;

    void setup() override;
    void run(float dt) override;
    void event() override;
    void clear() override;
};

