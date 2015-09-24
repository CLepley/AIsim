 //
//  main.cpp
//  AI sim
//

#include <iostream>
#include "person.h"
#include "sim.h"
#include "aStar.h"

using namespace std;

void printLegand();
void printPlayerStatus( Person people[], int peopleCount );
void addStatus(Person people[], int currentPersonIndex, int peopleCount );
void createSpawn();
void updateStatus();
void checkDeaths();
void setPersonLetter(int currentPersonIndex);

Person people[4];       //array of people
int numPeople = 0;      //number of people currently alive
int actualPeople = 0;

char alphbet[] = {'A','B','C','D','E','F','G','T','R','M'} ;
int alphbetCounter = 0;

int main(int argc, const char * argv[]) {
    
    Position startP;
	setMap( );
	char playerName[50];
    bool willDamange = false;
	int willTakeDamge;


	   //initalize the first two people
    startP = randomStart();
    people[0].setLastLetter(); 
    people[0].initPerson(startP.x ,startP.y);
    setPersonLetter( 0 );
    cout << "Please enter character 1's name (Male): ";
    cin >> playerName;

    people[0].setName(playerName);
    people[0].setGender(0);
    placeOnMap(people[0]);
    numPeople++;    
    actualPeople++;

    cout << "Please enter character 2's name (Female): ";
    cin >> playerName;
   
    startP = randomStart();
    people[1].initPerson(startP.x,startP.y);
    setPersonLetter( 1 );
    people[1].setGender(1);
    people[1].setName(playerName);
    placeOnMap(people[1]); 
    numPeople++;
    actualPeople++;
    srand( time( NULL ) );
       // Loop until the number of people are 0 
    while( actualPeople ) {

        for( int x = 0; x < numPeople; x++ ) {
            if( people[x].getPersonID() != -1 ) {
                if( people[x].isPerformingAction() == 1 ) {
                    addStatus(people, x, numPeople);
                }

                if( people[x].isPerformingAction() == 0 && people[x].isMoving() == 0 ) {
                    people[x].setActionID(pizza(people, x, numPeople)); 
					if( actualPeople == 1 && people[x].getActionID() == 5 ){
						while( people[x].getActionID() == 5 ) {
							people[x].setActionID(pizza(people, x, numPeople)); 
						}
					}
                    //sleep(5);
                }
            }
        }

        go(people, numPeople);

        willTakeDamge = rand() % 11;
        if( willTakeDamge < 6 ) {
            updateStatus();
        }

        checkDeaths();

            // This clears a linux terminal
        system("clear");

        if( actualPeople ) {
                // Prints what each room is
            printLegand();

            printPlayerStatus(people, numPeople);

                // This prints the map
            print();

                // Delays for days
            sleep(1);
        }
	}

    return 0;
}

void updateStatus() {
    int actionID;
    int randomHurt;

    srand( time(NULL) );

    for(int x=0; x < numPeople; x++ ) {
        if( people[x].getPersonID() != -1 ) {
            actionID = people[ x ].getActionID();
            
            if( actionID != 1 ){
                if( (people[ x ].getHunger() <= 33) && (people[ x ].getHunger() >= 15) ) {
                    people[ x ].setHealth( people[ x ].getHealth() - 1 );
                }
                else if ( people[ x ].getHunger() < 15 ) {
                    randomHurt = rand() % 9 + 2;
                    people[ x ].setHealth( people[ x ].getHealth() - randomHurt ); 
                }

                randomHurt = rand() % 4;
                people[ x ].setHunger(  people[ x ].getHunger() - randomHurt );
                if( people[ x ].getHunger() < 0 ) {
                    people[ x ].setHunger( 0 );
                }
            }
            if( actionID != 2 ){
                if( people[ x ].getHappines() <= 33 && people[ x ].getHappines() >= 15 ) {
                    people[ x ].setHealth( people[ x ].getHealth() - 1 );
                }
                else if ( people[ x ].getHappines() < 15 ) {
                    randomHurt = rand() % 9 + 2;
                    people[ x ].setHealth( people[ x ].getHealth() - randomHurt ); 
                }

                randomHurt = rand() % 6;
                people[ x ].setHappines(  people[ x ].getHappines() - randomHurt );
                if( people[ x ].getHappines() < 0 ) {
                    people[ x ].setHappines( 0 );
                }
            }
            if( actionID != 3 ){
                if( people[ x ].getExercise() <= 33 && people[ x ].getExercise() >= 15 ) {
                    people[ x ].setHealth( people[ x ].getHealth() - 1 );
                }
                else if ( people[ x ].getExercise() < 15 ) {
                     randomHurt = rand() % 9 + 2;
                    people[ x ].setHealth( people[ x ].getHealth() - randomHurt ); 
                }

                randomHurt = rand() % 6;
                people[ x ].setExercise(  people[ x ].getExercise() - randomHurt );
                if( people[ x ].getExercise() < 0 ) {
                    people[ x ].setExercise( 0 );
                }
            }
            if( actionID != 4 ){
                if( people[ x ].getSleep() <= 33 && people[ x ].getSleep() >= 15 ) {
                    people[ x ].setHealth( people[ x ].getHealth() - 1 );
                }
                else if ( people[ x ].getSleep() < 15 ) {
                     randomHurt = rand() % 9 + 2;
                    people[ x ].setHealth( people[ x ].getHealth() - randomHurt ); 
                }

                randomHurt = rand() % 6;
                people[ x ].setSleep(  people[ x ].getSleep() - randomHurt );
                if( people[ x ].getSleep() < 0 ) {
                    people[ x ].setSleep( 0 );
                }
            }
        }    
    }
}

