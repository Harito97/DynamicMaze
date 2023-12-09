#include "opencv4/opencv2/opencv.hpp"
#include "opencv4/opencv2/highgui/highgui.hpp"

int main() {
    // Tạo ảnh đen
    cv::Mat image(300, 300, CV_8UC3, cv::Scalar(0, 0, 0));

    // Tọa độ (i, j) trung tâm của vùng hình tròn mới
    int i = 50;
    int j = 50;

    // Bán kính của vùng hình tròn
    int radius = 3;

    // Tạo hình tròn màu đỏ
    cv::circle(image, cv::Point(3 * i, 3 * j), 3 * radius, cv::Scalar(0, 0, 255), -1);

    // Hiển thị ảnh
    cv::imshow("Circle Image", image);
    cv::waitKey(0);

    return 0;
}

// int main() {
//     // Tạo một đối tượng ảnh trống với kích thước 640x480 và kiểu dữ liệu là 8-bit unsigned integer, 3 kênh màu (BGR).
//     cv::Mat image(480, 640, CV_8UC3, cv::Scalar(256, 256, 256));

//     // Hiển thị ảnh trong một cửa sổ
//     cv::imshow("My Image", image);

//     // Chờ người dùng nhấn một phím bất kỳ để đóng cửa sổ
//     cv::waitKey(0);

//     return 0;
// }

// int main() {
//     // Đọc ảnh từ file
//     cv::Mat image = cv::imread("logo.png");

//     if (image.empty()) {
//         std::cerr << "Could not open or find the image!" << std::endl;
//         return -1;
//     }

//     // Phóng to ảnh với bicubic interpolation
//     cv::Mat enlargedImage;
//     cv::resize(image, image, cv::Size(), 2.0, 2.0, cv::INTER_CUBIC);

//     // Hiển thị ảnh gốc và ảnh phóng to
//     cv::imshow("Original Image", image);
//     // cv::imshow("Enlarged Image", enlargedImage);
//     cv::waitKey(0);

//     return 0;
// }

