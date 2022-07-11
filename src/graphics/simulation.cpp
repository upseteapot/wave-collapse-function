#include "graphics/simulation.hpp"


void Simulation::handle_camera()
{
  if (m_mouse_pressed) {
    sf::Vector2i new_mouse_pos = sf::Mouse::getPosition(m_renderer);
    sf::Vector2f offset = m_renderer.mapPixelToCoords(m_mouse_start_pos) - m_renderer.mapPixelToCoords(new_mouse_pos);
    m_mouse_start_pos = new_mouse_pos;
    m_view.move(offset);
    m_renderer.setView(m_view);
  }
}


void Simulation::setup()
{
  srand(time(0));

  m_tile_manager.create("assets/flower_tile.png", "assets/flower_tile.conf");
  m_tile_manager.set_tile_space(10, 10, 100, 100);
}

void Simulation::run(float dt)
{
  handle_camera();

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
    case sf::Event::MouseButtonPressed:
      if (m_event.mouseButton.button == sf::Mouse::Left) {
        m_mouse_pressed = true;
        m_mouse_start_pos = sf::Mouse::getPosition(m_renderer);
      }
      break;

    case sf::Event::MouseButtonReleased:
      if (m_event.mouseButton.button == sf::Mouse::Left)
        m_mouse_pressed = false;
      break;

    case sf::Event::MouseWheelMoved:
      m_view.zoom(1.0f - 0.05f * m_event.mouseWheel.delta);
      m_renderer.setView(m_view);
      break;

    case sf::Event::KeyPressed:
      switch (m_event.key.code) {
        case sf::Keyboard::Space:
          m_paused = !m_paused;
          break;
        case sf::Keyboard::R:
          m_paused = true;
          m_tile_manager.reset();
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

