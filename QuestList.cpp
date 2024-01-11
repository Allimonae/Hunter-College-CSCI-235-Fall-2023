/*
    CSCI 235 Fall 2023
    Project 5 - QuestList Class .cpp file
    Created by Allison Lee on November 3, 2023
    QuestList.hpp defines the Quest Struct and declares the QuestList class along with its private and public members
*/

#include "QuestList.hpp"
#include <fstream>
#include <sstream>

/**
    Default Constructor
*/
QuestList::QuestList() : DoublyLinkedList<Quest*>(){}

/**
    @param: a reference to string name of an input file
    @pre: Formatting of the csv file is as follows:
        Title: A string
        Description: A string
        Completion Status: 0 (False) or 1 (True)
        Experience Points: A non negative integer
        Dependencies: A list of Quest titles of the form [QUEST1];[QUEST2], where each quest is separated by a semicolon. The value may be NONE.
        Subquests: A list of Quest titles of the form [QUEST1];[QUEST2], where each quest is separated by a semicolon. The value may be NONE.
    Notes:
        - The first line of the input file is a header and should be ignored.
        - The dependencies and subquests are separated by a semicolon and may be NONE.
        - The dependencies and subquests may be in any order.
        - If any of the dependencies or subquests are not in the list, they should be created as new quests with the following information:
            - Title: The title of the quest
            - Description: "NOT DISCOVERED"
            - Completion Status: False
            - Experience Points: 0
            - Dependencies: An empty vector
            - Subquests: An empty vector
        - However, if you eventually encounter a quest that matches one of the "NOT DISCOVERED" quests while parsing the file, you should update all the quest details.
        Hint: update as needed using addQuest()
        
    @post: Each line of the input file corresponds to a quest to be added to the list. No duplicates are allowed.
*/
QuestList::QuestList(const std::string& filename) : QuestList(){
    std::ifstream file(filename);
    if(file.fail())
    {
        std::cerr << "File could not be opened for reading." << std::endl;
        exit(1);
    }
    std::string line, junk;
    getline(file, junk); 
    while(getline(file, line))
    {
        std::string title, description;
        int experience_points;
        bool completed;
        std::vector<Quest*> dependencies, subquests;
        std::string temp, temp_dependencies, temp_subquests;
        std::istringstream ss(line); 
        std::getline(ss, title, ',');
        getline(ss, description, ',');
        getline(ss, temp, ',');
        std::istringstream completed_(temp);
        completed_ >> completed;
        getline(ss, temp, ',');
        std::istringstream experience_(temp);
        experience_ >> experience_points;
        getline(ss, temp_dependencies, ',');
        getline(ss, temp_subquests);
        if (temp_dependencies != "NONE") 
        {
            std::stringstream dependencyStream(temp_dependencies);
            std::string dependency;
            while (getline(dependencyStream, dependency, ';')) 
            {
                if (!contains(dependency)) 
                {
                    Quest* quest = new Quest(dependency, "NOT DISCOVERED", false, 0, {}, {});
                    dependencies.push_back(quest);
                } 
                
                else 
                {
                    dependencies.push_back(getPointerTo(getPosOf(dependency))->getItem());
                }
            }
        }
        if (temp_subquests != "NONE") 
        {
            std::istringstream subquestStream(temp_subquests);
            std::string subquest;
            while (getline(subquestStream, subquest, ';')) 
            {
                if (!contains(subquest)) 
                {
                    Quest* quest = new Quest(subquest, "NOT DISCOVERED", false, 0, {}, {});
                    subquests.push_back(quest);
                } 
                
                else 
                {
                    subquests.push_back(getPointerTo(getPosOf(subquest))->getItem());
                }
            }
        }
        // Add the quest with parsed details to the list
        addQuest(title, description, completed, experience_points, dependencies, subquests);
    }
    file.close();
}

