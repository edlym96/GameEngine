#pragma once
#include <string>
#include "GLTexture.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"
namespace DawnEngine {
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}