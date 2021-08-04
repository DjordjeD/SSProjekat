#pragma once

#include <iostream>


class Section {

	public:
	int getID();
	std::string getSectionName();
	int getSectionSize();

	void setId(int _id);
	void setSectionName(std::string _sectionName);
	void setSectionSize(int _sectionSize);

	Section(int _id, std::string _sectionName) : id(_id), sectionName(_sectionName) { sectionSize = 0; }

	private :

	int id;
	std::string sectionName;
	int sectionSize;

};