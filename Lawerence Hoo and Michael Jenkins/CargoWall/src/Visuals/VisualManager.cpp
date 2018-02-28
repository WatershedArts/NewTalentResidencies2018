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
	videoHandler.loadDirectory("Visuals/videos");
	poemHandler.loadPoems("Visuals/poems");
	photoHandler.loadPhotos("Visuals/photos");
}

//--------------------------------------------------------------
void VisualManager::update() {
	
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
		default:
			break;
	}
}
