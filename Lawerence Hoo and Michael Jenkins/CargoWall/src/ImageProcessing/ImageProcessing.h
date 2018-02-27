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
#include "ImageSelection.h"

enum IMAGE_PROCESSING_MODE {
	DEBUG_PROCESSING = 0,
	VIDEO_PROCESSING,
	LIVE_PROCESSING
};

using namespace cv;
using namespace ofxCv;

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
		ofParameter<bool> bShowCvCalibration{"Show CV Calibratoin", false};
		ofParameter<int> cvImagesOpacity{"CV Images Opacity", 64, 1,255};
		ofParameter<bool> bFillArea{"Fill Blob Area", false};
		ofParameter<int> fadeLevel{"Fade Level", 1, 1,255};
		ofParameter<int> brushScale{"Brush Scale",5,25,100};
		ofParameter<int> thresholdAmount{"Image Threshold",230,0,255};
		ofParameter<int> maxArea{"Max Tracking Size", 150,1,500};
		ofParameter<int> minArea{"Min Tracking Size", 30,1,500};
		ofParameter<int> contourThreshold{"Contour Threshold",100,1,255};
	
	
		ofParameterGroup imageProc{ "Image Processing",bShowCv,bShowCvCalibration,cvImagesOpacity,bFillArea,fadeLevel,brushScale,thresholdAmount,maxArea,minArea,contourThreshold };
	
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
	
		ImageSelection imageSelection;
	
	private:
		ofVideoPlayer player;
		
		ofVideoGrabber grabber;
		ofFbo resultingImage;
	
		Mat selectedImage;
		Mat croppedImage;
	
		IMAGE_PROCESSING_MODE currentMode;
		ofxImGui::Gui gui;
		ofImage debugImg;
		ofImage preProcessedImage;
		ofImage processedImage;
		ofxCv::ContourFinder contourFinder;
	
		void mousePressed(ofMouseEventArgs &e);
		void mouseReleased(ofMouseEventArgs &e);
		void mouseDragged(ofMouseEventArgs &e);
};

#endif /* ImageProcessing_h */
