//
// Created by Mirkwood Ape on 10/25/2020.
//

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include "cv_wrapper.h"

using namespace std;

int play_vid(const string& filepath){
    cv::VideoCapture cap(filepath);

    if (!cap.isOpened()){
        cout << "Cannot open the video file" << endl;
        cin.get();
        return -1;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    double fps_w = 1e6/fps;
    cout << "Video: FPS is " << fps << endl;
    cout << "Frametime: " << fps_w/1e3 << " miliseconds" << endl;

    string window_name = "OLAS";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);

    while(true){
        cv::Mat frame;
        bool bSuccess = cap.read(frame);

        if (!bSuccess){
            cout << "Found the end of the video" << endl;
            break;
        }

        cv::imshow(window_name, frame);

        usleep(fps_w);

        if(cv::waitKey(5) == 27){
            cout << "Esc key is pressed. Stopping the video..." << endl;
            break;
        }
    }

    return 0;
}

//static GLuint matToTexture(const cv::Mat& mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter) {
//    // generate a number for our textureID's unique handle
//    GLuint textureID;
//    glad_glGenTextures(1, &textureID);
//
//    // bind to our texture handle
//    glad_glBindTexture(GL_TEXTURE_2D, textureID);
//
//    // catch mistake for texture interpolation method for magnification
//    if (magFilter == GL_LINEAR_MIPMAP_LINEAR ||
//        magFilter == GL_LINEAR_MIPMAP_NEAREST ||
//        magFilter == GL_NEAREST_MIPMAP_LINEAR ||
//        magFilter == GL_NEAREST_MIPMAP_NEAREST) {
//        cout << "You can't use MIPMAPs for magnification - setting filter to GL_LINEAR" << endl;
//        magFilter = GL_LINEAR;
//    }
//
//    // set texture interpolation methods for minification and magnification
//    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
//    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
//
//    // set texture clamping method
//    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
//    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);
//
//    // set incoming texture format to
//    GLenum inputColourFormat = GL_BGR;
//    if (mat.channels() == 1)
//        inputColourFormat = GL_LUMINANCE;
//
//    // create the texture
//    glad_glTexImage2D(GL_TEXTURE_2D,
//                      0,
//                      GL_RGB,
//                      mat.cols,
//                      mat.rows,
//                      0,
//                      inputColourFormat,
//                      GL_UNSIGNED_BYTE,
//                      mat.ptr());
//
//    // if we're using mipmaps then generate them. note: this requires OpenGL 3.0 or higher
//    if (minFilter == GL_LINEAR_MIPMAP_LINEAR  ||
//        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
//        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
//        minFilter == GL_NEAREST_MIPMAP_NEAREST)
//    {
//        glad_glGenerateMipmap(GL_TEXTURE_2D);
//    }
//
//    // return texture ID
//    return textureID;
//}