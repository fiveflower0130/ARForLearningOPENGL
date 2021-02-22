#ifndef ZXINGLIB_H
#define ZXINGLIB_H

#include <vector>

//int DecodeBarcode(Image &image, bool hybrid, string &info);
int DecodeBarcode(unsigned char *pbImage, int width, int height, bool flip, bool hybrid, std::string &info);
int DecodeBarcode(std::string filename, bool hybrid, std::string &info);
//int DecodeMultiBarcode(Image& image, bool hybrid, vector<string> &info);
int DecodeMultiBarcode(unsigned char *pbImage, int width, int height, bool flip, bool hybrid, std::vector<std::string> &info);
int DecodeMultiBarcode(std::string filename, bool hybrid, std::vector<std::string> &info);

#endif