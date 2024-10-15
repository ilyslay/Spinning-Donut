# **Step-by-Step Documentation with Code Explanation: Running the Rotating Donut Animation on Windows**  

This guide explains how to **compile, run, and understand the rotating 3D donut animation** using C++ on a Windows machine. Follow each step closely to get the program working and learn what the code does at each stage.

---

## **Prerequisites**  
1. **Windows OS** (with Command Prompt or PowerShell).  
2. **C++ Compiler** (e.g., **MinGW**, **GCC**, or **Visual Studio**).  
3. **Text Editor or IDE** (e.g., Visual Studio Code, Notepad++, Visual Studio).  

---

## **Step 1: Install a C++ Compiler (MinGW)**  
If you don’t have a C++ compiler installed:  
1. **Download MinGW** from [MinGW Download](https://sourceforge.net/projects/mingw/).  
2. Install it, selecting the **C++ compiler** during setup.  
3. Add **MinGW’s `bin` directory** to your PATH:  
   - Example: `C:\MinGW\bin`  
   - Go to **Control Panel** → **System** → **Advanced System Settings** → **Environment Variables**.  
   - Under **System Variables**, find `Path`, click **Edit**, and add `C:\MinGW\bin`.

4. Verify the installation:
   ```bash
   g++ --version
   ```
   If installed correctly, the terminal will show the compiler version.

---

## **Step 2: Create the C++ Source File**  
1. Open your text editor or IDE.  
2. **Copy the code** into a new file named `donut.cpp`.  

### **Code Breakdown and Explanation**

```cpp
#include <iostream>      // For input/output operations
#include <cmath>         // For mathematical functions like sin, cos
#include <cstring>       // For memset() function to reset buffers
#include <windows.h>     // For Sleep() and console functions

#define PI 3.14159265359  // Define PI for calculations
```
- We **include necessary libraries**. `<windows.h>` is used for console operations like clearing the screen and pausing between frames using `Sleep()`.

---

### **Screen and Buffer Setup**
```cpp
const int screen_width = 80;
const int screen_height = 22;
const int buffer_size = screen_width * screen_height;

char buffer[buffer_size];      // Frame buffer to store characters for display
float zbuffer[buffer_size];    // Depth buffer to track visibility of points
```
- We define the **console dimensions** (80x22) and **allocate memory buffers** to store the characters (`buffer`) and depth values (`zbuffer`).  
- **zbuffer** ensures that only the closest surface points are visible.

---

### **Donut Geometry Parameters**
```cpp
const double R1 = 1;   // Radius of the inner circle (donut ring)
const double R2 = 2;   // Radius of the outer circle (the path of rotation)
const double K2 = 5;   // Distance of the donut from the viewer
const double K1 = screen_width * K2 * 3 / (8 * (R1 + R2));  // Scaling factor
```
- **`R1` and `R2`** define the donut’s shape.  
- **`K2`** is the distance between the donut and the camera (viewer).  
- **`K1`** helps scale the 3D object to fit the 2D screen.

---

## **Step 3: Implement the Render Logic**
### **Cursor Position Function**
```cpp
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}
```
- This function **positions the cursor** at `(x, y)` to print the next frame at the top of the console (without flickering).

---

### **Rendering a Single Frame**
```cpp
void renderFrame() {
    memset(buffer, ' ', buffer_size);  // Clear frame buffer
    memset(zbuffer, 0, buffer_size * sizeof(float));  // Reset depth buffer

    // Loop through angles theta and phi to generate the 3D points
    for (double theta = 0; theta < 2 * PI; theta += 0.07) {
        for (double phi = 0; phi < 2 * PI; phi += 0.02) {
```
- **`theta` and `phi`** are angles used to **draw the donut in 3D space**.  
- **`memset()`** resets the frame buffer and depth buffer for every frame.

```cpp
            double cosA = cos(A), sinA = sin(A);
            double cosB = cos(B), sinB = sin(B);
            double cosTheta = cos(theta), sinTheta = sin(theta);
            double cosPhi = cos(phi), sinPhi = sin(phi);
```
- We **precompute trigonometric values** to rotate the donut smoothly.

---

### **3D Point Calculations and Rotation**
```cpp
            double circleX = R2 + R1 * cosTheta;
            double circleY = R1 * sinTheta;

            double x = circleX * (cosB * cosPhi + sinA * sinB * sinPhi) - circleY * cosA * sinB;
            double y = circleX * (sinB * cosPhi - sinA * cosB * sinPhi) + circleY * cosA * cosB;
            double z = K2 + cosA * circleX * sinPhi + circleY * sinA;
            double ooz = 1 / z;  // Perspective (1/z)
```
- The donut is rotated around **two axes** using trigonometric functions.

---

### **Screen Projection and Rendering**
```cpp
            int xp = (int)(screen_width / 2 + K1 * ooz * x);
            int yp = (int)(screen_height / 2 - K1 * ooz * y);

            double L = cosPhi * cosTheta * sinB - cosA * cosTheta * sinPhi - 
                       sinA * sinTheta + cosB * (cosA * sinTheta - cosTheta * sinA * sinPhi);
```
- The `(x, y, z)` coordinates are **projected onto the 2D screen**.  
- **`L`** calculates the brightness of each point based on lighting.

---

### **Update Buffers and Display the Frame**
```cpp
            if (L > 0 && xp >= 0 && xp < screen_width && yp >= 0 && yp < screen_height) {
                int idx = xp + screen_width * yp;
                if (ooz > zbuffer[idx]) {  // Check if the point is closer
                    zbuffer[idx] = ooz;
                    buffer[idx] = ".,-~:;=!*#$@"[(int)(L * 8)];  // Brightness to character
                }
            }
        }
    }

    setCursorPosition(0, 0);  // Reset cursor to top-left

    for (int j = 0; j < buffer_size; j++) {
        if (j % screen_width == 0) std::cout << '\n';  // Newline at row end
        putchar(buffer[j]);  // Print each character
    }
}
```
- **`zbuffer` ensures the closest points are rendered**.  
- **Characters like '.', '~', and '@'** are used to represent different brightness levels.

---

## **Step 4: Compile the Code**
1. Open **Command Prompt** and navigate to the folder where `donut.cpp` is saved:
   ```bash
   cd C:\path\to\your\file
   ```
2. Compile the program:
   ```bash
   g++ donut.cpp -o donut.exe
   ```

---

## **Step 5: Run the Program**
1. In the same Command Prompt window, run:
   ```bash
   donut.exe
   ```
2. You should see the **rotating 3D donut animation** in your console.

---

## **Step 6: Troubleshooting**
- If there’s a compilation error, ensure **MinGW is installed** correctly and added to PATH.
- If the console flickers, try running the code in **Command Prompt**, not PowerShell.

---

## **Conclusion**
By following this guide, you’ve successfully **compiled, run, and understood** the rotating 3D donut animation. Explore the code by adjusting values (like rotation speed or screen size) for fun experiments!
