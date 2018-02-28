//
//  Photos.h
//  CargoWall
//
//  Created by David Haylock on 28/02/2018.
//

#ifndef Photos_h
#define Photos_h
#include "ofMain.h"

class Photos {
	public:
		void loadPhotos(string dirPath) {
			bool b = false;
			if(!b) {
				ofDirectory dir(dirPath);
				dir.sort();
				
				for(int i = 0; i < (int)dir.size(); i++){
					ofImage *v;
					v = new ofImage();
					v->load(dir.getPath(i));
					photos.push_back(v);
				}
			}
			
			for(int y = 0; y < 5; y++) {
				for(int x = 0; x < 5; x++) {
					int offset = 10;
					if (y % 2) {
						offset = 150;
					}
					photoPositions.push_back(ofVec2f(offset+(x*300),10+(y*200)));
				}
			}
			
			defaultImage.load(dirPath+"/default avatar_01.png");
		}
	
		void draw() {
			ofSetColor(255,255,255);
			for(int i = 0; i < photoPositions.size(); i++) {
				defaultImage.draw(photoPositions[i].x,photoPositions[i].y,125,125);
			}
		}
	
	private:
		vector<ofImage*> photos;
		ofImage defaultImage;
		vector<ofVec3f> photoPositions;
};

#endif /* Photos_h */
