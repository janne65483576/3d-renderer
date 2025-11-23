#include <raylib.h>
#include <stdio.h>
#include <math.h>

typedef struct
{
    // offset into the cube array
    int a, b;
}line;

// projection to screen space
// x_projected = (x_world / (z_world * tan(fov / 2)) / aspect_ratio
// y_projected = y_world / (z_world * tan(fov / 2))
Vector2 project_to_screen_space(Vector3 *world_vec, float fov, float aspect_ratio){
    Vector2 result_vec;

    result_vec.x = (world_vec->x / (world_vec->z * tanf(fov * DEG2RAD / 2))) / aspect_ratio;
    result_vec.y = world_vec->y / (world_vec->z * tanf(fov * DEG2RAD / 2));
    
    return result_vec;
}

Vector2 scale_to_screen(Vector2 *vec, int widht, int height)
{
    Vector2 scaled;
    scaled.x = vec->x * (widht * 0.5f) + widht * 0.5f;
    scaled.y = -vec->y * (height * 0.5f) + height * 0.5f;
    return scaled;
}

Vector3 rotate_y(Vector3 *vec, float angle)
{
    // pre compute needed values
    float s = sinf(angle);
    float c = cosf(angle);

    // save initial values for calculation
    float initial_x = vec->x;
    float initial_z = vec->z;

    Vector3 rotated_vec;

    rotated_vec.y = vec->y;
    rotated_vec.x = initial_x * c - initial_z * s;
    rotated_vec.z = initial_x * s + initial_z * c;

    return rotated_vec;
}

int main(void)
{
    const int screen_widht = 800;
    const int screen_height = 450;

    float aspect_ratio = (float)screen_widht / (float)screen_height;
    float fov = 90.0f;
    float angle = 0.0f;

    InitWindow(screen_widht, screen_height, "rotating cube");

    SetTargetFPS(60);

    const Vector3 cube[] = 
    {
        // front
        {-0.5, -0.5, -0.5},
        {-0.5, 0.5, -0.5},
        {0.5, 0.5, -0.5},
        {0.5, -0.5, -0.5},

        // side
        {-0.5, -0.5, 0.5},
        {-0.5, 0.5, 0.5},
        {0.5, 0.5, 0.5},
        {0.5, -0.5, 0.5}
    };

    const line lines[] = 
    {
        // front
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},

        // back
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},

        // side
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}
    };

    Vector2 cube_projected[sizeof(cube) / sizeof(Vector3)];

    while(!WindowShouldClose())
    {
        angle += 0.03f;
        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < sizeof(cube) / sizeof(Vector3); i++)
            {
                Vector3 world_vec = cube[i];
                
                world_vec = rotate_y(&world_vec, angle);
                
                // shift in the back
                world_vec.z += 4;

                cube_projected[i] = project_to_screen_space(&world_vec, fov, aspect_ratio);

                Vector2 scaled = scale_to_screen(&cube_projected[i], screen_widht, screen_height);

                DrawCircleV(scaled, 5, BLACK);
            }

            // draw lines
            for (int i = 0; i < sizeof(lines) / sizeof(line); i++)
            {
                Vector2 a, b;
                a = scale_to_screen(&cube_projected[lines[i].a], screen_widht, screen_height);
                b = scale_to_screen(&cube_projected[lines[i].b], screen_widht, screen_height);

                DrawLineV(a, b, BLACK);
            }

            EndDrawing();
    }

    CloseWindow();
    return 0;
}
