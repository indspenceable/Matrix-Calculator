/*
 *  HelpTopic.cpp
 *  Matrix_Calculator
 *
 *  Created by Daniel Spencer on 12/3/09.
 *  Copyright 2009 Oberlin College. All rights reserved.
 *
 */

#include "HelpTopic.h"
#include <iostream>


C_Ptr<HelpTopic> HelpTopic::noTopicFound(new HelpTopic("No topic found","Sorry, there is no help file on that topic. Please try again."));

void HelpTopic::addSubTopic(C_Ptr<HelpTopic> ht) {
	subTopics[ht->name] = ht;
}
void HelpTopic::addSubTopic(C_Ptr<HelpTopic> ht, std::string s) {
	subTopics[s] = ht;
}
void HelpTopic::display() {
	std::cout << "\t[[ " << name << " ]]" << std::endl;
	std::cout << description << std::endl;
	if ( subTopics.size() > 0 ) {
		std::cout << "Subtopics:" << std::endl;
		for (std::map<std::string, C_Ptr<HelpTopic> >::iterator it = subTopics.begin();
			 it != subTopics.end();
			 it++)
			 std::cout << "\t" << it->first << std::endl;

	}
}

C_Ptr<HelpTopic> HelpTopic::getSubTopic(std::string s) {
	if (subTopics.find(s) != subTopics.end())
		return subTopics[s];
	return noTopicFound;
}