void setPersonLetter(int currentPersonIndex) {
    people[currentPersonIndex].setMyLetter( alphbet[ alphbetCounter++ ] );
    if( alphbetCounter > 9)
        alphbetCounter = 0;
}

void checkDeaths() {

    for( int x=0; x < numPeople; x++ ) {
        if( people[x].getPersonID() != -1 ) {
            if( people[x].getHealth() <= 0 ) {
                people[x].setPersonID( -1 );
                removeFromMap( people[ x ] );
                actualPeople--;
            }
        }
    }
}

void createSpawn(int dad, int mom ) {
    char playerName[50];
    Position startP;
    bool hitNegOne = false;

    for( int x=0; x < numPeople; x++ ) {
        if( people[x].getPersonID() == -1 ) {

            startP = randomStart();
            people[x].setLastLetter(); 
            people[x].initPerson(startP.x ,startP.y);
            people[x].initChild(people, mom, dad);
            setPersonLetter( x );
            cout << "Congradulation a new ";
            if( people[x].getGender() == 0 ) {
                cout << "boy";
            }
            else {
                cout<< "girl";
            }
            cout << " was created. What would you like to name person number "<< actualPeople <<": ";
            cin >> playerName;
            cout << endl;
            people[x].setName(playerName);
            placeOnMap(people[x]);
            hitNegOne = true;
        }
    }
       
    if( !hitNegOne ) {
        startP = randomStart();
        people[numPeople].setLastLetter(); 
        people[numPeople].initPerson(startP.x ,startP.y);
        setPersonLetter( numPeople );
        cout << "Congradulation a new ";
        if( people[numPeople].getGender() == 0 ) {
            cout << "boy";
        }
        else {
            cout<< "girl";
        }
        cout << " was created. What would you like to name person number "<< actualPeople + 1 <<": ";
        cin >> playerName;
        cout << endl;
        people[numPeople].setName(playerName);
        placeOnMap(people[numPeople]); 
        numPeople++;
    }     
    actualPeople++;
}

void addStatus(Person people[], int currentPersonIndex, int peopleCount ) {

    int action = people[currentPersonIndex].getActionID();

    if(action == 1) {
        people[currentPersonIndex].setHunger( people[currentPersonIndex].getHunger() + 20 );
        if( people[currentPersonIndex].getHunger() >= 60 && people[currentPersonIndex].getHealth() <= 92 ) {
            people[currentPersonIndex].setHealth( people[currentPersonIndex].getHealth() + 8 );
        }
        if( people[currentPersonIndex].getHunger() >= 100 ) {
            people[currentPersonIndex].setHunger( 100 );
            people[currentPersonIndex].setPerformingAction(0);
        }
    }
    else if(action == 2) {
        people[currentPersonIndex].setHappines( people[currentPersonIndex].getHappines() + 20 );
        if( people[currentPersonIndex].getHappines() >= 60 && people[currentPersonIndex].getHealth() <= 92 ) {
            people[currentPersonIndex].setHealth( people[currentPersonIndex].getHealth() + 8 );
        }
        if( people[currentPersonIndex].getHappines() >= 100 ) {
            people[currentPersonIndex].setHappines( 100 );
            people[currentPersonIndex].setPerformingAction(0);
        }
    }
    else if(action == 3) {
        people[currentPersonIndex].setExercise( people[currentPersonIndex].getExercise() + 20 );
        if( people[currentPersonIndex].getExercise() >= 60 && people[currentPersonIndex].getHealth() <= 92 ) {
            people[currentPersonIndex].setHealth( people[currentPersonIndex].getHealth() + 8 );
        }
        if( people[currentPersonIndex].getExercise() >= 100 ) {
            people[currentPersonIndex].setExercise( 100 );
            people[currentPersonIndex].setPerformingAction(0);
        }
    }
    else if(action == 4) {
        people[currentPersonIndex].setSleep( people[currentPersonIndex].getSleep() + 20 );
        if( people[currentPersonIndex].getSleep() >= 60 && people[currentPersonIndex].getHealth() <= 92 ) {
            people[currentPersonIndex].setHealth( people[currentPersonIndex].getHealth() + 8 );
        }
        if( people[currentPersonIndex].getSleep() >= 100 ) {
            people[currentPersonIndex].setSleep( 100 );
            people[currentPersonIndex].setPerformingAction(0);
        }
    }
    else if(action == 5) {
        people[currentPersonIndex].setSleep( people[currentPersonIndex].getSleep() - 5 );
        people[currentPersonIndex].setHunger( people[currentPersonIndex].getHunger() - 5 );
        if( people[currentPersonIndex].getExercise() <= 92 ) {
            people[currentPersonIndex].setExercise( people[currentPersonIndex].getExercise() + 8 );
        }
        else {
            people[currentPersonIndex].setExercise( 100 );  
        }
         if( people[currentPersonIndex].getHappines() <= 92 ) {
            people[currentPersonIndex].setHappines( people[currentPersonIndex].getHappines() + 8 );
        }
        else {
            people[currentPersonIndex].setHappines( 100 );  
        }

        people[currentPersonIndex].setPerformingAction(0);

        if( people[currentPersonIndex].getHealth() > 85 ) {
            for(int x=0; x < peopleCount; x++ ) {
                if( x != currentPersonIndex ) {
                    if( people[currentPersonIndex].getMateID() == people[x].getPersonID() && people[x].getHealth() > 85 ) {
                        if( people[currentPersonIndex].getGender() != people[x].getGender() && peopleCount < 4 ) {
                            createSpawn( currentPersonIndex, x );
                        }
                    }
                }
            }
        }
        people[currentPersonIndex].setDestinationX(-1);
        people[currentPersonIndex].setDestinationY(-1);
        people[currentPersonIndex].setActionID(-1);
        people[currentPersonIndex].setMateID(-1 );
    }
}

