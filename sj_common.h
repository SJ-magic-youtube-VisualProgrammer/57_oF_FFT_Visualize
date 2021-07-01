/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "stdio.h"

#include "ofMain.h"
#include "ofxGui.h"

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/
enum{
	WINDOW_WIDTH	= 1280,	// 切れの良い解像度でないと、ofSaveScreen()での画面保存が上手く行かなかった(真っ暗な画面が保存されるだけ).
	WINDOW_HEIGHT	= 720,
};

enum{
	BUF_SIZE_S = 500,
	BUF_SIZE_M = 1000,
	BUF_SIZE_L = 6000,
	BUF_SIZE_LL = 100000,
	UDP_BUF_SIZE = 100000,
};


/************************************************************
************************************************************/

/**************************************************
**************************************************/
class GUI_GLOBAL{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	
	ofxGuiGroup Group_FFT;
		ofxFloatSlider Gain;
		ofxFloatSlider k_smooth;
		ofxFloatSlider dt_smooth_2;
		ofxFloatSlider bandRad;
		ofxFloatSlider bandVel;
		ofxToggle b_Window;
		ofxToggle b_Limit_Rad_min;
		ofxFloatSlider ArtSin_Band_min;
		ofxFloatSlider ArtSin_Band_max;
		ofxFloatSlider ArtSin_PhaseMap_k;
		ofxToggle dir_ArtSin_FW;
		ofxToggle b_ArtSin_abs;
		ofxToggle b_Window_artSin;
		ofxColorSlider col_ArtSin;
		ofxToggle b_DispArtSin;
		ofxToggle b_DispDanceCloud;
		
		
	
	/*
	ofxGuiGroup Group_Trainer;
		ofxVec3Slider Trainer_Ofs;
		
	ofxGuiGroup Group_Sync;
		ofxFloatSlider a_exp_quart;
		ofxFloatSlider a_smooth_quart;
		ofxFloatSlider a_smooth_vel;
		ofxFloatSlider CalSyncAve_ValidJoint_rot;
		ofxFloatSlider CalSyncAve_ValidJoint_vel;
	
	ofxGuiGroup Group_Light;
		ofxVec4Slider col_WhenTest;
		ofxFloatSlider Led_dimmer;
		ofxFloatSlider volLight_Back_max;
		
	ofxGuiGroup Group_VolLight;
		ofxFloatSlider Smooth_dt;
		ofxFloatSlider fft_map_max;
		
	ofxGuiGroup Group_Shutter;
		ofxFloatSlider DmxShutter_open;
		ofxFloatSlider DmxShutter_close;
	*/

	ofxPanel gui;
	
	bool b_Disp = false;
};

/************************************************************
************************************************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt);
double LPF(double LastVal, double CurrentVal, double Alpha);
double sj_max(double a, double b);

bool checkIf_ContentsExist(char* ret, char* buf);
void Align_StringOfData(string& s);

/************************************************************
************************************************************/
extern GUI_GLOBAL* Gui_Global;

extern FILE* fp_Log;

extern int GPIO_0;
extern int GPIO_1;


/************************************************************
************************************************************/

