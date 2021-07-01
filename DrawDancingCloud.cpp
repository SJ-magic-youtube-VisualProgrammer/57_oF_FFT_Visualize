/************************************************************
************************************************************/
#include "DrawDancingCloud.h"

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_DANCING_CLOUD::DRAW_DANCING_CLOUD()
{
	font.load("font/RictyDiminished-Regular.ttf", 10, true, true, true);
}

/******************************
******************************/
DRAW_DANCING_CLOUD::~DRAW_DANCING_CLOUD()
{
}

/******************************
******************************/
void DRAW_DANCING_CLOUD::setup(int N)
{
	/********************
	********************/
	for ( int j=0; j<NUM_CLOUD_POINTS; j++ ) {
		t_noise[j].x = ofRandom( 0, 1000 );
		t_noise[j].y = ofRandom( 0, 1000 );
	}
	
	/********************
	********************/
	img.load("img.png");

}

/******************************
******************************/
void DRAW_DANCING_CLOUD::toggle_HoriLine()
{
	b_HoriLine = !b_HoriLine;
}

/******************************
******************************/
void DRAW_DANCING_CLOUD::update(const FFT& fft)
{
	/********************
	********************/
	float now = ofGetElapsedTimef();
	float dt = now - t_Last;
	dt = ofClamp( dt, 0.0, 0.1 );	
	t_Last = now;
	
	bandRad = (int)Gui_Global->bandRad;
	bandVel = (int)Gui_Global->bandVel;

	Rad = ofMap( fft.get_Gain(bandRad), 1, 3, 400, 800, Gui_Global->b_Limit_Rad_min );
	Vel = ofMap( fft.get_Gain(bandVel), 0, 0.1, 0.05, 0.5 );
	
	add_color = ofMap( fft.get_Gain(bandRad), 1, 3, 0, 255, true );
	
	for (int j = 0; j < NUM_CLOUD_POINTS; j++) {
		t_noise[j].x += Vel * dt;
		t_noise[j].y += Vel * dt;
		
		p_cloud[j].x = ofSignedNoise( t_noise[j].x ) * Rad;		
		p_cloud[j].y = ofSignedNoise( t_noise[j].y ) * Rad;	
	}

}

/******************************
******************************/
void DRAW_DANCING_CLOUD::draw(const FFT& fft)
{
	/********************
	********************/
	ofSetColor(255, 255, 255, 100);
	img.draw(0, 0);
	
	/********************
	********************/
	{
		//Draw background rect for spectrum
		const int width_of_bar = 3;
		const int space_per_bar = 5;
		const int line_per = 5;
		const int BarHeight = 100;
		
		// 座布団 : spectrum
		ofSetColor(255, 255, 255, 20);
		ofFill();
		ofDrawRectangle( 0, 700, ofGetWidth(), -BarHeight );
		
		// 縦 line
		char buf[256];
		ofSetColor(255, 255, 255, 100);
		for(int i = 0; i * space_per_bar * line_per < ofGetWidth(); i++){
			int _x = i * space_per_bar * line_per; // i 本目のlineのx座標
			ofDrawLine(_x, 700, _x, 700 - BarHeight );
			
			sprintf( buf, "%d", i * line_per);
			font.drawString(buf, _x, 700 - BarHeight);
		}
		
		// 横 line
		if(b_HoriLine){
			ofSetColor(255, 0, 0, 200);
			ofDrawLine(0,ofGetAppPtr()-> mouseY, ofGetWidth(), ofGetAppPtr()->mouseY);	// ofApp でないので、mouseYは使えず、ofGetAppPtr()->mouseY とする必要がある。
		}
		
		
		// spectrum
		for (int i = 0; i < fft.get_sizeof_GainArray(); i++) {
			if ( i == bandRad || i == bandVel )	ofSetColor( 0, 255, 0 , 150);
			else								ofSetColor( 0, 100, 255, 150 );
			
			ofDrawRectangle( i * space_per_bar, 700, width_of_bar, -fft.get_Gain(i) * BarHeight );
		}
	}
	
	/********************
	artSin
	********************/
	if(Gui_Global->b_DispArtSin){
		//Draw background rect
		const int width_of_bar = 2;
		const int space_per_bar = 2;
		const int BarHeight = 100;
		const int ofs = 128;
		
		// 座布団 : spectrum
		ofSetColor(255, 255, 255, 20);
		ofFill();
		ofDrawRectangle( 0, 600, ofGetWidth(), -BarHeight * 2 );
		
		// artSin
		ofSetColor(Gui_Global->col_ArtSin);
		for (int i = 0; i < fft.get_sizeof_artSinArray(); i++) {
			ofDrawRectangle( ofs + i * space_per_bar, 500, width_of_bar, -fft.get_artSin(i) * BarHeight );
		}
		
	}
	
	/********************
	Dancing cloud
	********************/
	if(Gui_Global->b_DispDanceCloud){
		ofPushMatrix();
			ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
		
			//Draw points
			ofColor base(10, 100, 255);
			ofSetColor( min(base.r + (int)add_color, 255), min(base.g + (int)add_color, 255), min(base.b + (int)add_color, 255), 150);
			
			ofFill();
			for (int i=0; i<NUM_CLOUD_POINTS; i++) {
				ofDrawCircle( p_cloud[i], 2 );
			}
		
			//Draw lines between near points
			glLineWidth(1);
			float thresh_dist = 40;
			for (int j=0; j<NUM_CLOUD_POINTS; j++) {
				for (int k=j+1; k<NUM_CLOUD_POINTS; k++) {
					if ( ofDist( p_cloud[j].x, p_cloud[j].y, p_cloud[k].x, p_cloud[k].y ) < thresh_dist ) {
						ofDrawLine( p_cloud[j], p_cloud[k] );
					}
				}
			}
		
		ofPopMatrix();
	}
}

