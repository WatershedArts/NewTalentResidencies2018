//
//  VisualManager.cpp
//  CargoWall
//
//  Created by David Haylock on 22/02/2018.
//

#include "VisualManager.h"

bool isLocked = false;
//--------------------------------------------------------------
void VisualManager::setup() {
//	currentMode = VIDEOS;
	videoHandler.loadDirectory("Visuals/videos");
	poemHandler.loadPoems("Visuals/poems");
	photoHandler.loadPhotos("Visuals/photos");
	animatedPoemHandler.getPoems(poemHandler.getPoems(), poemHandler.font);
}

//--------------------------------------------------------------
void VisualManager::update(float x, float y) {
	if(ofDist(ofGetWidth()/2, ofGetHeight()/2, x, y) < 40 && !isLocked) {
		animatedPoemHandler.nextLine();
		isLocked = true;
	}
	else if(ofDist(ofGetWidth()/2, ofGetHeight()/2, x, y) > 40 && isLocked ) {
		isLocked = false;
	}
}

//--------------------------------------------------------------
void VisualManager::draw() {
	switch (currentMode) {
		case 0:
			videoHandler.draw();
			break;
		case 1:
			poemHandler.draw();
			break;
		case 2:
			photoHandler.draw();
			break;
		case 3:
			animatedPoemHandler.draw();
			break;
		default:
			break;
	}
}
