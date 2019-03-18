#include "IOManager.h"


bool IOManager::readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer) {
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	//seek to end of file to get size
	file.seekg(0, std::ios::end); //0 bytes offset from the end

	//get the file size
	int fileSize = file.tellg();

	file.seekg(0, std::ios::beg); // go back to start

	// Reduce file size by any header bytes that might be present
	fileSize -= file.tellg();

	buffer.resize(fileSize);

	file.read((char *)&(buffer[0]), fileSize); //get the address of first element in buffer vector. This is essentially a pointer to beginning of the c-string
	file.close();
}