/**
@param: A string reference to a quest title
@return: The integer position of the given quest if it is in the QuestList, -1 if not found.
*/
int QuestList::getPosOf(const std::string& title) const{
    int pos = 0;
    Node<Quest*> *pos_ptr = nullptr; 
    pos_ptr = first_;
    for (int i = 0; i < getLength(); i++){
        //std::cout << pos_ptr->getItem()->title_ << std::endl;
        if (pos_ptr->getItem()->title_ == title){
            return pos;
            break;
        }
        pos++;
        pos_ptr = pos_ptr->getNext(); 
    }
    return -1;
}

/**
    @param: A string reference to a quest title
    @return: True if the quest with the given title is already in the QuestList
*/
bool QuestList::contains(const std::string& title) const{
    return (getPosOf(title) != -1);
}

/**
    @pre: The given quest is not already in the QuestList
    @param:  A pointer to a Quest object
    @post:  Inserts the given quest pointer into the QuestList. Each of its dependencies and subquests are also added to the QuestList IF not already in the list.
            If the quest is already in the list but is marked as "NOT DISCOVERED", update its details. (This happens when a quest has been added to the list through a dependency or subquest list)
        
    @return: True if the quest was added or updated successfully, False otherwise
*/
bool QuestList::addQuest(Quest* quest_obj){
    Node<Quest*>* point_quest = getPointerTo(getPosOf(quest_obj->title_));
    if (contains(quest_obj->title_)){ //quest with the title exists in questlist
        if (point_quest->getItem()->description_ == "NOT DISCOVERED"){
            point_quest->getItem()->description_ = quest_obj->description_;
            point_quest->getItem()->completed_ = quest_obj->completed_;
            point_quest->getItem()->experience_points_ = quest_obj->experience_points_;
            point_quest->getItem()->dependencies_ = quest_obj->dependencies_;
            point_quest->getItem()->subquests_ = quest_obj->subquests_;
        }
        else{
            return false;
        }
    }
    else{
        insert(item_count_ + 1, quest_obj);
    }
    for (int i = 0; i < quest_obj->dependencies_.size(); i++){
        if(!contains(quest_obj->dependencies_[i]->title_)){
            quest_obj->dependencies_[i]->description_ = "NOT DISCOVERED";
            addQuest(quest_obj->dependencies_[i]);
        }
    }
    for (int i = 0; i < quest_obj->subquests_.size(); i++){
        if(!contains(quest_obj->subquests_[i]->title_)){
            quest_obj->subquests_[i]->description_ = "NOT DISCOVERED";
            addQuest(quest_obj->subquests_[i]);
        }
    }
    return true;
}

/**
    @param:  A reference to string representing the quest title
    @param:  A reference to string representing the quest description
    @param:  A reference to boolean representing if the quest is completed
    @param:  An reference to int representing experience points the quest rewards upon completion 
    @param:  A reference to vector of Quest pointers representing the quest's dependencies
    @param:  A reference to vector of Quest pointers representing the quest's subquests
    @post:   Creates a new Quest object and inserts a pointer to it into the QuestList. 
            If the quest is already in the list but is marked as "NOT DISCOVERED", update its details. 
            (This happens when a quest has been added to the list through a dependency or subquest list)
            Each of its dependencies and subquests are also added to the QuestList IF not already in the list.
            

    @return: True if the quest was added or updated successfully, False otherwise

*/
bool QuestList::addQuest(const std::string& title, const std::string& description, const bool& completed, const int& experience, 
    const std::vector<Quest*>& dependencies, const std::vector<Quest*>& subquests){
        std::string t = title;
        std::string d = description;
        bool c = completed;
        int e = experience;
        std::vector<Quest*> dl = dependencies;
        std::vector<Quest*> sl = subquests;
        Quest* q = new Quest(t, d, c, e, dl, sl);
        return addQuest(q);
    }

/**
    @param:  A Quest pointer
    @return: A boolean indicating if all the given quest's dependencies are completed
*/
bool QuestList::dependenciesComplete(Quest* quest_obj) const{
    int completed = 0;
    for(int i = 0; i < quest_obj->dependencies_.size(); i++){
        if (quest_obj->dependencies_[i]->completed_ == true){
            completed++;
        }
    }
    return (quest_obj->dependencies_.size() == completed);
}