void printPlayerStatus( Person people[], int peopleCount ) {
    int bar;
    for(int x=0; x < peopleCount; x++ ) {
        if( people[x].getPersonID() != -1 ) {
                // Name
            cout << "Person's name: " << people[x].getName() << " | Gender: ";
            if( people[x].getGender() == 0) {
                cout << "Male";
            }
            else {
                cout << "Female";
            }
            cout << " | Letter: " << people[x].getMyLetter() << endl;
                // Health
            cout << "Health: <";
            bar = people[x].getHealth() / 10;
            for (int i = 0; i < 10; ++i) {
                if(i<bar) {
                    cout << "#";
                }
                else {
                    cout << " ";
                }
            }
            cout << "> - " << people[x].getHealth() << "   Current Action - ";
            if ( people[x].getActionID() == 1 ) {
                cout << "Getting Food" << endl;
            } 
            else if( people[x].getActionID() == 2 ) {
                 cout << "Getting Entertainment" << endl;
            }
            else if ( people[x].getActionID() == 3 ) {
                 cout << "Getting Exercise" << endl;
            }
            else if ( people[x].getActionID() == 4 ) {
                 cout << "Getting Sleep" << endl;
            }
            else if ( people[x].getActionID() == 5 ) {
                cout << "Trying to Reproduce";
                int mateID = people[x].getMateID();
                for( int i=0; i < numPeople; i++ ){
                    if( people[i].getPersonID() == mateID ) {
                        cout << " with " << people[i].getName();
                    }
                }
                cout << endl;
            }
            else if ( people[x].getActionID() == 0 ) {
                 cout << "Wondering Around" << endl;
            }
                //Sleep
            cout << "        Sleep: <"; 
            bar = people[x].getSleep() / 10;
            for (int i = 0; i < 10; ++i) {
                if(i<bar) {
                    cout << "#";
                }
                else {
                    cout << " ";
                }
            }
            cout << "> - " << people[x].getSleep() ; 

                // Happiness     
            cout << "     Happiness: <";
            bar = people[x].getHappines() / 10;
            for (int i = 0; i < 10; ++i) {
                if(i<bar) {
                    cout << "#";
                }
                else {
                    cout << " ";
                }
            }
            cout << "> - " << people[x].getHappines() << endl;

                // Hunger
            cout << "       Hunger: <";
            bar = people[x].getHunger() / 10;
            for (int i = 0; i < 10; ++i) {
                if(i<bar) {
                    cout << "#";
                }
                else {
                    cout << " ";
                }
            }
            cout << "> - " << people[x].getHunger();

                // Exercise
            cout << "      Exercise: <";
                    bar = people[x].getExercise() / 10;
            for (int i = 0; i < 10; ++i) {
                if(i<bar) {
                    cout << "#";
                }
                else {
                    cout << " ";
                }
            }
            cout << "> - " << people[x].getExercise() << endl;

            cout << endl;
        }
    }
    cout << endl;
}

void printLegand() {
    cout << "-- Legand -------------------------------------------------------------|" << endl;
    cout << "-- Top left room is the KITCHEN which is where they get food           |" << endl;
    cout << "-- Top right room is the TV ROOM which is where they get entertainment |" << endl;
    cout << "-- Bottom left room is the GYM where they can get exercise             |" << endl;
    cout << "-- Bottom right room is the BEDROOM where they get some sleep          |" << endl;
    cout << "-----------------------------------------------------------------------|" << endl << endl;
}