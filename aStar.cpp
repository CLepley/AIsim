////////////////////////////////////////////////
// Sims 
// AI path finding using A*
// Creators: Chris Lepley and Tyler Vasko
////////////////////////////////////////////////

// Includes /////////////////////////////////// 
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>

#include "aStar.h"

using namespace std;
#define DEBUG 0
/////////////////////////////////////////////

int calcH(int currentX, int currentY);

// Variables /////////////////////////////////////////////////////////////////////////////////

			//2d array map of everything

	// This is what the a* is going off of
struct node {
	int xCoord;
	int yCoord;
	struct node* parent;
	struct node* next;
	struct node* prev;
	int h, g, f;
};

int xMap = 13;
int yMap = 17;	

char map[17][13];
	
	// This will be for the a* 
node* openList = NULL;
node* closedList = NULL;

	// This will keep track of the open list for easy traversing
node* openTailPointer = NULL;
node* openCurrentPointer = NULL;
	
	// This will keep track of the close list for easy traversing
node* closeTailPointer = NULL;
node* closeCurrentPointer = NULL;

   // Map Markers
				//size of the map
int xStart, yStart;				//starting postion on the map
int xGoal, yGoal;				//goal postion on the map

bool complete = false;			// Game won 
bool failed = false;			// Game lost 
/////////////////////////////////////////////////////////////////////////////////////////////

///////// Print ///////////////////////////////////
void print(){
	//Print Map
	for(int i = 0; i < yMap; i++) {
		for(int j = 0; j < xMap; j++) {
			cout << ' ' << map[i][j];
		}
		cout << "\n";
	}
}
////////////////////////////////////////////

Position randomStart() {
	srand( time( NULL ) );
	
	int goodLocation = 0;
	Position startLocation;
	while( goodLocation == 0 ) {
		startLocation.x = rand() % 4 + 6;
		startLocation.y = rand() % 4 + 5;

		if ( map[startLocation.y][startLocation.x] == ' ') {
			goodLocation = 1;
		}
	}
	
	return startLocation;
}
///////////////////////////////

void placeOnMap(Person person) {
	char letter = person.getMyLetter();
	map[person.getCurrentY()][person.getCurrentX()] = letter;
}
///////////////////////////////

