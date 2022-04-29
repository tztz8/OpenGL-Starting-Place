#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
/**
 * Main
 * @author Timbre Freeman (tztz8)
 */

//          --- Libraries ---

// Normal Lib
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// image lib (cscd377)
//#include <IL/il.h>
//#include <IL/ilu.h>
//#include <IL/ilut.h>

// Modified GLM (obj file loaders)
#include "GLM.h"

// image lib (glfw)
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// Math Lib
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

// h file of this assignment
#include "main.h"
#include "Cube.h"

//          --- Filled's ---

// glfw window id
GLFWwindow* window;

// Bool to know when to exit
bool exitWindowFlag = false;

// title info
#define TITLE_LENGTH 100

// Window GL variables
GLfloat aspect = 0.0;

// Booleans for current state
bool stop_rotate = true;
bool show_line = false;
bool show_line_new = false;
bool top_view_flag = false;

// GL loc
GLuint matrix_loc;
GLuint projection_matrix_loc;
GLuint view_matrix_loc;

// shader program
GLuint program;

// Matrix's
glm::mat4 view_matrix(1.0f);
glm::mat4 projection_matrix(1.0f);
glm::mat4 model_matrix(1.0f);

// Add light components
glm::vec4 light_position(10.0, 6.0, 8.0, 1.0);

glm::vec4 light_position_camera;

// uniform indices of light
//GLuint ambient_loc;
//GLuint light_source_loc;
GLuint light_position_loc;

// Angle
GLfloat rotateAngle = 0.0f;

//          --- Methods ---

/**
 * Read A file and out put a file a char list
 * @note code from Yasmin and commit and some modification make by Timbre Freeman
 * @param filename path to the file
 * @return pointer to a char list (String)
 */
char* ReadFile(const char* filename) {

    FILE* infile;
#ifdef WIN32
    fopen_s(&infile, filename, "rb");
#else
    infile = fopen(filename, "rb");
#endif


    if (!infile) {
        printf("Unable to open file %s\n", filename);
        return nullptr;
    }

    fseek(infile, 0, SEEK_END);
    int len = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    char* source = (char*)malloc(len + 1);
    fread(source, 1, len, infile);
    fclose(infile);
    source[len] = 0;
    return (source);

}

/**
 * Initshiled Shaders
 * @note code from Yasmin and commit and some modification make by Timbre Freeman
 * @param v_shader the vertex shader path
 * @param f_shader the fragment shader path
 * @return a gl program object
 */
GLuint initShaders(const char* v_shader, const char* f_shader) {

    GLuint p = glCreateProgram();

    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);

    const char * vs = ReadFile(v_shader);
    const char * fs = ReadFile(f_shader);

    glShaderSource(v, 1, &vs, nullptr);
    glShaderSource(f, 1, &fs, nullptr);

    free((char*)vs);
    free((char*)fs);

    glCompileShader(v);

    GLint compiled;

    glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLsizei len;
        glGetShaderiv(v, GL_INFO_LOG_LENGTH, &len);

        char* log = (char*)malloc(len + 1);

        glGetShaderInfoLog(v, len, &len, log);

        printf("Vertex Shader compilation failed: %s\n", log);

        free(log);

        tellWindowToClose();
    }

    glCompileShader(f);
    glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {

        GLsizei len;
        glGetShaderiv(f, GL_INFO_LOG_LENGTH, &len);
        char* log = (char*)malloc(len + 1);
        glGetShaderInfoLog(f, len, &len, log);
        printf("Vertex Shader compilation failed: %s\n", log);
        free(log);

        tellWindowToClose();
    }

    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    GLint linked;

    glGetProgramiv(p, GL_LINK_STATUS, &linked);

    if (!linked) {

        GLsizei len;
        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);
        char* log = (char*)malloc(len + 1);
        glGetProgramInfoLog(p, len, &len, log);
        printf("Shader linking failed: %s\n", log);
        free(log);

        tellWindowToClose();
    }

    glUseProgram(p);

    return p;

}

/**
 * Load Texture
 * @note code from Yasmin and commit and some modification make by Timbre Freeman
 * @note (used devil to load the image)
 * @param filename path to image file
 * @return GL Texture ID
 */
//unsigned int loadTexture(const char* filename) {
//
//    ILboolean success;
//    unsigned int imageID;
//    ilGenImages(1, &imageID);
//
//    ilBindImage(imageID); /* Binding of DevIL image name */
//    ilEnable(IL_ORIGIN_SET);
//    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
//    success = ilLoadImage((ILstring)filename);
//
//    if (!success) {
//        printf("Couldn't load the following texture file: %s", filename);
//        // The operation was not sucessfull hence free image and texture
//        ilDeleteImages(1, &imageID);
//        tellWindowToClose();
//        return 0;
//    }
//
//    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
//
//    GLuint tid;
//    glGenTextures(1, &tid);
//    glBindTexture(GL_TEXTURE_2D, tid);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
//                 GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//    glBindTexture(GL_TEXTURE_2D, 0);
//
//    /* Because we have already copied image data into texture data
//    we can release memory used by image. */
//
//    ilDeleteImages(1, &imageID);
//    return tid;
//}

