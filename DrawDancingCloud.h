/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "fft.h"
#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class DRAW_DANCING_CLOUD{
private:
	/****************************************
	****************************************/
	enum{
		NUM_CLOUD_POINTS	= 400,
	};
	
	float Rad = 500;		//Cloud raduis parameter
	float Vel = 0.1;		//Cloud points velocity parameter
	int bandRad = 2;		//Band index in spectrum, affecting Rad value
	int bandVel = 100;		//Band index in spectrum, affecting Vel value
	
	ofVec2f t_noise[NUM_CLOUD_POINTS];
	ofPoint p_cloud[NUM_CLOUD_POINTS];
	float add_color = 0;
	
	float t_Last = 0;
	
	ofImage img;
	
	ofTrueTypeFont font;
	
	bool b_HoriLine = false;
	
public:
	DRAW_DANCING_CLOUD();
	~DRAW_DANCING_CLOUD();
	void setup(int N);
	void update(const FFT& fft);
	void draw(const FFT& fft);
	void toggle_HoriLine();
	
	void update_spectrum(int id, double val);
	void update_phase(int id, double val);
};