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
    bool m_mouse_pressed = false;
    sf::Vector2i m_mouse_start_pos; 

    float m_counter = 0.0f;
    float m_delay = 0.2f;
    bool m_paused = true;

    TileManager m_tile_manager;
    
    void handle_camera();

    void setup() override;
    void run(float dt) override;
    void event() override;
    void clear() override;
};

