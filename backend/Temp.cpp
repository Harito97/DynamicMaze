#include "opencv4/opencv2/opencv.hpp"
#include "opencv4/opencv2/highgui/highgui.hpp"

int main() {
    // Tạo một đối tượng ảnh trống với kích thước 640x480 và kiểu dữ liệu là 8-bit unsigned integer, 3 kênh màu (BGR).
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(256, 256, 256));

    // Hiển thị ảnh trong một cửa sổ
    cv::imshow("My Image", image);

    // Chờ người dùng nhấn một phím bất kỳ để đóng cửa sổ
    cv::waitKey(0);

    return 0;
}
