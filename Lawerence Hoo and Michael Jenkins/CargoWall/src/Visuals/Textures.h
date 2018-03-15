//
//  Textures.h
//  CargoWall
//
//  Created by David Haylock on 08/03/2018.
//

#ifndef Textures_h
#define Textures_h

class Textures {
	public:
		Textures() {}
		~Textures() {}
	
		vector<ofImage> images;
		int currentImageIndex;
	
		void loadTextures(string dirPath) {
			images.clear();
			ofDirectory dir(dirPath);
			dir.sort();
			
			for(int i = 0; i < (int)dir.size(); i++){
				ofImage img;
				img.load(dir.getPath(i));
			
				images.push_back(img);
			}
			currentImageIndex = 0;
		}
	
		void nextTexture() {
			if(currentImageIndex != images.size()-1) {
				currentImageIndex++;
			}
		}
	
		void previousTexture() {
			if(currentImageIndex != 0) {
				currentImageIndex--;
			}
		}
	
		void draw() {
			if (!images.empty()) {
				images[currentImageIndex].draw(0,0,ofGetWidth(),ofGetHeight());
			}
		}
	
	
};
#endif /* Textures_h */
