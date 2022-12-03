#pragma once
#include <fstream>
#include <bitset>
#include <vector>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

/*!
 * Stores the specified input once.
 */
#define STORE_ONCE   1

/*!
 * Stores the specified input and fills the rest of the available space with zeros.
 */
#define STORE_FULL   2

/*!
 * Stores the specified input in a repeating manner.
 */
#define STORE_REPEAT 3

/*!
 * Returns the similarity between the original message and extracted message.
 *
 * \param original Original data.
 * \param extracted Extracted data.
 *
 * \return Similarity in percentages.
 */
inline float similarity(const std::string& original, const std::string& extracted)
{
	auto hits = 0;

	for (int i = 0; i < fmin(original.length(), extracted.length()); i++)
	{
		if (original[i] == extracted[i])
		{
			hits++;
		}
	}

	return float(hits) / original.length() * 100;
}

// Function to convert binary to decimal
int binaryToDecimal(std::string n)
{
	using namespace std;
    string num = n;
 
    // Stores the decimal value
    int dec_value = 0;
 
    // Initializing base value to 1
    int base = 1;
 
    int len = num.length();
    for (int i = len - 1; i >= 0; i--) {
 
        // If the current bit is 1
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }
 
    // Return answer
    return dec_value;
}

std::string setStringtoASCII(std::string str)
{
	using namespace std;
    // To store size of s
    int N = int(str.size());
 
    // If given string is not a
    // valid string
    if (N % 8 != 0) {
        printf("length is not divisible by 8! truncate string to decode...");
    }

    int remainder = N % 8;
    // remove that many chars from the end of the string
    for (int i = 0; i < remainder; i++) {
    	str.pop_back();
    }

    int Newlen = int(str.size());
    //printf("length of new string: %d\n", Newlen);
    

 
    // To store final answer
    string res = "";
 
    // Loop to iterate through string
    for (int i = 0; i < N; i += 8) {
        int decimal_value = binaryToDecimal((str.substr(i, 8)));

        // printf("this decimal: %s \n", str.substr(i, 8).c_str());
 
        // Apprend the ASCII character
        // equivalent to current value
        // res += char(decimal_value);
        char letter = char(decimal_value);

        // printf("%c \n", letter);
        res.push_back(letter);
    }
 
    // Return Answer
    return res;
}

// std::vector<int> strToBinary(std::string s)
// {
// 	std::vector<int> stringvals;

//     int n = s.length(); 
//  	std::string bin = "";

//     for (int i = 0; i <= n; i++)
//     {
//         // convert each char to
//         // ASCII value
//         int val = int(s[i]);
 
//         // Convert ASCII value to binary
//         // std::string bin = "";
//         while (val > 0)
//         {
//             // (val % 2)? bin.push_back('1') :
//             //            bin.push_back('0');
// 			(val % 2)? stringvals.push_back(1):
// 						stringvals.push_back(0);


//             val /= 2;
//         }
//         //reverse(bin.begin(), bin.end());
//         std::reverse(stringvals.begin(),stringvals.end());
 
//         // cout << bin << " ";
//     }
//     return stringvals;
//}


std::vector<int> TextToBinaryString(std::string words) {
	// this is messed up and some letters dont convert correctly. oh well.
	using namespace std;

	int N = words.size();
	vector<int> stringV;


	for (int i = 0; i < N; i++) {
		char letter = words[i];
		//printf("Letter: %c \n", letter);

		string newstr = bitset<8>(letter).to_string();
		//printf("newstring : %s\n" , newstr.c_str());

		int mybit = static_cast<int>(bitset<8>(newstr).to_ulong());

		// for this 8 bits, add each to the vector
		for (int x = 0; x < 8; x++) {

			char number = newstr[x];
			// printf("\tsub %c", number);

			// int mybit_int;
			// mybit_int = (int)(number.to_ulong());
			// printf("my bit %d\n", mybit_int);

		}
		// printf("\n");
		
	}

    string binaryString = "";
    for (char& _char : words) {
        binaryString +=bitset<8>(_char).to_string();
    }
    printf("binary string: %s\n", binaryString.c_str());


    std::vector<int> stringvals;

	for(string::size_type i = 0; i < binaryString.size(); ++i) {

		int val = int(binaryString[i]);
		// printf("%d ", val);

		if (val % 2 == 0) {
			stringvals.push_back(0);
		}
		else {
			stringvals.push_back(1);
		}
	}

    return stringvals;
}






