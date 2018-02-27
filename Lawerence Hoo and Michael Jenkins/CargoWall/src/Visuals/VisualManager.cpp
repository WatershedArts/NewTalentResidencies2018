//
//  VisualManager.cpp
//  CargoWall
//
//  Created by David Haylock on 22/02/2018.
//

#include "VisualManager.h"

//--------------------------------------------------------------
void VisualManager::setup() {
//	currentMode = VIDEOS;
	videoHandler.loadDirectory("videos");
}

//--------------------------------------------------------------
void VisualManager::update() {
	
}

//--------------------------------------------------------------
void VisualManager::draw() {
	switch (currentMode) {
		case 0: // Think this is Vidoes
			videoHandler.draw();
			break;
		default:
			break;
	}
}
