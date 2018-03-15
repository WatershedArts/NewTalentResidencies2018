//
//  Videos.h
//  CargoWall
//
//  Created by David Haylock on 26/02/2018.
//

#ifndef Videos_h
#define Videos_h

#include "ofMain.h"

class Cargo_VideoPlayer: public ofVideoPlayer {
	public:
	
		Cargo_VideoPlayer() {
			isDone = false;
		}
	
		bool isVideoDone() {
			return isDone;
		}
	
		void unlockVideo() {
			isDone = false;
		}
	
		bool isDone;
		ofEvent<string> videoFinished;
};

class Videos {
	public:
		Videos() {}
		~Videos() {}
	
		void draw() {
			if (!videoFiles.empty()) {
				videoFiles[currentVideoIndex].update();
				videoFiles[currentVideoIndex].draw(0,0,ofGetWidth(),ofGetHeight());
			}
		}
	
		void loadDirectory(string dirPath) {
			
			videoFiles.clear();
			ofDirectory dir(dirPath);
			dir.sort();
			
			for(int i = 0; i < (int)dir.size(); i++){
				Cargo_VideoPlayer v;
				v.load(dir.getPath(i));
				ofAddListener(v.videoFinished,this,&Videos::videoHasFinished);
				
				videoFiles.push_back(v);
			}
			currentVideoIndex = 0;
			videoFiles[currentVideoIndex].play();
		}
	
		void videoHasFinished(string &val) {
			cout << val << endl;
		}
	
		vector <string> getVideoNames() {
			return videoFilesNames;
		}
	
		void nextVideo() {
			if(currentVideoIndex != videoFiles.size()-1) {
				currentVideoIndex++;
				videoFiles[currentVideoIndex].play();
			}
		}
	
		void previousVideo() {
			if(currentVideoIndex != 0) {
				currentVideoIndex--;
				videoFiles[currentVideoIndex].play();
			}
		}
	
		int currentVideoIndex;
		vector <Cargo_VideoPlayer> videoFiles;
		vector <string> videoFilesNames;
};


#endif /* Videos_h */
