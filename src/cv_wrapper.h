//
// Created by Mirkwood Ape on 10/25/2020.
//

#ifndef OLAS_CV_WRAPPER_H
#define OLAS_CV_WRAPPER_H

#include <string>
#include "glad/glad.h"
#include "opencv2/opencv.hpp"

int play_vid(const std::string& filepath);
//static GLuint matToTexture(const cv::Mat& mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);

#endif //OLAS_CV_WRAPPER_H
