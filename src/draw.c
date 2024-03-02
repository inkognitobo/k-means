#include "draw.h"

#include "config.h"
#include "raylib.h"

// Draw the x and y axes to the screen.
void draw_axes() {
    // Get screen dimensions
    const float screen_width = GetScreenWidth();
    const float screen_height = GetScreenHeight();

    // x-Axis
    DrawLine(-screen_width / 2.0f, 0.0f, screen_width / 2.0f, 0.0f, BLACK);
    const float xticks = 5.0f;
    // Ticks right side
    for (int i = 0; i < screen_width / 2.0f; i += xticks) {
        Vector2 bottom = {.x = i, .y = -TICK_SIZE / 2.0f};
        Vector2 top = {.x = i, .y = TICK_SIZE / 2.0f};
        DrawLine(bottom.x, bottom.y, top.x, top.y, BLACK);
    }
    // Ticks left side
    for (int i = 0; i > -screen_width / 2.0f; i -= xticks) {
        Vector2 bottom = {.x = i, .y = -TICK_SIZE / 2.0f};
        Vector2 top = {.x = i, .y = TICK_SIZE / 2.0f};
        DrawLine(bottom.x, bottom.y, top.x, top.y, BLACK);
    }

    // y-Axis
    DrawLine(0.0f, -screen_height / 2.0f, 0.0f, screen_height / 2.0f, BLACK);
    const float yticks = 5.0f;
    // Ticks right side
    for (int i = 0; i < screen_height / 2.0f; i += yticks) {
        Vector2 left = {.x = -TICK_SIZE / 2.0f, .y = i};
        Vector2 right = {.x = TICK_SIZE / 2.0f, .y = i};
        DrawLine(left.x, left.y, right.x, right.y, BLACK);
    }
    // Ticks left side
    for (int i = 0; i > -screen_height / 2.0f; i -= yticks) {
        Vector2 left = {.x = -TICK_SIZE / 2.0f, .y = i};
        Vector2 right = {.x = TICK_SIZE / 2.0f, .y = i};
        DrawLine(left.x, left.y, right.x, right.y, BLACK);
    }
}

// Scatter plot the provided data points in the specified color.
void scatter(Vector2 *points, size_t n_points, Color color) {
    for (size_t i = 0; i < n_points; i++) {
        Vector2 point = points[i];
        DrawCircle(point.x, point.y, POINT_RADIUS, color);
    }
}
