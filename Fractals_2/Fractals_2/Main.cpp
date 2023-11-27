#include <chrono>
#include <random>
#include <vector>
#include <algorithm>  
#include <SFML/Graphics.hpp>

#include "Cell.hpp"
#include "DrawText.hpp"
#include "Field.hpp"
#include "GetCell.hpp"
#include "Global.hpp"

using namespace std;

template <typename T>
constexpr const T& clamp(const T& value, const T& low, const T& high) {
	return (value < low) ? low : ((value > high) ? high : value);
}

int main()
{
	unsigned lag = 0;

	
	unsigned char Alexander_state = 0;


	std::chrono::time_point<std::chrono::steady_clock> previous_time;

	// Тут починається створення графічного вікна
	sf::Event event;

	
	sf::RenderWindow window(sf::VideoMode(CELL_SIZE * COLUMNS * SCREEN_RESIZE, SCREEN_RESIZE * (FONT_HEIGHT + CELL_SIZE * COLUMNS)), "Minesweeper", sf::Style::Close);
	
	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * COLUMNS, FONT_HEIGHT + CELL_SIZE * ROWS)));

	
	sf::Sprite Alexander;

	
	sf::Texture Alexander_texture;
	
	Alexander_texture.loadFromFile("Alexander.png");

	
	Alexander.setPosition(static_cast<float>(CELL_SIZE * COLUMNS - Alexander_texture.getSize().y), CELL_SIZE * ROWS);
	Alexander.setTexture(Alexander_texture);

	
	Field field;

	// Зберігаємо поточний час у зміній
	previous_time = std::chrono::steady_clock::now();

	while (1 == window.isOpen())
	{
		unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();

		lag += delta_time;

		previous_time += std::chrono::microseconds(delta_time);

		
		while (FRAME_DURATION <= lag)
		{
			//Пробуємо отримати координати комірки 
			unsigned char mouse_cell_x = clamp(static_cast<int>(floor(sf::Mouse::getPosition(window).x / static_cast<float>(CELL_SIZE * SCREEN_RESIZE))), 0, COLUMNS - 1);
			unsigned char mouse_cell_y = clamp(static_cast<int>(floor(sf::Mouse::getPosition(window).y / static_cast<float>(CELL_SIZE * SCREEN_RESIZE))), 0, ROWS - 1);

			
			lag -= FRAME_DURATION;

			while (1 == window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
				{
					window.close();
					break;
				}
				case sf::Event::KeyReleased:
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Enter:
					{
						field.restart();
					}
					}

					break;
				}
				case sf::Event::MouseButtonReleased:
				{
					switch (event.mouseButton.button)
					{
					case sf::Mouse::Left:
					{
						field.open_cell(mouse_cell_x, mouse_cell_y);

						break;
					}
					case sf::Mouse::Right:
					{
						field.flag_cell(mouse_cell_x, mouse_cell_y);
					}
					}
				}
				}
			}

			//Тут визначаємо картинку
			if (1 == sf::Mouse::isButtonPressed(sf::Mouse::Left) || 1 == sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				Alexander_state = 1;

				field.set_mouse_state(2, mouse_cell_x, mouse_cell_y);
			}
			else
			{
				Alexander_state = 0;

				field.set_mouse_state(1, mouse_cell_x, mouse_cell_y);
			}
			if (-1 == field.get_game_over())
			{

				Alexander_state = 2;
			}
			else if (1 == field.get_game_over())
			{
				Alexander_state = 3;
			}

			if (FRAME_DURATION > lag)
			{
				window.clear();

				field.draw(window);

				//Тут вилазить текст з інформацією чи виграна гра чи ні 
				if (1 == field.effect_over())
				{
					if (1 == field.get_game_over())
					{
						draw_text(1, static_cast<unsigned short>(round(0.5f * (CELL_SIZE * COLUMNS - 8 * FONT_WIDTH))), static_cast<unsigned short>(round(0.5f * (CELL_SIZE * ROWS - FONT_HEIGHT))), "VICTORY!", window);
					}
					else
					{
						draw_text(1, static_cast<unsigned short>(round(0.5f * (CELL_SIZE * COLUMNS - 4 * FONT_WIDTH))), static_cast<unsigned short>(round(0.5f * (CELL_SIZE * ROWS - 2 * FONT_HEIGHT))), "GAME\nOVER", window);
					}
				}

				// Скільки мін лишилось 
				draw_text(0, 0, CELL_SIZE * ROWS, "Mines:" + std::to_string(MINES - field.get_flags()), window);

				Alexander.setTextureRect(sf::IntRect(Alexander_state * Alexander_texture.getSize().y, 0, Alexander_texture.getSize().y, Alexander_texture.getSize().y));

				//Малюємо Олександра всюди , бо він цього заслужив 
				window.draw(Alexander);
				window.display();
			}
		}
	}
}