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


Mat merge_y(Mat y, Mat g) {
	vector<Mat> y_merged;
	Mat y_fin_img;

	// luminance
	y_merged.push_back(y);
	y_merged.push_back(y);
	y_merged.push_back(y);

	merge(y_merged, y_fin_img);

	return y_fin_img;
}

Mat merge_cr(Mat cr, Mat g) {
	vector<Mat> cr_merged;
	Mat cr_fin_img;

	// // midgray
	// Mat g = Mat(Size(img.rows, img.cols), CV_8UC1, 128);

	// red chrominance
	cr_merged.push_back(g);
	cr_merged.push_back(g);
	cr_merged.push_back(cr);

	merge(cr_merged, cr_fin_img);

	return cr_fin_img;
}

Mat merge_cb(Mat cb, Mat g) {
	vector<Mat> cb_merged;
	Mat cb_fin_img;

	// // midgray
	// Mat g = Mat(Size(img.rows, img.cols), CV_8UC1, 128);

	// blue chrominance
	cb_merged.push_back(cb);
	cb_merged.push_back(g);
	cb_merged.push_back(g);

	merge(cb_merged, cb_fin_img);

	return cb_fin_img;
}

void display_split_channels(Mat y, Mat cr, Mat cb, Mat g) {
	// display 
	// unaltered image channels

	Mat c_y = merge_y(y,g);
	Mat c_cr = merge_cr(cr,g);
	Mat c_cb = merge_cb(cb,g);

	imshow("luminance before", c_y);
	imshow("red chrominance before", c_cr);
	imshow("blue chrominance before", c_cb);

	waitKey(0);
	destroyAllWindows();//closing all windows//
}

void display_before_after(Mat y, Mat cr, Mat cb, Mat g, Mat stega_y, Mat stega_cr, Mat stega_cb) {
	// display
	// altered and unaltered image channels

	Mat c_y = merge_y(y,g);
	Mat c_cr = merge_cr(cr,g);
	Mat c_cb = merge_cb(cb,g);

	Mat c_stega_y = merge_y(stega_y,g);
	Mat c_stega_cr = merge_cr(stega_cr,g);
	Mat c_stega_cb = merge_cb(stega_cb,g);


	// imwrite("out/luminance.jpg", y, vector<int> { IMWRITE_JPEG_QUALITY, 99 });
	// imwrite("out/luminance_dct.jpg", stega_y, vector<int> { IMWRITE_JPEG_QUALITY, 99 });


	imshow("luminance before", c_y);
	imshow("red chrominance before", c_cr);
	imshow("blue chrominance before", c_cb);

	imshow("luminance after", c_stega_y);
	imshow("red chrominance after", c_stega_cr);
	imshow("blue chrominance after", c_stega_cb);

	waitKey(0);
	destroyAllWindows();//closing all windows//

}


void write_original_channels(Mat y, Mat cr, Mat cb, Mat g) {
	// display
	// unaltered image channels
	Mat c_y = merge_y(y,g);
	Mat c_cr = merge_cr(cr,g);
	Mat c_cb = merge_cb(cb,g);

	imwrite("out/lena_orig_luminance.jpg", c_y, vector<int> { IMWRITE_JPEG_QUALITY, 99 });
	imwrite("out/lena_orig_cr.jpg", c_cr, vector<int> { IMWRITE_JPEG_QUALITY, 99 });
	imwrite("out/lena_orig_cb.jpg", c_cb, vector<int> { IMWRITE_JPEG_QUALITY, 99 });
	cout << endl << "  " << Format::Green << Format::Bold << "Success:" << Format::Normal << Format::Default << " Altered image written." << endl;

}



