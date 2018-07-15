#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
using namespace std;
using namespace cv;



int main(int, char** argv)
{
	// Load the image
	Mat src = imread(argv[1],CV_LOAD_IMAGE_COLOR);
	// Check if image is loaded fine
	if (!src.data) 
		cerr << "Problem loading image!!!" << endl;
	// Show source image
	namedWindow("src", WINDOW_NORMAL);
	//resize(src, src, Size(800,500));
	imshow("src", src);
	// Transform source image to gray if it is not
	Mat gray;
	if (src.channels() == 3)
	{
		cvtColor(src, gray, CV_BGR2GRAY);
	}
	else
	{
		gray = src;
	}

	printf("width %d", src.cols);
	printf("height %d", src.rows);
	// Show gray image
	namedWindow("gray", WINDOW_NORMAL);
	imshow("gray", gray);
	// Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
	Mat bw;
	Mat bw1;
	adaptiveThreshold(~gray, bw, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -30);
	
	
	
	// Show binary image
	namedWindow("binary", WINDOW_NORMAL);
	imshow("binary", bw);
	
/*=========================================================================================*/
// Step 1
	Mat edges;
	adaptiveThreshold(bw, edges, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
	namedWindow("edges", WINDOW_NORMAL);
	imshow("edges", edges);
	// Step 2
	Mat kernel = Mat::ones(2, 2, CV_8UC1);
	dilate(edges, edges, kernel);
	
	namedWindow("dialate", WINDOW_NORMAL);
	imshow("dialate", bw);
	// Step 3
	Mat smooth;
	bw.copyTo(smooth);
	// Step 4
	blur(smooth, smooth, Size(1, 1));
	// Step 5
	smooth.copyTo(bw, edges);

	namedWindow("smooth", WINDOW_NORMAL);
	imshow("smooth",bw);
	

	/*=========================================================================================*/

	bitwise_not(bw, bw1);
	namedWindow("bit_inverse", WINDOW_NORMAL);
	imshow("bit_inverse", bw1);
	// Create the images that will use to extract the horizontal and vertical lines
	Mat horizontal = bw.clone();
	Mat vertical = bw.clone();
	// Specify size on horizontal axis
	int horizontalsize = horizontal.cols / 30;
	// Create structure element for extracting horizontal lines through morphology operations
	Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize, 1));
	// Apply morphology operations
	erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
	dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
	// Show extracted horizontal lines
	Mat horizontal_remove;
	namedWindow("horizontal", WINDOW_NORMAL);
	imshow("horizontal", horizontal);
	namedWindow("horizontal_remove", WINDOW_NORMAL);
	//horizontal_remove =bw - horizontal;
	absdiff(bw, horizontal, horizontal_remove);
	//blur(horizontal_remove, horizontal_remove, Size(2, 2));
	imshow("horizontal_remove", horizontal_remove);

	/*=========================================================================================
	Mat edges2;
	//Mat erodes;
	adaptiveThreshold(horizontal_remove, edges2, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
	namedWindow("edges2", WINDOW_NORMAL);
	imshow("edges2", edges2);
	// Step 2
	Mat kernel2 = Mat::ones(2, 2, CV_8UC1);
	erode(edges2, edges2, kernel2);
	namedWindow("erodes", WINDOW_NORMAL);
	imshow("horizontal_remove_erodes", edges2);


	=========================================================================================*/




	// Specify size on vertical axis
	int verticalsize = vertical.rows / 30;
	// Create structure element for extracting vertical lines through morphology operations
	Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, verticalsize));
	// Apply morphology operations
	erode(vertical, vertical, verticalStructure, Point(-1, -1));
	dilate(vertical, vertical, verticalStructure, Point(-1, -1));
	// Show extracted vertical lines
	namedWindow("vertical", WINDOW_NORMAL);
	imshow("vertical", vertical);
	Mat vertical_remove;
	subtract(bw ,vertical, vertical_remove);
	//blur(vertical_remove, vertical_remove, Size(2, 2));
	namedWindow("vertical_remove", WINDOW_NORMAL);
	imshow("vertical_remove",vertical_remove);

	Mat final_image_vertical;
	Mat final_image;
	absdiff(bw, vertical, final_image_vertical);
	absdiff(final_image_vertical, horizontal, final_image);
	//blur(final_image, final_image, Size(4, 4));
	namedWindow("final_image", WINDOW_NORMAL);
	imshow("final_image", final_image);
	Mat final_gates;
	bitwise_not(final_image, final_gates);
	namedWindow("final_inverse", WINDOW_NORMAL);
	imshow("final_inverse", final_gates);
	
	
	

	//horizontal.empty();
	//edges.copyTo(horizontal);

	//namedWindow("smooth", WINDOW_NORMAL);
	//imshow("smooth",horizontal);
	



	

	waitKey(0);
	return 0;
}