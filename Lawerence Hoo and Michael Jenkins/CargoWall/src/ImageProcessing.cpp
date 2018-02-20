//
//  ImageProcessing.cpp
//  CargoWall
//
//  Created by David Haylock on 24/01/2018.
//

#include "ImageProcessing.h"

//--------------------------------------------------------------
void ImageProcessing::setup() {
	setMode(LIVE_PROCESSING);
	
	resultingImage.allocate(ofGetWidth(), ofGetHeight());
	resultingImage.begin();
	ofClear(0, 0, 0, 255);
	resultingImage.end();
	preProcessedImage.allocate(640, 480, OF_IMAGE_GRAYSCALE);
	processedImage.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);

	debugImg.load("brush.png");
	
}

//--------------------------------------------------------------
void ImageProcessing::update() {
	ofEnableAlphaBlending();
	
	resultingImage.begin();
	
	ofSetColor(0, 0, 0,iFadeLevel);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	switch (currentMode) {
		case DEBUG_PROCESSING: {
			ofSetColor(255, 255, 255);
			float scaleX = debugImg.getWidth()/2 * float(iBrushScale * 0.01);
			float scaleY = debugImg.getHeight()/2 * float(iBrushScale * 0.01);
			float scaleW = debugImg.getWidth() * float(iBrushScale * 0.01);
			float scaleH = debugImg.getHeight() * float(iBrushScale * 0.01);
			debugImg.draw(ofGetMouseX()-(scaleX), ofGetMouseY()-(scaleY),scaleW,scaleH);
		}
		break;
		case VIDEO_PROCESSING: {
			player.update();
			if (player.isFrameNew()) {
				convertColor(player, preProcessedImage, CV_RGB2GRAY);
				threshold(preProcessedImage, thresholdAmount);
				GaussianBlur(preProcessedImage, 5);
				contourFinder.setMinAreaRadius(iMinArea);
				contourFinder.setMaxAreaRadius(iMaxArea);
				contourFinder.setThreshold(thresholdAmount);
				contourFinder.findContours(preProcessedImage);
				Mat n,p;
				copy(preProcessedImage,n);
				distanceTransform(n, p, CV_DIST_C, 3);
				normalize(p, p);
				copy(p,preProcessedImage);
				
				preProcessedImage.update();
				convertColor(preProcessedImage, processedImage, CV_GRAY2RGBA);
				processedImage.update();
			}
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			ofSetColor(255,255,255,200);
			processedImage.draw(0, 0,resultingImage.getWidth(),resultingImage.getHeight());
			ofDisableBlendMode();
		}
		break;
		case LIVE_PROCESSING: {
			grabber.update();
			if (grabber.isFrameNew()) {
				calibrationManager.update(toCv(grabber));
				convertColor(grabber, preProcessedImage, CV_RGB2GRAY);
				threshold(preProcessedImage, thresholdAmount);
				GaussianBlur(preProcessedImage, 5);
				contourFinder.setMinAreaRadius(iMinArea);
				contourFinder.setMaxAreaRadius(iMaxArea);
				contourFinder.setThreshold(thresholdAmount);
				contourFinder.findContours(preProcessedImage);
				Mat n,p;
				copy(preProcessedImage,n);
				distanceTransform(n, p, CV_DIST_C, 3);
				normalize(p, p);
				copy(p,preProcessedImage);

				preProcessedImage.update();
				convertColor(preProcessedImage, processedImage, CV_GRAY2RGBA);
				processedImage.update();
			}
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			ofSetColor(255,255,255,200);
			processedImage.draw(0, 0,resultingImage.getWidth(),resultingImage.getHeight());
			ofDisableBlendMode();
			}
			break;
		default:break;
	}
	
	resultingImage.end();
}

//--------------------------------------------------------------
void ImageProcessing::draw() {
	if (bShowCv) {
		drawCv();
		calibrationManager.draw();
	}
	
}

//--------------------------------------------------------------
void ImageProcessing::drawGui() {

}

