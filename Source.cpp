#pragma once
#include "Header.h"



using namespace std;
using namespace cv;


bool leftDown = false, leftup = false;
cv::Mat img;
cv::Point cor1, cor2;
cv::Rect box;
ofstream myfile;

void mouse_call(int event, int x, int y, int, void*)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		leftDown = true;
		cor1.x = x;
		cor1.y = y;
		cout << "Corner 1: " << cor1 << endl;
		//myfile << x << ":" << y << ":";
	}
	if (event == EVENT_LBUTTONUP)
	{
		if (abs(x - cor1.x) > 20 && abs(y - cor1.y) > 20) //checking whether the region is too small
		{
			leftup = true;
			cor2.x = x;
			cor2.y = y;
			cout << x << ":" << y << ":" << cor2.x << ":" << cor2.y <<endl;
			myfile << x << ":" << y << ":" << cor2.x << ":" << cor2.y << ",";
		}
		else
		{
			cout << "Select a region more than 20 pixels" << endl;
		}
	}

	if (leftDown == true && leftup == false) //when the left button is down
	{
		Point pt;
		pt.x = x;
		pt.y = y;
		Mat temp_img = img.clone();
		rectangle(temp_img, cor1, pt, Scalar(255, 0, 0)); //drawing a rectangle continuously
		imshow("Original", temp_img);
		
	}
	if (leftDown == true && leftup == true) //when the selection is done
	{
		try {
			box.width = abs(cor1.x - cor2.x);
			box.height = abs(cor1.y - cor2.y);
			box.x = min(cor1.x, cor2.x);
			box.y = min(cor1.y, cor2.y);
			cout << " box.width-" << box.width << "  box.height - " << box.height << "  box.x -" << box.x << "  box.y- ";
			Mat crop(img, box); //Selecting a ROI(region of interest) from the original pic
			namedWindow("Cropped Image");
			imshow("Cropped Image", crop); //showing the cropped image
			leftDown = false;
			leftup = false;
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;

		}
		

		
		
	}

}

int mymain(std::string filename)
{
	try {
		img = imread(filename);
		namedWindow("Original", WINDOW_GUI_NORMAL);
		imshow("Original", img);

		setMouseCallback("Original", mouse_call); //setting the mouse callback for selecting the region with mouse
		myfile.open("example.csv", std::ios_base::app);
		
		//Give column names according to many region you want to select. 
		// myfile - filename which is selected

		myfile << "FILENAME" << "," << "OUTPUT2" << "," << "OUTPUT3" << "," << "OUTPUT4" << "," << "Output5" << "," << "Output6\n";
		myfile << "\n";
		myfile << filename << ",";
		while (char(waitKey(0) != 'q')) //waiting for the 'q' key to finish the execution
		{

		}
		destroyWindow("Original");
		destroyWindow("Cropped Image");
		
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		
	}finally {
		if (myfile) {
			myfile.close();
		}
	}
	
	return 0;
}
