#include"s.hpp"


cv::RotatedRect rectToRotatedRect(const cv::Rect& rect, float angle)
{
    return cv::RotatedRect(
        cv::Point2f(rect.x + rect.width * 0.5f, rect.y + rect.height * 0.5f),
        cv::Size2f(rect.width, rect.height),
        angle
    );
}

void display(std::vector<cv::Mat>& images) {
    if (images.empty() || images[0].empty()) {
        std::cerr << "Error: No valid images to display." << std::endl;
        return;
    }

    int times = images.size() > 3 ? images.size() / 3 : 0;
    int width = images.size() > 3 ? 3 : images.size();
    const int w = images[0].cols * width + 10 * width;
    const int h = images[0].rows * (times + 1) + 10 * (times + 1);
    const int cols = images[0].cols;
    const int rows = images[0].rows;
    const int type = images[0].type();

    if (w <= 0 || h <= 0) {
        std::cerr << "Error: Invalid image dimensions." << std::endl;
        return;
    }

    cv::Mat result = cv::Mat::zeros(cv::Size(w, h + 10), images[0].type());
    cv::Rect box(0, 0, cols, rows);

    for (size_t i = 0; i < images.size(); ++i) {
        if (images[i].empty()) {
            std::cerr << "Warning: Image at index " << i << " is empty." << std::endl;
            continue;
        }
        if (images[i].channels() == 1 && images[0].channels() == 3) {
            cv::cvtColor(images[i], images[i], cv::COLOR_GRAY2BGR);
        }
        int level = i / 3;
        int pos = i % 3;
        box.x = cols * pos + 1 * pos;
        box.y = rows * level + (level >= 1 ? 2 : 0);

        // 检查ROI是否超出边界
        if (box.x + box.width > result.cols || box.y + box.height > result.rows) {
            std::cerr << "Error: ROI exceeds image boundaries." << std::endl;
            continue;
        }

        images[i].copyTo(result(box));
    }
    cv::namedWindow("res", cv::WINDOW_AUTOSIZE);
    //cv::resizeWindow("res", cv::Size(w, h));
    cv::imshow("res", result);

}

cv::Mat get_contours(cv::Mat Input, cv::Mat ori, int minarea) {    //只能处理一通道
    std::vector<std::vector<cv::Point>> contours;
    //cv::Mat conImg = cv::Mat::zeros(Input.size(), CV_8UC3);
    cv::Mat conImg = ori.clone();
    std::vector<cv::Rect> light;
    cv::findContours(Input, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::Point Pos1, Pos2;
    for (auto s : contours) {

        long area = cv::contourArea(s);
        if (area > minarea) {
            //cv::drawContours(conImg, contours, -1, cv::Scalar(255, 0, 255), 7);


            double peri = cv::arcLength(s, true); // 计算周长
            std::vector<cv::Point> approx;
            cv::approxPolyDP(s, approx, 0.02 * peri, true); // 多边形拟合
            if (approx.empty()) std::cerr << "no approx points" << std::endl;


            cv::Rect rect = cv::boundingRect(approx); // 获取最小外接矩形  cv::minAreaRect()矩形有可能会旋转以获取绝对意义上的最小外围矩形

            cv::rectangle(conImg, rect, cv::Scalar(255, 0, 255), 1);

            cv::Point pos1 = { rect.x , rect.y + 20 };
            cv::Point pos2 = { rect.x , rect.y + 40 };

            cv::putText(conImg, "Points: " + std::to_string(approx.size()), pos1, cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(255, 0, 0), 1);
            cv::putText(conImg, "Points: " + std::to_string((int)area), pos2, cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(255, 0, 0), 1);
            //get_light(rect,Light);
            if (rect.area() > 500 && rect.area() < 1000) {
                light.push_back(rect);
            }
        }
    }
    //std::cout << contours.size() << std::endl;
    if (light.size() == 2) {
        cv::Rect light_rect(light[0].tl(), light[1].br());
        cv::Point center = cv::Point((light_rect.x + light_rect.width / 2), (light_rect.y + light_rect.height / 2));
        cv::circle(conImg, center, 5, cv::Scalar(255, 0, 255), 2);
        cv::rectangle(conImg, light_rect, cv::Scalar(0, 255, 0), 1);
    }
    return conImg;
}