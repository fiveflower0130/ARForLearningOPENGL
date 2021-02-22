#include <iostream>//�Ĥ@�ӭn���g�o��
#include <string>
#include <GL/glut.h>
#include <windows.h>

#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <vector>

#include "OpenGLAR.h"
#include "ParseQRCode.h"
#include "OpenGLWindow.h"
#include "OpenGLDraw.h"



using std::cout;//�ϥ� std �ؿ��U�� cout
using std::endl;//
using std::vector;//
using std::string;//

char *vconf = "C:\\Users\\user\\Desktop\\ARToolKit\\bin\\Data\\WDM_camera_flipV.xml";
int xsize, ysize;
ARParam cparam;
char *cparam_name    = "C:\\Users\\user\\Desktop\\ARToolKit\\bin\\Data\\camera_para.dat";
char *patt_name      = "C:\\Users\\user\\Desktop\\ARToolKit\\bin\\Data\\patt.hiro";
int patt_id;

int count = 0;
int mode = 1;
int thresh = 100;
int selectedObj = -1;

double selfrotate = 0.0;

int             patt_width     = 80.0;
double          patt_center[2] = {0.0, 0.0};
double          patt_trans[3][4];

static void init( void );
static void keyEvent( unsigned char key, int x, int y);
static void mainLoop(void);
static void cleanup(void);
static void draw( double trans[3][4] );

vector<GLAR_Object> objectList(0);
vector<GLAR_Object>::size_type i=0;
vector<GLAR_LightSource> lightList(0);
vector<GLAR_Action> actionList(0);

Camera cam;

int showFlag=0;

int GridLine = -1;
int winAR;

void ShowProgramCode(void);


void main(int argc, char **argv)
{
	glutInit(&argc, argv);//�NOPENGL Utility ToolKit��l��

	CreateOpenGLWindow();

	init();//��l�Ƴo�{��

    arVideoCapStart();//�Ұʵ��T

    argMainLoop( NULL, keyEvent, mainLoop );

}

static void init( void )
{
    ARParam  wparam;

    /* open the video path */
    if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    if( (patt_id=arLoadPatt(patt_name)) < 0 ) {
        printf("pattern load error !!\n");
        exit(0);
    }

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 0, 0, 0 );

	winAR = glutGetWindow();//�}��OPENGL����


	GLAR_LightSource light;
	light.isEnable = true;
	lightList.push_back(light);
}

/* cleanup function called when program exits */
static void cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}


static void   keyEvent( unsigned char key, int x, int y)
{
    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        cleanup();
        exit(0);
    }

    if( key == 'c' ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        count = 0;

        mode = 1 - mode;
        if( mode ) printf("Continuous mode: Using arGetTransMatCont.\n");
         else      printf("One shot mode: Using arGetTransMat.\n");
    }

	if( key == 's' ) 
	{
		//showFlag=1;
		ShowProgramCode();
	}
	if( key == 'a' ) 
	{
		showFlag=2;
	}
	if( key == 'd' ) 
	{
		showFlag=4;
	}
	if( key == 'g' ) 
	{
		GridLine*=-1;
	}

}


