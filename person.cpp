//
//  person.cpp
//  AI sim
//
#include <iostream>
#include <stdlib.h>
#include "person.h"
#include <time.h>
#include <string.h>

using namespace std;

void Person::initPerson(int x, int y){
    srand( (unsigned int)time(NULL) );
    initPet();
	personID = rand() % 10000;
    currentX = x;
    currentY = y;
    sleep = rand() % (100 - 51) + 50;
    happines = rand() %(100 - 51) + 50;
    hunger = rand() % (100 - 51) + 50;
    exercise = rand() % (100 - 51) + 50;
    gender = rand() % 2;
    health = rand() % (100 - 81) + 80;
    actionID = 0;
    destinationX = -1;
    destinationY = -1;
    mateID = -1;
    performingAction = 0;
    moving = 0;

}

void Person::setLastLetter(){
 int person = 1 + 2;
}

void Person::printPerson(){
    std::cout << "Sleep: " << sleep << "\nhappines: " << happines << std::endl;
}

int Person::getPersonID(){
    return personID;
}

void Person::setPersonID(int newID){
    personID = newID;
}

int Person::getCurrentX(){
    return currentX;
}

void Person::setCurrentX(int x){
    currentX = x;
}
int Person::getCurrentY(){
    return currentY;
}
void Person::setCurrentY(int y){
    currentY = y;
}
int Person::getSleep(){
    return sleep;
}
void Person::setSleep(int s){
    sleep = s;
}
int Person::getHappines(){
    return happines;
}
void Person::setHappines(int h){
    happines = h;
}
int Person::getHunger(){
    return hunger;
}
void Person::setHunger(int h){
    hunger = h;
}
int Person::getExercise(){
    return exercise;
}
void Person::setExercise(int e){
    exercise = e;
}
int Person::getGender(){
    return gender;
}
void Person::setGender(int g){
    gender = g;
}
int Person::getHealth(){
    return health;
}
void Person::setHealth(int h){
    health = h;
}
int Person::getDestinationX(){
    return destinationX;
}
void Person::setDestinationX(int x){
    destinationX = x;
}
int Person::getDestinationY(){
    return destinationY;
}
void Person::setDestinationY(int y){
    destinationY = y;
}
int Person::getActionID(){
    return actionID;
}
void Person::setActionID(int a){
    actionID = a;
}
int Person::getMateID(){
    return mateID;
}
void Person::setMateID(int m){
    mateID = m;
}
int Person::isPerformingAction(){
    return performingAction;
}
void Person::setPerformingAction(int a){
    performingAction = a;
}
int Person::isMoving(){
    return moving;
}
void Person::setMoving(int m){
    moving = m;
}
char Person::getMyLetter() {
    return myLetter;
}

void Person::setMyLetter( char letter ) {
    myLetter = letter;
}

char* Person::getName() {
    return name;
}

void Person::setName(char* newName) {
    strncpy( name, newName, strlen(newName) +1 );
}




////////////////////////
//makeing the list for the classifier
void Person::initChild(Person p[], int dad, int mom){
    //make copy of classifier list then sort
    classifier_t dadList[MAX_CLASSIFIERS];
    classifier_t momList[MAX_CLASSIFIERS];
    
    for (int i = 0; i < MAX_CLASSIFIERS; i++){
        dadList[i] = p[dad].list[i];
        momList[i] = p[mom].list[i];
    }

    for (int i = 0; i < (MAX_CLASSIFIERS / 2); i++){
        list[i] = dadList[i];
    }
    for (int i = (MAX_CLASSIFIERS / 2); i < MAX_CLASSIFIERS; i++){
        list[i] = momList[i];
    }
}


unsigned char Person::getRandomCharacter( void ){
    const unsigned char alphabet[3] = {'0', '1', '#'};
    return( alphabet[(int)((double)(3)*rand()/(RAND_MAX+1.0))] );
}

void Person::calculateSpecificity( int classifier ){
    int i;
    
    list[classifier].specificity = 0.0;
    
    for (i = 0 ; i < MAX_COND_WIDTH ; i++) {
        if (list[classifier].condition[i] != '#') {
            list[classifier].specificity++;
        }
    }
    list[classifier].specificity /= (double)MAX_COND_WIDTH;
}

