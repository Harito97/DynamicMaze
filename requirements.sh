#!/bin/bash

# Kiểm tra và cài đặt npm
if ! command -v npm &> /dev/null; then
    echo "npm is not installed. Installing..."
    sudo pacman -S npm
else
    echo "npm is already installed."
fi

# Kiểm tra và hiển thị phiên bản npm
npm_version=$(npm --version)
echo "npm version: $npm_version"

# Kiểm tra và cài đặt opencv
if ! command -v opencv_version &> /dev/null; then
    echo "OpenCV is not installed. Installing..."
    sudo pacman -S opencv
else
    echo "OpenCV is already installed."
fi

# Kiểm tra và cài đặt rapidjson
if ! command -v rapidjson_version &> /dev/null; then
    echo "RapidJSON is not installed. Installing..."
    sudo pacman -S rapidjson
else
    echo "RapidJSON is already installed."
fi

# Kiểm tra và cài đặt Qt6-Base
if ! command -v qmake &> /dev/null; then
    echo "Qt6-Base is not installed. Installing..."
    sudo pacman -S qt6-base
else
    echo "Qt6-Base is already installed."
fi

# Kiểm tra và cài đặt cpp-httplib
if ! command -v httplib_version &> /dev/null; then
    echo "cpp-httplib is not installed. Installing..."
    yay -S cpp-httplib
else
    echo "cpp-httplib is already installed."
fi

# Thông báo hoàn tất
echo "Package installation completed."
