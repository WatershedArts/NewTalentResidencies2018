//
//  VideoHandler.cpp
//  InstagramTv
//
//  Created by David Haylock on 02/03/2018.
//

#include "VideoHandler.h"

float scaledWidth = 0;
float scaledHeight = 0;
float paddingHeight = 0;

//--------------------------------------------------------------
void VideoHandler::setup(int graceTime,int tweenTime) {
	// We want to scale the images correctly
	// So we take our width over the height
	// to get the multiplier
	float aspectRatio =  9.0 / 16.0;
	
	cout << "Aspect Ratio is:" <<  aspectRatio << endl;
	paddingHeight = 0;
	
	scaledHeight = ofGetHeight() - (paddingHeight*2);
	
	scaledWidth = scaledHeight * aspectRatio;
	
	cout << "Width: " << scaledWidth << endl;
	cout << "Height: " << scaledHeight << endl;
	
	currentPosition = 0;
	currentVideo = 0;
	replayVideoId = 0;
	indexOfNextContainerToLoad = 0;
	whichSequence = 0;
	bStartSequence = true;
	
	this->graceTime = graceTime;
	this->tweenTime = tweenTime;
	
	bFirst = true;
	mainPosition = ofGetWidth()/2 - (scaledWidth/2);
	waitingPosition = ofGetWidth()+400;
	donePosition = -400;
	
#ifndef TARGET_RASPBERRY_PI
	videos.assign(3, new ofVideoPlayer_Ext());
#else
	videos.assign(3, new ofxOMXPlayer());
#endif

	noVideoTimer.setup(1000, "Start Timer", false);
	
	ofAddListener(noVideoTimer.timerFinished,this,&VideoHandler::noVideoTimerFinished);
	
	graceTimer.setup(graceTime, "Grace Timer", false);
	graceTimer.start();
	ofAddListener(graceTimer.timerStarted,this,&VideoHandler::graceTimerStarted);
	ofAddListener(graceTimer.timerFinished,this,&VideoHandler::graceTimerFinished);
	ofAddListener(graceTimer.timerInvalidated,this,&VideoHandler::graceTimerInvalidated);
	
	ofAddListener(tween_one.end_E, this, &VideoHandler::tweenComplete);
	ofAddListener(tween_two.end_E, this, &VideoHandler::tweenComplete);
	ofAddListener(tween_three.end_E, this, &VideoHandler::tweenComplete);
	
	tween_one.setParameters(100, sine, ofxTween::easeInOut, waitingPosition, waitingPosition, tweenTime, 500);
	tween_two.setParameters(101, sine, ofxTween::easeInOut, waitingPosition, waitingPosition, tweenTime, 500);
	tween_three.setParameters(102, sine, ofxTween::easeInOut, waitingPosition, waitingPosition, tweenTime, 500);
}

//--------------------------------------------------------------
void VideoHandler::loadVideo(int index,string videoFile) {

#ifndef TARGET_RASPBERRY_PI
	videos[index] = new ofVideoPlayer_Ext();
	videos[index]->load(videoFile);
	videos[index]->setLoopState(OF_LOOP_NONE);
	
	ofAddListener(videos[index]->videoFinished, this, &VideoHandler::hasVideoFinished);
	
	videos[index]->firstFrame();
	videos[index]->update();
	videos[index]->stop();
#else
	videos[index].close();
//	videos[index] = new ofxOMXPlayer();
	settings.videoPath = videoFile;
	settings.useHDMIForAudio = false;	//default true
	settings.enableLooping = false;		//default true
	settings.enableTexture = true;		//default true
	settings.listener = this;			//this app extends ofxOMXPlayerListener so it will receive events ;
	videos[index]->setup(settings);
//	videos[index]->restartMovie();
//	videos[index]->togglePause();

#endif

}

