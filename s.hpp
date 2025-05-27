#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include <sstream>
#include <iomanip>

class LightPair {
public:
    cv::Rect leftLight;
    cv::Rect rightLight;
    cv::Rect greenRect;
    cv::Point center;
    bool valid;
    float aspectRatio;

    static float TargetAspect;    // 目标长宽比 (默认2.60)
    static float AspectTolerance; // 允许误差 (默认±0.2)

    LightPair(const cv::Rect& rect1, const cv::Rect& rect2) 
        : valid(false), aspectRatio(-1.0f) 
    {
        // 1. 确定左右灯条顺序
        if (rect1.x < rect2.x) {
            leftLight = rect1;
            rightLight = rect2;
        } else {
            leftLight = rect2;
            rightLight = rect1;
        }

        // 2. 生成绿色包围矩形
        greenRect = cv::Rect(leftLight.tl(), rightLight.br());

        // 3. 计算长宽比（若高度为0则直接返回）
        if (greenRect.height == 0) return;
        aspectRatio = static_cast<float>(greenRect.width) / greenRect.height;

        // 4. 验证长宽比是否符合条件
        if (std::abs(aspectRatio - TargetAspect) <= AspectTolerance) {
            // 5. 仅当验证通过时计算中心点
            center = cv::Point(
                greenRect.x + greenRect.width / 2,
                greenRect.y + greenRect.height / 2
            );
            valid = true;
        }
    }

    static std::vector<LightPair> findValidPairs(const std::vector<cv::Rect>& lights) {
        std::vector<LightPair> validPairs;
        for (size_t i = 0; i < lights.size(); ++i) {
            for (size_t j = i + 1; j < lights.size(); ++j) {
                LightPair pair(lights[i], lights[j]);
                if (pair.valid) validPairs.push_back(pair);
            }
        }
        return validPairs;
    }
};



void display(std::vector<cv::Mat>&);
std::vector<cv::Rect> get_contours1(cv::Mat, cv::Mat, int);
cv::Mat get_contours(cv::Mat Input, cv::Mat ori, int minarea);
int judge_shape(std::vector<cv::Point>);