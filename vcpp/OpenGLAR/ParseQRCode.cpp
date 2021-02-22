#include "ParseQRCode.h"
#include "OpenGLAR.h"


using std::string;
using std::vector;

extern int selectedObj;


bool str_to_number(string s, double &d)//函式多功能化一個用int一個用double
{
	std::istringstream is(s);
	is >> d;

	if(!is) return false;
	else return true;
}

bool str_to_number(string s, int &d)
{
	std::istringstream is(s);
	is >> d;
	if(!is) return false;
	else return true;
}


void ParseQRCode(string info, vector<string> &info_list)
{
	std::istringstream input_stream(info);

	string data;

	while(input_stream >> data)
	{
		info_list.push_back(data);
	}
}

int DecodeQRCode( ARUint8 *dataPtr, int width, int height, string &info)
{
	unsigned char *pbQRCode;

	pbQRCode = new unsigned char[3*width*height];

	for(int i=0; i!=width*height; ++i)
	{
		pbQRCode[3*i] = dataPtr[4*i+1];
		pbQRCode[3*i+1] = dataPtr[4*i+2];
		pbQRCode[3*i+2] = dataPtr[4*i+3];
	}

    int rtn = DecodeBarcode(pbQRCode,width,height,false,true,info);

	delete [] pbQRCode;
	return rtn;
}

