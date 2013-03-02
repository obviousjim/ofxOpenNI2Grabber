#include "DepthSource.h"

DepthSource::DepthSource()
{
	width = 0;
	height = 0;
	backPixels = NULL;
	currentPixels = NULL;
	deviceMaxDepth = 0;
}
bool DepthSource::setup(Device& device)
{
	bool isReady = false;
	Status status = STATUS_OK;
	
	status = videoStream.create(device, SENSOR_COLOR);
	if (status == STATUS_OK)
	{
		ofLogVerbose() << "Find DepthSource stream PASS";
		status = videoStream.start();
		if (status == STATUS_OK)
		{
			ofLogVerbose() << "Start DepthSource stream PASS";
		}else 
		{
			
			ofLogError() << "Start DepthSource stream FAIL: " << OpenNI::getExtendedError();
			videoStream.destroy();
		}
	}else
	{
		ofLogError() << "Find DepthSource stream FAIL: " <<  OpenNI::getExtendedError();
	}
	if (videoStream.isValid())
	{
		allocateBuffers();
		deviceMaxDepth	= videoStream.getMaxPixelValue();
		ofLogVerbose() << "deviceMaxDepth: " << deviceMaxDepth;
		status = videoStream.addNewFrameListener(this);
		if (status == STATUS_OK)
		{
			ofLogVerbose() << "DepthSource videoStream addNewFrameListener PASS";
		}else
		{
			ofLogError() << "DepthSource videoStream addNewFrameListener FAIL: " <<  OpenNI::getExtendedError();
		}

		
		isReady = true;
	}else 
	{
		ofLogError() << "DepthSource is INVALID";
	}
	
	
	return isReady;
}
void DepthSource::draw()
{
	texture.loadData(*backPixels);
	texture.draw(width, 0);
}
void DepthSource::allocateBuffers()
{
	videoMode = videoStream.getVideoMode();
	width = videoMode.getResolutionX();
	height = videoMode.getResolutionY();
	
	pixels[0].allocate(width, height, OF_IMAGE_COLOR_ALPHA);
	pixels[1].allocate(width, height, OF_IMAGE_COLOR_ALPHA);
	currentPixels = &pixels[0];
	backPixels = &pixels[1];
	
	texture.allocate(width, height, GL_RGBA);
	
	rawPixels[0].allocate(width, height, OF_PIXELS_MONO);
	rawPixels[1].allocate(width, height, OF_PIXELS_MONO);
	currentRawPixels = &rawPixels[0];
	backRawPixels = &rawPixels[1];
}

void DepthSource::onNewFrame(VideoStream& stream)
{
	//ofLogVerbose() << "DepthSource::onNewFrame";
	Status status = stream.readFrame(&videoFrameRef);
	if (status != STATUS_OK)
	{
		ofLogError() << "DepthSource readFrame FAIL: " <<  OpenNI::getExtendedError();
	}
	const DepthPixel* oniDepthPixels = (const DepthPixel*)videoFrameRef.getData();
	
	//backRawPixels->setFromPixels(oniDepthPixels, width, height, 1);
	//swap(backRawPixels,currentRawPixels);
	
	float max = 255;
	for (unsigned short y = 0; y < height; y++) 
	{
		unsigned char * pixel = backPixels->getPixels() + y * width * 4;
		for (unsigned short  x = 0; x < width; x++, oniDepthPixels++, pixel += 4)
		{
			unsigned char red = 0;
			unsigned char green = 0;
			unsigned char blue = 0;
			unsigned char alpha = 255;
			
			unsigned short col_index;
			
			unsigned char a = (unsigned char)(((*oniDepthPixels) / ( deviceMaxDepth / max)));
			red		= a;
			green	= a;
			blue	= a;
			
			pixel[0] = red;
			pixel[1] = green;
			pixel[2] = blue;
			
			if (*oniDepthPixels == 0)
			{
				pixel[3] = 0;
			} else	
			{
				pixel[3] = alpha;
			}
		}
	}
	//swap(backPixels,currentPixels);
	
}

void DepthSource::close()
{
	videoStream.stop();
	videoStream.destroy();
}