//
//  ImageProcessing.cpp
//  CargoWall
//
//  Created by David Haylock on 24/01/2018.
//

#include "ImageProcessing.h"

//--------------------------------------------------------------
void ImageProcessing::setup() {
	
	// Go Straight into Live Mode
	setMode(LIVE_PROCESSING);
	
	// Make an frame buffer the same size of the screen
	resultingImage.allocate(ofGetWidth(), ofGetHeight());
	resultingImage.begin();
	// Clear it
	ofClear(0, 0, 0, 255);
	resultingImage.end();
	
	// Make Two Image containers
	preProcessedImage.allocate(640, 480, OF_IMAGE_GRAYSCALE);
	processedImage.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);

	// Load the Debug Brush
	debugImg.load("brush.png");
	
	// Add Listeners for the Mouse Events
	ofAddListener(ofEvents().mousePressed, this,&ImageProcessing::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this,&ImageProcessing::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this,&ImageProcessing::mouseReleased);
	
	// Setup the Image Selection System
	imageSelection.setup("Dev");
	
	// Generate Default Points
	ofPoint p[4];
	p[0] = ofPoint(0,0);
	p[1] = ofPoint(640,0);
	p[2] = ofPoint(640,480);
	p[3] = ofPoint(0,480);
	imageSelection.setQuadPoints(p);
}

//--------------------------------------------------------------
void ImageProcessing::update() {
	
	// This is the Hard bit to explain so bare with me here
	// Enable Alpha images
	ofEnableAlphaBlending();
	
	// Open the Framebuffer
	resultingImage.begin();
	
	// This allows the buffer to gradually fadeout
	ofSetColor(0, 0, 0,fadeLevel);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	
	// Modes
	// Debug Uses a brush texture and draws it into the buffer.
	switch (currentMode) {
		case DEBUG_PROCESSING: {
			ofSetColor(255, 255, 255);
			float scaleX = debugImg.getWidth()/2 * float(brushScale * 0.01);
			float scaleY = debugImg.getHeight()/2 * float(brushScale * 0.01);
			float scaleW = debugImg.getWidth() * float(brushScale * 0.01);
			float scaleH = debugImg.getHeight() * float(brushScale * 0.01);
			debugImg.draw(ofGetMouseX()-(scaleX), ofGetMouseY()-(scaleY),scaleW,scaleH);
		}
		break;
		case LIVE_PROCESSING: {
			// Update the Grabber
			grabber.update();
			
			// Check if the Grabber has a new frame
			if (grabber.isFrameNew()) {
				
				// Transfer the frame to a Mat object
				selectedImage = toCv(grabber);
				
				// Get the Selection Frame
				ofPoint topLeft = imageSelection.getScaledQuadPoints(640, 480)[0];
				ofPoint topRight = imageSelection.getScaledQuadPoints(640, 480)[1];
				ofPoint bottomRight = imageSelection.getScaledQuadPoints(640, 480)[2];
				ofPoint bottomLeft = imageSelection.getScaledQuadPoints(640, 480)[3];
				
				// Create two new vectors of points
				// One for the source of the selection image one for where itll select into
				vector <Point2f> dst;
				vector <Point2f> src;
				
				// Default Frame
				src.push_back(Point2f(0,0));
				src.push_back(Point2f(640,0));
				src.push_back(Point2f(640,480));
				src.push_back(Point2f(0,480));
				
				// Selection Frame
				dst.push_back(Point2f(topLeft.x,topLeft.y));
				dst.push_back(Point2f(topRight.x,topRight.y));
				dst.push_back(Point2f(bottomRight.x,bottomRight.y));
				dst.push_back(Point2f(bottomLeft.x,bottomLeft.y));
				
				// Do the homography to transfer the matrix
				Mat homography = findHomography(Mat(dst), Mat(src));
				
				// Warp the Image
				cv::warpPerspective(selectedImage, croppedImage, homography, selectedImage.size(),CV_INTER_LINEAR);
				
				// Convert the Color Image to Grayscale
				convertColor(croppedImage, preProcessedImage, CV_RGB2GRAY);
				
				// Threshold the Mat
				threshold(preProcessedImage, thresholdAmount);
				
				// Blur the Mat
				GaussianBlur(preProcessedImage, 5);
				
				// Do Contour Detection
				contourFinder.setMinAreaRadius(minArea);
				contourFinder.setMaxAreaRadius(maxArea);
				contourFinder.setThreshold(thresholdAmount);
				
				// Find the Contours
				contourFinder.findContours(preProcessedImage);
				
				// Create Two Mats
				Mat n,p;
				
				// Copy the processed mat into a container
				copy(preProcessedImage,n);
				
				// Distance the image
				distanceTransform(n, p, CV_DIST_C, 3);
				
				// Convert to 0 - 1
				normalize(p, p);
				
				// Copy it back
				copy(p,preProcessedImage);

				// Update the Image Container
				preProcessedImage.update();
				
				// Convert the Image to be a Alpha channel enabled
				convertColor(preProcessedImage, processedImage, CV_GRAY2RGBA);
				
				// Update the Image
				processedImage.update();
			}
			
			// Enable a more aggressive blend mode
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			ofSetColor(255,255,255,200);
			processedImage.draw(0, 0,resultingImage.getWidth(),resultingImage.getHeight());
			ofDisableBlendMode();
			
			}
			break;
		default:
			break;
	}
	// Close the Framebuffer
	resultingImage.end();
}

//--------------------------------------------------------------
void ImageProcessing::draw() {
	
	if (bShowCv) {
		drawCv();
	}
	
	if(bShowCvCalibration) {
		ofSetColor(ofColor::white);
		drawMat(selectedImage, 0, 0);
		imageSelection.draw(0, 0, 640, 480, 225, 255, 255, 3, true);
		
		ofSetColor(ofColor::white);
		stringstream ss;
		
		ss << "Hello!" << endl;
		ss << "This is the calibration screen." << endl;
		ss << "The theory is as follows." << endl;
		ss << "We need to be able to tell the projector what the camera can see." << endl;
		ss << "By doing this we are able to align the camera coordinates to the projectors coordinates." << endl;
		ss << "Using the Mouse, click and grab the four corners of the box to the left" << endl;
		ss << "and align with the edge of the projected image." << endl;

		ofDrawBitmapString(ss.str(), 650, 25);
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
			ofSetColor(255,cvImagesOpacity);
			drawMat(croppedImage, 0, 0);
			ofSetColor(37, 204, 247);
			contourFinder.draw();
	
			if( bFillArea ) {
				for (int i = 0; i < contourFinder.size(); i++) {
					int p = contourFinder.getContour(i).size();
					ofFill();
					ofSetColor(37, 204, 247,150);
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
		case LIVE_PROCESSING: {
			player.close();
			grabber.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
			grabber.setup(640, 480);
			grabber.getGrabber<ofxPS3EyeGrabber>()->setAutogain(true);
			grabber.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(true);
			grabber.getGrabber<ofxPS3EyeGrabber>()->setDesiredFrameRate(60);
			
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
void ImageProcessing::mousePressed(ofMouseEventArgs &e) {
	imageSelection.selectPoint(e.x, e.y, 0, 0, 640, 480, 30);
}

//--------------------------------------------------------------
void ImageProcessing::mouseDragged(ofMouseEventArgs &e) {
	imageSelection.updatePoint(e.x, e.y, 0, 0, 640, 480);
}

//--------------------------------------------------------------
void ImageProcessing::mouseReleased(ofMouseEventArgs &e) {
	imageSelection.releaseAllPoints();
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
