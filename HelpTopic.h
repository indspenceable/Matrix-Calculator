/*
 *  HelpTopic.h
 *  Matrix_Calculator
 *
 *  Created by Daniel Spencer on 12/3/09.
 *  Copyright 2009 Oberlin College. All rights reserved.
 *
 */

#include "C_Ptr.h"
#include <map>
#include <string>

class HelpTopic
{
public:
	HelpTopic(std::string n, std::string desc): name(n), description(desc) {}
	void display();
	C_Ptr<HelpTopic> getSubTopic(std::string s);
	void addSubTopic(C_Ptr<HelpTopic> i);
	void addSubTopic(C_Ptr<HelpTopic> i, std::string s);
private:
	std::map<std::string, C_Ptr<HelpTopic> > subTopics;
	static C_Ptr<HelpTopic> noTopicFound;
	
	std::string name;
	std::string description;
};