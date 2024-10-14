Here's a step-by-step documentation to explain the process of creating the **3D Spinning Donut** in C++. This guide breaks down each significant aspect of the code so that the logic and flow are clear.

---

### **Step 1: Setting up Constants and Variables**

#### Code:
```cpp
#define PI 3.14159265359
double A = 0, B = 0; // Rotation angles
const int screen_width = 80;
const int screen_height = 22;
char buffer[buffer_size]; 
float zbuffer[buffer_size];
const double theta_spacing = 0.07;
const double phi_spacing = 0.02;
const double R1 = 1, R2 = 2, K2 = 5;
const double K1 = screen_width * K2 * 3 / (8 * (R1 + R2));
```

#### Explanation:
- **A and B**: These are rotation angles for the donut around two different axes. They increment over time to make the donut spin.
- **Screen dimensions**: `screen_width` and `screen_height` define the size of the ASCII output.
- **Buffer arrays**: `buffer` stores the ASCII characters for each frame, while `zbuffer` holds depth information to ensure proper rendering of 3D objects.
- **Spacing values**: `theta_spacing` and `phi_spacing` control the resolution of the donut by setting the increments for two angles (`theta` and `phi`).
- **Donut radii**: `R1` is the radius of the small ring, and `R2` is the distance from the center of the donut to the center of the ring.
- **Projection scaling**: `K1` is a constant used to scale the donut's 3D coordinates into 2D space for proper rendering.

---

### **Step 2: Clearing Buffers**

#### Code:
```cpp
memset(buffer, ' ', buffer_size);
memset(zbuffer, 0, buffer_size * sizeof(float));
```

#### Explanation:
- Before rendering each frame, the `buffer` is filled with spaces (i.e., the empty background), and the `zbuffer` is reset to 0.
- This ensures that each new frame starts fresh, with nothing drawn, and that depth calculations will work correctly.

---

### **Step 3: Looping Through Angles Theta and Phi**

#### Code:
```cpp
for (double theta = 0; theta < 2 * PI; theta += theta_spacing) {
    for (double phi = 0; phi < 2 * PI; phi += phi_spacing) {
```

#### Explanation:
- **Theta and Phi** represent two angles in the torus (donut) model. `Theta` is for the rotation around the ring, and `Phi` is for the rotation around the donut's center.
- The code iterates through these angles to generate the 3D points on the surface of the donut. Increasing `theta_spacing` and `phi_spacing` would result in lower resolution but faster rendering.

---

### **Step 4: Calculate 3D Coordinates of the Donut**

#### Code:
```cpp
double circleX = R2 + R1 * cosTheta;
double circleY = R1 * sinTheta;
```

#### Explanation:
- For every combination of `theta` and `phi`, the code calculates the 3D coordinates (`x`, `y`, `z`) of points on the surface of the donut.
- The `circleX` and `circleY` represent the points on the 2D cross-section of the donut (the small ring) before applying any rotation. The circle is defined by trigonometric functions (`cosTheta` and `sinTheta`).

---

### **Step 5: Apply Rotation to 3D Points**

#### Code:
```cpp
double x = circleX * (cosB * cosPhi + sinA * sinB * sinPhi) - circleY * cosA * sinB;
double y = circleX * (sinB * cosPhi - sinA * cosB * sinPhi) + circleY * cosA * cosB;
double z = K2 + cosA * circleX * sinPhi + circleY * sinA;
```

#### Explanation:
- These equations rotate the 3D point (`circleX`, `circleY`, `z`) in space around two axes (`A` and `B`).
- The trigonometric functions of `A` (rotation around the X-axis) and `B` (rotation around the Z-axis) apply the rotation to the donut in 3D space.
- This is the step that makes the donut "spin" by changing its position in 3D as `A` and `B` increment in each frame.

---

### **Step 6: Perform Perspective Projection**

#### Code:
```cpp
double ooz = 1 / z;
int xp = (int)(screen_width / 2 + K1 * ooz * x);
int yp = (int)(screen_height / 2 - K1 * ooz * y);
```

#### Explanation:
- **`ooz` (1 / z)**: This gives a simple perspective effect. As `z` increases (objects move farther away), the points get smaller.
- **2D Screen coordinates** (`xp`, `yp`): These map the 3D points onto the 2D console. The donut’s 3D position is scaled and translated to fit within the screen's width and height.

---

### **Step 7: Calculate Surface Brightness**

#### Code:
```cpp
double L = cosPhi * cosTheta * sinB - cosA * cosTheta * sinPhi - sinA * sinTheta + cosB * (cosA * sinTheta - cosTheta * sinA * sinPhi);
```

#### Explanation:
- This calculates the brightness `L` of each point based on the angle between the light source and the surface of the donut.
- The lighting equation combines multiple trigonometric terms to simulate how light interacts with the donut’s surface.

---

### **Step 8: Depth Buffering and Character Selection**

#### Code:
```cpp
if (L > 0 && xp >= 0 && xp < screen_width && yp >= 0 && yp < screen_height) {
    int idx = xp + screen_width * yp;
    if (ooz > zbuffer[idx]) {
        zbuffer[idx] = ooz;
        int luminance_index = L * 8;
        buffer[idx] = ".,-~:;=!*#$@"[luminance_index];
    }
}
```

#### Explanation:
- **Depth Buffer Check**: This ensures that only the closest (nearest) points on the donut are rendered by comparing the `ooz` (inverse of `z`) value to the `zbuffer`. If the current point is closer than the previous point in that location, it updates the `zbuffer` and draws that point.
- **Character Selection**: Based on the brightness `L`, a corresponding character from the string `.,-~:;=!*#$@` is chosen. Brighter points are rendered with characters like `#`, while darker ones use `.` or `,` to simulate shading.

---

### **Step 9: Output the Frame**

#### Code:
```cpp
std::cout << "\x1b[H"; // Move cursor to top-left corner
for (int j = 0; j < buffer_size; j++) {
    putchar(j % screen_width ? buffer[j] : '\n');
}
```

#### Explanation:
- **ANSI Escape Codes**: The `\x1b[H` escape sequence moves the cursor back to the top-left corner of the terminal, which prevents the program from scrolling and makes the donut animation appear in-place.
- **Printing the Frame**: The characters stored in the `buffer` are printed row by row. `putchar(j % screen_width ? buffer[j] : '\n')` ensures that a newline is inserted at the end of each row to maintain the correct shape of the frame.

---

### **Step 10: Increment Rotation Angles and Loop**

#### Code:
```cpp
A += 0.04;
B += 0.02;
usleep(30000);
```

#### Explanation:
- The angles `A` and `B` are incremented after every frame, causing the donut to rotate around two axes.
- The `usleep(30000)` introduces a 30 ms delay between frames, which creates the animation effect at ~33 frames per second (FPS).

---

### **Step 11: Main Loop**

#### Code:
```cpp
while (true) {
    renderFrame();
}
```

#### Explanation:
- The entire process of rendering each frame, rotating the donut, and updating the screen is repeated in an infinite loop, continuously animating the spinning donut until the program is stopped.

---

### Conclusion:

This spinning 3D donut project is a great example of how basic mathematical concepts (trigonometry, 3D transformations, perspective projection, and depth buffering) can be used to create visual effects in a text-based environment. The program manipulates the position of each point on the donut and calculates shading based on the angle of light, giving the illusion of 3D in a 2D ASCII art form.
