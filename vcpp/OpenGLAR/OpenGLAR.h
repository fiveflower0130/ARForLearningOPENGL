#ifndef OPENGLAR_H
#define OPENGLAR_H

#include <vector>

#define SPHERE 0
#define CUBE 1
#define TEAPOT 2
#define _3DS 3
#define OBJECT_NONE -1

#define ACTION_TRANSLATE 0
#define ACTION_ROTATE 1
#define ACTION_SCALE 2
#define ACTION_GLULOOKAT 3
//#define ACTION_SELFROTATE 3
//#define ACTION_TIMER 3

//#define LIGHTTYPE_POINTLIGHT 0
//#define LIGHTTYPE_PARALLELLIGHT 1
//#define LIGHTTYPE_SPOTLIGHT 2

#define ACTION_NONE -1

class GLAR_Action
{
public:
	int actionID; //ACTION_TRANSLAT or ACTION_ROTATE or  ACTION_SCALE or ACTION_GLULOOKAT
	double x,y,z;
	double angle;
	double ex,ey,ez,cx,cy,cz,ux,uy,uz;//攝影機參數
	int rx,ry;
	bool loopr;

	GLAR_Action()
	{
		actionID = ACTION_NONE;
		x = y = z = 0.0;
        
        ex = 0.0;
		ey = 0.0;
		ez = 0.0;
        cx = 0.0; 
		cy = 0.0;
		cz =-1.0;
		ux = 0.0;
		uy = 1.0;
		uz = 0.0;

		rx = ry = 30;
		angle =0.0;
		loopr = false;
	}
};


class GLAR_Object
{
public:
	int objectID; //SPHERE or  CUBE or _3DS
	bool isWire;
	float R,G,B,A;//做材質使用

	
	//double slices,stacks,base,height,innerRadius,outesRadius,nsides,rings,size;
	     //水平切片 垂直切片 圓錐底 圓錐高 圓環內徑 圓環外徑 圓環分割片數 大小     

	double size;
	std::vector<GLAR_Action> actionList;

	GLAR_Object()
	{
		actionList.resize(0);
		objectID = OBJECT_NONE;
		isWire = false;
		size = 0.0;
		//R=1.0;
		//G=1.0;
		//B=1.0;
		//A=1.0;
	}
};

class GLAR_LightSource
{
public:
	//int type; //0:點光源LightType_PointLight 1:平行光LightType_ParallelLight 2:探照燈LightType_SpotLight
	bool isEnable; // false:Disable true:Enable
	float position[4]; // light position 
	float direction[3]; // light direction (for spot light)
	float exponent; // exponent of spot light
	float cut_off; //cut off angle of spot light 
	float ambient[4];
	float diffuse[4];
	float specular[4];

	std::vector<GLAR_Action> actionList;

	GLAR_LightSource()
	{
		actionList.resize(0);
		isEnable = 0;

		position[0] = 0.0f;
		position[1] = 0.0f;
		position[2] = 1.0f;
		position[3] = 0.0f;

		direction[0] = 0.0f;
		direction[1] = 0.0f;
		direction[2] =-1.0f;

		exponent = 0.0f;//衰弱指數spotlight用的
		cut_off = 180.0f;

		ambient[0] = 0.0f;
		ambient[1] = 0.0f;
		ambient[2] = 0.0f;
		ambient[3] = 1.0f;

		diffuse[0] = 1.0f;
		diffuse[1] = 1.0f;
		diffuse[2] = 1.0f;
		diffuse[3] = 1.0f;

		specular[0] = 1.0f;
		specular[1] = 1.0f;
		specular[2] = 1.0f;
		specular[3] = 1.0f;
	}
};

class Camera
{
public:
	double position[3];
	double look[3];
	double up[3];

	Camera()
	{
        position[0] = 0.0;
		position[1] = 0.0;
		position[2] = 50.0;

		look[0] = 0.0;
		look[1] = 0.0;
		look[2] = -1.0;

		up[0] = 0.0;
		up[1] = 1.0;
		up[2] = 0.0;


	}
};




#endif