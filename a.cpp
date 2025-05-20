#include"s.hpp"

void Imgupdate(int, void*) {
    ;
}

int main() {


    int frame_width = 640;
    int frame_height = 480;
    int minarea = 0;
    cv::Mat img = cv::imread("/home/yunxi/test.jpeg");
    if (!img.empty()) {
        std::cout << "image is loaded" << std::endl;
    }
    cv::resize(img, img, cv::Size(frame_width, frame_height));
    cv::GaussianBlur(img, img, cv::Size(7, 7), 1);   //高斯模糊
   
    cv::Mat HSV_img;
    cv::cvtColor(img, HSV_img, cv::COLOR_BGR2HSV);
    // cv::imshow("img", img);
    // cv::imshow("HSV_img", HSV_img);
    // cv::waitKey(0);
    cv::Mat Gray_img;



    int HUE_Min = 41, HUE_Max = 119, SAT_Min = 38, SAT_Max = 255, VALUE_Min = 0, VALUE_Max = 255;


    cv::namedWindow("HSV", cv::WINDOW_FREERATIO); //色调最大值为179  饱和度为255
    cv::resizeWindow("HSV", cv::Size(320, 280));
    cv::createTrackbar("HUE_Min", "HSV", &HUE_Min, 179, Imgupdate);//HUE色度
    cv::createTrackbar("HUE_Max", "HSV", &HUE_Max, 179, Imgupdate);
    cv::createTrackbar("SAT_Min", "HSV", &SAT_Min, 255, Imgupdate);//SAT饱和度
    cv::createTrackbar("SAT_Max", "HSV", &SAT_Max, 255, Imgupdate);
    cv::createTrackbar("VALUE_Min", "HSV", &VALUE_Min, 255, Imgupdate);//VALUE明度
    cv::createTrackbar("VALUE_Max", "HSV", &VALUE_Max, 255, Imgupdate);

    while (1) {
        auto h_min = cv::getTrackbarPos("HUE_Min", "HSV");
        auto h_max = cv::getTrackbarPos("HUE_Max", "HSV");
        auto s_min = cv::getTrackbarPos("SAT_Min", "HSV");
        auto s_max = cv::getTrackbarPos("SAT_Max", "HSV");
        auto v_min = cv::getTrackbarPos("VALUE_Min", "HSV");
        auto v_max = cv::getTrackbarPos("VALUE_Max", "HSV");

        cv::Scalar lower(h_min, s_min, v_min);
        cv::Scalar upper(h_max, s_max, v_max);
        cv::Mat mask;
        cv::inRange(HSV_img, lower, upper, mask);
        cv::Mat result;
        cv::bitwise_and(img, img, result, mask);

            //  lightsbar lights;
            //  std::vector<light> contours = lights.find_lights(get_contours(mask, img, minarea));
            //  std::vector<std::vector<light>> contours2 = lights.match_lights(contours);
            //  lights.Display(contours2,img);
        cv::Mat contours_img = get_contours(mask, img, minarea);
        std::vector<cv::Mat> images = { img ,HSV_img,result
                                        ,mask ,contours_img };
        display(images);

        if (cv::waitKey(1) == 27) exit(0);
    }
    cv::destroyAllWindows();
    return 0;
}