int do_stuff(const string& inputfile, string secretfile, int channel) {
	// parse input image 
	// encode one (or all) of the channels
	// display encoded images vs original
	Mat img = imread(inputfile); //It returns a matrix object


	// to use some of these operations the ohoto has to be
	// perfectly square, FYI
	if (img.rows != img.cols) {
		printf("\nThis image is not square!!!! Must be square. \n");
	}
	cout << img.rows << "\n";
	cout << img.cols << "\n";

	// 1a. convert the color scheme to YCrCb
	Mat ycbcr;
	cvtColor(img, ycbcr, COLOR_BGR2YCrCb);

	// // 1b. split channels
	Mat channels[3];
	split(ycbcr, channels);//splitting images into 3 different channels//  
	Mat y = channels[0];
	Mat cr = channels[1];
	Mat cb = channels[2];
	// do work

	// based on the channel they selected, encode that channel
	// since im testing ill start with a single channel
	string secret = read_file(secretfile);
	printf("string: %s \n", secret.c_str());

	// try to convert the string to binary first
	// string binstring = TextToBinaryString(secret);
	// vector<int> binstring = strToBinary(secret);


	////https://www.delftstack.com/howto/cpp/convert-string-to-binary-in-cpp/
	// first COUT binary string to text file
	// confirm the text file can be converted back, and is correct. via web:
	// https://www.rapidtables.com/convert/number/binary-to-ascii.html
	string s = "Hi";
	convert_to_binary(s);

	// this writes to a file the binary string
	text_to_binary_1(s);
	// now, read the file.
	string binfile = ("./test/example_binary.txt");
	string binst = read_file(binfile);
	string response = setStringtoASCII(binst);
	printf("\n\nTESTING\n\n");
	cout << response;


	// char q = strToChar_1();

	vector<int> binstring = TextToBinaryString(secret);

	// printf("string binary: %s \n", binstring.c_str());

	printf("binary string:\n");
	for (auto i = binstring.begin(); i != binstring.end(); ++i){
	    // std::cout << *i << ' ';
	    printf("%d",*i);
	}
	printf(".\n");


	// STORE_ONCE   1 = Stores the specified input once.
	// STORE_FULL   2 = Stores the specified input and fills the rest of the available space with zeros.
	// STORE_REPEAT 3 = Stores the specified input in a repeating manner.


	// persistence percentage
	//		param intensity Persistence of the hidden data.

	// compression percentage

	//auto store = STORE_REPEAT, persistence = 30;
	auto store = STORE_REPEAT, persistence = 30;



	// you pass in the entire ycbcr image and the channel
	Mat stega;
	stega = encode_dct(ycbcr, binstring, store, channel, persistence);
	printf("done encoding.");

	// now, convert it back to bgr
	Mat bgr_img;
	cvtColor(stega, bgr_img, COLOR_YCrCb2BGR);



	// now that you've encoded the data. 
	// see if you can read it back out from the photo ok.
	auto altered = remove_extension(inputfile) + ".dct.jpg";
	imwrite(altered, bgr_img, vector<int> { IMWRITE_JPEG_QUALITY, 90 });
	cout << endl << "  " << Format::Green << Format::Bold << "Success:" << Format::Normal << Format::Default << " Altered image written to '" << altered << "'." << endl;


	printf("starting decode of %s...", altered.c_str());
	Mat alter_img = imread(altered);
	// take the altered image and split it into ycbcr again
	Mat o_ycrcb;
	cvtColor(alter_img, o_ycrcb, COLOR_BGR2YCrCb);
	string output;


	//output = decode_dct(o_ycrcb, channel);
	output = decode_dct(stega, channel);

	printf("string out : %s", output.c_str());


    // cout << setStringtoASCII(output);
    string ascii = setStringtoASCII(output);

	// displays

	// // split channels after processing
	// Mat stego_channels[3];
	// split(stego, stego_channels); 
	// Mat stego_y = stego_channels[0];
	// Mat stego_cr = stego_channels[1];
	// Mat stego_cb = stego_channels[2];

	// // create a midgray to use to display chrominance channels nicely
	Mat g = Mat(Size(img.rows, img.cols), CV_8UC1, 128);


	// // display unaltered image in each color channel.
	// //display_split_channels(y, cr, cb, g);
	write_original_channels(y,cr,cb,g);

	// display_before_after(y, cr, cb, g, stego_y, stego_cr, stego_cb);



	return 0;
}


int main(int argc, char **argv) {

	//  ./stega -c[y|r|b] [-d | -l] <photo_file> <secret_file>
	// ./stega -d -cy test/orange.jpeg test/test.txt
	// ./stega -d -cy test/lena.jpg test/test.txt

	int args;
	int encode_channel;

	while (( args = getopt (argc, argv, "ldc:" )) != -1)
		switch (args) {

			case 'c':
				// which channel to encode
				if (!(optarg==NULL)) {

					char letter = optarg[0];
					switch(letter) {

						case 'y':
							// Luminance
							encode_channel = 0;
							printf("encode luminance channel\n");
							break;

						case 'r':
							// Red Chrominance
							encode_channel = 1;
							printf("encode red chrominance channel\n");
							break;

						case 'b':
							// Blue Chrominance
							encode_channel = 2;
							printf("encode blue chrominance channel\n");
							break;
					}
				}
				break;

			case 'd':
				// Discrete Cosine Transform
				printf("use DCT\n");
				break;
			case 'l':
				// Least Significant Bit
				printf("use LSB\n");
				break;

			case '?':
				if (optopt == 'c')
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

	int resp = do_stuff(inputfile, secretfile, encode_channel);

	return 0;
}





