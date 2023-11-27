#include <vector>
#include <algorithm>  

#include "Cell.hpp"
#include "GetCell.hpp"
#include "Global.hpp"

Cell::Cell(unsigned char i_x, unsigned char i_y) :
	mines_around(0),
	x(i_x),
	y(i_y)
{
	reset();
}

bool Cell::get_is_flagged()
{
	return is_flagged;
}

bool Cell::get_is_mine()
{
	return is_mine;
}

bool Cell::get_is_open()
{
	return is_open;
}

bool Cell::open(std::vector<Cell>& i_cells)
{
	if (0 == is_open)
	{
		is_open = 1;

		// Якщо тут нема міни , і поблиззу , тоді відкриваємо всі 
		if (0 == is_mine && 0 == mines_around)
		{
			for (char a = -1; a < 2; a++)
			{
				for (char b = -1; b < 2; b++)
				{
					if ((0 == a && 0 == b) || (0 > a + x || 0 > b + y || COLUMNS == a + x || ROWS == b + y))
					{
						continue;
					}

					// відкриваємо сусідні 
					get_cell(a + x, b + y, i_cells)->open(i_cells);
				}
			}
		}

		// Анімація програшу 
		effect_timer -= is_mine;

		return is_mine;
	}

	return 0;
}

bool Cell::update_effect_timer()
{
	if (0 < effect_timer)
	{
		effect_timer--;

		if (0 == effect_timer)
		{
			return 1;
		}
	}
	return 0;
}

unsigned char Cell::get_effect_timer()
{
	return effect_timer;
}

unsigned char Cell::get_mines_around()
{
	return mines_around;
}

unsigned char Cell::get_mouse_state()
{
	return mouse_state;
}

void Cell::count_mines_around(std::vector<Cell>& i_cells)
{
	mines_around = 0;

	//Якщо в цій комірці є міна, то немає сенсу рахувати міни навколо неї
	if (0 == is_mine)
	{
		for (char a = -1; a < 2; a++)
		{
			for (char b = -1; b < 2; b++)
			{
				if ((0 == a && 0 == b) || (0 > a + x || 0 > b + y || COLUMNS == a + x || ROWS == b + y))
				{
					continue;
				}

				//Якщо в сусідній є 
				if (1 == get_cell(a + x, b + y, i_cells)->get_is_mine())
				{
					
				    mines_around++;
					
				}
			}
		}
	}
}

void Cell::flag()
{
	if (0 == is_open)
	{
		is_flagged = 1 - is_flagged;
	}
}

void Cell::reset()
{
	is_flagged = 0;
	is_mine = 0;
	is_open = 0;

	effect_timer = EFFECT_DURATION;
	mouse_state = 0;
}

void Cell::set_effect_timer(unsigned char i_effect_timer)
{
	effect_timer = i_effect_timer;
}

void Cell::set_mine()
{
	is_mine = 1;
}

void Cell::set_mouse_state(unsigned char i_mouse_state)
{
	mouse_state = i_mouse_state;
}