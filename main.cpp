//Project 5 main function
//Created by Allison Lee on November 8, 2023
//main.cpp tests the functions from Project 5

#include <iostream>
#include <vector>
#include "QuestList.hpp"

int main(){
    // QuestList* q = new QuestList;
    // Quest* a = new Quest;
    // Quest* b = new Quest;
    // Quest* c = new Quest;
    // Quest* d = new Quest;
    // a->title_ = "hi";
    // b->title_ = "there";
    // c->title_ = "son";
    // d->title_ = "hooray";
    // a->experience_points_ = 4;
    // std::vector<Quest*> bl = {b,c};
    // //std::vector<Quest*> cl = {c};
    // std::vector<Quest*> dl = {d};
    // b->experience_points_ = 10;
    // c->experience_points_ = 10;
    // d->experience_points_ = 10;
    // a->subquests_ = bl;
    // //b->subquests_ = cl;
    // c->subquests_ = dl;
    // a->completed_ = true;
    // b->completed_ = true;
    // c->completed_ = true;
    // d->completed_ = true;
    // q->addQuest(a);

    // std::cout << q->calculatePathwayExperience(a);  
    
    // std::cout << "Testing questQuery:" << std::endl<< std::endl;
    // std::string i = "Quest 4";
    // QuestList questList("debug.csv");
    // questList.questQuery("Quest 4");
    // std::cout<<std::endl;
    // std::cout<< questList.questAvailable(getItem(getPosOf(i))) << std::endl;
    QuestList questList("quests.csv");
}