/**
    @param: A Quest pointer
    @return: A boolean if the given quest is available.
    Note: For a quest to be available, it must not be completed, and its dependencies must be complete.
*/
bool QuestList::questAvailable(Quest* quest_obj) const{
    return (!(quest_obj->completed_) && dependenciesComplete(quest_obj));
}

/**
    @param: A Quest pointer
    @post: Prints the quest title and completion status
    The output should be of the form:
    [Quest Title]: [Complete / Not Complete]
    [Quest Description]\n\n
*/
void QuestList::printQuest(Quest* quest_obj) const{
    std::string complete = "Not Complete";
    if(quest_obj->completed_){
        complete = "Complete";
    }
    std::cout << quest_obj->title_ << ": " << complete << "\n" << quest_obj->description_ << "\n\n";
}

/**
    @param: A string reference to a quest title
    @post:  Prints a list of quests that must to be completed before the given quest can be started (incomplete dependencies).
            If any of the quest's incomplete dependencies have an incomplete dependency, recursively print the quests that need to be done in order, indenting incomplete quests. 
            The indentation for incomplete quests is 2 spaces: "  "    
            The format of the list should be of the following forms for each different case:

            Query: [Quest Title]
            No such quest.
    
            Query: [Quest Title]
            Quest Complete
    
            Query: [Quest Title]
            Ready: [Quest Title]
    
            Query: [Quest Title]
            Ready: [Dependency0]
            [Quest Title]
            
            Query: [Quest Title]
            Ready: [Dependency0]
            Ready: [Dependency1]
            [Quest Title]
    
            Query: [Quest Title]
            Ready: [Dependency0]
            Ready: [Dependency1]
            [Dependency2]
            [Quest Title]
            
    If the given quest title is not found in the list, print "No such quest."
*/
void QuestList::questQuery(const std::string& title) const{ 
    std::cout << "Query: " << title << std::endl;
    if(!contains(title)){
        std::cout << "No such quest." << std::endl;
        return;
    }
    Quest* q = getItem(getPosOf(title));
    if (q->completed_){
        std::cout << "Quest Complete" << std::endl;
    }
    else{
        helperQuestQuery(title);
    }
}

/**
    @param: A string reference to a quest title
    @post:  Recursively prints all available quests and incomplete dependencies
            Case 1: doesn't contain main quest
            Case 2: main quest is complete
            case 3: quest is available
            case 4: dependencies incomplete
*/
void QuestList::helperQuestQuery(const std::string& title) const{
    Quest* q = getItem(getPosOf(title));
    if(questAvailable(q)){
        std::cout << "Ready: " << title << std::endl;
        return;
    }
    for (int i = 0; i < q->dependencies_.size(); i++){
        if(!q->dependencies_[i]->completed_){
            helperQuestQuery(q->dependencies_[i]->title_);
        }     
    }
    std::cout << "  " << title << std::endl;
}

/**
    @return: An integer sum of all the experience gained
    Note: This should only include experience from completed quests 
*/
int QuestList::calculateGainedExperience() const{
    int exp = 0;
    for (int i = 0; i < getLength(); i++){
        if(getItem(i)->completed_){
            exp += getItem(i)->experience_points_;
        }
    }
    return exp;
}

/**
    @param: A quest pointer to a main quest
    @return: An integer sum of all the experience that can be gained from completing the main quest AND all its subquests.
    Note: Also consider the potential experience if a subquest itself has subquests.
*/
int QuestList::calculateProjectedExperience(Quest* quest_obj) const{
    int exp = quest_obj->experience_points_;
    for (int i = 0; i < quest_obj->subquests_.size(); i++){
        exp += calculateProjectedExperience(quest_obj->subquests_[i]);
    }
    return exp;
}

/**
    @param: A quest pointer to a main quest
    @return: An integer sum of all the experience that has already been gained by completing the given quest's subquests.
    Note: Also consider the experience gained if a completed subquest itself has subquests.  
*/
int QuestList::calculatePathwayExperience(Quest* main_quest) const{
    int exp = 0;
    if(main_quest->completed_){
        exp += main_quest->experience_points_;
    }
    for (int i = 0; i < main_quest->subquests_.size(); i++){
        if(main_quest->subquests_[i]->completed_){
            exp += calculatePathwayExperience(main_quest->subquests_[i]); //calculatePathwayExperience(main_quest->subquests_[i]);
        }
    } 
    return exp;
}

