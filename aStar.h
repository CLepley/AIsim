#include "person.h"

struct Position{
	int x;
	int y;
};

void go ( Person people[],int peopleCount );
Position randomStart();
void placeOnMap(Person person);
void setMap( );
void print();
void removeFromMap( Person person );