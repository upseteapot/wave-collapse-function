#include "graphics/simulation.hpp"


void Simulation::setup()
{
  srand(time(0));

  m_tile_manager.create("assets/tetris_tile.png", "assets/tetris_tile.conf");
  m_tile_manager.set_tile_space(60, 60, 10, 10);
}

void Simulation::run(float dt)
{
  m_renderer.clear(sf::Color::Black);
  
  if (!m_paused) {
    if ((m_counter += dt) >= m_delay) {
      m_counter = 0.0f;
      m_paused = !m_tile_manager.wave_collapse_step();
    }
  }

  m_tile_manager.draw(m_renderer);
  m_renderer.display();
}

void Simulation::event()
{
  switch (m_event.type) {
    case sf::Event::KeyPressed:
      switch (m_event.key.code) {
        case sf::Keyboard::Space:
          m_paused = !m_paused;
          break;

        default:
          break;
      }
      break;
    default: 
      break;
  }
}

void Simulation::clear()
{
}

