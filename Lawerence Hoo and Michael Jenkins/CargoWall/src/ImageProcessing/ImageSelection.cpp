//
//  ImageSelection.cpp
//  CargoWall
//
//  Created by David Haylock on 21/02/2018.
//

#include "ImageSelection.h"

//----------------------------------------------------
ImageSelection::ImageSelection(){
	selected = -1;
	quadName = "QUAD_";
}

//----------------------------------------------------
void ImageSelection::setup(string _quadName){
	quadName = _quadName;
}

//----------------------------------------------------
void ImageSelection::releaseAllPoints(){
	selected = -1;
}

//----------------------------------------------------
//these should be in the range x(0-maxX) y(0-maxH)
//with 	width  = maxW
//		height = maxH
void ImageSelection::setQuadPoints( ofPoint * inPts ){
	
	for(int i = 0; i < 4; i++){
		srcZeroToOne[i].x	= inPts[i].x;
		srcZeroToOne[i].y	= inPts[i].y;
		
		if(srcZeroToOne[i].x > 1) srcZeroToOne[i].x = 1;
		if(srcZeroToOne[i].y > 1) srcZeroToOne[i].y = 1;
	}
}

//----------------------------------------------------
void ImageSelection::readFromFile(string filePath){
	
	xml.loadFile(filePath);
	loadSettings();
}

//----------------------------------------------------
void ImageSelection::loadSettings(){
	
	string str;
	
	for(int i = 0; i < 4; i++){

		str = quadName;
		str += ofToString(i);
		
		srcZeroToOne[i].x = xml.getValue(str+"_X", srcZeroToOne[i].x);
		srcZeroToOne[i].y = xml.getValue(str+"_Y", srcZeroToOne[i].y);
		
		if(srcZeroToOne[i].x > 1) srcZeroToOne[i].x = 1;
		if(srcZeroToOne[i].y > 1) srcZeroToOne[i].y = 1;
		
	}
	
}

//----------------------------------------------------
bool ImageSelection::selectPoint(float x, float y, float offsetX, float offsetY, float width, float height, float hitArea){
	
	//make sure selected is -1 unless we really find a point
	selected = -1;
	
	if(width == 0 || height == 0 || x < offsetX || x > offsetX + width || y < offsetY ||  y > offsetY + height){
		//then we are out of our possible quad area
		//so we ignore :)
		return false;
	}
	
	//lets get it in range x(0 - width) y(0 - height)
	float px = x - offsetX;
	float py = y - offsetY;
	
	//now get in 0-1 range
	px /= width;
	py /= height;
	
	hitArea /= width;
	
	//we want to store the smallest distance found
	//because in the case when two points are in the
	//hit area we want to pick the closet
	float storeDist = 9999999.0;
	
	for(int i = 0; i < 4; i++){
		float dx = fabs(px -  srcZeroToOne[i].x);
		float dy = fabs(py -  srcZeroToOne[i].y);
		
		float dist = sqrt(dx*dx + dy*dy);
		
		if(dist > hitArea)continue;
		
		if(dist < storeDist){
			selected = i;
			storeDist = dist;
		}
	}
	
	if(selected != -1){
		srcZeroToOne[selected].x 	= px;
		srcZeroToOne[selected].y 	= py;
		srcScaled[selected].x		= px;
		srcScaled[selected].y		= py;
		return true;
	}
	
	return false;
}

//----------------------------------------------------
bool ImageSelection::updatePoint(float x, float y, float offsetX, float offsetY, float width, float height){
	
	//nothing to update
	if(selected == -1) return false;
	
	if(width == 0 || height == 0){
		//dangerous so we ignore :)
		return false;
	}
	
	if( x < offsetX ) 			x = offsetX;
	if( x > offsetX + width ) 	x = offsetX + width;
	if( y < offsetY ) 			y = offsetY;
	if( y > offsetY + height) 	y = offsetY + height;
	
	//lets get it in range x(0 - width) y(0 - height)
	float px = x - offsetX;
	float py = y - offsetY;
	
	//now get in 0-1 range
	px /= width;
	py /= height;
	
	srcZeroToOne[selected].x 	= px;
	srcZeroToOne[selected].y 	= py;
	srcScaled[selected].x		= px;
	srcScaled[selected].y		= py;
	
	return true;
}

//----------------------------------------------------
//returns pts to width by height range
ofPoint * ImageSelection::getScaledQuadPoints(float width, float height){
	
	for(int i = 0; i < 4; i++){
		srcScaled[i].x = srcZeroToOne[i].x * width;
		srcScaled[i].y = srcZeroToOne[i].y * height;
	}
	
	return srcScaled;
}


//----------------------------------------------------
//returns pts in 0-1 scale
ofPoint * ImageSelection::getQuadPoints(){
	return srcZeroToOne;
}


//----------------------------------------------------
void ImageSelection::saveToFile(string filePath, string newQuadName){
	string str;
	
	for(int i = 0; i < 4; i++){
		
		str = newQuadName;
		str += ofToString(i);
		
		xml.setValue(str+"_X", srcZeroToOne[i].x);
		xml.setValue(str+"_Y", srcZeroToOne[i].y);
	}
	
	xml.saveFile(filePath);
}

//----------------------------------------------------
void ImageSelection::saveToFile(string filePath){
	saveToFile(filePath, quadName);
}

//----------------------------------------------------
void ImageSelection::draw(float x, float y, float width, float height, int red, int green, int blue, int thickness,bool grid){
	
	getScaledQuadPoints(width, height);
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	ofNoFill();
	
	ofSetColor(253, 114, 114);
	glLineWidth(thickness);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < 4; i++){
		glVertex2f(srcScaled[i].x, srcScaled[i].y);
	}
	glEnd();
	
	glLineWidth(2);
	ofSetRectMode(OF_RECTMODE_CENTER);
	for(int i = 0; i < 4; i++){
		string message = "";
		ofColor c = ofColor(253, 114, 114);
		if( i == 0){
			ofSetColor(c);
			message = "Top Left";
		}
		if( i == 1){
			ofSetColor(c);
			message = "Top Right";
		}
		if( i == 2){
			ofSetColor(c);
			message = "Bottom Right";
		}
		if( i == 3){
			ofSetColor(c);
			message = "Bottom Left";
		}
		
		ofDrawBitmapString(message, srcScaled[i].x+20, srcScaled[i].y+10);
		ofDrawRectangle(srcScaled[i].x, srcScaled[i].y, 30, 30);
	}
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofFill();
	glPopMatrix();
}

//----------------------------------------------------
void ImageSelection::draw(float x, float y, float width, float height){
	
	//default to a think yellow line
	draw(x, y, width, height, 255, 255, 0, 1,true);
}

