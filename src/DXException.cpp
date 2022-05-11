#include "DXException.hpp"
#include <sstream>


DXException::DXException(int line, const char* file ) noexcept :
	line(line),
	file(file) {}

const char* DXException::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << '\n'
		<< getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* DXException::getType() const noexcept {
	return "DX Exception";
}

int DXException::getLine() const noexcept {
	return line;
}

const std::string& DXException::getFile() const noexcept {
	return file;
}

std::string DXException::getOriginString() const noexcept {
	std::ostringstream oss;
	oss << "[File] " << file << '\n'
		<< "[Line] " << line;
	return oss.str();
}