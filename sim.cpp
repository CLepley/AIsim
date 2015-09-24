/*
 *  Classifer System implementation
 *
 *
 *
 */
#include <iostream>

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "sim.h"

//classifier_t list[MAX_CLASSIFIERS];
int episode = 1;
int c;
actionTypes actionList[6];
unsigned char detectors[MAX_COND_WIDTH];

unsigned char getRandomCharacter( void ){
    const unsigned char alphabet[3] = {'0', '1', '#'};
    return( alphabet[getRand(3)] );
}

void readDetectors( unsigned char *condition, Person p[], int position ){
    for (int i = 0; i < MAX_COND_WIDTH; i++){
        detectors[i] = '0';
    }
    if (p[position].getSleep() < 61){ detectors[0] = '1';}
    if (p[position].getHappines() < 61){ detectors[1] = '1';}
    if (p[position].getHunger() < 61){ detectors[2] = '1';}
    if (p[position].getExercise() < 61){ detectors[3] = '1';}
}

int match( unsigned char cl, unsigned char cn ){
    if      (cl == '#') return 1;
    else if (cl == cn) return 1;
    
    return 0;
}

void calculateSpecificity( int classifier, Person p[], int position ){
    int i;
    
    p[position].list[classifier].specificity = 0.0;
    
    for (i = 0 ; i < MAX_COND_WIDTH ; i++) {
        if (p[position].list[classifier].condition[i] != '#') {
            p[position].list[classifier].specificity++;
        }
    }
    
    p[position].list[classifier].specificity /= (double)MAX_COND_WIDTH;
    
    return;
}

int createCoveringClassifier( int classifier, unsigned char *condition , Person p[], int position){
    int c, act;
    double minStrength = 99.0;
    
    if (classifier == -1) {
        
        /* Find the lowest strength classifier */
        for (c = 0 ; c < MAX_CLASSIFIERS ; c++) {
            if (p[position].list[c].strength < minStrength) {
                classifier = c;
                minStrength = p[position].list[c].strength;
            }
        }
        
    }
    
    /* Replace this classifier with a covering classifier */
    for (c = 0 ; c < MAX_COND_WIDTH ; c++) {
        if (getSRand() > 0.25) {
            p[position].list[classifier].condition[c] = condition[c];
        } else {
            p[position].list[classifier].condition[c] = '#';
        }
    }
    
    for (act = 0 ; act < MAX_ACTION_WIDTH ; act++) {
        p[position].list[classifier].action[act] = 0x30 + getRand(2);
    }
    
    p[position].list[classifier].strength = 1.0;
    p[position].list[classifier].episode = 0;
    
    calculateSpecificity( classifier , p, position);
    
    return classifier;
}

int matchClassifiers( unsigned char *condition , Person p[], int position){
    int c;
    int i;
    double bidSum = 0.0;
    int matchFound = 0;
    
    /* Match condition with the list of classifiers */
    for (c = 0 ; c < MAX_CLASSIFIERS ; c++) {
        
        p[position].list[c].match = 0;
        
        for (i = 0 ; i < MAX_COND_WIDTH ; i++) {
            if (!match(p[position].list[c].condition[i], condition[i])) break;
            
        }
        //std::cout << "ioioioioioio i = " << i << "max cond witch = " << MAX_COND_WIDTH << std::endl;
        if (i == MAX_COND_WIDTH) {
            p[position].list[c].match = 1;
            p[position].list[c].bid = RISK_FACTOR * p[position].list[c].strength * p[position].list[c].specificity;
            //std::cout << "-=-==--bid should be this" << RISK_FACTOR * p[position].list[c].strength * p[position].list[c].specificity << std::endl;
            //std::cout << "-=-=-=actual bis is " << p[position].list[c].bid << std::endl;
            bidSum += p[position].list[c].bid;
            matchFound = 1;
        }
        
    }
    
    if (matchFound) {
        
        int tries = MAX_CLASSIFIERS;
        
        c = getRand(MAX_CLASSIFIERS);
        while (1) {
            
            if (p[position].list[c].match) {
                if ( (p[position].list[c].bid / bidSum) > getSRand()) break;
                if ( --tries == 0) break;
            }
            
            if (++c >= MAX_CLASSIFIERS) c = 0;
            
        }
        
    } else {
        
        if (c == MAX_CLASSIFIERS) {
            c = createCoveringClassifier( -1, condition , p, position);
        }
        
    }
    
    return c;
}

