#include "graphics/tile_manager.hpp"


void TileManager::create(std::string sheet_path, std::string conf_path)
{
  std::ifstream config_file;
  config_file.open(conf_path);
  
  int width, height;
  config_file >> width;
  config_file >> height;
  config_file >> m_tile_types;
  
  m_sorted_tile_connections.resize(m_tile_types);
  
  std::size_t tile_connection;
  for (std::size_t i=0; i < m_tile_types; i++)
    for (std::size_t dir=0; dir < 4; dir++) {
      config_file >> tile_connection;
      m_sorted_tile_connections[i].push_back(tile_connection);
    }

  config_file.close();
  
  m_sheet_texture.loadFromFile(sheet_path);
  m_tiles.resize(m_tile_types);
  for (std::size_t i=0; i < m_tile_types; i++) {
    m_tiles[i].setTexture(m_sheet_texture);
    m_tiles[i].setTextureRect(sf::IntRect(width * i, 0, width, height));
  }
}

void TileManager::set_tile_space(std::size_t rows, std::size_t cols, int width, int height)
{
  m_rows = rows;
  m_cols = cols;
  m_width = width;
  m_height = height;

  m_options.resize(rows * cols);
  
  m_background.setFillColor(sf::Color(127, 127, 127));
  m_background.setSize(sf::Vector2f((float)(m_cols * m_width), (float)(m_rows * m_height)));
  m_background.setPosition(-(m_cols * m_width / 2.f), -(m_rows * m_height / 2.f));

  for (auto &option : m_options)
    for (std::size_t i=0; i < m_tile_types; i++)
      option.push_back(i);

  for (auto &tile : m_tiles)
    tile.setScale(
      (float)m_width  / tile.getLocalBounds().width,
      (float)m_height / tile.getLocalBounds().height 
    );
}

bool TileManager::wave_collapse_step()
{
  std::size_t minimum_option_size = m_tile_types + 1;
  std::vector<std::size_t> possible_tiles;
  std::size_t i=0;

  for (auto &option : m_options) {
    if (option.size() < minimum_option_size && option.size() != 1) {
      minimum_option_size = option.size();
      possible_tiles.clear();
      possible_tiles.push_back(i);
    } else if (option.size() == minimum_option_size) {
      possible_tiles.push_back(i);
    }
    i++;
  }
  
  if (possible_tiles.size() == 0) 
    return false;

  std::size_t tile = possible_tiles[0];
  if (possible_tiles.size() > 1) {
    i = std::round((possible_tiles.size() - 1) * ((float)rand() / (float)RAND_MAX));
    tile = possible_tiles[i];
  }
  
  i = std::round((m_options[tile].size() - 1) * ((float)rand() / (float)RAND_MAX));
  std::size_t collapsed_tile_type = m_options[tile][i];
  m_options[tile].clear();
  m_options[tile].push_back(collapsed_tile_type);
  
  std::size_t x = tile % m_cols;
  std::size_t y = std::floor((float)tile / m_rows);

  if (y > 0)
    m_collapse_tile(tile, 0, tile - m_rows, 2);
  if (y < m_rows - 1)
    m_collapse_tile(tile, 2, tile + m_rows, 0);
  if (x > 0)
    m_collapse_tile(tile, 3, tile - 1, 1);
  if (x < m_cols - 1)
    m_collapse_tile(tile, 1, tile + 1, 3);
  
  return true;
}

void TileManager::draw(sf::RenderWindow &renderer)
{
  renderer.draw(m_background);

  sf::Vector2f start_pos = {-(m_cols * m_width / 2.f), -(m_rows * m_height / 2.f)};
  for (std::size_t y=0; y < m_rows; y++)
    for (std::size_t x=0; x < m_cols; x++) {
      if (m_options[x + y * m_rows].size() == 1) {
        std::size_t index = m_options[x + y * m_rows][0];
        m_tiles[index].setPosition(start_pos + sf::Vector2f(x * m_width, y * m_height));
        renderer.draw(m_tiles[index]);
      }
    }
}


void TileManager::m_collapse_tile(std::size_t from, std::size_t edge_from, std::size_t to, std::size_t edge_to)
{
  std::size_t connection = m_sorted_tile_connections[m_options[from][0]][edge_from];
  if (m_options[to].size() != 1) {
    auto iter = m_options[to].begin();
    while (iter != m_options[to].end()) {
      if (m_sorted_tile_connections[*iter][edge_to] != connection)
        iter = m_options[to].erase(iter);
      else 
        iter++;
    }
    
    if (m_options[to].size() == 1) {
      std::size_t x = to % m_cols;
      std::size_t y = std::floor((float)to / m_rows);

      if (y > 0)
        m_collapse_tile(to, 0, to - m_rows, 2);
      if (y < m_rows - 1)
        m_collapse_tile(to, 2, to + m_rows, 0);
      if (x > 0)
        m_collapse_tile(to, 3, to - 1, 1);
      if (x < m_cols - 1)
        m_collapse_tile(to, 1, to + 1, 3);
    }

  }
}

