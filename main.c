#include <raylib.h>
#include <stdio.h>
#include <math.h>

typedef struct
{
    // offset into the cube array
    int a, b;
}line;

typedef struct
{
    Vector3 points[3];
}triangle;

void draw_triangle(Vector2 a, Vector2 b, Vector2 c, Color color)
{
    DrawLineV(a, b, color);
    DrawLineV(b, c, color);
    DrawLineV(c, a, color);
}

// projection to screen space
// x_projected = (x_world / (z_world * tan(fov / 2)) / aspect_ratio
// y_projected = y_world / (z_world * tan(fov / 2))
Vector2 project_to_screen_space(Vector3 *world_vec, float fov, float aspect_ratio){
    Vector2 result_vec;

    result_vec.x = (world_vec->x / (world_vec->z * tanf(fov * DEG2RAD / 2))) / aspect_ratio;
    result_vec.y = world_vec->y / (world_vec->z * tanf(fov * DEG2RAD / 2));
    
    return result_vec;
}

Vector2 scale_to_screen(Vector2 *vec, int width, int height)
{
    Vector2 scaled;
    scaled.x = vec->x * (width * 0.5f) + width * 0.5f;
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
    const int screen_width = 800;
    const int screen_height = 450;

    float aspect_ratio = (float)screen_width / (float)screen_height;
    float fov = 90.0f;
    float angle = 0.0f;

    InitWindow(screen_width, screen_height, "rotating cube");

    SetTargetFPS(60);

    const triangle mesh[] = 
    {
        // south
        {{{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}},
        {{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}},

        // east                                                      
        {{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}},
        {{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}}},

        // north                                                     
        {{{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}}},
        {{{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}},

        // west                                                      
        {{{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}},
        {{{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},

        // top                                                       
        {{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}},
        {{{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}}},

        // bottom                                                    
        {{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}},
        {{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}},
    };

    while(!WindowShouldClose())
    {
        angle += 0.03f;
        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < sizeof(mesh) / sizeof(triangle); i++)
            {
                triangle curr_triangle = mesh[i];
                
                curr_triangle.points[0] = rotate_y(&curr_triangle.points[0], angle);
                curr_triangle.points[1] = rotate_y(&curr_triangle.points[1], angle);
                curr_triangle.points[2] = rotate_y(&curr_triangle.points[2], angle);
                
                // shift in the back
                curr_triangle.points[0].z += 4;
                curr_triangle.points[1].z += 4;
                curr_triangle.points[2].z += 4;
                
                Vector2 screen_space[3];

                screen_space[0] = project_to_screen_space(&curr_triangle.points[0], fov, aspect_ratio);
                screen_space[1] = project_to_screen_space(&curr_triangle.points[1], fov, aspect_ratio);
                screen_space[2] = project_to_screen_space(&curr_triangle.points[2], fov, aspect_ratio);
                
                screen_space[0] = scale_to_screen(&screen_space[0], screen_width, screen_height);
                screen_space[1] = scale_to_screen(&screen_space[1], screen_width, screen_height);
                screen_space[2] = scale_to_screen(&screen_space[2], screen_width, screen_height);

                draw_triangle(screen_space[0], screen_space[1], screen_space[2], BLACK);
            }

            EndDrawing();
    }

    CloseWindow();
    return 0;
}
