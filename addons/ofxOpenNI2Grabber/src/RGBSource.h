#pragma once

#include "ofMain.h"
#include "OpenNI.h"

using namespace openni;

class RGBSource : public VideoStream::NewFrameListener
{
public:
	RGBSource();
	int width;
	int height;
	VideoStream videoStream;
	
	ofTexture texture;
	ofPixels pixels[2];
	ofPixels* backPixels;
	ofPixels* currentPixels;
	
	VideoMode videoMode;
	
	VideoFrameRef videoFrameRef;
	bool setup(Device& device);
	void allocateBuffers();
	
	void close();
	void draw();
	void onNewFrame(VideoStream&);
};

