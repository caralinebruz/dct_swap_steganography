#include <iostream>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/algorithm/string.hpp>
#include "format.h"
#include "helpers.hpp"
// #include "lsb.hpp"
// #include "lsb_alt.hpp"
#include "dct.hpp"
// #include "dwt.hpp"
// #include "tlv.hpp"

#if _WIN32
	#include <conio.h>
#else
	#define _getche getchar
#endif

using namespace cv;
using namespace std;
using namespace boost;

// bool v_verbose = true;
bool z_verbose = false;


/*!
 * Runs the discrete cosine transformation method.
 *
 * \param input Path to original image.
 * \param secret Path to the data to be hidden.
 * \param store Storage mode.
 * \param channel Channels to encode.
 * \param persistence Persistence value.
 * \param compression JPEG compression percentage.
 */
void do_dct(const string& input, const string& secret, int store, int channel, int persistence, int compression)
{
	// the image file we will perform DCT on
	auto img = imread(input);

	if (!img.data)
	{
		cerr << endl << "  " << Format::Red << Format::Bold << "Error:" << Format::Normal << Format::Default << " Failed to open input image from '" << input << "'." << endl << endl;
		return;
	}

	// show_image(img, "Original");

	// read the secret data to hide in image
	auto data = read_file(secret);

	Mat stego;

	if (channel == 0)
	{
		stego = encode_dct(img,   data, store, 0, persistence);
		stego = encode_dct(stego, data, store, 1, persistence);
		stego = encode_dct(stego, data, store, 2, persistence);
	}
	else
	{
		stego = encode_dct(img, data, store, channel - 1, persistence);
	}

	auto altered = remove_extension(input) + ".dct.jpg";

	imwrite(altered, stego, vector<int> { IMWRITE_JPEG_QUALITY, compression });

	cout << endl << "  " << Format::Green << Format::Bold << "Success:" << Format::Normal << Format::Default << " Altered image written to '" << altered << "'." << endl;

	stego = imread(altered);

	// string output;

	// if (channel == 0)
	// {
	// 	output = repair(vector<string>
	// 		{
	// 			decode_dct(stego, 0),
	// 			decode_dct(stego, 1),
	// 			decode_dct(stego, 2)
	// 		});
	// }
	// else
	// {
	// 	output = decode_dct(stego, channel - 1);
	// }

	// print_debug(data, output);
	// show_image(stego, "Altered");
}


int main(int argc, char **argv) {

	//  ./iosched [ –s<schedalgo> | -v | -q | -f ] <inputfile>
	//  ./iosched -si -vqf lab4_assign/input0

	// ./steganography test/lena.jpg test/test.tx

	// parsing the flags, arguments
	int s;
	// opterr = 0;

	while (( s = getopt (argc, argv, "s:u:q:f:z:v:y:" )) != -1)
		switch (s) {
			case 'z':
				//personal logging arg
				printf("something");
				break;

			case 'v':
				// execution trace flag
				printf("something");
				break;

			case 's':
				// algo
				if (!(optarg==NULL)) {
					char letter = optarg[0];
					switch(letter) {
						case 'i':
							// FIFO
							if (z_verbose) {printf("FIFO \n");}
							printf("something");

							break;
						case 'j':
							// SSTF
							if (z_verbose) {printf("SSTF \n");}
							printf("something");

							break;
						case 's':
							// LOOK
							if (z_verbose) {printf("LOOK \n");}
							printf("something");

							break;
						case 'c':
							// C-LOOK
							if (z_verbose) {printf("C-LOOK \n");}
							printf("something");

							break;
						case 'f':
							// F-LOOK
							if (z_verbose) {printf("F-LOOK \n");}
							printf("something");
							// ioscheduler->init_deques();

							break;
					}
				}
				break;

			case 'f':
				// F-LOOK additional queue info
				printf("something");
				break;

			case 'q':
				// io queue details (+ direction of movement)
				printf("something");
				break;

			case 'y':
				// io queue details (+ direction of movement)
				printf("something");
				break;

			case 'u':
				// wow ok the letter V is not working for some reason..... annoying.
				printf("something");
				break;

			case '?':
				if (optopt == 's')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
				return 1;
			default:
				abort ();
		}


	// parsing the files at the end of args
	string inputfile;
	string secretfile;
	for (int i = 0; i < argc; i++) {
		// printf("%s \n", argv[i]);
		string arg = argv[i];       
		// final argument
		if (argc - i == 1) {
			secretfile = arg;
		}
		// second to last argument
		if (argc - i == 2) {
			inputfile = arg;
		}
	}

	printf("input file: %s\n second file: %s\n\n", inputfile.c_str(), secretfile.c_str());

	// auto store = STORE_FULL, channel = 0, persistence = 30, compression = 80;
	// //do_dct(const string& input, const string& secret, int store, int channel, int persistence, int compression)
	// do_dct(inputfile, secretfile, store, channel, persistence, compression);
	// waitKey();



	string window_original="original_image";
	string window_gray="gray_image";
	Mat img = imread(inputfile);//It returns a matrix object


	// to use some of these operations the ohoto has to be
	// perfectly square, FYI
	if (img.rows != img.cols) {
		printf("\nThis image is not square!!!! Must be square. \n");
	}
	cout << img.rows << "\n";
	cout << img.cols << "\n";


	// Mat graymat;

	// cvtC2olor(img, graymat, COLOR_BGR2GRAY);

	// namedWindow(window_original,WINDOW_AUTOSIZE);
	// imshow(window_original,img);

	// namedWindow(window_gray,WINDOW_AUTOSIZE);
	// imshow(window_gray,graymat);

	// waitKey(0);

	string window_Y="luminance";
	string window_Cr="red chrominance";
	string window_Cb="blue chrominance";
	Mat ycbcr;
	cvtColor(img, ycbcr, COLOR_BGR2YCrCb);

	// split channels
	Mat channels[3];




	split(ycbcr, channels);//splitting images into 3 different channels//  
	Mat y = channels[0];//loading blue channels//
	Mat cr = channels[1];//loading green channels//
	Mat cb = channels[2];//loading red channels// 


	// do work
	// Mat ycrcb_merged;

	vector<Mat> y_merged;
	vector<Mat> cr_merged;
	vector<Mat> cb_merged;
	// vector<Mat> ycrcb_merged; for putting together at end


	// Just create two empty matrix of the same size for Blue and Green
	// Mat g = Mat::zeros(Size(img.rows, img.cols), CV_8UC1);

	Mat g = Mat(Size(img.rows, img.cols), CV_8UC1, 128);


	// luminance
	y_merged.push_back(y);
	y_merged.push_back(y);
	y_merged.push_back(y);

	Mat y_fin_img;
	merge(y_merged, y_fin_img);


	// red chrominance
	cr_merged.push_back(g);
	cr_merged.push_back(g);
	cr_merged.push_back(cr);

	Mat cr_fin_img;
	merge(cr_merged, cr_fin_img);


	// blue chrominance
	cb_merged.push_back(cb);
	cb_merged.push_back(g);
	cb_merged.push_back(g);

	Mat cb_fin_img;
	merge(cb_merged, cb_fin_img);

	
	imshow("luminance", y_fin_img);
	imshow("red chrominance 1", cr_fin_img);
	imshow("blue chrominance", cb_fin_img); // correct order for merge


	waitKey(0);
	destroyAllWindows();//closing all windows//
	return 0;
}





