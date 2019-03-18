#pragma once
#include <string>
#include "GLTexture.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"

class ImageLoader
{
public:
	static GLTexture loadPNG(std::string filePath);
};