/**
 * Called set setup open gl things (for example making the models)
 */
void Initialize(){

    program = initShaders("shader.vs", "shader.fs");

    if (exitWindowFlag) {
        return;
    }

    glUseProgram(program);

    // attribute indices
    model_matrix = glm::mat4(1.0f);
    view_matrix_loc = glGetUniformLocation(program, "view_matrix");
    matrix_loc = glGetUniformLocation(program, "model_matrix");
    projection_matrix_loc = glGetUniformLocation(program, "projection_matrix");

    glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);
    projection_matrix = glm::perspective(glm::radians(90.0f), aspect, 1.0f, 80.0f);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    light_position_loc = glGetUniformLocation(program, "LightPosition");

    createCube();
}

/**
 * Called for every frame to draw on the screen
 */
void Display()
{
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    if (show_line_new) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }

    if (show_line || show_line_new) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glPointSize(10);

    if (top_view_flag) {
        view_matrix = lookAt(
                glm::vec3(0.0, 8.0f, 0.0), // camera is at the top
                glm::vec3(0, 0, 0), // look at the center
                glm::vec3(
                        8.0f * sinf(glm::radians(rotateAngle)),
                        0.0f,
                        8.0f * cosf(glm::radians(rotateAngle))
                ) // rotating the camera around
        );
    } else {
        view_matrix = lookAt(
                glm::vec3(
                        8.0f * sinf(glm::radians(rotateAngle)),
                        3.0f,
                        8.0f * cosf(glm::radians(rotateAngle))
                ), // Moving around the center in a cerical
                glm::vec3(0, 0, 0), // look at the center
                glm::vec3(0, 1, 0) // keeping the camera up
        );
    }
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, (GLfloat*)&view_matrix[0]);

    light_position_camera = view_matrix * light_position;
//    light_position_camera = light_position;
    glUniform4fv(light_position_loc, 1, (GLfloat*)&light_position_camera[0]);

    projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 0.3f, 100.0f);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, (GLfloat*)&projection_matrix[0]);

    // Draw things
    model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, (GLfloat*)&model_matrix[0]);

    drawCube();

    // End of Draw things
    glFlush();
}

std::map<char, bool> keyPressed; // key was pressed on last frame
std::map<char, bool> keyCurrentlyPressed; // key is pressed this frame

/**
 * On each frame it check for user input to toggle a flag
 * @return if the program got a exit request from the user
 */
