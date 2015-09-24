/*
 *  Classifer Systems Types and Symbolics
 *
 *
 *
 */

#include <stdlib.h>
#include "person.h"

#define MAX_COND_WIDTH		4
#define MAX_ACTION_WIDTH	3
#define MAX_CLASSIFIERS		30 // More Classifiers, more options
#define MAX_EPISODE		3

/*
typedef struct {
     
    unsigned char condition[MAX_COND_WIDTH];
    unsigned char action[MAX_ACTION_WIDTH];
    double        strength;
    double        specificity;
    double        bid;
    int           match;
    int           episode;
    
} classifier_t;
*/
typedef struct{
    unsigned char actionType[3];
} actionTypes;

#define RISK_FACTOR		((double)0.5)

#define getRand(x)		(int)((double)(x)*rand()/(RAND_MAX+1.0))
#define getSRand()      	((float)rand() / (float)RAND_MAX)

#define MAX_ACT			3

#define REWARD		1.0

int pizza( Person p[], int position, int numPeople );
