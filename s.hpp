#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>

void display(std::vector<cv::Mat>&);
std::vector<cv::Rect> get_contours1(cv::Mat, cv::Mat, int);
cv::Mat get_contours(cv::Mat Input, cv::Mat ori, int minarea);
int judge_shape(std::vector<cv::Point>);