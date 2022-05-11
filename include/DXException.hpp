/******************************************************************************************
*	Chili Direct3D Engine																  *
*	Copyright 2018 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of Chili Direct3D Engine.											  *
*																						  *
*	Chili Direct3D Engine is free software: you can redistribute it and/or modify		  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili Direct3D Engine is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili Direct3D Engine.  If not, see <http://www.gnu.org/licenses/>.    *
******************************************************************************************/
#ifndef DXEXCEPTION_HPP
#define DXEXCEPTION_HPP

#include <exception>
#include <string>

class DXException : public std::exception
{
public:
	DXException(int line, const char* file ) noexcept;

	const char* what() const noexcept override;

	virtual const char* getType() const noexcept;

	int getLine() const noexcept;

	const std::string& getFile() const noexcept;

	std::string getOriginString() const noexcept;

private:
	int line;
	std::string file;

protected:
	mutable std::string whatBuffer;
};

#endif // DXEXCEPTION_HPP