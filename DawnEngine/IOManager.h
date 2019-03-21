#pragma once
#include <vector>
#include <fstream>
namespace DawnEngine {
	class IOManager
	{
	public:
		static bool readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);
	};

}