//--------------------------------------------------------------
void VideoHandler::close() {
	ofRemoveListener(graceTimer.timerStarted,this,&VideoHandler::graceTimerStarted);
	ofRemoveListener(graceTimer.timerFinished,this,&VideoHandler::graceTimerFinished);
	ofRemoveListener(graceTimer.timerInvalidated,this,&VideoHandler::graceTimerInvalidated);
	
	ofRemoveListener(tween_one.end_E, this, &VideoHandler::tweenComplete);
	ofRemoveListener(tween_two.end_E, this, &VideoHandler::tweenComplete);
	ofRemoveListener(tween_three.end_E, this, &VideoHandler::tweenComplete);
}

//--------------------------------------------------------------
void VideoHandler::sequenceOne() {
	tween_one.setParameters(1, sine, ofxTween::easeInOut, waitingPosition, mainPosition, tweenTime, 500);
	tween_three.setParameters(-3, sine, ofxTween::easeInOut, mainPosition, donePosition, tweenTime, 0);
	if( !videoData.empty() )
		loadVideo(0, videoData[currentVideo].filename);
}

//--------------------------------------------------------------
void VideoHandler::sequenceTwo() {
	tween_one.setParameters(-1, sine, ofxTween::easeInOut, mainPosition, donePosition, tweenTime, 0);
	tween_two.setParameters(2, sine, ofxTween::easeInOut, waitingPosition, mainPosition, tweenTime, 500);
	if( !videoData.empty() )
		loadVideo(1, videoData[currentVideo].filename);
}

//--------------------------------------------------------------
void VideoHandler::sequenceThree() {
	tween_two.setParameters(-2, sine, ofxTween::easeInOut, mainPosition, donePosition, tweenTime, 0);
	tween_three.setParameters(3, sine, ofxTween::easeInOut, waitingPosition, mainPosition, tweenTime, 500);
	if( !videoData.empty() )
		loadVideo(2, videoData[currentVideo].filename);
}

//--------------------------------------------------------------
void VideoHandler::nextVideo() {
	
	if (whichSequence > 3) { whichSequence = 0; }
	
	switch (whichSequence) {
		case 0: sequenceOne(); break;
		case 1: sequenceTwo(); break;
		case 2: sequenceThree(); break;
		default:break;
	}
	currentVideo++;
}

//--------------------------------------------------------------
void VideoHandler::replayVideo() {
	graceTimer.invalidate();
	videoData[currentVideo].plays++;
	
#ifndef TARGET_RASPBERRY_PI
	videos[replayVideoId]->firstFrame();
	videos[replayVideoId]->play();
#else
	videos[replayVideoId]->restartMovie();
#endif
}

//--------------------------------------------------------------
void VideoHandler::draw(bool bDebug) {
	if(bDebug) {
		ofNoFill();
		ofSetColor(ofColor::white);
		ofDrawRectangle(ofGetWidth()/2-scaledWidth/2, paddingHeight, scaledWidth, scaledHeight);
	}

	if (!videos.empty()) {
#ifndef TARGET_RASPBERRY_PI
		videos[0]->update();
		videos[1]->update();
		videos[2]->update();
#else
#endif
		
		if(!bDebug) {
			ofFill();
			ofSetColor(ofColor::white);
			videos[0]->draw(tween_one.update(), paddingHeight, scaledWidth, scaledHeight);
		}
		else {
			ofFill();
			ofSetColor(ofColor::white);
			videos[0]->draw(ofGetWidth()/2 - scaledWidth, paddingHeight, scaledWidth/2, scaledHeight/2);
			ofNoFill();
			ofSetColor(0, 184, 148,255);
			ofDrawRectangle(tween_one.update(), paddingHeight, scaledWidth, scaledHeight);
		}

		if(!bDebug) {
			ofFill();
			ofSetColor(ofColor::white);
			videos[1]->draw(tween_two.update(), paddingHeight, scaledWidth, scaledHeight);
		}
		else {
			ofFill();
			ofSetColor(ofColor::white);
			videos[1]->draw(ofGetWidth()/2 - scaledWidth/4, paddingHeight, scaledWidth/2, scaledHeight/2);
			ofNoFill();
			ofSetColor(232, 67, 147,255);
			ofDrawRectangle(tween_two.update(),paddingHeight , scaledWidth, scaledHeight);
		}
		
		if(!bDebug) {
			ofFill();
			ofSetColor(ofColor::white);
			videos[2]->draw(tween_three.update(), paddingHeight, scaledWidth, scaledHeight);
		}
		else {
			ofFill();
			ofSetColor(ofColor::white);
			videos[2]->draw(ofGetWidth()/2 + scaledWidth/2, paddingHeight, scaledWidth/2, scaledHeight/2);
			ofNoFill();
			ofSetColor(253, 203, 110,255);
			ofDrawRectangle(tween_three.update(), paddingHeight, scaledWidth, scaledHeight);
		}
	}
	
	if(bDebug) {
		for (int i = 0; i < videoData.size(); i++) {
			string file = ofSplitString(videoData[i].filename,"/").back();
			if(i == currentVideo) {
				ofSetColor(232, 67, 147);
			}
			else {
				ofSetColor(ofColor::white);
			}
			ofDrawBitmapString("Id: " + ofToString(i) + " File: " + file + " P: " +ofToString(videoData[i].plays), 10,(ofGetHeight()/2+30) + (i*15));
		}
	}
}

