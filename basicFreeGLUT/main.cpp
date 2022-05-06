#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
/**
 * Main
 */

//          --- Libraries ---

// Normal Lib
#include <cstdio>
#include <cstdlib>
//#include <cstring>
//#include <map>

// Include GLEW
#include <GL/glew.h>

#include <GL/freeglut.h>

// image lib (cscd377)
//#include <IL/il.h>
//#include <IL/ilu.h>
//#include <IL/ilut.h>

// Math Lib
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

// h file of this assignment
#include "Cube.h"

//          --- Filled's ---

// Window GL variables
GLfloat aspect = 0.0;

// Booleans for current state
bool stop_rotate = true;
bool show_line = false;
bool show_line_new = false;
bool top_view_flag = false;

// GL loc
GLint matrix_loc;
GLint projection_matrix_loc;
GLint view_matrix_loc;

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
//GLint ambient_loc;
//GLint light_source_loc;
GLint light_position_loc;

// Angle (for this It's for the light)
GLfloat rotateAngle = 0.0f;
//GLfloat lightRotateAngle = 0.0f;

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
    int len = static_cast<int>(ftell(infile));
    fseek(infile, 0, SEEK_SET);
    char* source = (char*)malloc(len + 1);
    if (source == nullptr) {
        printf("Unable to get memory to read file %s\n", filename);
        return nullptr;
    }
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

        if (log == nullptr) {
            printf("Was not able to get memory to get error code for compiled shader\n");
            exit(EXIT_FAILURE);
        }

        glGetShaderInfoLog(v, len, &len, log);

        printf("Vertex Shader compilation failed: %s\n", log);

        free(log);
    }

    glCompileShader(f);
    glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {

        GLsizei len;
        glGetShaderiv(f, GL_INFO_LOG_LENGTH, &len);
        char* log = (char*)malloc(len + 1);

        if (log == nullptr) {
            printf("Was not able to get memory to get error code for compiled shader\n");
            exit(EXIT_FAILURE);
        }

        glGetShaderInfoLog(f, len, &len, log);
        printf("Vertex Shader compilation failed: %s\n", log);
        free(log);
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

        if (log == nullptr) {
            printf("Was not able to get memory to get error code for compiled shader\n");
            exit(EXIT_FAILURE);
        }

        glGetProgramInfoLog(p, len, &len, log);
        printf("Shader linking failed: %s\n", log);
        free(log);
    }

    glUseProgram(p);

    return p;

}

///**
// * Load Texture
// * @note code from Yasmin and commit and some modification make by Timbre Freeman
// * @note (used devil to load the image)
// * @param filename path to image file
// * @return GL Texture ID
// */
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
    // Create the program for rendering the model
    program = initShaders("shader.vert", "shader.frag");

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

    // Lines
    // Using CULL FACE or not
    if (show_line_new) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
    // If it Lines or Filled
    if (show_line || show_line_new) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Size of Points if drawn
    glPointSize(10);

    // Top View
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
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, (GLfloat*)&view_matrix[0]); // Update GPU about the view (camera)

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
    glutSwapBuffers();
}

/**
 * called when key on the keyboard is pressed
 * @param key the key being pressed
 * @param x x of "mouse location in window relative coordinates when the key was pressed"
 * @param y y of "mouse location in window relative coordinates when the key was pressed"
 */
void keyboard(unsigned char key, int x, int y){

    switch (key){
        case 'q':case 'Q':
            exit(EXIT_SUCCESS);
            break;
        case 's':
            show_line = !show_line;
            break;
        case 'S':
            show_line_new = !show_line_new;
            break;
        case 't':
        case 'T':
        case 'u':
        case 'U':
            top_view_flag = !top_view_flag;
            break;
        case 'r':
        case 'R':
            stop_rotate = !stop_rotate;
            break;
        default: ;
            // Do nothing
    }
    glutPostRedisplay();
}

/**
 * Is called when the window is resize
 * @param width the new width of the window
 * @param height the new height of the window
 */
void Reshape(int width, int height) {
    glViewport(0, 0, width, height);
    aspect = float(width) / float(height);
    glutPostRedisplay();
}

/**
 * Default method to use with glutTimerFunc to update things often rotate
 * @param n witch thing we are updating
 */
void rotate(int n) {
    switch (n) {
        case 1:
            if (!stop_rotate) {
                rotateAngle += 2.75f;
            }

            glutPostRedisplay(); // Redraw the screen
            // restart timer
            glutTimerFunc(100, rotate, 1); // update forever (not just ones)
            break;
        default: ;
            // do nothing
    }

}

// ------------------ Main ---------------------------

/**
 * Start of the program
 * @param argc number of arguments
 * @param argv pointer to array of arguments (string (array of char))
 * @return Exit code (0 is often means no problems)
 */
int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(512, 512);

    glutCreateWindow("FreeGLUT - OpenGL - Basic");

    if (glewInit()) {
        printf("Unable to initialize GLEW ... exiting\n");
        return EXIT_FAILURE;
    }

//    ilInit();
    Initialize(); // Our Initialize method

    // GL info
    fprintf(stdout, "Info: GL Vendor : %s\n", glGetString(GL_VENDOR));
    fprintf(stdout, "Info: GL Renderer : %s\n", glGetString(GL_RENDERER));
    fprintf(stdout, "Info: GL Version (string) : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    fprintf(stdout, "Info: GLSL Version : %s\n", glGetString(GL_VERSION));

    glutDisplayFunc(Display); // Tell glut our display method
    glutKeyboardFunc(keyboard); // Tell glut our keyboard method
    glutReshapeFunc(Reshape); // Tell glut our reshape method
    glutTimerFunc(100, rotate, 1); // First timer for rotate
    glutMainLoop(); // Start glut infinite loop

    return EXIT_SUCCESS;
}
#pragma clang diagnostic pop