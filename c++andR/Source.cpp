#include "Header.h"

cv::Mat find_colorchecker(cv::Mat& find_colorchecker_img, cv::Mat& img_ref) {

	//Crop la partie haute de l'image et réduire la taille.
	find_colorchecker_img = find_colorchecker_img(cv::Rect(0, 0, find_colorchecker_img.cols, 1600));
	cv::resize(find_colorchecker_img, find_colorchecker_img, find_colorchecker_img.size() / 8);

	//Seuillage 
	cv::Mat find_colorchecker_img_gray;
	cv::cvtColor(find_colorchecker_img, find_colorchecker_img_gray, cv::COLOR_BGR2GRAY);
	cv::threshold(find_colorchecker_img_gray, find_colorchecker_img_gray, 40, 255, CV_THRESH_OTSU);
	bitwise_not(find_colorchecker_img_gray, find_colorchecker_img_gray);

	//cv::imshow("test", find_colorchecker_img_gray);
	//imwrite("thresh.jpg", find_colorchecker_img_gray);
	//cv::waitKey(0);

	//Chercher contours exterieur, puis prendre la plus grande zone.
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours(find_colorchecker_img_gray, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
	std::vector<std::vector<cv::Point>> approx(contours.size());
	cv::Mat drawing_labels = cv::Mat::zeros(find_colorchecker_img.size(), CV_8U);

	for (int i = 0; i < contours.size(); i++)
	{
		cv::drawContours(drawing_labels, contours, i, i + 1, cv::FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
	}

	//imwrite("thresh_concave.jpg", drawing_labels);

	int* class_surface = new int[size(contours)];
	for (int i = 0; i < size(contours); i++) {
		class_surface[i] = 0;
	}
	for (int i = 0; i < drawing_labels.rows; i++) {
		for (int j = 0; j < drawing_labels.rows; j++) {
			class_surface[drawing_labels.at<uchar>(i, j) - 1]++;
		}
	}
	int label_ColorChecker = 0;
	int max = 0;
	for (int i = 0; i < size(contours); i++) {
		if (max < class_surface[i]) {
			max = class_surface[i];
			label_ColorChecker = i;
		}
	}



	// Transformation
	cv::Mat ColorChecker_mask = cv::Mat::zeros(find_colorchecker_img.size(), CV_8U);
	drawContours(ColorChecker_mask, contours, label_ColorChecker, 255, cv::FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());

	/*std::cout << label_ColorChecker << std::endl;
	std::cout << class_surface[label_ColorChecker] << std::endl;
	std::cout << size(contours) << std::endl;
	cv::imshow("ColorChecker_mask", ColorChecker_mask);
	cv::imshow("drawing_labels", drawing_labels);
	cv::waitKey(0);*/
	//imwrite("colorchecker_mask.jpg", ColorChecker_mask);

	std::vector<cv::Point> locations;
	cv::findNonZero(ColorChecker_mask, locations);

	if (size(locations)<100) {
		std::cout << "Error in thresh or drawcontours" << std::endl;
		cv::Mat Error = cv::Mat::zeros(img_ref.size(),CV_8U);
		return(Error);
	}

	cv::Point2f ColorChecker_tab_points[4];
	cv::Mat ColorChecker_mat_points = cv::Mat::zeros(4, 1, CV_32FC2);
	cv::Mat Ref_points = cv::Mat::zeros(4, 1, CV_32FC2);

	Ref_points.at<cv::Point2f>(3, 0) = cv::Point2f(0, img_ref.rows);
	Ref_points.at<cv::Point2f>(0, 0) = cv::Point2f(0, 0);
	Ref_points.at<cv::Point2f>(1, 0) = cv::Point2f(img_ref.cols, 0);
	Ref_points.at<cv::Point2f>(2, 0) = cv::Point2f(img_ref.cols, img_ref.rows);

	minAreaRect(locations).points(ColorChecker_tab_points);
	float min = sqrt(pow(ColorChecker_tab_points[0].x, 2) + pow(ColorChecker_tab_points[0].y, 2));
	float dist = 0;
	int ind_min;

	for (int i = 0; i < 4; i++) {
		dist = sqrt(pow(ColorChecker_tab_points[i].y, 2) + pow(img_ref.rows - ColorChecker_tab_points[i].x, 2));
		if (dist < min) {
			ind_min = i;
			min = dist;
		}
	}

	/*std::cout << Ref_points << std::endl;
	std::cout << ColorChecker_mat_points << std::endl;
	std::cout << ColorChecker_tab_points[0] << " " << ColorChecker_tab_points[1] << ColorChecker_tab_points[2] << " " << ColorChecker_tab_points[3] << std::endl;
	std::cout << ind_min << " " << (ind_min + 1) % 4 << (ind_min + 2) % 4 << " " <<(ind_min + 3) % 4 << std::endl;
	system("PAUSE");*/

	ColorChecker_mat_points.at<cv::Point2f>(0, 0) = ColorChecker_tab_points[ind_min];
	ColorChecker_mat_points.at<cv::Point2f>(1, 0) = ColorChecker_tab_points[(ind_min + 1) % 4];
	ColorChecker_mat_points.at<cv::Point2f>(2, 0) = ColorChecker_tab_points[(ind_min + 2) % 4];
	ColorChecker_mat_points.at<cv::Point2f>(3, 0) = ColorChecker_tab_points[(ind_min + 3) % 4];

	cv::Mat homography;
	homography = cv::findHomography(ColorChecker_mat_points, Ref_points);

	cv::Mat result;
	cv::warpPerspective(find_colorchecker_img, result, homography, img_ref.size());

	/*cv::imshow("registration", result);
	cv::waitKey();*/

	return(result);
}

std::vector<cv::Scalar> extract_color(cv::Mat& colorchecker, cv::Mat& position, cv::Mat conteneur) {
	int size_square_sample = 3;
	int size_square_conteneur = conteneur.rows/position.rows;
	cv::Point2f square_center_sample;
	cv::Rect square_sample;
	cv::Scalar mean_sample;
	std::vector<cv::Scalar> result;

	for (int i = 0; i < position.rows; i++) {
		for (int j = 0; j < position.cols; j++) {
			square_center_sample = position.at<cv::Point2f>(i, j);
			square_sample = cv::Rect(square_center_sample.y - size_square_sample, square_center_sample.x - size_square_sample, size_square_sample, size_square_sample);
			mean_sample = cv::mean(colorchecker(square_sample));
			result.push_back(mean_sample);
			cv::Mat mean_sample_square(size_square_conteneur, size_square_conteneur, CV_8UC3, mean_sample);
			mean_sample_square.copyTo(conteneur(cv::Rect(j*size_square_conteneur, i*size_square_conteneur, size_square_conteneur, size_square_conteneur)));
		}
	}

	return result;
}

void write_summary_csv(std::string filename, std::vector<cv::Scalar> color_list) {
	std::ofstream outputfile;
	outputfile.open(filename);
	outputfile << "B" << ";" << "G" << ";" << "R" << std::endl;
	for (int i = 0; i < size(color_list); i++) {
		outputfile << color_list[i][0] << ";" << color_list[i][1] << ";" << color_list[i][2] << std::endl;
	}
	outputfile.close();
}

