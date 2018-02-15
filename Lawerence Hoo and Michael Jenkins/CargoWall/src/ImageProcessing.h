//
//  ImageProcessing.h
//  CargoWall
//
//  Created by David Haylock on 24/01/2018.
//

#ifndef ImageProcessing_h
#define ImageProcessing_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxImGui.h"
#include "ofxPS3EyeGrabber.h"

enum IMAGE_PROCESSING_MODE {
	DEBUG_PROCESSING = 0,
	VIDEO_PROCESSING,
	LIVE_PROCESSING
};

using namespace cv;
using namespace ofxCv;


const float diffThreshold = 2.5; // maximum amount of movement
const float timeThreshold = 1; // minimum time between snapshots
const int startCleaning = 10; // start cleaning outliers after this many samples

class ImageProcessing {
	public:
	
		//--------------------------------------------------------------
		/**
			 Setup the System
		*/
		//--------------------------------------------------------------
		void setup();
	
		//--------------------------------------------------------------
		/**
			 Update
		*/
		//--------------------------------------------------------------
		void update();
	
		//--------------------------------------------------------------
		/**
			 Draw
		*/
		//--------------------------------------------------------------
		void draw();
	
		//--------------------------------------------------------------
		/**
			 Draw Cv
		*/
		//--------------------------------------------------------------
		void drawCv();
	
		//--------------------------------------------------------------
		/**
			 Draw the Gui
		*/
		//--------------------------------------------------------------
		void drawGui();
	
		//--------------------------------------------------------------
		/**
			Sets the Processor to the new mode

			@param newMode new mode
		*/
		//--------------------------------------------------------------
		void setMode(IMAGE_PROCESSING_MODE newMode);
	
		//--------------------------------------------------------------
		/**
			Return the image texture from the framebuffer

			@return image texture
		*/
		//--------------------------------------------------------------
		ofTexture getMaskImage();
	
		//--------------------------------------------------------------
		/**
			 Get the Biggest Blobs Coordinates

			 @return contourFinder blob zero ofPint
		*/
		//--------------------------------------------------------------
		ofPoint getBiggestCoordinate();
	
		//--------------------------------------------------------------
		/**
			Get the Gui's Parameters

			@return parameters
		*/
		//--------------------------------------------------------------
		ofParameterGroup getParameters();
	
		ofParameter<bool> bShowCv{"Show CV", false};
		ofParameter<int> iCvImagesOpacity{"CV Images Opacity", 64, 1,255};
		ofParameter<bool> bFillArea{"Fill Area", false};
		ofParameter<int> iFadeLevel{"Fade Level", 1, 1,255};
		ofParameter<int> iBrushScale{"Brush Scale",5,25,100};
		ofParameter<int> thresholdAmount{"Image Threshold",230,0,255};
		ofParameter<int> iMaxArea{"Max Tracking Size", 150,1,500};
		ofParameter<int> iMinArea{"Min Tracking Size", 30,1,500};
		ofParameter<int> iContourThreshold{"Contour Threshold",100,1,255};
	
	
		ofParameterGroup imageProc{ "Image Processing",bShowCv,iCvImagesOpacity,bFillArea,iFadeLevel,iBrushScale,thresholdAmount,iMaxArea,iMinArea,iContourThreshold};
	
		ofParameter<int> cameraBrightness{"Camera Brightness", 128,0,255};
		ofParameter<int> cameraExposure{"Camera Exposure", 128,0,255};
		ofParameter<int> cameraGain{"Camera Gain", 0, 0,63};
		ofParameter<int> cameraContrast{"Camera Contrast",128,0,255};
		ofParameter<int> cameraFramerate{"Camera Framerate",60,0,120};
		ofParameter<bool> cameraVFlip{"Flip V", false};
		ofParameter<bool> cameraHFlip{"Flip H", false};
	ofParameterGroup cameraSettings{"Camera Settings",cameraBrightness,cameraExposure,cameraGain,cameraContrast,cameraFramerate,cameraVFlip,cameraHFlip};
	
	
		void onBrightness(int &val);
		void onContrast(int &val);
		void onGain(int &val);
		void onExposure(int &val);
		void onFramerate(int &val);
		void onVFlip(bool &val);
		void onHFlip(bool &val);
	
	private:
		ofVideoPlayer player;
		
		ofVideoGrabber grabber;
		ofFbo resultingImage;
	
		IMAGE_PROCESSING_MODE currentMode;
		ofxImGui::Gui gui;
		ofImage debugImg;
		ofImage preProcessedImage;
		ofImage processedImage;
		ofxCv::ContourFinder contourFinder;
	
};

#endif /* ImageProcessing_h */