// Add Path to Map ////////////////////////////////////////////////////////////////////////////////////
void addPath() {
	closeCurrentPointer = closeTailPointer -> parent;

	while(closeCurrentPointer -> parent) {
		map[closeCurrentPointer->yCoord][closeCurrentPointer->xCoord] = '*';
		closeCurrentPointer = closeCurrentPointer -> parent;
	}	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void findMate( Person people[], int currentPersonIndex, int matesindex ) {

	int distanceUp=0, distanceDown=0, distanceLeft=0, distanceRight=0;

		// Right
	if( map[ people[matesindex].getCurrentY() ][people[matesindex].getCurrentX() + 1 ] == ' ' ){
		xGoal =  people[matesindex].getCurrentX() + 1;
		yGoal =  people[matesindex].getCurrentY();
		distanceRight = calcH(people[currentPersonIndex].getCurrentX(), people[currentPersonIndex].getCurrentY());
	}
	else {
		distanceRight = 1000;
	}
		// Left
	if( map[people[matesindex].getCurrentY()][people[matesindex].getCurrentX() - 1] == ' ' ){
		xGoal =  people[matesindex].getCurrentX() - 1;
		yGoal =  people[matesindex].getCurrentY();
		distanceLeft = calcH(people[currentPersonIndex].getCurrentX(), people[currentPersonIndex].getCurrentY());
	}
	else {
		distanceLeft = 1000;
	}
		// Down
	if( map[people[matesindex].getCurrentY() + 1][people[matesindex].getCurrentX()] == ' ' ){
		xGoal =  people[matesindex].getCurrentX();
		yGoal =  people[matesindex].getCurrentY() + 1;
		distanceDown = calcH(people[currentPersonIndex].getCurrentX(), people[currentPersonIndex].getCurrentY());
	}
	else {
		distanceDown = 1000;
	}
		// Up
	if( map[people[matesindex].getCurrentY()-1][people[matesindex].getCurrentX() ] == ' ' ){
		xGoal =  people[matesindex].getCurrentX();
		yGoal =  people[matesindex].getCurrentY()-1;
		distanceUp = calcH(people[currentPersonIndex].getCurrentX(), people[currentPersonIndex].getCurrentY());
	}
	else {
		distanceUp = 1000;
	}

	cout << "Up: " << distanceUp << ", Down: " << distanceDown << ", Left: " << distanceLeft << ", Right: " << distanceRight;

		// Up
	if( distanceUp <= distanceDown && distanceUp <= distanceRight && distanceUp <= distanceLeft ) {
		xGoal =  people[matesindex].getCurrentX();
		yGoal =  people[matesindex].getCurrentY() - 1;
		cout << "Up!! ";
	}
		// Down
	else if( distanceDown<= distanceUp  && distanceDown <= distanceRight && distanceDown <= distanceLeft ) {
		xGoal =  people[matesindex].getCurrentX();
		yGoal =  people[matesindex].getCurrentY() + 1;
		cout << "Down!! ";
	}
		// Left
	else if( distanceLeft <= distanceUp  && distanceLeft <= distanceRight && distanceLeft <= distanceDown ) {
		xGoal =  people[matesindex].getCurrentX() - 1;
		yGoal =  people[matesindex].getCurrentY();
		cout << "Left!! ";
	}
		// Right
	else if( distanceRight <= distanceUp  && distanceRight <= distanceDown && distanceRight <= distanceLeft ) {
		xGoal =  people[matesindex].getCurrentX() + 1;
		yGoal =  people[matesindex].getCurrentY();
		cout << "Right!!";
	}
	cout << endl;
}


int setPosition( Person people[], int currentPerson, int peopleCount ) {
	xStart = people[currentPerson].getCurrentX();
	yStart = people[currentPerson].getCurrentY();
	int found = 0;

	if( people[currentPerson].getMateID() == -1 ) {
		xGoal = people[currentPerson].getDestinationX();
		yGoal = people[currentPerson].getDestinationY();
	}
	else {
		int mateID = people[currentPerson].getMateID();
		
		for(int x=0; x < peopleCount && found == 0; x++ ) {
			if( x != currentPerson && people[x].getPersonID() != -1) {
				if( people[x].getPersonID() == mateID ) {

					findMate( people, currentPerson, x );
					found = 1;
				}
			}
		}
	}
	if(DEBUG) {
		cout << people[ currentPerson ].getMyLetter() << " ---  xGoal: " << xGoal << " --- yGoal: " << yGoal << endl;
		//sleep(2);
	}
	return found;
}
///////////////////////////////

// Progress Smigla /////////////////////////////////////////////////////////////////////////////////////////////////
void moveSmigla( Person people[], int currentPerson ) {

	closeCurrentPointer = closeTailPointer;
	
	while ( closeCurrentPointer && map[closeCurrentPointer->parent->yCoord][closeCurrentPointer->parent->xCoord] != people[currentPerson].getMyLetter() ) {
			closeCurrentPointer = closeCurrentPointer -> parent;
	}
	map[closeCurrentPointer->parent->yCoord][closeCurrentPointer->parent->xCoord] = ' ';
	map[closeCurrentPointer->yCoord][closeCurrentPointer->xCoord] = people[currentPerson].getMyLetter();
	people[currentPerson].setCurrentX( closeCurrentPointer->xCoord );
	people[currentPerson].setCurrentY( closeCurrentPointer->yCoord );
	
	if( people[currentPerson].getCurrentX() == xGoal && people[currentPerson].getCurrentY() == yGoal ) {
		people[ currentPerson ].setMoving( 0 );
		if( people[currentPerson].getActionID() != 0 ) {
			people[ currentPerson ].setPerformingAction( 1 );
		}
	}
	
}
///////////////////////////////

// Clear the Open and Closed Lists /////////////////////////////////////////////////////////////////////////////////////////////////////
void clearLists() {
	char* responce;
	int num = 0;
		
		// Clears the closed
	closeCurrentPointer = closeTailPointer-> prev;
	closeTailPointer = NULL;
	while( closeCurrentPointer ) {
		free( closeCurrentPointer->next );
		closeCurrentPointer = closeCurrentPointer->prev;
	 }
	free( closedList );
	closedList = NULL;

		//clears the open
	openCurrentPointer = openList-> next;
	openList = NULL;
	while( openCurrentPointer ) {
		free( openCurrentPointer->prev );
		openCurrentPointer = openCurrentPointer->next;
	}
	free( openTailPointer );
	openTailPointer = NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Calculate H /////////////////////////////////////////////////////////////////////
int calcH(int currentX, int currentY) {
		// Storage variables
	int a, b;
	
	if (currentX == xGoal) {
		return abs(currentX - xGoal);
	}
	else if (currentY == yGoal) {
		return abs(currentY - yGoal);
	}
	else {
		a = abs(currentY - yGoal);
		b = abs(currentX - xGoal);
		a *= a;
		b *= b;
		return (int)sqrt(a+b);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check if move is in map ////////////////////////////////////////////////////////////////////
bool isInBounds ( int currentX, int currentY ) {

	if( currentX < 0 || currentY < 0) {
		return false;
	}
	else if(currentX > xMap-1 || currentY > yMap-1){
		return false;
	}
	else if( map[currentY][currentX] != ' ' ) {
		return false;
	}
	else {
		return true;
	}
}
//////////////////////////////////////////////////////////////////////////

// Check if node is on the Closed List /////////////////////////////////////////////////////////////////////////////////////////////////
bool onClosedList( int xCoord, int yCoord ) {

	closeCurrentPointer = closeTailPointer;
	while( closeCurrentPointer ) {
		if ( closeCurrentPointer->xCoord == xCoord && closeCurrentPointer->yCoord == yCoord ) {
			closeCurrentPointer = NULL;
			return true;
		}
		else {
			closeCurrentPointer = closeCurrentPointer -> prev;
		}		
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Gets the best move //////////////////////////////////////////////////////////////////////////////////////////////////
void getBestOpen() {

	node* lowest;
	openCurrentPointer = openTailPointer;
	lowest = openCurrentPointer;

	if( openTailPointer && openList ) {
		while( openCurrentPointer ) {
			if( lowest->f  > openCurrentPointer->f ) {
				lowest = openCurrentPointer;
			}
			openCurrentPointer = openCurrentPointer->prev;
		}
		
			//remove only one
		if(!lowest->prev && !lowest->next){
			openList = NULL;
			openTailPointer = NULL;
		}
			//remove from front of list
		else if( !lowest->prev && lowest->next ){
			openList = lowest->next;
			openList->prev = NULL;
			lowest->next = NULL;
		}
			//remove from back of list
		else if(!lowest->next && lowest->prev ){
			openTailPointer = lowest->prev;
			openTailPointer->next = NULL;
			lowest->prev = NULL;
		}	
			//remove from middle
		else{
			lowest->prev->next = lowest->next;
			lowest->next->prev = lowest->prev;
			lowest->next = NULL;
			lowest->prev = NULL;
		}
			// addes to the closed list
		lowest ->next = NULL;
		lowest ->prev = NULL;
		lowest-> prev = closeTailPointer;
		closeTailPointer-> next = lowest;
		closeTailPointer = lowest;
	}
	else {
		failed = true;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

// up, down, right, left
// calcPath ////////////////////////////////////////////////////////////////////////////////////////////
bool calcPath(int tick) {
	
	node* up;
	node* down;
	node* right;
	node* left;
	
	if(tick == 0) {
		openList = NULL;
		closedList = NULL;
		closeTailPointer = NULL;

		closedList = new node;
		closedList -> xCoord = xStart;
		closedList -> yCoord = yStart;
		closedList -> parent = NULL;
		closedList -> h = calcH(xStart, yStart);
		closedList -> g = 0;
		closedList -> f = closedList -> h + closedList -> g;
		closedList -> next = NULL;
		closedList -> prev = NULL;
		closeTailPointer = closedList;
	}
		// up
	if( isInBounds( closeTailPointer->xCoord, closeTailPointer->yCoord+1 )) {
		if( !onClosedList( closeTailPointer->xCoord, closeTailPointer->yCoord+1 ) ) { 
			up = new node;
			up->xCoord = closeTailPointer->xCoord;
			up->yCoord = closeTailPointer->yCoord + 1;
			up->parent = closeTailPointer;
			up->h = calcH(up ->xCoord, up->yCoord);
			up-> next = NULL;
			up->prev = NULL;
			up->g = closeTailPointer-> g + 1;
			up->f = up-> g + up-> h;
			
			//add to open array
			//new list
			if(!openList){
				openList = up;
				//openHeadPointer = up;
				openTailPointer = up;
			}
			//add to end of list
			else{
				up->next=NULL;
				openTailPointer->next = up;
				up->prev = openTailPointer;
				openTailPointer = up;
			}		
		}
	}
		//down
	if( isInBounds( closeTailPointer->xCoord, closeTailPointer->yCoord-1 )) {
		if( !onClosedList( closeTailPointer->xCoord, closeTailPointer->yCoord-1 ) ) {
			down = new node;
			down->xCoord = closeTailPointer->xCoord;
			down->yCoord = closeTailPointer->yCoord -1;
			down->parent = closeTailPointer;
			down->h = calcH(down ->xCoord, down->yCoord);
			down->g = closeTailPointer-> g + 1;
			down->f = down-> g + down-> h;
			down-> next = NULL;
			down->prev = NULL;;

			//new list
			if(!openList){
				openList = down;
				//openHeadPointer = down;
				openTailPointer = down;
			}
			//add to end of list
			else{
				down->next=NULL;
				openTailPointer->next = down;
				down->prev = openTailPointer;
				openTailPointer = down;
			}	
		}	
	}
		//right
	if( isInBounds( closeTailPointer->xCoord+1, closeTailPointer->yCoord )) {
		if( !onClosedList( closeTailPointer->xCoord+1, closeTailPointer->yCoord ) ) {
			right = new node;
			right->xCoord = closeTailPointer->xCoord + 1;
			right->yCoord = closeTailPointer->yCoord;
			right->parent = closeTailPointer;
			right->h = calcH(right ->xCoord, right->yCoord);
			right->g = closeTailPointer-> g + 1;
			right->f = right-> g + right-> h;
			right-> next = NULL;
			right->prev = NULL;
			
				//new list
			if(!openList){
				openList = right;
				//openHeadPointer = right;
				openTailPointer = right;
			}
				//add to end of list
			else{
				right->next=NULL;
				openTailPointer->next = right;
				right->prev = openTailPointer;
				openTailPointer = right;
			}
		}	
	}
	//left
	if( isInBounds( closeTailPointer->xCoord-1, closeTailPointer->yCoord )) {
		if( !onClosedList( closeTailPointer->xCoord-1, closeTailPointer->yCoord ) ) {
			left = new node;
			left->xCoord = closeTailPointer->xCoord -1;
			left->yCoord = closeTailPointer->yCoord;
			left->parent = closeTailPointer;
			left->h = calcH(left ->xCoord, left->yCoord);
			left->g = closeTailPointer-> g + 1;
			left->f = left-> g + left-> h;
			left-> next = NULL;
			left->prev = NULL;
			
				//new list
			if(!openList){
				openList = left;
				//openHeadPointer = left;
				openTailPointer = left;
			}
				//add to end of list
			else{
				left->next=NULL;
				openTailPointer->next = left;
				left->prev = openTailPointer;
				openTailPointer = left;
			}	
		}	
	}
			
	getBestOpen();
	
	if ( failed ) {
		return false;
	}
	
	if( closeTailPointer->xCoord == xGoal && closeTailPointer->yCoord == yGoal ) {
		return true;
	}	
	else {
		return false;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Map Creation ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setMap( ) {
		// Sets met to be all blank spaces
	for(int i = 0; i < yMap; i++) {
		for(int j = 0; j < xMap; j++) {
			map[ i ][ j ] = ' ';
		}
	}

		// Sets top and bottom row bounds
	for(int j = 0; j < xMap; j++) {
			map[ 0 ][ j ] = '*';
			map[ yMap-1 ][ j ] = '*';
	}

		// Sets left and right sides
	for(int j = 1; j < yMap-1; j++) {
			map[ j ][ 0 ] = '*';
			map[ j ][ xMap-1 ] = '*';
	}
	
		// Tedious bull shit that is needed to be hard coded
		// Kitchen
	map[ 1 ][ 4 ] = '*';
	map[ 4 ][ 4 ] = '*';
	map[ 5 ][ 4 ] = '*';
	map[ 5 ][ 1 ] = '*';
	//map[ 5 ][ 2 ] = '*';
	map[ 5 ][ 3 ] = '*';
		// TV Room
	map[ 1 ][ 8 ] = '*';
	map[ 4 ][ 8 ] = '*';
	map[ 5 ][ 8 ] = '*';
	map[ 5 ][ 9 ] = '*';
	//map[ 5 ][ 10 ] = '*';
	map[ 5 ][ 11 ] = '*';	
		// Gym
	map[ 15 ][ 4 ] = '*';
	//map[ 14 ][ 4 ] = '*';
	map[ 13 ][ 4 ] = '*';
	map[ 12 ][ 4 ] = '*';
	map[ 11 ][ 4 ] = '*';
	map[ 11 ][ 3 ] = '*';
		// Bedroom
	map[ 13 ][ 8 ] = '*';
	//map[ 12 ][ 8 ] = '*';
	map[ 11 ][ 8 ] = '*';
	map[ 11 ][ 9 ] = '*';
	map[ 11 ][ 10 ] = '*';
	map[ 11 ][ 11 ] = '*';

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check for Desination ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int checkForSetPath( int x, int y ) {
	if( x != -1 && y != -1 ) 
		return 1;
	else
		return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This checks map avability ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int checkAviability( int x, int y, Person person ) {
	
	if( x != person.getCurrentX() || y != person.getCurrentY() ) {
		if( x != person.getDestinationX() || y != person.getDestinationY() ) {
			return 1;
		}
		//cout << "Person: " << person.getMyLetter() << " is going to this spot \n";
		return 0;
	}
	//cout << "Person: " << person.getMyLetter() << " has this spot \n";
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This gives a person a location //////////////////////////////////////////////////////////////////////////////////////////////////////////
void giveDestination( Person people[], int currentPersonIndex, int peopleCount ) {
		// This is what room the person will be going to
	int room = people[ currentPersonIndex ].getActionID();
		// Thsi will be used as a found or ready to leave variable
	int freeSpotFound = 1;
	
	switch( room ) {
		case 0:
			bool foundNextSpot;
			foundNextSpot = false;
			int direction;
			//
			srand( time( NULL ) );
			while( !foundNextSpot ){
				
				direction = rand() % 4;
				if( direction == 0 ){ // Up
					if( map[ people[ currentPersonIndex ].getCurrentY() -1 ][ people[ currentPersonIndex ].getCurrentX() ] == ' ' ){
						people[currentPersonIndex].setDestinationX( people[ currentPersonIndex ].getCurrentX() );
						people[currentPersonIndex].setDestinationY( people[ currentPersonIndex ].getCurrentY()-1 );
						foundNextSpot = true;	
					}
					else {
						direction = 1;
					}
				}
				else if( direction == 1 ){ // Right
					if( map[ people[ currentPersonIndex ].getCurrentY() ][ people[ currentPersonIndex ].getCurrentX() +1] == ' ' ){
						people[currentPersonIndex].setDestinationX( people[ currentPersonIndex ].getCurrentX()+1 );
						people[currentPersonIndex].setDestinationY( people[ currentPersonIndex ].getCurrentY() );
						foundNextSpot = true;		
					}
					else {
						direction = 2;
					}
				}
				else if( direction == 2 ){ // Down
					if( map[ people[ currentPersonIndex ].getCurrentY() +1 ][ people[ currentPersonIndex ].getCurrentX() ] == ' ' ){
						people[currentPersonIndex].setDestinationX( people[ currentPersonIndex ].getCurrentX() );
						people[currentPersonIndex].setDestinationY( people[ currentPersonIndex ].getCurrentY()+1 );
						foundNextSpot = true;		
					}
					else {
						direction = 3;
					}
				}
				else if( direction == 3 ){ // Left
					if( map[ people[ currentPersonIndex ].getCurrentY() ][ people[ currentPersonIndex ].getCurrentX() -1] == ' ' ){
						people[currentPersonIndex].setDestinationX( people[ currentPersonIndex ].getCurrentX()-1 );
						people[currentPersonIndex].setDestinationY( people[ currentPersonIndex ].getCurrentY() );
						foundNextSpot = true;		
					}
					else {
						srand( time( NULL ) );
					}
				}		
			}

			break;
		case 1:
				// Loops until no more people/ a spot is taken( freeSpotFound = 0 ) 
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 ) {
					freeSpotFound = checkAviability( 2, 1, people[ y ] );
				}
			}
				// if A free spot is found set thi location
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 2 );
				people[currentPersonIndex].setDestinationY( 1 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 2, 2, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 2 );
				people[currentPersonIndex].setDestinationY( 2 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 2, 3, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 2 );
				people[currentPersonIndex].setDestinationY( 3 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 1, 3, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 1 );
				people[currentPersonIndex].setDestinationY( 3 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			break;
		case 2:
					// Loops until no more people/ a spot is taken( freeSpotFound = 0 ) 
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 10, 1, people[ y ] );
			}
				// if A free spot is found set thi location
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 10 );
				people[currentPersonIndex].setDestinationY( 1 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 10, 2, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 10 );
				people[currentPersonIndex].setDestinationY( 2 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 10, 3, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 10 );
				people[currentPersonIndex].setDestinationY( 3 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 11, 3, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 11 );
				people[currentPersonIndex].setDestinationY( 3 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			break;
		case 3:
								// Loops until no more people/ a spot is taken( freeSpotFound = 0 ) 
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 2, 13, people[ y ] );
			}
				// if A free spot is found set thi location
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 2 );
				people[currentPersonIndex].setDestinationY( 13 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 2, 14, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 2 );
				people[currentPersonIndex].setDestinationY( 14 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 2, 15, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 2 );
				people[currentPersonIndex].setDestinationY( 15 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 1, 13, people[ y ] );
			
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 1 );
				people[currentPersonIndex].setDestinationY( 13 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			break;
		case 4:
				// Loops until no more people/ a spot is taken( freeSpotFound = 0 ) 
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 10, 13, people[ y ] );
			}
				// if A free spot is found set thi location
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 10 );
				people[currentPersonIndex].setDestinationY( 13 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 10, 14, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 10 );
				people[currentPersonIndex].setDestinationY( 14 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ ) {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 10, 15, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 10 );
				people[currentPersonIndex].setDestinationY( 15 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			for( int y=0; y < peopleCount && freeSpotFound != 0; y++ )  {
				if( people[y].getPersonID() != -1 )
					freeSpotFound = checkAviability( 11, 13, people[ y ] );
			}
			if( freeSpotFound == 1 ) {
				people[currentPersonIndex].setDestinationX( 11 );
				people[currentPersonIndex].setDestinationY( 13 );
				break;
			}
			else {
				freeSpotFound = 1;
			}
			break;
		case 5: 
			srand( time( NULL ) );
			int player = 0;
			freeSpotFound = 0;
			while( freeSpotFound == 0 ) {
				player = rand() % peopleCount;

				if( people[player].getPersonID() != people[currentPersonIndex].getPersonID() && people[player].getPersonID() != -1) {
					freeSpotFound = 1;
				}
			}
			people[currentPersonIndex].setMateID( people[player].getPersonID() );
			break;
	
	}
	people[ currentPersonIndex ].setMoving( 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool spaceIsStillFree( int x, int y ){
	if( map[y][x] == ' ' )
		return true;
	else
		return false;
}

bool isNextToMate( Person people[], int peopleCount, int currentPersonIndex ) {
	int mateID = people[currentPersonIndex].getMateID();

	for(int x=0; x < peopleCount; x++ ) {
		if( x != currentPersonIndex && people[x].getPersonID() != -1) {
			if( people[x].getPersonID() == mateID ) {
					//Up
				if( map[ people[ currentPersonIndex ].getCurrentY() -1 ][ people[ currentPersonIndex ].getCurrentX() ] == people[x].getMyLetter() ){
					return true;
				}
					// Right
				else if( map[ people[ currentPersonIndex ].getCurrentY() ][ people[ currentPersonIndex ].getCurrentX() + 1] == people[x].getMyLetter() ){
					return true;
				}
					// Down
				else if( map[ people[ currentPersonIndex ].getCurrentY() +1 ][ people[ currentPersonIndex ].getCurrentX() ] == people[x].getMyLetter() ){
					return true;
				}
					//left
				else if( map[ people[ currentPersonIndex ].getCurrentY() ][ people[ currentPersonIndex ].getCurrentX() -1] == people[x].getMyLetter() ){
					return true;
				}
			}
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void go ( Person people[], int peopleCount ) {
	
	//int isOnPath = 0;
	int turns = 0;
	bool pathFound = false;
	int noMate = 1;
	bool nextTo = false;
	for( int x = 0; x < peopleCount; x++ ) {
		if( people[x].getPersonID() != -1 ) {

			if(DEBUG) {
				cout << "Person: " <<  people[ x ].getMyLetter() << " is at position x: " << people[ x ].getCurrentX() << " y: " << people[ x ].getCurrentY() << " going to position x: " << people[ x ].getDestinationX() << " y: " << people[ x ].getDestinationY() <<" \n-- Action is " <<  people[ x ].getActionID() << ", isPerformingAction: " << people[ x ].isPerformingAction() << ", isMoving: " << people[ x ].isMoving() << endl;
				//sleep(1);
			}
					// This makes sure they are not in the middle of an action
			if( people[ x ].isPerformingAction() == 0 ) {
					// This is checking if they have a current destation
				if( people[ x ].isMoving() ) {
					if( !spaceIsStillFree( people[x].getDestinationX(),  people[x].getDestinationY() ) ) {
						giveDestination( people, x, peopleCount );
					}
					noMate = setPosition( people, x, peopleCount );
					if( noMate != 1 ) {
						nextTo = isNextToMate( people, peopleCount, x );
					}
					if( !nextTo ) {
						while(!pathFound) {
							pathFound = calcPath(turns);
							turns++;
						}
						turns = 0;
						moveSmigla( people, x );
						clearLists();
					}
					else {
						people[ x ].setMoving( 0 );
						people[ x ].setMateID( -1 );
						people[ x ].setPerformingAction( 1 );
					}
				}
				else {	
					giveDestination( people, x, peopleCount );
					noMate = setPosition( people, x, peopleCount );
					if( noMate != 1 ) {
						nextTo = isNextToMate( people, peopleCount, x );
					}
					if( !nextTo ) {
						while(!pathFound) {
							pathFound = calcPath( turns);
							turns++;  
						}
						turns = 0;
						moveSmigla( people, x );
						clearLists();
					}
					else {
						people[ x ].setMoving( 0 );
						people[ x ].setPerformingAction( 1 );
					}
				}
					// Clears the list for the next person
				
				pathFound = false;
				noMate = 0;
				nextTo = false;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void removeFromMap( Person person ) {
	map[person.getCurrentY()][person.getCurrentX()] = ' ';
}