#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "DeviceController.h"
#include "ofxOpenNI2GrabberSettings.h"

using namespace openni;

class DepthSource : public VideoStream::NewFrameListener
{
public:
	DepthSource();
	int width;
	int height;
	VideoStream videoStream;
	
	ofTexture texture;
	ofPixels pixels[2];
	ofPixels* backPixels;
	ofPixels* currentPixels;
	
	
	ofShortPixels rawPixels[2];
	ofShortPixels* backRawPixels;
	ofShortPixels* currentRawPixels;
	
	
	VideoMode videoMode;
	
	VideoFrameRef videoFrameRef;
	bool setup(DeviceController& deviceController);
	void allocateBuffers();
	void draw();
	void close();
	int	deviceMaxDepth;
	void onNewFrame(VideoStream&);
	bool doDoubleBuffering;
	ofxOpenNI2GrabberSettings settings;
};

