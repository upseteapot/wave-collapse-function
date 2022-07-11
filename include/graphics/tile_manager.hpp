#pragma once
#include <iostream>
#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>


class TileManager
{
  public:
    TileManager() = default;
    void create(std::string sheet_path, std::string config_path);
    void set_tile_space(std::size_t rows, std::size_t cols, int width, int height);
    bool wave_collapse_step();
    void draw(sf::RenderWindow &renderer);
    void reset();

  private:
    std::size_t m_rows, m_cols;
    std::size_t m_tile_types;
    int m_width, m_height;
  
    sf::RectangleShape m_background;
    sf::Texture m_sheet_texture;
    std::vector<sf::Sprite> m_tiles;

    std::vector<std::vector<std::size_t>> m_sorted_tile_connections;
    std::vector<std::vector<std::size_t>> m_options;
  
    void m_collapse_tile(std::size_t from, std::size_t edge_from, std::size_t to, std::size_t edge_to);
};

