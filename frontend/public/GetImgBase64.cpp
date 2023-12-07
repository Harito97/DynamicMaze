#include "opencv4/opencv2/opencv.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <httplib.h>

std::string encodeImageToBase64(const std::string &imagePath)
{
    // Read the content of the image file
    std::ifstream imageFile(imagePath, std::ios::binary);
    std::vector<char> imageData((std::istreambuf_iterator<char>(imageFile)), std::istreambuf_iterator<char>());
    std::string imageString(imageData.begin(), imageData.end());

    // Convert the image to base64
    std::string img_base64 = httplib::detail::base64_encode(imageString);

    return img_base64;
}

int main()
{
    // Path to the image file
    std::string imagePath = "logo.png";

    // Convert the image to base64
    std::string encodedImage = encodeImageToBase64(imagePath);

    // Use the encoded value for your purposes
    std::cout << "Encoded Image: " << encodedImage << std::endl;

    return 0;
}
