#include "Helper.h"
#include <iostream>
#include "../GSM/GameStateManager.h"
#include "../ComponentManager/GameObjectManager.h"

GLint Helper::W_WIDTH;
GLint Helper::W_HEIGHT;
GLdouble Helper::fps;
GLdouble Helper::delta_time; // time taken to complete most recent game loop
std::string Helper::W_TITLE;
GLFWwindow* Helper::ptr_window;
bool Helper::editMode = false;

bool Helper::key_space = false;

bool Helper::init(GLint width, GLint height, std::string title)
{
    W_WIDTH = width;
    W_HEIGHT = height;
    W_TITLE = title;

    if (!glfwInit()) {
        std::cout << "fail to init GLFW" << std::endl;
        return false;
    }
    glfwSetErrorCallback(error_cb);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	

    ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!ptr_window) {
        std::cerr << "GLFW unable to create OpenGL context - abort program\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(ptr_window);

    setup_event_callbacks();

    glfwSetKeyCallback(ptr_window, key_cb);
    glfwSetMouseButtonCallback(ptr_window, mousebutton_cb);
    //glfwSetCursorPosCallback(ptr_window, mousepos_cb);
    glfwSetScrollCallback(ptr_window, mousescroll_cb);

    glfwSetInputMode(ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
        glfwTerminate();
        exit(-1);
        return false;
    }
    if (GLEW_VERSION_4_5) {
        std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
    }
    else {
        std::cerr << "Warning: The driver may lack full compatibility with OpenGL 4.5, potentially limiting access to advanced features." << std::endl;
    }

    return true;
}

void Helper::cleanUp()
{
    glfwTerminate();
}

void Helper::error_cb(int error, char const* description)
{
#ifdef _DEBUG
    std::cerr << "GLFW error: " << description << std::endl;
#endif
}

void Helper::fbsize_cb(GLFWwindow* ptr_win, int width, int height)
{
#ifdef _DEBUG
    //std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
    W_WIDTH= width;
    W_HEIGHT = height;
}

void Helper::key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod)
{
    if (GLFW_PRESS == action) {
        if (key == GLFW_KEY_SPACE)
            key_space = true;
#ifdef _DEBUG
       // std::cout << "Key pressed" << std::endl;
#endif
    }
    else if (GLFW_REPEAT == action) {
        //if (key == GLFW_KEY_SPACE)
        //    key_space = false;

#ifdef _DEBUG
        //std::cout << "Key repeatedly pressed" << std::endl;
#endif
    }
    else if (GLFW_RELEASE == action) {
        //if (key == GLFW_KEY_SPACE)
        //    key_space = false;
#ifdef _DEBUG
        //std::cout << "Key released" << std::endl;
#endif
    }

    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(pwin, GLFW_TRUE);
    }
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        Helper::editMode = !Helper::editMode;
        if (Helper::editMode)
            GameObjectManager::GetPtr()->RestoreSnapshot();
    }
}

void Helper::mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod)
{
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
#ifdef _DEBUG
        //std::cout << "Left mouse button ";
#endif
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
#ifdef _DEBUG
        //std::cout << "Right mouse button ";
#endif
        break;
    }
    switch (action) {
    case GLFW_PRESS:
#ifdef _DEBUG
        //std::cout << "pressed!!!" << std::endl;
#endif
        break;
    case GLFW_RELEASE:
#ifdef _DEBUG
        //std::cout << "released!!!" << std::endl;
#endif
        break;
    }
}

void Helper::mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset)
{
#ifdef _DEBUG
    //std::cout << "Mouse scroll wheel offset: ("
    //    << xoffset << ", " << yoffset << ")" << std::endl;
#endif
}

void Helper::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos)
{
#ifdef _DEBUG
    //std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
#endif
}

void Helper::setup_event_callbacks()
{
    glfwSetFramebufferSizeCallback(ptr_window, fbsize_cb);
    glfwSetKeyCallback(ptr_window, key_cb);
    glfwSetMouseButtonCallback(ptr_window, mousebutton_cb);
    //glfwSetCursorPosCallback(ptr_window, mousepos_cb);
    glfwSetScrollCallback(ptr_window, mousescroll_cb);
}

void Helper::update_time(double fps_calc_interval)
{
    // get elapsed time (in seconds) between previous and current frames
    static double prev_time = glfwGetTime();
    double curr_time = glfwGetTime();
    delta_time = curr_time - prev_time;
    prev_time = curr_time;

    // fps calculations
    static double count = 0.0; // number of game loop iterations
    static double start_time = glfwGetTime();
    // get elapsed time since very beginning (in seconds) ...
    double elapsed_time = curr_time - start_time;

    ++count;

    // update fps at least every 10 seconds ...
    fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
    fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
    if (elapsed_time > fps_calc_interval) {
        Helper::fps = count / elapsed_time;
        start_time = curr_time;
        count = 0.0;
    }
}

void Helper::print_specs()
{
    std::cout << "OpenGL version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;
}

