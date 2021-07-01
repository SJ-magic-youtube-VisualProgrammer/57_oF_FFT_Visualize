/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
/********************
********************/
int GPIO_0 = 0;
int GPIO_1 = 0;

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;

FILE* fp_Log = nullptr;


/************************************************************
func
************************************************************/
/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt)
{
	double Alpha;
	if((Alpha_dt <= 0) || (Alpha_dt < dt))	Alpha = 1;
	else									Alpha = 1/Alpha_dt * dt;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha)
{
	if(Alpha < 0)		Alpha = 0;
	else if(1 < Alpha)	Alpha = 1;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double sj_max(double a, double b)
{
	if(a < b)	return b;
	else		return a;
}

/******************************
******************************/
bool checkIf_ContentsExist(char* ret, char* buf)
{
	if( (ret == NULL) || (buf == NULL)) return false;
	
	string str_Line = buf;
	Align_StringOfData(str_Line);
	vector<string> str_vals = ofSplitString(str_Line, ",");
	if( (str_vals.size() == 0) || (str_vals[0] == "") ){ // no_data or exist text but it's",,,,,,,".
		return false;
	}else{
		return true;
	}
}

/******************************
******************************/
void Align_StringOfData(string& s)
{
	size_t pos;
	while((pos = s.find_first_of(" 　\t\n\r")) != string::npos){ // 半角・全角space, \t 改行 削除
		s.erase(pos, 1);
	}
}

/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	Group_FFT.setup("FFT");
		Group_FFT.add(Gain.setup("Gain", 1.0, 1.0, 5.0));
		Group_FFT.add(k_smooth.setup("k_smooth", 0.98, 0.8, 1.0));
		Group_FFT.add(dt_smooth_2.setup("dt_smooth_2", 167, 10, 300));
		Group_FFT.add(bandRad.setup("bandRad", 2.0, 1.0, 10.0));
		Group_FFT.add(bandVel.setup("bandVel", 100.0, 1.0, 255.0));
		Group_FFT.add(b_Window.setup("b_window", true));
		Group_FFT.add(b_Limit_Rad_min.setup("b_Limit_Rad_min", true));
		Group_FFT.add(ArtSin_Band_min.setup("ArtSin_Band_min", 1.0, 1.0, 255.0));
		Group_FFT.add(ArtSin_Band_max.setup("ArtSin_Band_max", 1.0, 1.0, 255.0));
		Group_FFT.add(ArtSin_PhaseMap_k.setup("ArtSin_PhaseMap_k", 1.0, 0.0, 2.0));
		Group_FFT.add(dir_ArtSin_FW.setup("dir_ArtSin_FW", false));
		Group_FFT.add(b_ArtSin_abs.setup("b_ArtSin_abs", false));
		Group_FFT.add(b_Window_artSin.setup("b_Window_artSin", false));
		
		ofColor initColor = ofColor(0, 100, 255, 150);
		ofColor minColor = ofColor(0, 0, 0, 0);
		ofColor maxColor = ofColor(255, 255, 255, 255);
		Group_FFT.add(col_ArtSin.setup("col_ArtSin", initColor, minColor, maxColor));
		
		Group_FFT.add(b_DispArtSin.setup("b_DispArtSin", true));
		Group_FFT.add(b_DispDanceCloud.setup("b_DispDanceCloud", true));
	gui.add(&Group_FFT);
	
	/*
	Group_Trainer.setup("Trainer");
		const float Range = 5.0;
		ofVec3f ofs_init = ofVec3f(0.0, 0.0, 0.0);
		ofVec3f ofs_min = ofVec3f(-Range, -Range, -Range);
		ofVec3f ofs_max = ofVec3f(Range, Range, Range);
	
		Group_Trainer.add(Trainer_Ofs.setup("offset", ofs_init, ofs_min, ofs_max));
	gui.add(&Group_Trainer);
	
	Group_Sync.setup("Sync");
		Group_Sync.add(a_exp_quart.setup("a_exp_quart", 1.4, 0.1, 2.0));
		Group_Sync.add(a_smooth_quart.setup("a_smooth_quart", 0.02, 0, 1.0));
		Group_Sync.add(a_smooth_vel.setup("a_smooth_vel", 0.2, 0, 1.0));
		Group_Sync.add(CalSyncAve_ValidJoint_rot.setup("CalSyncAve_rot", 1.0, 0.0, 2.0));
		Group_Sync.add(CalSyncAve_ValidJoint_vel.setup("CalSyncAve_vel", 0.0, 0.0, 2.0));
	gui.add(&Group_Sync);

	Group_Light.setup("Light");
		ofVec4f ColWhenTest_init = ofVec4f(0.0, 0.0, 1.0, 0.0);
		ofVec4f ColWhenTest_min = ofVec4f(0.0, 0.0, 0.0, 0.0);
		ofVec4f ColWhenTest_max = ofVec4f(1.0, 1.0, 1.0, 1.0);
		Group_Light.add(col_WhenTest.setup("ColTest", ColWhenTest_init, ColWhenTest_min, ColWhenTest_max));
		
		Group_Light.add(Led_dimmer.setup("dimmer", 1.0, 0, 1.0));
		Group_Light.add(volLight_Back_max.setup("L_Back_max", 0.5, 0.01, 1.0)); // XBCamの検知と相談しながら.
	gui.add(&Group_Light);
	
	Group_VolLight.setup("VolLight");
		Group_VolLight.add(Smooth_dt.setup("Smooth_dt", 193, 0, 1500));
		Group_VolLight.add(fft_map_max.setup("fft_map_max", 0.3, 0, 1.0));
	gui.add(&Group_VolLight);
	
	Group_Shutter.setup("Shutter");
		Group_Shutter.add(DmxShutter_open.setup("open", 150, 0, 255));
		Group_Shutter.add(DmxShutter_close.setup("close", 0, 0, 255));
	gui.add(&Group_Shutter);
	*/

	/********************
	********************/
	gui.minimizeAll();
}

