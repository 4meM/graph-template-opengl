#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/molecule.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = 0.0f, lastY = 0.0f;
float rotY = 0.0f, rotX = 0.0f;
float cameraZ = -4.0f;
bool mousePressed = false;
/****************************************************************************************\
*                                    ROTACION ESCENARIO                                  *
\****************************************************************************************/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) mousePressed = true;
        else if (action == GLFW_RELEASE) mousePressed = false;
    }
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        float dx = xpos - lastX;
        float dy = ypos - lastY;
        rotY += dx * 0.5f;
        rotX += dy * 0.5f;
    }
    lastX = xpos;
    lastY = ypos;
}
/****************************************************************************************\
*                              ZOOM ESCENARIO                                            *
\****************************************************************************************/
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraZ += (float)yoffset * 0.2f;
    if (cameraZ > -1.0f) cameraZ = -1.0f;
    if (cameraZ < -20.0f) cameraZ = -20.0f;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Paracetamol", NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) return -1;
    glEnable(GL_DEPTH_TEST);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Molecule mol("../SDF/paracetamol.sdf");
    mol.init();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mol.draw(rotX, rotY);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    mol.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
