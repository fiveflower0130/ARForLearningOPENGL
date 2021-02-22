#ifndef PARSEQRCODE_H
#define PARSEQRCODE_H

#include <string>
#include <sstream>
#include <vector>
#include <AR/ar.h>
#include "OpenGLAR.h"
#include "ZXingLib.h"
#include <GL/glut.h>


void ParseQRCode(std::string info, std::vector<std::string> &info_list);
int DecodeQRCode( ARUint8 *dataPtr, int width, int height, std::string &info);
//void TranslateQRCodeInformationList(std::vector<std::string> &info_list, std::vector<GLAR_Object> &objectList);
void TranslateQRCodeInformationList(std::vector<std::string> &info_list, std::vector<GLAR_Object> &objectList,std::vector<GLAR_LightSource> &lightList, Camera &cam);

#endif