/*!
 * Tries to recover the original string by comparing multiple extracted data
 * from multiple channels or methods.
 *
 * \param texts List of the same string extracted from different channels/methods.
 *
 * \return Recovered string.
 */
inline std::string repair(const std::vector<std::string>& texts)
{
	using namespace std;

	auto longest = max_element(texts.begin(), texts.end(), [](auto a, auto b) { return a.size() < b.size(); })->size();
	string result(longest, 0);

	for (int i = 0; i < longest; i++)
	{
		unordered_map<char, uchar> freq;

		for (int j = 0; j < texts.size(); j++)
		{
			if (texts[j].size() <= i)
			{
				continue;
			}

			freq[texts[j][i]]++;
		}

		auto frequent = max_element(freq.begin(), freq.end(), [](auto a, auto b) { return a.second < b.second; })->first;

		result[i] = frequent;
	}

	return result;
}

/*!
 * Reads the specified file into a string.
 *
 * \param file Path to the file.
 *
 * \return Contents of the file.
 */
inline std::string read_file(const std::string& file)
{
	std::ifstream fs(file);
	std::string text((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
	fs.close();
	return text;
}

/*!
 * Provides a primitive way to remove the file extension.
 *
 * \param file File name or path.
 *
 * \param File name or path without extension.
 */
inline std::string remove_extension(const std::string& file)
{
	auto dot = file.find_last_of(".");

	if (dot == std::string::npos)
	{
		return file;
	}

	return file.substr(0, dot);
}

/*!
 * Removes non-printable characters from the input and trims any leading or trailing whitespace.
 *
 * \param text Text to be cleaned.
 *
 * \return Cleaned text.
 */
inline std::string clean(const std::string& text)
{
	auto copy(text);
	copy.erase(remove_if(copy.begin(), copy.end(), [](char c)
		{
			static auto l = boost::is_print(std::locale());
			return !l(c) && c != '\r' && c != '\n';
		}), copy.end());
	boost::trim(copy);
	return copy;
}

/*!
 * Displays the histogram of the specified image.
 *
 * \param mat Input image.
 * \param title Name of the window.
 */
inline void showHistogram(const cv::Mat& mat, const char* title = "Histogram")
{
	using namespace cv;
	using namespace std;

	Mat b_hist, g_hist, r_hist;

	vector<Mat> bgrs;
	split(mat, bgrs);

	auto size   = 256;
	auto hist_w = 512;
	auto hist_h = 400;
	auto bin_w  = cvRound(double(hist_w) / size);

	float range[] = { 0, 256 };
	const float* histRange = { range };

	calcHist(&bgrs[0], 1, 0, Mat(), b_hist, 1, &size, &histRange, true, false);
	calcHist(&bgrs[1], 1, 0, Mat(), g_hist, 1, &size, &histRange, true, false);
	calcHist(&bgrs[2], 1, 0, Mat(), r_hist, 1, &size, &histRange, true, false);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < size; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - b_hist.at<float>(i - 1)), Point(bin_w*i, hist_h - b_hist.at<float>(i)), Scalar(255, 0, 0), 1, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - g_hist.at<float>(i - 1)), Point(bin_w*i, hist_h - g_hist.at<float>(i)), Scalar(0, 255, 0), 1, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - r_hist.at<float>(i - 1)), Point(bin_w*i, hist_h - r_hist.at<float>(i)), Scalar(0, 0, 255), 1, 8, 0);
	}

	namedWindow(title, WINDOW_AUTOSIZE);
	imshow(title, histImage);
}