/**
    @param: A string reference to a filter with a default value of "NONE".
    @post: With default filter "NONE": Print out every quest in the list.
        With filter "COMPLETE":   Only print out the completed quests in the list.
        With filter "INCOMPLETE": Only print out the incomplete quests in the list.
        With filter "AVAILABLE":  Only print out the available quests in the list.
        If an invalid filter is passed, print "Invalid Filter\n"
    Printing quests should be of the form:
    [Quest title]: [Complete / Not Complete]  
    [Quest description]\n
*/
void QuestList::questHistory(const std::string& filter) const{
    if (filter == "NONE"){
        for (int i = 0; i < getLength(); i++){
            printQuest(getItem(i));
        }
    }
    else if(filter == "COMPLETE"){
        for (int i = 0; i < getLength(); i++){
            if(getItem(i)->completed_){
                printQuest(getItem(i));
            }
        }
    }
    else if (filter == "INCOMPLETE"){
        for (int i = 0; i < getLength(); i++){
            if(!(getItem(i)->completed_)){
                printQuest(getItem(i));
            }
        }
    }
    else if (filter == "AVAILABLE"){
        for (int i = 0; i < getLength(); i++){
            if(questAvailable(getItem(i))){
                printQuest(getItem(i));
            }
        }
    }
    else{
        std::cout << "Invalid Filter\n";
    }
}

/**
    @param: A quest pointer to a main quest
    @post:  Outputs subquest pathway. Print quest names with two spaces ("  ") of indentation for each subquest, recursively.
            Also print the percentage of experience gained in this pathway, rounded down to the lower integer.
            The format should be of the form:
            [Main Quest] ([Pathway XP] / [Projected XP]% Complete)
                [Subquest0]: [Complete / Not Complete]
                    [Sub-Subquest01]: [Complete / Not Complete]
                        [Sub-Subquest011]: [Complete / Not Complete]
                    [Subquest02]: [Complete / Not Complete]
            Hint: You can write a helper function to print the subquests recursively. If the given quest is already marked as completed, you can assume that all of its subquests are also completed.
*/
void QuestList::printQuestDetails(Quest* main_quest) const{
    int completed = calculatePathwayExperience(main_quest);
    if(main_quest->completed_){
        completed = calculateProjectedExperience(main_quest);
    }
    std::cout << main_quest->title_ << "(" << (completed * 100)/calculateProjectedExperience(main_quest) << "% Complete)\n";
    int indent = 0;
    helperPrintQuestDetails(main_quest, indent);
}

/**
    @param: A string reference to a quest title
    @param: An integer amount of indentations
    @post:  Recursively prints all available quests and incomplete dependencies
            Case 1: doesn't contain main quest
            Case 2: main quest is complete
            case 3: quest is available
            case 4: dependencies incomplete
*/
void QuestList::helperPrintQuestDetails(Quest* main_quest, int indent) const {
    indent++;
    for (int i = 0; i < main_quest->subquests_.size(); i++){
        for (int i = 0; i < indent; i++){
            std::cout << "  ";
        }
        std::string complete = "Not Complete";
        if(main_quest->subquests_[i]->completed_){
            complete = "Complete";
        }
        std::cout << main_quest->subquests_[i]->title_ << ": " << complete << std::endl;
        helperPrintQuestDetails(main_quest->subquests_[i], indent);
    }
}

/**
    @param:  A Quest pointer
    @return: A boolean indicating if all the given quest's subquests are completed
*/
bool QuestList::subquestsComplete(Quest* quest_obj) const{
    int completed = 0;
    for(int i = 0; i < quest_obj->subquests_.size(); i++){
        if (quest_obj->subquests_[i]->completed_ == true){
            completed++;
        }
    }
    return (quest_obj->subquests_.size() == completed);
}