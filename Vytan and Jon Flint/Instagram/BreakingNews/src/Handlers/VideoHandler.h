//
//  VideoHandler.h
//  InstagramTv
//
//  Created by David Haylock on 02/03/2018.
//

#ifndef VideoHandler_h
#define VideoHandler_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxTween.h"
#include "Timer.h"

#ifndef TARGET_RASPBERRY_PI
	#include "ofxTextAlignTTF.h"
#else
	#include "ofxOMXPlayer.h"
#endif

class ofVideoPlayer_Ext : public ofVideoPlayer {
	public:
		ofEvent<string> videoFinished;
		bool hasTriggered = false;
	
		void firstFrame() {
			hasTriggered = false;
			ofVideoPlayer::firstFrame();
		}
	
		void update() {
			ofVideoPlayer::update();
			if (!hasTriggered && this->getCurrentFrame() >= this->getTotalNumFrames()-30) {
				string ss = this->getMoviePath();
				ofNotifyEvent(videoFinished, ss, this);
				hasTriggered = true;
			}
		}
};

//--------------------------------------------------------------
struct VideoData {
	
	VideoData() {}
	
	VideoData(string filename,int plays = 0) {
		this->filename = filename;
		this->plays = plays;
	}
	
	string filename;
	int plays = 0;
};


#ifndef TARGET_RASPBERRY_PI
	class VideoHandler {
#else
	class VideoHandler : public ofxOMXPlayerListener {
#endif
//--------------------------------------------------------------

	public:
	
		//--------------------------------------------------------------
		/**
			Setup the deque and positions
		*/
		//--------------------------------------------------------------
		void setup(int graceTime,int tweenTime);
	
		//--------------------------------------------------------------
		/**
			Load te Video
		*/
		//--------------------------------------------------------------
		void loadVideo(int index, string videoFile);
	
		//--------------------------------------------------------------
		/**
			Close the Handler
		*/
		//--------------------------------------------------------------
		void close();
	
		//--------------------------------------------------------------
		/**
			Play the Next Video
		*/
		//--------------------------------------------------------------
		void nextVideo();
	
		//--------------------------------------------------------------
		/**
			Replay the Video
		*/
		//--------------------------------------------------------------
		void replayVideo();
	
		//--------------------------------------------------------------
		/**
			Draw the Videos
		*/
		//--------------------------------------------------------------
		void draw(bool bDebug = false);
	
		//--------------------------------------------------------------
		/**
			Animation Sequences
		*/
		//--------------------------------------------------------------
		void sequenceOne();
		void sequenceTwo();
		void sequenceThree();
	
		//--------------------------------------------------------------
		/**
			 Add New File into the queue

			 @param filename the file to add
		*/
		//--------------------------------------------------------------
		void addNewVideoToQueue(string filename);
	
		//--------------------------------------------------------------
		/**
			Get the Plays for the Current Video

			@return current plays
		*/
		//--------------------------------------------------------------
		int getPlaysForCurrentVideo();
	
	private:

		deque<VideoData> videoData;
	
		ofxTween tween_one;
		ofxTween tween_two;
		ofxTween tween_three;
		ofxEasingQuart sine;
	
		bool bFirst;
		bool bStartSequence;
	
		int graceTime;
		int tweenTime;
		
		int mainPosition;
		int waitingPosition;
		int donePosition;
		int whichSequence;
		int replayVideoId;
		int currentPosition;
		int currentVideo;
		int indexOfNextContainerToLoad;
	
		Timer graceTimer;
		Timer noVideoTimer;
		
		//--------------------------------------------------------------
		/**
		 Has the Grace Timer Finished
		 
		 @param val Timer Name
		 */
		//--------------------------------------------------------------
		void noVideoTimerFinished(string &val);
		
		//--------------------------------------------------------------
		/**
			Has the Tween Completed

			@param id The Id of the Tween Action
		*/
		//--------------------------------------------------------------
		void tweenComplete(int &id);
	
		//--------------------------------------------------------------
		/**
			Has the Grace Timer Started

			@param val Timer Name
		*/
		//--------------------------------------------------------------
		void graceTimerStarted(string &val);
	
		//--------------------------------------------------------------
		/**
			Has we invalidated the Timer

			@param val Timer Name
		*/
		//--------------------------------------------------------------
		void graceTimerInvalidated(string &val);
		
		//--------------------------------------------------------------
		/**
			Has the Grace Timer Finished

			@param val Timer Name
		*/
		//--------------------------------------------------------------
		void graceTimerFinished(string &val);
	
#ifndef TARGET_RASPBERRY_PI

		// We only need to have 3 videos
		// Whilst one is loading one will be playing and the third will be ready for the next play
		deque<ofVideoPlayer_Ext*> videos;
		
		//--------------------------------------------------------------
		/**
			Has the Video Finished Playing

			@param val What is the Video Called
		*/
		//--------------------------------------------------------------
		void hasVideoFinished(string & val);
#else
		// We only need to have 3 videos
		// Whilst one is loading one will be playing and the third will be ready for the next play
		deque<ofxOMXPlayer*> videos;
		ofxOMXPlayerSettings settings;
		void onVideoEnd(ofxOMXPlayerListenerEventData& e);
		void onVideoLoop(ofxOMXPlayerListenerEventData& e){  };
#endif

};

#endif /* VideoHandler_h */
