#include "Car.h"
#include "CarView.h"
#include <iostream>
#include <ncurses.h>
#include <unistd.h>

CarView::CarView(Car &car, int colorId) : car(car), colorId(colorId) {}

void CarView::draw()
{
    // Enable color attributes
    attron(COLOR_PAIR(colorId));
    
    for (int i = car.y; i < car.y + car.size; ++i)
    {
        for (int j = car.x; j < car.x + car.size; ++j)
        {
            mvaddch(i, j, '#');
        }
    }
    
    // Disable color attributes
    attroff(COLOR_PAIR(colorId));
}

void CarView::setColor(int newColorId)
{
    colorId = newColorId;
}
