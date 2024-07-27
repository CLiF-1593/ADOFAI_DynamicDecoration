#include "ImageProcessing.h"
#include "Color.h"
#include <filesystem>
using namespace std;

int GetVideoFrame(const string video_path) {
	VideoCapture src_video; //Load video
	src_video.open(video_path);
	if (!src_video.isOpened()) {
		cout << "ERROR : VIDEO couldn't open";
		system("pause");
		exit(-1);
	}
	return src_video.get(CAP_PROP_FRAME_COUNT);
}

void ExtractImages(const string video_path, vector<Mat>& images, double extract_unit) {
	VideoCapture src_video; //Load video
	src_video.open(video_path);
	if (!src_video.isOpened()) {
		cout << "ERROR : VIDEO couldn't open";
		return;
	}

	VideoWriter writer;
	int cur_frame = 0;
	double cnt = 0;
	while (src_video.isOpened()) {
		Mat src_img;
		bool ret = src_video.read(src_img); //Load image
		if (!ret) {
			break;
		}
		if ((int)cnt <= cur_frame) {
			cv::Mat input_bgra;
			register rgb clr_rgb;
			register hsv clr_hsv;
			cv::cvtColor(src_img, input_bgra, COLOR_BGR2BGRA);
			/*for (register int y = 0; y < input_bgra.rows; ++y) {
				for (register int x = 0; x < input_bgra.cols; ++x) {
					register cv::Vec4b& pixel = input_bgra.at<cv::Vec4b>(y, x);
					clr_rgb.r = pixel[0];
					clr_rgb.g = pixel[1];
					clr_rgb.b = pixel[2];
					clr_hsv = rgb2hsv(clr_rgb);
					int alpha = clr_hsv.v;
					clr_hsv.v = 255;
					clr_rgb = hsv2rgb(clr_hsv);
					pixel[0] = clr_rgb.r;
					pixel[1] = clr_rgb.g;
					pixel[2] = clr_rgb.b;
					pixel[3] = alpha;
				}
			}*/
			images.push_back(input_bgra);
			cnt += extract_unit;
		}
		cur_frame++;
	}
}

void SaveImages(const string save_path, const string deco_name, vector<Mat>& images) {
	for (int i = 0; i < images.size(); i++) {
		imwrite(filesystem::path(filesystem::u8path(save_path + DecoFileName(deco_name, i))).string(), images[i]);
	}
}

void RemoveImages(const string save_path, const string deco_name, int size) {
	for (int i = 0; i < size; i++) {
		if (filesystem::exists(save_path + DecoFileName(deco_name, i))) {
			filesystem::remove(save_path + DecoFileName(deco_name, i));
		}
	}
}