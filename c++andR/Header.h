#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/features2d.hpp"

cv::Mat find_colorchecker(cv::Mat& find_colorchecker_img, cv::Mat& img_ref);
std::vector<cv::Scalar> extract_color(cv::Mat& colorchecker, cv::Mat& position, cv::Mat conteneur);
void write_summary_csv(std::string filename, std::vector<cv::Scalar> color_list);
bool is_empty(std::ifstream& pFile);