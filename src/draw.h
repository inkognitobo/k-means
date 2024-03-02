#include "raylib.h"

#include <stdlib.h>

#ifndef DRAW_H
#define DRAW_H

void draw_axes();
void scatter(Vector2 *points, size_t n_points, Color color);

#endif // !DRAW_H