void keyboard() {
    keyCurrentlyPressed['q'] = glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS;
    if (!keyPressed['q'] && keyCurrentlyPressed['q']) {
        tellWindowToClose();
    }
    keyPressed['q'] = keyCurrentlyPressed['q'];

    keyCurrentlyPressed['s'] = glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS;
    if (!keyPressed['s'] && keyCurrentlyPressed['s']) {
        show_line = !show_line;
    }
    keyPressed['s'] = keyCurrentlyPressed['s'];

    keyCurrentlyPressed['S'] =
            keyCurrentlyPressed['s'] &&
            (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
             glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
    if (!keyPressed['S'] && keyCurrentlyPressed['S']) {
        show_line_new = !show_line_new;
    }
    keyPressed['S'] = keyCurrentlyPressed['S'];

    keyCurrentlyPressed['u'] = glfwGetKey(window, GLFW_KEY_U ) == GLFW_PRESS;
    keyCurrentlyPressed['t'] = glfwGetKey(window, GLFW_KEY_T ) == GLFW_PRESS;
    if ((!keyPressed['t'] && keyCurrentlyPressed['t']) ||
        (!keyPressed['u'] && keyCurrentlyPressed['u'])) {
        top_view_flag = !top_view_flag;
    }
    keyPressed['t'] = keyCurrentlyPressed['t'];
    keyPressed['u'] = keyCurrentlyPressed['u'];

    keyCurrentlyPressed['r'] = glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS;
    if (!keyPressed['r'] && keyCurrentlyPressed['r']) {
        stop_rotate = !stop_rotate;
    }
    keyPressed['r'] = keyCurrentlyPressed['r'];
}

/**
 * Auto update GL Viewport when window size changes <br>
 * This is a callback method for GLFW
 * @param thisWindow the window that updated
 * @param width the new width
 * @param height the new height
 */
void windowSizeChangeCallback([[maybe_unused]] GLFWwindow* thisWindow, int width, int height) {
    glViewport(0, 0, width, height);
    aspect = float(width) / float(height);
}

/**
 * Update Angle on each frame
 * @note was the timer in Assignment 4
 * @param deltaTime the time between frames
 */
void updateAngle(GLfloat deltaTime) {

    if (!stop_rotate) {
        rotateAngle += 2.75f * 10 * deltaTime;
    }

}

/**
 * Set the window flag to exit window
 */
void tellWindowToClose() {
    exitWindowFlag = true;
}

// ------------------ Main ---------------------------

/**
 * Start of the program MAIN <br>
 * Has the main loop
 * @return Exit code
 */
int main() {

    // Initialise GLFW
    fprintf(stdout, "Info: Initialise GLFW\n");
    if (!glfwInit()) {
        fprintf(stderr,"Error: initializing GLFW failed\n");
//        getchar(); // so it will wait for users input
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Info: Setting window hint's\n");
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Open a window and create its OpenGL context
    fprintf(stdout, "Info: Open a window and create its OpenGL context\n");
    char orginal_title[TITLE_LENGTH];
    strcpy(orginal_title, "GLFW - OpenGL - Basic");
    window = glfwCreateWindow(512, 512, orginal_title, nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr,"Error: Failed to open GLFW window\n");
//        getchar(); // so it will wait for users input
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    // resize
    fprintf(stdout, "Info: Setup resize (size change callback)\n");
    glfwSetWindowSizeCallback(window, windowSizeChangeCallback);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    windowSizeChangeCallback(window, width, height);

    // icon
    fprintf(stdout, "Info: Setup icon for the window\n");
    GLFWimage icons[1];
    icons[0].pixels = stbi_load(
            "res/icon/cube.png",
            &icons[0].width,
            &icons[0].height,
            nullptr, 4);
    if (icons[0].pixels == nullptr) {
        fprintf(stderr, "Error: Unable to load icon\n");
    } else {
        glfwSetWindowIcon(window, 1, icons);
        stbi_image_free(icons[0].pixels);
    }

    // Initialize devil
//    fprintf(stdout,"Info: Initialize DevIL\n");
//    ilInit();

    // Initialize GLEW
    fprintf(stdout,"Info: Initialize GLEW\n");
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Error: Failed to initialize GLEW\n");
//        getchar();
        glfwTerminate();
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Info: Running Initialize method\n");
    Initialize();

    // GL info
    fprintf(stdout, "Info: GL Vendor : %s\n", glGetString(GL_VENDOR));
    fprintf(stdout, "Info: GL Renderer : %s\n", glGetString(GL_RENDERER));
    fprintf(stdout, "Info: GL Version (string) : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    fprintf(stdout, "Info: GLSL Version : %s\n", glGetString(GL_VERSION));


    // Ensure we can capture the escape key being pressed below
    fprintf(stdout, "Info: Setup user input mode\n");
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    fprintf(stdout, "Info: setting up variables for the loop\n");

    // DeltaTime variables
    GLfloat lastFrame = 0.0f;

    // FPS variables
    GLfloat lastTimeFPS = 0.0f;
    GLint numberOfFrames = 0;
    double fps;
    double avgFPS = 0.0;
    int qtyFPS = 0;

    fprintf(stdout,
            "Info: Start window loop with exit:%s and glfwWindowShouldClose(window):%s\n",
            exitWindowFlag ? "true" : "false",
            glfwWindowShouldClose(window) ? "true" : "false");
    while (!exitWindowFlag && !glfwWindowShouldClose(window)) {

        // Calculate delta time
        GLfloat currentFrame;
        currentFrame = static_cast<GLfloat>(glfwGetTime());
        GLfloat deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // FPS
        {
            GLfloat deltaTimeFPS = currentFrame - lastTimeFPS;
            numberOfFrames++;
            if (deltaTimeFPS >= 1.0f) {
                fps = double(numberOfFrames) / deltaTimeFPS;
                qtyFPS++;
                avgFPS += (fps - avgFPS) / qtyFPS;

                char title[TITLE_LENGTH];
                snprintf(title, TITLE_LENGTH - 1,
                         "%s - [FPS: %3.2f]", orginal_title,
                         fps);
                glfwSetWindowTitle(window, title);
                //fprintf(stdout, "Info: FPS: %f\n", fps);

                numberOfFrames = 0;
                lastTimeFPS = currentFrame;
            }
        }


        // Render
        Display();

        // Swap buffers
        glfwSwapBuffers(window);

        // Get evens (ex user input)
        glfwPollEvents();

        // check for user input to exit
        exitWindowFlag = glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || exitWindowFlag;

        // check for user input
        keyboard();

        // update data (often angles of things)
        updateAngle(deltaTime);

    }
    fprintf(stdout, "Info: Avg FPS: %f\n", avgFPS);

    // Close OpenGL window and terminate GLFW
    fprintf(stdout, "Info: Close OpenGL window and terminate GLFW\n");
    glfwTerminate();

    return EXIT_SUCCESS;
}
#pragma clang diagnostic pop