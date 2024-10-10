#include <iostream>
#include <cmath>
#include <cstring>
#include <unistd.h> // for usleep function (Linux/Mac). On Windows, use <windows.h> with Sleep(ms)

#define PI 3.14159265359

int k; // Loop counter for the animation
double A = 0, B = 0; // Rotation angles

// Screen resolution and buffer settings
const int screen_width = 80;
const int screen_height = 22;
const int buffer_size = screen_width * screen_height;
char buffer[buffer_size]; // Array for storing characters (frame buffer)
float zbuffer[buffer_size]; // Depth buffer

const double theta_spacing = 0.07;
const double phi_spacing = 0.02;
const double R1 = 1; // Radius of the donut ring
const double R2 = 2; // Radius of the circle the ring revolves on
const double K2 = 5; // Distance of the donut from the viewer
const double K1 = screen_width * K2 * 3 / (8 * (R1 + R2)); // Scaling factor based on screen size

void renderFrame() {
    // Clear buffers
    memset(buffer, ' ', buffer_size);
    memset(zbuffer, 0, buffer_size * sizeof(float));

    // Theta and Phi represent angles for drawing the donut in 3D
    for (double theta = 0; theta < 2 * PI; theta += theta_spacing) {
        for (double phi = 0; phi < 2 * PI; phi += phi_spacing) {
            // 3D coordinates of a point on the donut
            double cosA = cos(A), sinA = sin(A);
            double cosB = cos(B), sinB = sin(B);
            double cosTheta = cos(theta), sinTheta = sin(theta);
            double cosPhi = cos(phi), sinPhi = sin(phi);

            // Compute the 3D (x, y, z) coordinates of the point on the surface of the torus
            double circleX = R2 + R1 * cosTheta;
            double circleY = R1 * sinTheta;

            // 3D rotation of the point around the X and Z axes
            double x = circleX * (cosB * cosPhi + sinA * sinB * sinPhi) - circleY * cosA * sinB;
            double y = circleX * (sinB * cosPhi - sinA * cosB * sinPhi) + circleY * cosA * cosB;
            double z = K2 + cosA * circleX * sinPhi + circleY * sinA;
            double ooz = 1 / z; // One over z (inverse of z, for perspective)

            // Projection onto 2D screen (x, y)
            int xp = (int)(screen_width / 2 + K1 * ooz * x);
            int yp = (int)(screen_height / 2 - K1 * ooz * y);

            // Calculate brightness of the surface (based on angle between light vector and surface normal)
            double L = cosPhi * cosTheta * sinB - cosA * cosTheta * sinPhi - sinA * sinTheta + cosB * (cosA * sinTheta - cosTheta * sinA * sinPhi);

            // Only display the point if it's within screen boundaries and brighter than the background
            if (L > 0 && xp >= 0 && xp < screen_width && yp >= 0 && yp < screen_height) {
                int idx = xp + screen_width * yp;
                if (ooz > zbuffer[idx]) { // Only render point if it's closer than the previously drawn point
                    zbuffer[idx] = ooz;
                    int luminance_index = L * 8; // Scale luminance to match characters (0-7)
                    buffer[idx] = ".,-~:;=!*#$@"[luminance_index];
                }
            }
        }
    }

    // Print frame
    std::cout << "\x1b[H"; // ANSI escape code for moving cursor to the top of the screen
    for (int j = 0; j < buffer_size; j++) {
        putchar(j % screen_width ? buffer[j] : '\n'); // Print each character (new line at the end of each row)
    }
}

int main() {
    std::cout << "\x1b[2J"; // Clear screen using ANSI escape code
    while (true) {
        renderFrame();
        A += 0.04; // Increment rotation angle A
        B += 0.02; // Increment rotation angle B
        usleep(30000); // Pause between frames (30 ms delay, 33 FPS)
    }
    return 0;
}