void sendEffectors( int classifier, Person p[], int position){
    int i;
    int act=0;
    
    for (i = MAX_ACTION_WIDTH-1 ; i >= 0 ; i--) {
        
        if ( p[position].list[classifier].action[i] == '1') {
            
            act |= (1 << ((MAX_ACTION_WIDTH-1) - i));
            
        }
        
        
    }
    
    //list[classifier].strength += REWARD;
    
    p[position].list[classifier].episode = 1;
    
    return;
}
/*
void initClassifier( int classifier ){//only allow 3 "1"
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

void initPet( void ){
    int c;
    
    // Create the classifier population 
    for (c = 0 ; c < MAX_CLASSIFIERS ; c++) {
        initClassifier( c );
    }
}
*/
void initActions(){
    actionList[0].actionType[0] = 0x30;actionList[0].actionType[1] = 0x30;actionList[0].actionType[2] = 0x30;
    actionList[1].actionType[0] = 0x30;actionList[1].actionType[1] = 0x30;actionList[1].actionType[2] = 0x31;
    actionList[2].actionType[0] = 0x30;actionList[2].actionType[1] = 0x31;actionList[2].actionType[2] = 0x30;
    actionList[3].actionType[0] = 0x30;actionList[3].actionType[1] = 0x31;actionList[3].actionType[2] = 0x31;
    actionList[4].actionType[0] = 0x31;actionList[4].actionType[1] = 0x30;actionList[4].actionType[2] = 0x30;
    actionList[5].actionType[0] = 0x31;actionList[5].actionType[1] = 0x30;actionList[5].actionType[2] = 0x31;
}

// write to file
void printRules(Person p[], int position){
    int i,e;
    
    // Writing episode
    for (i = 0; i < MAX_CLASSIFIERS; i++){
        printf("%i|",i);
        
        for (e = 0; e < MAX_COND_WIDTH; e++){
            printf("%c",p[position].list[i].condition[e]);
        }
        printf("|");
        for (e = 0; e < MAX_ACTION_WIDTH; e++){
            printf("%c",p[position].list[i].action[e]);
        }
        printf("|%f\n",p[position].list[i].strength);
    }
}

