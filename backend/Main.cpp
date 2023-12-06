#include <httplib.h>
#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "Problem.cpp"

int main()
{
        httplib::Server svr;
        std::string data;
        std::string text_result = "Harito";
        bool currentWork = false;
        std::string* encodedImage;

        // Endpoint để xử lý yêu cầu GET
        svr.Get("/", [](const httplib::Request &, httplib::Response &res)
                { res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
            res.set_content("Hello from C++ server!", "text/plain"); });

        svr.Get("/get-txt-result", [&](const httplib::Request &, httplib::Response &res)
                { res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
            res.set_content(text_result, "text/plain"); });

        svr.Get("/get-img-result", [&](const httplib::Request &, httplib::Response &res)
                {
                        // // Tạo một ảnh mẫu (điều này có thể được thay thế bằng việc đọc từ tệp ảnh hoặc quá trình xử lý ảnh thực tế)
                        // cv::Mat img = cv::Mat::zeros(100, 100, CV_8UC3);
                        // cv::circle(img, cv::Point(50, 50), 30, cv::Scalar(255, 255, 255), -1);

                        // // Chuyển đổi ảnh thành dạng base64
                        // std::vector<uchar> img_vec;
                        // cv::imencode(".png", img, img_vec);
                        // std::string img_base64 = httplib::detail::base64_encode(reinterpret_cast<const char*>(img_vec.data()), img_vec.size());

                        // // Trả về ảnh dưới dạng base64
                        // res.set_content(img_base64, "text/plain");
                });

        // Endpoint để nhận dữ liệu từ React
        svr.Post("/load-custom-data", [&](const httplib::Request &req, httplib::Response &res)
                 {
                         if (currentWork == true)
                         {
                                 std::cout << "Another process is working!\n";
                                 res.set_content("Another process is working!\n", "text/plain");
                                 return 0;
                         }
                         currentWork = true;
                         res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
                         data = req.body;
                         std::cout << "Received data from React: " << data << std::endl;
                         res.set_content("Custom data received successfully", "text/plain");
                         const char *jsonData = data.c_str();
                         rapidjson::Document jsonDoc;
                         jsonDoc.Parse(jsonData);

                         if (jsonDoc.HasParseError())
                         {
                                 std::cerr << "JSON parse error: " << jsonDoc.GetParseError() << std::endl;
                                 return 1; // Thêm lệnh return ở đây
                         }

                         int numObject = jsonDoc["numObject"].GetInt();                         
                         int sizeMaze = static_cast<int>(sqrt(jsonDoc["sizeMaze"].GetInt()));;
                         double diffLevel = jsonDoc["diffLevel"].GetInt() / 100.0;
                         int distance = jsonDoc["distance"].GetInt();

                         std::cout << "numObject: " << numObject << std::endl;
                         std::cout << "sizeMaze: " << sizeMaze << std::endl;
                         std::cout << "diffLevel: " << diffLevel << std::endl;
                         std::cout << "distance: " << distance << std::endl;
                         Problem problem(numObject, {sizeMaze, sizeMaze}, diffLevel, distance);
                         text_result = problem.solve();

                         currentWork = false;
                         return 0; // Thêm lệnh return ở đây hoặc có thể không trả về gì cả
                 });

        svr.Post("/load-experiment-data", [&](const httplib::Request &req, httplib::Response &res)
                 { 
                         if (currentWork == true) 
                         {
                                 std::cout << "There is another process are working!\n";
                                 res.set_content("There is another process are working!\n", "text/plain");
                                 return 0;
                         }
                         currentWork = true; 
                         res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
                         // Lấy dữ liệu từ body của request
                         data = req.body;
                         // In dữ liệu lên console
                         std::cout << "Received data from React: " << data << std::endl;
                         // Phản hồi về React
                         res.set_content("Data received successfully", "text/plain");
                         currentWork = false; 
                         return 0; });

        // svr.Post("/stop", [&](const httplib::Request &req, httplib::Response &res)
        //          { svr.stop(); });

        // Khởi động server trên cổng 8080
        svr.listen("0.0.0.0", 8080);
        return 0;
}
