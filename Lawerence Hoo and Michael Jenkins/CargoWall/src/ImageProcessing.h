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

enum IMAGE_PROCESSING_MODE {
	DEBUG_PROCESSING = 0,
	VIDEO_PROCESSING,
	LIVE_PROCESSING
};

using namespace cv;
using namespace ofxCv;

class ImageProcessing {
	public:
	
		void setup();
		void update();
		void draw();
		void drawCv();
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
	
	private:
		ofVideoPlayer player;
		ofVideoGrabber grabber;
		ofFbo resultingImage;
	
		IMAGE_PROCESSING_MODE currentMode;
		ofxImGui::Gui gui;
		int iFadeLevel;
		int iBrushScale;
		int thresholdAmount;
		int iMaxArea;
		int iMinArea;
		int iContourThreshold;
		ofImage debugImg;
		ofImage preProcessedImage;
		ofImage processedImage;
		ofxCv::ContourFinder contourFinder;
	
};

#endif /* ImageProcessing_h */