//--------------------------------------------------------------
void ImageProcessing::drawCv() {
	ofPushStyle();
		ofPushMatrix();
			ofScale((ofGetWidth()/grabber.getWidth()), (ofGetHeight()/grabber.getHeight()));
			ofSetColor(255,iCvImagesOpacity);
			grabber.draw(0, 0, 640, 480);
			ofSetColor(175,0,255);
			contourFinder.draw();
	
			if( bFillArea ) {
				for (int i = 0; i < contourFinder.size(); i++) {
					int p = contourFinder.getContour(i).size();
					ofFill();
					ofSetColor(255, 255, 0);
					ofBeginShape();
					for (int j = 0; j < p; j++) {
						ofVertex(contourFinder.getContour(i)[j].x, contourFinder.getContour(i)[j].y);
					}
					ofEndShape(true);
				}
			}
		ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
ofPoint ImageProcessing::getBiggestCoordinate() {
	if (contourFinder.size() > 0)
		return toOf(contourFinder.getCenter(0));
	
	return ofPoint(0,0);
}

//--------------------------------------------------------------
void ImageProcessing::setMode(IMAGE_PROCESSING_MODE newMode) {
	
	switch (newMode) {
		case DEBUG_PROCESSING: {
			grabber.close();
			player.close();
			
		}
		break;
		case VIDEO_PROCESSING: {
			grabber.close();
			player.load("outofbounds.mov");
			processedImage.allocate(player.getWidth(), player.getHeight(), OF_IMAGE_GRAYSCALE);
			player.setSpeed(0.75);
			player.play();
		}
		break;
		case LIVE_PROCESSING: {
			player.close();
			grabber.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
			grabber.setup(640, 480);
			grabber.getGrabber<ofxPS3EyeGrabber>()->setAutogain(true);
			grabber.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(true);
			grabber.getGrabber<ofxPS3EyeGrabber>()->setDesiredFrameRate(60);
			
			calibrationManager.setup(toCv(grabber));
			
			cameraBrightness.addListener(this, &ImageProcessing::onBrightness);
			cameraExposure.addListener(this, &ImageProcessing::onExposure);
			cameraGain.addListener(this, &ImageProcessing::onGain);
			cameraContrast.addListener(this, &ImageProcessing::onContrast);
			cameraFramerate.addListener(this, &ImageProcessing::onFramerate);
			cameraVFlip.addListener(this, &ImageProcessing::onVFlip);
			cameraHFlip.addListener(this, &ImageProcessing::onHFlip);
			
			processedImage.allocate(grabber.getWidth(), grabber.getHeight(), OF_IMAGE_GRAYSCALE);
		}
		break;
			
		default:
			break;
	}
	
	currentMode = newMode;
}

//--------------------------------------------------------------
ofTexture ImageProcessing::getMaskImage() {
	return resultingImage.getTexture();
}

//--------------------------------------------------------------
void ImageProcessing::onBrightness(int &val) {
	grabber.getGrabber<ofxPS3EyeGrabber>()->setBrightness(val);
}

//--------------------------------------------------------------
void ImageProcessing::onContrast(int &val) {
	grabber.getGrabber<ofxPS3EyeGrabber>()->setContrast(val);
}

//--------------------------------------------------------------
void ImageProcessing::onGain(int &val) {
	grabber.getGrabber<ofxPS3EyeGrabber>()->setGain(val);
}

//--------------------------------------------------------------
void ImageProcessing::onExposure(int &val) {
	grabber.getGrabber<ofxPS3EyeGrabber>()->setExposure(val);
}

//--------------------------------------------------------------
void ImageProcessing::onFramerate(int &val) {
	grabber.getGrabber<ofxPS3EyeGrabber>()->setDesiredFrameRate(val);
}

//--------------------------------------------------------------
void ImageProcessing::onVFlip(bool &val) {
	grabber.getGrabber<ofxPS3EyeGrabber>()->setVerticalFlip(val);
}

//--------------------------------------------------------------
void ImageProcessing::onHFlip(bool &val) {
	grabber.getGrabber<ofxPS3EyeGrabber>()->setHorizontalFlip(val);
}