void printSignleRule(int c){
    //printf("%i|",c);
    int e;
    for (e = 0; e < MAX_COND_WIDTH; e++){
        //printf("%c",list[c].condition[e]);
    }
    //printf("|");
    for (e = 0; e < MAX_ACTION_WIDTH; e++){
        //printf("%c",list[c].action[e]);
    }
    //printf("|%f\n",list[c].strength);  
}
/*
void printEnviroment( unsigned char enviroment[] ) {
    // Print conditions
    
    int i;
    for (i = 0; i < MAX_COND_WIDTH;i++){
        //printf("%c",enviroment[i]);
    }
    printf( "\nEnviroment Conditions: " );
    
    // If it is day or night
    if( enviroment[ 0 ] == '1'  )
        printf( "It is day " );
    else
        printf( "It is night" );
    
    // Doorbell
    if( enviroment[ 1 ] == '1'  )
        printf( ", Doorbell is ringing" );
    // Stranger Command
    if( enviroment[ 2 ] == '1'  )
        printf( ", Stranger is saying \"Lay down\"" );
    // Stranger in the house
    if( enviroment[ 3 ] == '1'  )
        printf( ", A stranger is in the house" );
    // Stranger outside
    if( enviroment[ 4 ] == '1'  )
        printf( ", A stranger is outside the house" );
    // Cat outside
    if( enviroment[ 5 ] == '1'  )
        printf( ", A cat is outside the house" );
    // The pet is outside
    if( enviroment[ 6 ] == '1'  )
        printf( ", The virtual pet is outside the house" );
    // Owner commands
    if( enviroment[ 7 ] == '1'  )
        printf( ", Owner is saying \"Lay down\"" );
    
    printf( "\n\n" );
}

void printAction( unsigned char action[] ) {
    
    printf( "Pets Action: " );
    // Starts with 0
    if( action[ 0 ] == '0' ) {
        // Middle is 0
        if( action[ 1 ] == '0' ) {
            // Ends with 0 else 1
            if( action[ 2 ] == '0' )
                printf( "Go Potty \n" );
            else
                printf( "Lay Down \n" );
        }
        // Middle is 1
        else
            // Ends with 0 else 1
            if( action[ 2 ] == '0' )
                printf( "Bite Nearest Person \n" );
            else
                printf( "Run Around \n" );
    }
    // Starts with 1
    else {
        // Ends with 0 else 1
        if( action[ 2 ] == '0' )
            printf( "Ignore \n" );
        else
            printf( "Bark%c \n",0x7 );
    } 
}
/*
void initSim(){
    srand( (unsigned int)time(NULL) );
    
    initActions();
    
    initPet();
}
*/
void rewardSim( unsigned char action[], unsigned char *condition , Person p[], int position){
    double treats = 0.0;
    if( action[ 0 ] == '0' ) {
        // Middle is 0
        if( action[ 1 ] == '0' ) {
            // Ends with 0 else 1
            if( action[ 2 ] == '0' ){
                //hangout
                for (int i = 0; i < MAX_COND_WIDTH; i++){
                    if (condition[i] == 1){
                        treats -= 1.5;
                    }
                }
            }
            else{
                //kitchen
                if (condition[2] == 1){
                    treats += 2.0;
                }
                else {
                    treats -= 2.5;
                }
            }
        }
        // Middle is 1
        else{
            // Ends with 0 else 1
            if( action[ 2 ] == '0' ){
                //tv
                if (condition[1] == 1){
                    treats += 2.0;
                }
                else {
                    treats -= 2.5;
                }
            }
            else{
                //gym
                if (condition[3] == 1){
                    treats += 2.0;
                }
                else {
                    treats -= 2.5;
                }
            }
        }   
    }
    // Starts with 1
    else {
        // Ends with 0 else 1
        if( action[ 2 ] == '0' ){
            //bed
            if (condition[0] == 1){
               treats += 2.0;
            }
            else {
                    treats -= 2.5;
                }
        }
    }

    /* Tax the last classifer */
    //std::cout << "the bid for this person is" << p[position].list[c].bid << std::endl;
    p[position].list[c].strength -= p[position].list[c].bid;
    //printf("Taxing classifer %i by the amount %f\n", c, list[c].bid);
    if (p[position].list[c].strength < 0.0) p[position].list[c].strength = 0.0;
    
    /* Reward classifiers that supported the episode */
    //give puupy treats here....or dont, I dont care
    //giveTreat(usedClassifiers, treats);
    for (int i = 0 ; i < MAX_CLASSIFIERS ; i++) {
        if ((p[position].list[i].episode) && (i != c)) {
            p[position].list[i].strength += (treats);
            if (p[position].list[i].strength < 0){
                p[position].list[i].strength = 0;
            }
            p[position].list[i].episode = 0;
        }
    }
}

int getAction( unsigned char action[] ){
    // Starts with 0
        //std::cout << "this is the action " << action << std::endl;

    if( action[ 0 ] == '0' ) {
        // Middle is 0
        if( action[ 1 ] == '0' ) {
            // Ends with 0 else 1
            if( action[ 2 ] == '0' )
                //hangout
                return 0;
            else
                //kitchen
                return 1;
        }
        // Middle is 1
        else
            // Ends with 0 else 1
            if( action[ 2 ] == '0' )
                //tv
                return 2;
            else
                //gym
                return 3;
    }
    // Starts with 1
    else {
        // Ends with 0 else 1
        if( action[ 2 ] == '0' )
            //bed
            return 4;
        else 
            //mate
            return 5;
    }
 }
 
int pizza(Person p[], int position, int numPeople){

        int check = 1;
        int isMating = 0;
        int actionID;
        initActions();     

        while(check){
            isMating = 0;
            srand( time( NULL ) );
            readDetectors( detectors, p, position );  //sets an action ie: day | cat outside | etc
            c = matchClassifiers( detectors ,p, position);
            sendEffectors(c,p, position);
            
            actionID = getAction(p[position].list[c].action);
            //check to see if anyoneelse is mating
            if (actionID == 5 && numPeople > 1){
                for (int i = 0; i < numPeople; i++){
                    if (p[i].getActionID() == 5 && i != position){
                        isMating = 1;
                    }
                }
            }
            if (!isMating){
                //reward them
                check = 0;
                rewardSim(p[position].list[c].action, detectors, p, position);
                return actionID;
            }
        }


        // ARE WE EVEN INITING THE SIMs
}
