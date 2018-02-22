//
//  ImageSelection.h
//  CargoWall
//
//  Original I think was from Theo Watson
//  Recreated by David Haylock on 21/02/2018.
//

#ifndef ImageSelection_h
#define ImageSelection_h
#include <stdio.h>

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ImageSelection {
	
public:
	ImageSelection();
	
	void setup(string _quadName);
	void readFromFile(string filePath);
	void loadSettings();
	
	void releaseAllPoints();
	void setQuadPoints( ofPoint * inPts );
	
	bool selectPoint(float x, float y, float offsetX, float offsetY, float width, float height, float hitArea);
	bool updatePoint(float x, float y, float offsetX, float offsetY, float width, float height);
	
	
	ofPoint * getScaledQuadPoints(float width, float height);
	ofPoint * getQuadPoints();
	
	void saveToFile(string filePath, string newQuadName);
	void saveToFile(string filePath);
	void draw(float x, float y, float width, float height, int red, int green, int blue, int thickness,bool grid);
	void draw(float x, float y, float width, float height);
	
	
protected:
	ofxXmlSettings xml;
	ofPoint srcZeroToOne[4];
	ofPoint srcScaled[4];
	string quadName;
	int selected;
};

#endif /* ImageSelection_h */
