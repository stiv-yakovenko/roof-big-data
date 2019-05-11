// prepare.cpp : Defines the entry point for the console application.
//
#include <opencv2/core/core.hpp>
#include <iomanip>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <map>
#include <random>
using namespace cv;
using namespace std;
namespace fs = experimental::filesystem;

int main() {
	string path = "../../full/";
	map<string, string> src;
	for (auto & p : fs::recursive_directory_iterator(path)) {
		if (!is_regular_file(p.path())) continue;
		src[p.path().stem().string()] = p.path().string();
	}
	std::random_device rd; 
	std::mt19937 eng(rd());
	int W = 256;
	auto isred = [](Mat&m, int x, int y) {
		auto cl = m.at<cv::Vec3b>(y, x);
		return  (cl[2] == 237 && cl[1] == 28 && cl[0] == 36);
	};
	int numblocks = 0;
	while (numblocks < 300) {
		for (auto & p : fs::recursive_directory_iterator("../../labeled/")) {
			if (!is_regular_file(p.path())) continue;
			string name = p.path().string();
			auto img = imread(name);
			string n1 = p.path().stem().string();
			auto img0 = imread(src[n1]);
			for (int d = 0; d < 90; d++) {
				uniform_int_distribution<> dx(0, img.cols - W);
				uniform_int_distribution<> dy(0, img.rows - W);
				int x0 = dx(eng);
				int y0 = dy(eng);
				Rect rect = Rect(x0, y0, W, W);
				int rcnt = 0;
				Mat block = img(rect);
				for (int y = 0; y < block.rows; ++y) {
					for (int x = 0; x < block.cols; ++x) {
						if (isred(block, x, y)) {
							rcnt++;
							break;
						}
					}
					if (rcnt) break;
				}
				bool badmargin = false;
				for (int y = 0; y < block.rows; y++) {
					if (isred(block, 0, y) || isred(block, block.cols - 1, y)) badmargin=1;
				}
				for (int x = 0; x < block.cols; x++) {
					if (isred(block, x, 0) || isred(block, x,block.rows - 1)) badmargin=1;
				}
				if (badmargin) continue;

				if (numblocks % 2 == 0 && !rcnt) continue;
				Mat mask = block.clone();
				for (int y = 0; y < block.rows; ++y) {
					for (int x = 0; x < block.cols; ++x) {
						if (isred(block, x, y)) {
							rcnt++;
							break;
						}
					}
					if (rcnt) break;
				}
				for (int y = 0; y < block.rows; ++y) {
					for (int x = 0; x < block.cols; ++x) {
						Vec3b cl = isred(block,x,y) ? Vec3b(255, 255, 255) : Vec3b(0, 0, 0);
						mask.at<Vec3b>(y, x) = cl;
					}
				}

				std::ostringstream ss;
				ss << std::setw(4) << std::setfill('0') << numblocks;
				string str = ss.str();
				Mat block0 = img0(rect);
				imwrite("../../blocks/train/image/" + str + ".png", block0);
				imwrite("../../blocks/train/label/" + str + ".png", mask);
				imshow("prt", block);
				waitKey(1);
				numblocks++;
			}
		}
	}
	cout << "done" << endl;
	int n;
	cin >> n;
    return 0;
}

