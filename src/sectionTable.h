#pragma once

#include <iostream>
#include <vector>

using namespace std;

class Section {

public:
	int getID();
	std::string getSectionName();
	int getSectionSize();

	void setId(int _id);
	void setSectionName(std::string _sectionName);
	void setSectionSize(int _sectionSize);
	void increaseSize(int size);
	void increaseSize();

	Section() = default;
	Section(int _id, std::string _sectionName) : id(_id), sectionName(_sectionName) { sectionSize = 0; }
	//Section(Section&& section) {
	//	id = section.id;
	//	sectionName = section.sectionName;
	//	sectionSize = section.sectionSize;
	//}

	//Section(const Section& s1) {
	//	id = s1.id;
	//	sectionName = s1.sectionName;
	//	sectionSize = s1.sectionSize;
	//}

	//void operator = (const Section& section) {
	//	id = section.id;
	//	sectionName = section.sectionName;
	//	sectionSize = section.sectionSize;

	//}
	vector<vector<char>> data;
	vector<int> offsets;

private:

	int id;
	std::string sectionName;
	int sectionSize;



};