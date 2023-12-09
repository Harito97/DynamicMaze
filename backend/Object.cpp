// __Author: Pham Ngoc Hai _ Harito__
#pragma once
#include <iostream>
#include <vector>

class Object
{
private:
    std::pair<int, int> current;
    int lengthPath;
    bool getTarget;

public:
    // Object() {}     // don't delete this or Main.cpp be crash

    Object(std::pair<int, int> start) {
        current = start;
        lengthPath = 0;
        getTarget = false;
    }

    void move(std::pair<int, int> point) {
        // Khi sử dụng cần đảm bảo trước sử dụng point là hàng xóm của current 
        // Tránh trường hợp dịch chuyển tức thời 
        current = point;
        ++lengthPath;
    }

    bool isTarget() {
        return getTarget;
    }

    void gotTarget() {
        getTarget = true;
    }

    // constexpr return value when compile to make can use operator[]
    constexpr std::pair<int, int> currentPoint() {
        return current;
    }

    int length() {
        return lengthPath;
    }
};