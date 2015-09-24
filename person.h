#ifndef PERSON_H
#define PERSON_H

#define MAX_COND_WIDTH      4
#define MAX_ACTION_WIDTH    3
#define MAX_CLASSIFIERS     30 // More Classifiers, more options
#define MAX_EPISODE     3

typedef struct {
     
    unsigned char condition[MAX_COND_WIDTH];
    unsigned char action[MAX_ACTION_WIDTH];
    double        strength;
    double        specificity;
    double        bid;
    int           match;
    int           episode;
    
} classifier_t;

typedef struct{
    unsigned char actionType[3];
} actionTypeP;

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
        char name[50];
		
	public:
        classifier_t list[MAX_CLASSIFIERS];
        actionTypeP actionList[6];
        void initPerson(int x, int y);
        void initChild(Person p[], int dad, int mom);
        void setLastLetter();
        void printPerson();
        int getPersonID();
        void setPersonID(int Id);
        int getCurrentX();
        void setCurrentX(int x);
        int getCurrentY();
        void setCurrentY(int y);
        int getSleep();
        void setSleep(int s);
        int getHappines();
        void setHappines(int h);
        int getHunger();
        void setHunger(int h);
        int getExercise();
        void setExercise(int e);
        int getGender();
        void setGender(int g);
        int getHealth();
        void setHealth(int h);
        int getDestinationX();
        void setDestinationX(int x);
        int getDestinationY();
        void setDestinationY(int y);
        int getActionID();
        void setActionID(int a);
        int getMateID();
        void setMateID(int m);
        int isPerformingAction();
        void setPerformingAction(int a);
        int isMoving();
        void setMoving(int m);
		char getMyLetter();
		void setMyLetter( char letter );
        char* getName();
        void setName(char* newName);

        void initPet( void );
        void initClassifier( int classifier );
        void calculateSpecificity( int classifier );
        unsigned char getRandomCharacter( void );
        void initActions();
};

#endif