void TranslateQRCodeInformationList(vector<string> &info_list, vector<GLAR_Object> &objectList,vector<GLAR_LightSource> &lightList, Camera &cam)
{
	if(info_list[0] == "Obj")
	{
		GLAR_Object obj;
		
		if(info_list[1] == "glutSphere")
		{
			obj.objectID = SPHERE;
			
			if(info_list[2] == "Wire") obj.isWire = true;
			else if (info_list[2] == "Solid") obj.isWire = false;

			double x,y,z;
			if(str_to_number(info_list[3],x)==true)//球大小
			{
				obj.size = x;
			}
			/*
			if(str_to_number(info_list[4],y)==true) 
			{
				//obj.slices = y;
			}
			if(str_to_number(info_list[5],y)==true) 
			{
				//obj.stacks = z;
			}
			*/
            
			
			objectList.push_back(obj);
			selectedObj = objectList.size()-1;
		}

		if(info_list[1] == "glutCube")
		{
			obj.objectID = CUBE;
			if(info_list[2] == "Wire") obj.isWire = true;
			else if (info_list[2] == "Solid") obj.isWire = false;

			double x;
			if(str_to_number(info_list[3],x)==true)
			{
				obj.size = x;
			}
			
			
			objectList.push_back(obj);
			selectedObj = objectList.size()-1;
		}

		if(info_list[1] == "glutTeapot")
		{
			obj.objectID = TEAPOT;
			if(info_list[2] == "Wire") obj.isWire = true;
			else if (info_list[2] == "Solid") obj.isWire = false;

			double x;
			if(str_to_number(info_list[3],x)==true)
			{
				obj.size = x;
			}
			

			objectList.push_back(obj);
			selectedObj = objectList.size()-1;
		}
		
		
	}
	else if(info_list[0] == "Ins")
	{
		GLAR_Action action;
		if(info_list[1] == "Translated")
		{
			action.actionID = ACTION_TRANSLATE;
			double x,y,z;
			if(str_to_number(info_list[2],x)==true)
			{
				action.x = x;
			}
			if(str_to_number(info_list[3],y)==true)
			{
				action.y = y;
			}
			if(str_to_number(info_list[4],z)==true)
			{
				action.z = z;
			}
			if(selectedObj >= 0 && selectedObj < objectList.size()) objectList[selectedObj].actionList.push_back(action);
			//if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].actionList.push_back(action);

		}
		if(info_list[1] == "Rotate")
		{
			action.actionID = ACTION_ROTATE;
			double angle,x,y,z;
			if(str_to_number(info_list[2],angle)==true)
			{
				action.angle = angle;
			}
			if(str_to_number(info_list[3],x)==true)
			{
				action.x = x;
			}
			if(str_to_number(info_list[4],y)==true)
			{
				action.y = y;
			}
			if(str_to_number(info_list[5],z)==true)
			{
				action.z = z;
			}
			if(selectedObj >= 0 && selectedObj < objectList.size()) objectList[selectedObj].actionList.push_back(action);
			//if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].actionList.push_back(action);

		}
		
		if(info_list[1] == "Scale")
		{
			action.actionID = ACTION_SCALE;
			double x,y,z;
			if(str_to_number(info_list[2],x)==true)
			{
				action.x = x;
			}
			if(str_to_number(info_list[3],y)==true)
			{
				action.y = y;
			}
			if(str_to_number(info_list[4],z)==true)
			{
				action.z = z;
			}
			//if(type==OBJECT) if(selectedObj >= 0 && selectedObj < objectList.size()) objectList[selectedObj].actionList.push_back(action);
			//else if(type==LIGHT) if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].actionList.push_back(action);
			if(selectedObj >= 0 && selectedObj < objectList.size()) objectList[selectedObj].actionList.push_back(action);
			//if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].actionList.push_back(action);
		}

		if(info_list[1] == "SelectNext")
		{
			if(selectedObj >= 0 && selectedObj < objectList.size()) 
			{
				selectedObj++;
				if(selectedObj >= objectList.size()) selectedObj = 0;
				if(selectedObj >= lightList.size()) selectedObj = 0;
				
			}
		}
		
		if(info_list[1] == "SelectPrevious")
		{
			
			if(selectedObj >= 0 && selectedObj < objectList.size())
			{
				selectedObj--;
				if(selectedObj < 0) selectedObj = objectList.size()-1;
				if(selectedObj < 0) selectedObj = lightList.size()-1;
			}
		}
		if(info_list[1] == "ObjDeleteAll")
		{
			objectList.clear();
			
		}
		/*
		if(info_list[1] == "Timer")
		{
			action.actionID = ACTION_TIMER;
			 loopr = true;
			 //timer(10);
			 glutTimerFunc(10,timer,0);			
		}
		*/
	}
	else if(info_list[0] == "Light")
	{
        GLAR_LightSource light;
		light.isEnable = true;       
		lightList.push_back(light);
		selectedObj = lightList.size()-1;
		if(info_list[1] == "ambient")
			{
                 double R1,G1,B1,A1;
				 if(str_to_number(info_list[2],R1)==true)
			     {
					 light.ambient[0]=R1;
			     }
				 if(str_to_number(info_list[3],G1)==true)
			     {				
					 light.ambient[1]=G1;
			     }
				 if(str_to_number(info_list[4],B1)==true)
			     {
					 light.ambient[2]=B1;
			     }
				 if(str_to_number(info_list[5],A1)==true)
			     {
					 light.ambient[3]=A1;
			     }
				 if(info_list[6] == "diffuse")
				 {
					 double R2,G2,B2,A2;
					 if(str_to_number(info_list[7],R2)==true)
					 {
						 light.diffuse[0]=R2;
					 }
					 if(str_to_number(info_list[8],G2)==true)
					 {				
						 light.diffuse[1]=G2;
					 }
					 if(str_to_number(info_list[9],B2)==true)
					 {
						 light.diffuse[2]=B2;
					 }
					 if(str_to_number(info_list[10],A2)==true)
					 {
						 light.diffuse[3]=A2;
					 }
					 if(info_list[6] == "specular")
				     {
						 double R3,G3,B3,A3;
						 if(str_to_number(info_list[7],R3)==true)
						 {
							 light.specular[0]=R3;
						 }
						 if(str_to_number(info_list[8],G3)==true)
						 {				
							 light.specular[1]=G3;
						 }
						 if(str_to_number(info_list[9],B3)==true)
						 {
							 light.specular[2]=B3;
						 }
						 if(str_to_number(info_list[10],A3)==true)
						 {
							 light.specular[3]=A3;
						 }

					 }
				 }


				 if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].ambient[4];
				 
			}
			if(info_list[1] == "diffuse")
			{
                 double R,G,B,A;
				 if(str_to_number(info_list[2],R)==true)
			     {
					 light.diffuse[0]=R;
			     }
				 if(str_to_number(info_list[3],G)==true)
			     {				
					 light.diffuse[1]=G;
			     }
				 if(str_to_number(info_list[4],B)==true)
			     {
					 light.diffuse[2]=B;
			     }
				 if(str_to_number(info_list[5],A)==true)
			     {
					 light.diffuse[3]=A;
			     }
				 if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].diffuse[4];
				 
			}
			if(info_list[1] == "specular")
			{
                 double R,G,B,A;
				 if(str_to_number(info_list[2],R)==true)
			     {
					 light.specular[0]=R;
			     }
				 if(str_to_number(info_list[3],G)==true)
			     {				
					 light.specular[1]=G;
			     }
				 if(str_to_number(info_list[4],B)==true)
			     {
					 light.specular[2]=B;
			     }
				 if(str_to_number(info_list[5],A)==true)
			     {
					 light.specular[3]=A;
			     }
				 if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].specular;
				 
			}
            if(info_list[1] == "position")
			{
                 double X,Y,Z,U;
				 if(str_to_number(info_list[2],X)==true)
			     {
					 light.position[0]=X;
			     }
				 if(str_to_number(info_list[3],Y)==true)
			     {				
					 light.position[1]=Y;
			     }
				 if(str_to_number(info_list[4],Z)==true)
			     {
					 light.position[2]=Z;
			     }
				 if(str_to_number(info_list[5],U)==true)
			     {
					 light.position[3]=U;
			     }
				 if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].position;
				 
			}
			if(info_list[1] == "direction")
			{
                 double X,Y,Z;
				 if(str_to_number(info_list[2],X)==true)
			     {
					 light.direction[0]=X;
			     }
				 if(str_to_number(info_list[3],Y)==true)
			     {				
					 light.direction[1]=Y;
			     }
				 if(str_to_number(info_list[4],Z)==true)
			     {
					 light.direction[2]=Z;
			     }		
				 if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].direction;
				 
			}
			if(info_list[1] == "exponent")
			{
                 double E;
				 if(str_to_number(info_list[2],E)==true)
			     {
					 light.exponent=E;
				 }
				 if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].exponent;
				 
			}
			if(info_list[1] == "cutoff")
			{
                 double angle;
				 if(str_to_number(info_list[2],angle)==true)
			     {
					 light.cut_off=angle;
				 }
				 if(selectedObj >= 0 && selectedObj < lightList.size()) lightList[selectedObj].cut_off;
				 
			}

	}
	
	else if(info_list[0] == "Camera")
	{
		double x;
		if(str_to_number(info_list[1],x)==true)
		{
			cam.position[0] = x;
		}
		if(str_to_number(info_list[2],x)==true)
		{
			cam.position[1] = x;
		}
		if(str_to_number(info_list[3],x)==true)
		{
			cam.position[2] = x;
		}
		if(str_to_number(info_list[4],x)==true)
		{
			cam.look[0] = x;
		}
		if(str_to_number(info_list[5],x)==true)
		{
			cam.look[1] = x;
		}
		if(str_to_number(info_list[6],x)==true)
		{
			cam.look[2] = x;
		}
		if(str_to_number(info_list[7],x)==true)
		{
			cam.up[0] = x;
		}
		if(str_to_number(info_list[8],x)==true)
		{
			cam.up[1] = x;
		}
		if(str_to_number(info_list[9],x)==true)
		{
			cam.up[2] = x;
		}
	}
	
	
	
}
