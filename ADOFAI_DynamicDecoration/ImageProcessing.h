#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

#define DecoFileName(deco_name, i) (deco_name + " " + to_string(i) + ".png")

int GetVideoFrame(const string video_path);

void ExtractImages(const string video_path, vector<Mat>& images, double extract_unit);

void SaveImages(const string save_path, const string deco_name, vector<Mat>& images);

void RemoveImages(const string save_path, const string deco_name, int size);