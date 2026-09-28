#include "sim.h"

#define WIDTH SIM_X_SIZE / 4
#define HEIGHT SIM_Y_SIZE / 4

void compute_julia(int* buffer, int width, int height, float cx, float cy) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float zr = (x - width / 2.0f) * 3.0f / width;
            float zi = (y - height / 2.0f) * 3.0f / height;
            int count = 0;

            while (zr * zr + zi * zi < 4.0f && count < 255) {
                float temp = zr * zr - zi * zi + cx;
                zi = 2.0f * zr * zi + cy;
                zr = temp;
                count++;
            }
            buffer[y * width + x] = count;
        }
    }
}

void app(void) {
    int buffer[WIDTH * HEIGHT];

    float cx = -0.7f;
    float cy = 0.27015f;

    float dx = 0.002f;
    float dy = 0.0015f;

    while (1) {

        compute_julia(buffer, WIDTH, HEIGHT, cx, cy);

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                int count = buffer[y * WIDTH + x];
                int color = (count == 255) ? 0 : (count * 9) % 256;
                simPutPixel(x, y, color);
            }
        }
        simFlush();

        cx += dx;
        cy += dy;

        if (cx > -0.6f || cx < -0.8f) dx = -dx;
        if (cy > 0.35f || cy < 0.2f)  dy = -dy;
    }
}
