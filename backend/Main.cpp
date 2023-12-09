// __Author: Pham Ngoc Hai _ Harito__
#include <httplib.h>
#include <iostream>
#include <string>
#include <cmath>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "opencv4/opencv2/opencv.hpp"
#include "opencv4/opencv2/highgui/highgui.hpp"
#include "Problem.cpp"

int main()
{
        httplib::Server svr;
        std::string data;
        std::string text_result = "Harito hello Terminal!";
        bool currentWork = false;
        std::string *encodedImage = new std::string("");

        // Endpoint để xử lý yêu cầu GET
        svr.Get("/", [](const httplib::Request &, httplib::Response &res)
                { 
                        // res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
                        // res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
                        // res.set_header("Access-Control-Allow-Headers", "Content-Type");
                        // res.set_header("Access-Control-Allow-Credentials", "true");
                        res.set_content("Hello from C++ server!", "text/plain"); });

        svr.Get("/get-txt-result", [&](const httplib::Request &, httplib::Response &res)
                { 
                        res.set_header("Access-Control-Allow-Origin", "*");
                        // res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
                        // res.set_header("Access-Control-Allow-Methods", "GET");
                        // res.set_header("Access-Control-Allow-Headers", "Origin, Content-Type, Accept");
                        // res.set_header("Access-Control-Allow-Credentials", "true");
                        res.set_content(text_result, "text/plain"); });


        svr.Get("/get-img-result", [&](const httplib::Request &, httplib::Response &res)
                { res.set_header("Access-Control-Allow-Origin", "*");
                        res.set_content(*encodedImage, "text/plain"); });

        // Endpoint để nhận dữ liệu từ React
        svr.Post("/load-custom-data", [&](const httplib::Request &req, httplib::Response &res)
                 {
                        //  res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
                        //  res.set_header("Access-Control-Allow-Credentials", "true");
                        //  res.set_header("Access-Control-Allow-Methods", "POST, GET, DELETE, HEAD, OPTIONS");
                         if (currentWork == true)
                         {
                                 std::cout << "Load custom data failed as another process is working!\n";
                                 res.set_content("Load custom data failed as another process is working!\n", "text/plain");
                                 return 0;
                         }
                         currentWork = true;
                         data = req.body;
                         std::cout << "Received data from React: " << data << std::endl;
                         res.set_content("Custom data received successfully", "text/plain");
                         const char *jsonData = data.c_str();
                         rapidjson::Document jsonDoc;
                         jsonDoc.Parse(jsonData);

                         if (jsonDoc.HasParseError())
                         {
                                 std::cerr << "JSON parse error: " << jsonDoc.GetParseError() << std::endl;
                                 return 1;
                         }

                         int numObject = jsonDoc["numObject"].GetInt();
                         int sizeMaze = static_cast<int>(sqrt(jsonDoc["sizeMaze"].GetInt()));
                         double diffLevel = jsonDoc["diffLevel"].GetInt() / 100.0;
                         int distance = jsonDoc["distance"].GetInt();
                         Problem problem(numObject, {static_cast<int>(sizeMaze / 1.75), sizeMaze}, diffLevel, distance);
                        // Problem problem(numObject, {sizeMaze, sizeMaze}, diffLevel, distance);
                         text_result = problem.solve(encodedImage);

                         currentWork = false;
                         return 0; });

        svr.Post("/load-experiment-data", [&](const httplib::Request &req, httplib::Response &res)
                 { 
                        //  res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
                        //  res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS"); 
                        //  res.set_header("Access-Control-Allow-Headers", "Content-Type"); 
                        //  res.set_header("Access-Control-Allow-Credentials", "true");
                         if (currentWork == true) 
                         {
                                 std::cout << "Load experiment data failed as another process is working!\n";
                                 res.set_content("Load experiment data failed as another process is working!\n", "text/plain");
                                 return 0;
                         }
                         currentWork = true;
                         res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
                         Problem problem(100);
                         text_result = problem.solve(encodedImage);
                         // In dữ liệu lên console
                         std::cout << "Received request from React: " << data << std::endl;
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
