//
// Created by Mirkwood Ape on 10/28/2020.
//

#include <iostream>
#include <unistd.h>
#include "cv_wrapper.h"
#include "GLFW/glfw3.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

using namespace std;
using namespace cv;
using namespace glm;

int window_width  = 1280;
int window_height = 960;
int frame_count = 0;
double frame_start_time, frame_end_time, frame_draw_time;

mat4 camera(float m_translate, vec2 const& m_rotate){
    mat4 projection = perspective(glm::pi<float>() * 0.25f, 4.0f/3.0f, 0.1f, 100.f);
    mat4 view = translate(mat4(1.0f), vec3(0.0f, 0.0f, -m_translate));
    view = rotate(view, m_rotate.y, vec3(-1.0f, 0.0f, 0.0f));
    view = rotate(view, m_rotate.x, vec3(0.0f, 1.0f, 0.0f));
    mat4 model = scale(mat4(1.0f), vec3(0.5f));
    return projection * view * model;
}

static GLuint matToTexture(const cv::Mat& mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter) {
    // generate a number for our textureID's unique handle
    GLuint textureID;
    glad_glGenTextures(1, &textureID);

    // bind to our texture handle
    glad_glBindTexture(GL_TEXTURE_2D, textureID);

    // catch mistake for texture interpolation method for magnification
    if (magFilter == GL_LINEAR_MIPMAP_LINEAR ||
        magFilter == GL_LINEAR_MIPMAP_NEAREST ||
        magFilter == GL_NEAREST_MIPMAP_LINEAR ||
        magFilter == GL_NEAREST_MIPMAP_NEAREST) {
        cout << "You can't use MIPMAPs for magnification - setting filter to GL_LINEAR" << endl;
        magFilter = GL_LINEAR;
    }

    // set texture interpolation methods for minification and magnification
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    // set texture clamping method
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

    // set incoming texture format to
    GLenum inputColourFormat = GL_BGR;
    if (mat.channels() == 1)
        inputColourFormat = GL_LUMINANCE;

    // create the texture
    glad_glTexImage2D(GL_TEXTURE_2D,
                      0,
                      GL_RGB,
                      mat.cols,
                      mat.rows,
                      0,
                      inputColourFormat,
                      GL_UNSIGNED_BYTE,
                      mat.ptr());

    // if we're using mipmaps then generate them. note: this requires OpenGL 3.0 or higher
    if (minFilter == GL_LINEAR_MIPMAP_LINEAR  ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_NEAREST_MIPMAP_NEAREST)
    {
        glad_glGenerateMipmap(GL_TEXTURE_2D);
    }

    // return texture ID
    return textureID;
}

static void draw_frame(const Mat& frame){
    // clear color and depth buffer
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glad_glMatrixMode(GL_MODELVIEW);

    glad_glEnable(GL_TEXTURE_2D);
    GLuint image_tex = matToTexture(frame, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP);

    // draw a quad
    glad_glBegin(GL_QUADS);
    glad_glTexCoord2i(0, 0); glad_glVertex2i(0, 0);
    glad_glTexCoord2i(0, 1); glad_glVertex2i(0, window_height);
    glad_glTexCoord2i(1, 1); glad_glVertex2i(window_width, window_height);
    glad_glTexCoord2i(1, 0); glad_glVertex2i(window_width, 0);
    glad_glEnd();

    // cleanup
    glad_glDeleteTextures(1, &image_tex);
    glad_glDisable(GL_TEXTURE_2D);
}

void lock_frame_rate(double frame_rate) {
    static double allowed_frame_time = 1.0 / frame_rate;

    // Note: frame_start_time is called first thing in the main loop
    frame_end_time = glfwGetTime();  // in seconds

    frame_draw_time = frame_end_time - frame_start_time;

    double sleep_time = 0.0;

    if (frame_draw_time < allowed_frame_time) {
        sleep_time = allowed_frame_time - frame_draw_time;
        usleep(1000000 * sleep_time);
    }

    // Debug stuff
    double potential_fps = 1.0 / frame_draw_time;
    double locked_fps    = 1.0 / (glfwGetTime() - frame_start_time);
    cout << "Frame [" << frame_count << "] ";
    cout << "Draw: " << frame_draw_time << " Sleep: " << sleep_time;
    cout << " Pot. FPS: " << potential_fps << " Locked FPS: " << locked_fps << endl;
}

static void init_gl(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w, h, 0.0, 0.0, 100.0);

    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void error_callback(int error, const char* desc){
    fprintf(stderr, "Error: %s\n", desc);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char* argv[]){
    if (argc != 2){
        cout << "Usage: " << argv[0] << "<path to image file>" << endl;
        exit(EXIT_FAILURE);
    }

//    Mat img = imread(argv[1]);
//    if (img.empty()){
//        cout << "Cannot load image: " << argv[1] << endl;
//        exit(EXIT_FAILURE);
//    }
    VideoCapture capture(argv[1]);
    if (!capture.isOpened()){
        cout << "Cannot open video: " << argv[1] << endl;
        exit(EXIT_FAILURE);
    }

//    window_width = img.cols;
//    window_height = img.rows;

    double fps = 0.0;
    fps = capture.get(CAP_PROP_FPS);
    if (fps != fps) { // NaN
        fps = 25.0;
    }

    cout << "FPS: " << fps << endl;

    window_width = (int) capture.get(CAP_PROP_FRAME_WIDTH);
    window_height = (int) capture.get(CAP_PROP_FRAME_HEIGHT);

    // glfw
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()){
        cout << "Failed on initializing GLFW3" << endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(window_width, window_height, "OPENCV to GL Texture", nullptr, nullptr);
    if (!window){
        cout << "Window creation failed. Terminating..." << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // init glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    init_gl(window_width, window_height);

    // main state loop
    double video_start_time = glfwGetTime();
    double video_end_time = 0.0;
    Mat frame;
    while (!glfwWindowShouldClose(window)){
//        draw_frame(img);
        frame_start_time = glfwGetTime();
        if (!capture.read(frame)) {
            cout << "Cannot grab a frame." << endl;
            break;
        }

        draw_frame(frame);
        video_end_time = glfwGetTime();

        glfwSwapBuffers(window);
        glfwPollEvents();

        ++frame_count;
        lock_frame_rate(0);
    }

    // cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}