#pragma once
#include "Space/video.h"

class myvideo : public video
{
public:
	myvideo(void);
	~myvideo(void);
	void onend();
};
