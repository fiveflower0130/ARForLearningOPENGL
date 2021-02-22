#include <iostream>//第一個要先寫這個
#include <vector>

//#include "PrintCode.h"
#include "ParseQRCode.h"
#include "OpenGLAR.h"


using std::string;
using std::vector;
using std::cout;//使用 std 目錄下的 cout
using std::endl;//使用 std 目錄下的 endl

extern vector<GLAR_Object> objectList;
extern vector<GLAR_Action> actionList;

extern Camera cam;

void ShowProgramCode(void)
{
	system("CLS");

	printf("void Display()\n");
	printf("{\n");
	printf("\tglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);\n");
	printf("\tglMatrixMode(GL_MODELVIEW);\n");
	printf("\tglLoadIdentity();\n");
	cout << "\tgluLookAt(" << cam.position[0] << "," << cam.position[1] << "," << cam.position[2] << "," << cam.look[0] << "," << cam.look[1] << "," << cam.look[2] << "," << cam.up[0] << "," << cam.up[1] << "," << cam.up[2] << ");" << endl;
    printf("{\n");

	for(vector<GLAR_Object>::size_type i=0; i!=objectList.size(); ++i)
	{
		switch(objectList[i].objectID)
		{
		case SPHERE:
			glPushMatrix();
			printf("\tglPushMatrix();\n"); 
			for(int j=objectList[i].actionList.size()-1; j>=0; --j)
			{
				switch(objectList[i].actionList[j].actionID)
				{
				case ACTION_TRANSLATE:
					
					cout << "\tglTranslated(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_ROTATE:
					
					cout << "\tglRotated(" << objectList[i].actionList[j].angle <<"," << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_SCALE:
					
					cout << "\tglScaled(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
				    break;
				}
			}
			if(objectList[i].isWire)
			{			
				 cout << "\tglutWireSphere(" << objectList[i].size << "," <<32 << "," <<64 << ");" << endl;
			}
			else 
			{
				 cout << "\tglutSolidSphere(" << objectList[i].size << "," <<32 << "," <<64 << ");" << endl;
			}
			glPopMatrix();
			printf("\tglPopMatrix();\n\n");
			break;

		case CUBE:
			glPushMatrix();
			printf("\tglPushMatrix();\n");
			for(int j=objectList[i].actionList.size()-1; j>=0; --j)
			{
				switch(objectList[i].actionList[j].actionID)
				{
				case ACTION_TRANSLATE:
					
					cout << "\tglTranslated(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_ROTATE:
					
					cout << "\tglRotated("<< objectList[i].actionList[j].angle <<","<< objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_SCALE:
	
					cout << "\tglScaled(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				}
			}
			if(objectList[i].isWire)
			{
				 cout << "\tglutWireCube(" << objectList[i].size << ");" << endl;
			}
			else 
			{
				 cout << "\tglutSolidCube(" << objectList[i].size << ");" << endl;
			}
			glPopMatrix();
			printf("\tglPopMatrix();\n\n");
			break;

		case TEAPOT:
			glPushMatrix();
			printf("\tglPushMatrix();\n");
			for(int j=objectList[i].actionList.size()-1; j>=0; --j)
			{
				switch(objectList[i].actionList[j].actionID)
				{
				case ACTION_TRANSLATE:
					
					cout << "\tglTranslated(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_ROTATE:
					
					cout << "\tglRotated("<< objectList[i].actionList[j].angle <<","<< objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_SCALE:
					
					cout << "\tglScaled(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;

				}
			}
			if(objectList[i].isWire)
			{
				 cout << "\tglutWireTeapot(" << objectList[i].size << ");" << endl;
			}
			else 
			{
				 cout << "\tglutSolidTeapot(" << objectList[i].size << ");" << endl;
			}
			glPopMatrix();
			printf("\tglPopMatrix();\n\n");
			
			break;
		}
	}
	
	printf("\tglutSwapBuffers();\n\n");
	printf("}\n");
}