void Person::initClassifier( int classifier ){//only allow 3 "1"
    int cond, act, count = 0, newChar, i;
    unsigned char c, tester;
    tester = '1';
    list[classifier].condition[0] = 0x30 + rand() % 2;
    for (cond = 1 ; cond < MAX_COND_WIDTH ; cond++) {
        
        c = getRandomCharacter();
        if (c == tester){
            count++;
            if (count > 3){
                newChar = (rand() % 2);
                if (newChar == 0){
                    c = '0';
                }
                else{
                    c = '#';
                }
            }
        }
        list[classifier].condition[cond] = c;
    }
    
    count = 0;
    for (act = 0 ; act < MAX_ACTION_WIDTH ; act++) {
        newChar = rand() % 6;
        for (i = 0; i < 3; i++){
            list[classifier].action[i] = actionList[newChar].actionType[i];
        }
    }
    
    list[classifier].strength = 1.0;
    list[classifier].episode = 0;
    
    calculateSpecificity( classifier );
    
    return;
}

void Person::initPet( void ){
    int c;
    srand( time( NULL ) );
    /* Create the classifier population */
    initActions();
    for (c = 0 ; c < MAX_CLASSIFIERS ; c++) {
        initClassifier( c );
    }
}

void Person::initActions(){
    actionList[0].actionType[0] = 0x30;actionList[0].actionType[1] = 0x30;actionList[0].actionType[2] = 0x30;
    actionList[1].actionType[0] = 0x30;actionList[1].actionType[1] = 0x30;actionList[1].actionType[2] = 0x31;
    actionList[2].actionType[0] = 0x30;actionList[2].actionType[1] = 0x31;actionList[2].actionType[2] = 0x30;
    actionList[3].actionType[0] = 0x30;actionList[3].actionType[1] = 0x31;actionList[3].actionType[2] = 0x31;
    actionList[4].actionType[0] = 0x31;actionList[4].actionType[1] = 0x30;actionList[4].actionType[2] = 0x30;
    actionList[5].actionType[0] = 0x31;actionList[5].actionType[1] = 0x30;actionList[5].actionType[2] = 0x31;
}

///////////////////////







/*
class Person{
    
 
    private:
       // classifier list goes here
        int personID;
        int currentX, currentY;
        int sleep;
        int happines;
        int hunger;
        int exercise;
        int gender;
        int health;
        int destinationX, destinationY;
        int actionID;
        int mateID;
        int performingAction;
        int moving;
		unsigned char myLetter;
		
	public:
        void initPerson(int x, int y){
             srand( (unsigned int)time(NULL) );
             personID = rand() % 10000;
            currentX = x;
            currentY = y;
            sleep = rand() % 50 + 1;
            happines = rand() % 50 + 1;
            hunger = rand() % 50 + 1;
            exercise = rand() % 50 + 1;
            gender = rand() % 2;
            health = rand() % 50 + 1;
            actionID = 0;
            destinationX = -1;
            destinationY = -1;
            mateID = -1;
            performingAction = 0;
            moving = 0;
        }
        void setLastLetter(){
		 int person = 1 + 2;
       }
        void printPerson(){
            std::cout << "Sleep: " << sleep << "\nhappines: " << happines << std::endl;
        }

        int getPersonID(){
            return personID;
        }

        int getCurrentX(){
			return currentX;
		}
		
        void setCurrentX(int x){
            currentX = x;
        }
        int getCurrentY(){
            return currentY;
        }
        void setCurrentY(int y){
            currentY = y;
        }
        int getSleep(){
            return sleep;
        }
        void setSleep(int s){
            sleep = s;
        }
        int getHappines(){
            return happines;
        }
        void setHappines(int h){
            happines = h;
        }
        int getHunger(){
            return hunger;
        }
        void setHunger(int h){
            hunger = h;
        }
        int getExercise(){
            return exercise;
        }
        void setExercise(int e){
            exercise = e;
        }
        int getGender(){
            return gender;
        }
        void setGender(int g){
            gender = g;
        }
        int getHealth(){
            return health;
        }
        void setHealth(int h){
            health = h;
        }
        int getDestinationX(){
            return destinationX;
        }
        void setDestinationX(int x){
            destinationX = x;
        }
        int getDestinationY(){
            return destinationY;
        }
        void setDestinationY(int y){
            destinationY = y;
        }
        int getActionID(){
            return actionID;
        }
        void setActionID(int a){
            actionID = a;
        }
        int getMateID(){
            return mateID;
        }
        void setMateID(int m){
            mateID = m;
        }
        int isPerformingAction(){
            return performingAction;
        }
        void setPerformingAction(int a){
            performingAction = a;
        }
        int isMoving(){
            return moving;
        }
        void setMoving(int m){
            moving = m;
        }
		char getMyLetter() {
			return myLetter;
		}
		
		void setMyLetter( char letter ) {
			myLetter = letter;
		}
};
*/