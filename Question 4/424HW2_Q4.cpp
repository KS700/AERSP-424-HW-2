#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

// This is a Drag coefficent VS L/D plot
// Includes 3 lines (Total, Induced, and Parasite)

// Constants for drag coefficient equation (Random constants picked)
const float CD0 = 0.02; // Zero-lift drag coefficient
const float k = 0.03;   // Constant related to the aircraft
const float AR = 8.0;    // Aspect ratio of the wing
const float e = 0.9;     // Oswald efficiency factor

// Function to calculate parasite drag coefficient for given lift-to-drag ratio
float calculateParasiteDragCoefficient(float LD) {
    return CD0 + k * std::pow(LD, 2);
}

// Function to calculate induced drag coefficient for given lift coefficient
float calculateInducedDragCoefficient(float CL) {
    return std::pow(CL, 2) / (3.14159 * AR * e);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 800, "Drag Coefficient vs. L/D", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_LINES);

        // Draw X and Y axis
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(-1.0, 0.0);
        glVertex2f(1.0, 0.0);
        glVertex2f(0.0, 1.0);
        glVertex2f(0.0, -1.0);

        // Plot Parasite Drag Coefficient vs. L/D
        glColor3f(0.0, 1.0, 0.0);   // Green Line
        for (float LD = 0.1; LD <= 5.0; LD += 0.0001) {
            float CD_parasite = calculateParasiteDragCoefficient(LD);
            float x = (LD - 0.1) / 5.0;
            float y = CD_parasite;
            glVertex2f(x, y - 0.001);
            glVertex2f(x, y);
        }

        // Plot Induced Drag Coefficient vs. L/D
        glColor3f(1.0, 0.0, 0.0);   // Red Line
        for (float LD = 0.1; LD <= 5.0; LD += 0.0001) {
            float CL = 1.0 / LD;
            float CD_induced = calculateInducedDragCoefficient(CL);
            float x = (LD - 0.1) / 5.0;
            float y = CD_induced;
            glVertex2f(x, y - 0.001);
            glVertex2f(x, y);
        }

        // Plot Total Drag Coefficient vs. L/D
        glColor3f(0.0, 0.0, 1.0);   // Blue Line
        for (float LD = 0.1; LD <= 5.0; LD += 0.0001) {
            float CD_parasite = calculateParasiteDragCoefficient(LD);
            float CL = 1.0 / LD;
            float CD_induced = calculateInducedDragCoefficient(CL);
            float CD_total = CD_parasite + CD_induced;
            float x = (LD - 0.1) / 5.0;
            float y = CD_total;
            glVertex2f(x, y - 0.001);
            glVertex2f(x, y);
        }

        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
