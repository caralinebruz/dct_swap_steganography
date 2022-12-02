#pragma once
#include <opencv2/core/core.hpp>

// basic file operations
#include <iostream>
#include <fstream>

/*!
 * Uses discrete cosine transformation to hide data in the coefficients of a channel of an image.
 *
 * \param img Input image.
 * \param text Text to hide.
 * \param mode Storage mode, see STORE_* constants.
 * \param channel Channel to manipulate.
 * \param intensity Persistence of the hidden data.
 *
 * \return Altered image with hidden data.
 */
inline cv::Mat encode_dct(const cv::Mat& img, const std::string& text, int mode = STORE_FULL, int channel = 0, int intensity = 30)
{
	using namespace cv;
	using namespace std;

	auto block_width  = 8;
	auto block_height = 8;
	auto grid_width   = img.cols / block_width;
	auto grid_height  = img.rows / block_height;

	auto i = 0;
	auto size = text.length() * 8;

	Mat imgfp;

	// CV_32F - 32-bit floating-point numbers
	img.convertTo(imgfp, CV_32F);

	vector<Mat> planes;
	split(imgfp, planes);

	cout << "img cols = " << endl << " "  << img.cols << endl << endl;
	cout << "img rows = " << endl << " "  << img.rows << endl << endl;

	cout << "Grid Width = " << endl << " "  << grid_width << endl << endl;
	cout << "Grid Height = " << endl << " "  << grid_height << endl << endl;


	// lena is 512 x 512
	// img.cols = 512
	// img.rows = 512

	// grid width = 64
	// grid height = 64


	for (int x = 1; x < grid_width; x++)
	{
		for (int y = 1; y < grid_height; y++)
		{

			// cout << "x = " << endl << " "  << x << endl << endl;
			// cout << "y = " << endl << " "  << y << endl << endl;

			auto px = (x - 1) * block_width;
			auto py = (y - 1) * block_height;

			Mat block(planes[channel], Rect(px, py, block_width, block_height));
			Mat trans(Size(block_width, block_height), block.type());

			// https://docs.opencv.org/4.x/d2/de8/group__core__array.html#ga85aad4d668c01fbd64825f589e3696d4
			// dct(input array, output array)
			dct(block, trans);

			auto a = trans.at<float>(6, 7);
			auto b = trans.at<float>(5, 1);


			if ( (x <= 2) && (y <= 2) ) {
				// print it.
				ofstream myfile;
				myfile.open("./out/example.txt");
				myfile << "Writing this to a file.\n";
				myfile << "M = ";
				myfile << trans;
				myfile.close();

				cout << "M = " << endl << " "  << trans << endl << endl;
				cout << "a = " << endl << " "  << a << endl << endl;
				cout << "b = " << endl << " "  << b << endl << endl;

			}



			if (i >= size)
			{
				if (mode == STORE_ONCE)
				{
					break;
				}
				else if (mode == STORE_REPEAT)
				{
					i = 0;
				}
			}

			auto val = 0;
			if (i < size)
			{
				val = (text[i / 8] & 1 << i % 8) >> i % 8;
				i++;
			}

			if (val == 0)
			{
				if (a > b)
				{
					swap(a, b);
				}
			}
			else
			{
				if (a < b)
				{
					swap(a, b);
				}
			}

			if (a > b)
			{
				auto d = (intensity - (a - b)) / 2;
				     a = a + d;
				     b = b - d;
			}
			else
			{
				auto d = (intensity - (b - a)) / 2;
				     a = a - d;
				     b = b + d;
			}

			trans.at<float>(6, 7) = a;
			trans.at<float>(5, 1) = b;

			Mat stego(Size(block_width, block_height), block.type());

			idct(trans, stego);

			stego.copyTo(planes[channel](Rect(px, py, block_width, block_height)));
		}

		if (i >= size && mode == STORE_ONCE)
		{
			break;
		}
	}

	Mat mergedfp;
	merge(planes, mergedfp);

	Mat merged;
	mergedfp.convertTo(merged, CV_8U);

	return merged;
}

/*!
 * Uses discrete cosine transformation to recover data hidden in the coefficients of an image.
 *
 * \param img Input image with hidden data.
 * \param channel Channel to manipulate.
 *
 * \return Hidden data extracted form image.
 */
inline std::string decode_dct(const cv::Mat& img, int channel = 0)
{
	using namespace cv;
	using namespace std;

	auto block_width  = 8;
	auto block_height = 8;
	auto grid_width   = img.cols / block_width;
	auto grid_height  = img.rows / block_height;

	auto i = 0;
	string bits(grid_width * grid_height / 8, 0);

	Mat imgfp;
	img.convertTo(imgfp, CV_32F);

	vector<Mat> planes;
	split(imgfp, planes);

	for (int x = 1; x < grid_width; x++)
	{
		for (int y = 1; y < grid_height; y++)
		{
			auto px = (x - 1) * block_width;
			auto py = (y - 1) * block_height;

			Mat block(planes[channel], Rect(px, py, block_width, block_height));
			Mat trans(Size(block_width, block_height), block.type());

			dct(block, trans);

			auto a = trans.at<float>(6, 7);
			auto b = trans.at<float>(5, 1);

			if (a > b)
			{
				bits[i / 8] |= 1 << i % 8;
			}

			i++;
		}
	}

	return bits;
}
