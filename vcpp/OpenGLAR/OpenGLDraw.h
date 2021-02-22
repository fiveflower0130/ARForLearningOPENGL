#ifndef OPENGLDRAW_H
#define OPENGLDRAW_H

#include <vector>

void DrawOpenGLObj(std::vector<GLAR_Object> &objectList, int hasBoundingBox,int hasGridLine);
void DrawOpenGLLight(std::vector<GLAR_LightSource> &lightList);
void DrawOpenGLLookAt(std::vector<GLAR_Action> &actionList);

#endif