/* main loop */
static void mainLoop(void)
{
    static int      contF = 0;
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             j, k;

	glutSetWindow(winAR);//�R�OOPENGL�������Ƨ��
	
    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) 
	{
        arUtilSleep(2);
        return;
    }
    if( count == 0 ) arUtilTimerReset();
    count++;

    argDrawMode2D();
    argDispImage( dataPtr, 0,0 );

    /*�B�zQRCODE���X�����*/
	static std::string pre_info;//�ŧi�s��r��@���O���ѽX�L���r��
	static double preTime;
	double curTime = arUtilTimer();
	std::string info;//�ŧi�Ȯ��x�s�r���x�s�{�ѽX���r��

	//printf("preTime=%f, curTime=%f\n",preTime,curTime);
    

	int rtn = DecodeQRCode(dataPtr,xsize,ysize,info);//rtn�|�QDecode�ѽX�A�Ǧ^��
	if(rtn==0)//�p�G�ѽX�L�~
	{
		cout << info << endl;// ��ܳB�zQR Code�̭�����T
		vector<string> info_list;
		ParseQRCode(info,info_list);

		if(info_list[0]=="Obj")
		{
			if(info!=pre_info) //�p�G�{�ѽX�����e������W�@�ӸѽX���e
			{
				//TranslateQRCodeInformationList(info_list,objectList); //����ѽX�᪺�ʧ@
				TranslateQRCodeInformationList(info_list,objectList,lightList,cam); //����ѽX�᪺�ʧ@
				pre_info = info;//�ñN�{�ѽX�r��s��W�@�Ӧr���
				preTime = arUtilTimer();
			}
			else
			{
				if(curTime-preTime > 3.0)
				{
					//TranslateQRCodeInformationList(info_list,objectList); //����ѽX�᪺�ʧ@
					TranslateQRCodeInformationList(info_list,objectList,lightList,cam); //����ѽX�᪺�ʧ@
					preTime = curTime;
					pre_info = info;

				}
			}
		}
		
		else if(info_list[0]=="Light")
		{
			if(info!=pre_info) //�p�G�{�ѽX�����e������W�@�ӸѽX���e
			{
				TranslateQRCodeInformationList(info_list,objectList,lightList,cam); //����ѽX�᪺�ʧ@
				pre_info = info;//�ñN�{�ѽX�r��s��W�@�Ӧr���
				preTime = arUtilTimer();
			}
			else
			{
				if(curTime-preTime > 3.0)
				{
					TranslateQRCodeInformationList(info_list,objectList,lightList,cam); //����ѽX�᪺�ʧ@
					preTime = curTime;
					pre_info = info;

				}
			}
		
		}

		else 
		{
			if(curTime-preTime > 1.0)
			{
				//TranslateQRCodeInformationList(info_list,objectList); //����ѽX�᪺�ʧ@
				TranslateQRCodeInformationList(info_list,objectList,lightList,cam); //����ѽX�᪺�ʧ@
				preTime = curTime;

			}
		}

		RefreshOpenGLWindow();
	}


    /* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 )
	{
        cleanup();
        exit(0);
    }

    arVideoCapNext();

    /* check for object visibility */
    k = -1;
    for( j = 0; j < marker_num; j++ ) {
        if( patt_id == marker_info[j].id ) {
            if( k == -1 ) k = j;
            else if( marker_info[k].cf < marker_info[j].cf ) k = j;
        }
    }
    if( k == -1 ) {
        contF = 0;
        argSwapBuffers();
        return;
    }

    /* get the transformation between the marker and the real camera */
    if( mode == 0 || contF == 0 ) 
	{
        arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);
    }
    else 
	{
        arGetTransMatCont(&marker_info[k], patt_trans, patt_center, patt_width, patt_trans);
    }
    contF = 1;

    draw( patt_trans );

    argSwapBuffers();
}

static void draw( double trans[3][4] )
{
    double    gl_para[16];

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);

    /* load the camera transformation matrix */
    argConvGlpara(trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

	//glEnable(GL_COLOR_MATERIAL);


    glMatrixMode(GL_MODELVIEW);

	if(showFlag==2) cout << "Object Size:" << objectList.size() << endl;

	if(showFlag==4) objectList.clear();
	
	glRotated(90.0,1.0,0.0,0.0);
	DrawOpenGLLight(lightList);
	DrawOpenGLObj(objectList,1,1);//�ץXOPENGL�ϧ�
	
	//if(showFlag==4) objectList.clear();
    
	if(showFlag>0) 
	{
		//cout<< "----------------------------------------------------------------------"<<endl;
		showFlag=0;
	}

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
}