//--------------------------------------------------------------
void VideoHandler::addNewVideoToQueue(string filename) {
	videoData.push_back(VideoData(filename,0));
	if(bStartSequence) {
		noVideoTimer.start();
		bStartSequence = false;
	}
}

//--------------------------------------------------------------
void VideoHandler::tweenComplete(int &id) {
	
	if(whichSequence == 0 && id == 1) {
		cout << "Play Video One" << endl;
		replayVideoId = 0;
		#ifndef TARGET_RASPBERRY_PI
			videos[0]->play();
		#endif
	
		whichSequence = 1;
	}
	
	if(whichSequence == 1 && id == 2) {
		cout << "Play Video Two" << endl;
		replayVideoId = 1;
		
		#ifndef TARGET_RASPBERRY_PI
			videos[1]->play();
		#endif
		
		whichSequence = 2;
	}
	
	if(whichSequence == 2 && id == 3) {
		cout << "Play Video Three" << endl;
		#ifndef TARGET_RASPBERRY_PI
			videos[2]->play();
		#endif
		replayVideoId = 2;
		whichSequence = 0;
	}
	
	if (id == -1) {
		#ifndef TARGET_RASPBERRY_PI
			videos[0]->stop();
			videos[0]->firstFrame();
		#endif
	}
	
	if (id == -2) {
		#ifndef TARGET_RASPBERRY_PI
			videos[1]->stop();
			videos[1]->firstFrame();
		#endif
	}
	
	if (id == -3) {
		#ifndef TARGET_RASPBERRY_PI
			videos[2]->stop();
			videos[2]->firstFrame();
		#endif
	}
}

//--------------------------------------------------------------
void VideoHandler::graceTimerStarted(string &val) {
	cout << "Grace Timer Started" << endl;
}

//--------------------------------------------------------------
void VideoHandler::graceTimerInvalidated(string &val) {
	cout << "Grace Timer Invalidated" << endl;
}

//--------------------------------------------------------------
void VideoHandler::graceTimerFinished(string &val) {
	
	if( currentVideo >= videoData.size() ) {
		currentVideo = 0;
	}
	nextVideo();
}

//--------------------------------------------------------------
void VideoHandler::noVideoTimerFinished(string &val) {
	graceTimer.start();
}

//--------------------------------------------------------------
int VideoHandler::getPlaysForCurrentVideo() {
	if ( !videoData.empty() )
		return videoData[currentVideo].plays;
	
	return 0;
}
#ifndef TARGET_RASPBERRY_PI
//--------------------------------------------------------------
void VideoHandler::hasVideoFinished(string &val) {
	cout << val << endl;
	graceTimer.start();
}
#else
//--------------------------------------------------------------
void VideoHandler::onVideoEnd(ofxOMXPlayerListenerEventData& e) {
	graceTimer.start();
}
#endif
