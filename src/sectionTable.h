#pragma once

#include <iostream>


class Sections {

	int getID();
	std::string getSectionName();
	int getSectionSize();

	void setId(int _id);
	void setSectionName(std::string _sectionName);
	void setSectionSize(int _sectionSize);

	

private :

	int id;
	std::string sectionName;
	int sectionSize;

};