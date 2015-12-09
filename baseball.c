/**************************************************************************
 * NAME        : NILES
 * COURSE      : ENGE300
 * DATE        : DEC. 2007
 * DESCRIPTION : Structs up the wazoo
 * FILES       : Master.csv, Batting.csv, Pitching.csv, structs.h
 *
* Program Grading Criteria
* 
* PROGRAM CORRECTNESS
* a) the program listing is complete and correct    (10)_________
* b) verification tests are complete and documented (20)_________
* c) program flowchart is complete and correct      (20)_________
* c) functions correctly                            (80)_________
*
* PROGRAM DESIGN
* a) variable names/types are well chosen           (10)_________
* b) modularity/parameters are passed appropriately (10)_________
* c) overall logic                                  (20)_________
*
* PROGRAM READABILITY AND DOCUMENTATION
* a) header comments are complete and clear         (10)_________
* b) internal comments are complete and clear       (10)_________
* c) white space and indents are used appropriately (10)_________
*
*                                           TOTAL  (200)_________
 * 
**************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include"structs.h"

#define HASHSIZE 101

#if 0
/* for reference purposes only (the implemented hash was the key deliverable) */
#define USE_LINKED_LIST 1
#endif
#if 0
#define DBG_LINKED_LIST 1
#endif

void print_line(char *line);
char * get_field();
void print_struct(MASTER *play);
void print_bat_struct(BAT *play);
void print_pit_struct(PITCH *play);
int  whapow(MASTER *selected);
unsigned int hashup();

MASTER * read_mcsv();
PITCH * read_pcsv();
BAT * read_bcsv();

//File pointers to .csv files
FILE *masfp; 
FILE *batfp;
FILE *pitfp;

/**************************
**********M*A*I*N**********
**************************/
int
main(void)
{
  MASTER *nameHashTab[HASHSIZE];    //hashTable to player names in master struct 
  MASTER *idHashTab[HASHSIZE];      //hashTable to player IDs in master struct 
  MASTER *hashTemp;             //temp struct used in hash process for reaching end of a ll
  MASTER *selected;             //temp struct used to hold a selected master record
  MASTER *play;                 //pointer to master struct 
  PITCH *pplay;                 //pointer to pitching struct 
  PITCH *pit_temp;
  BAT *bplay;                   //pointer to batting struct 
  BAT *bat_temp;
#ifdef USE_LINKED_LIST
  MASTER *head;
  MASTER *tail;
  PITCH *phead;
  PITCH *ptail;
  BAT *bhead;
  BAT *btail;
#endif
 
  int i, hashval;   //hashval for hashing, choice for input routine, i=used&abused counter
  int opt = 1;
  //char *field; 
  char filename[255];
  char temp_id[31];
  char temp_id_2[31];
  char c, choice;

  //malloc pointers
  hashTemp = (MASTER *) malloc(sizeof(MASTER));
  selected = (MASTER *) malloc(sizeof(MASTER));
  
  //intialize all linked list pointers
#ifdef USE_LINKED_LIST
  head = NULL;
  tail = NULL;
  bhead = NULL;
  btail = NULL;
  phead = NULL;
  ptail = NULL;
#endif
  bat_temp = NULL;
  pit_temp = NULL;
  hashTemp = NULL;
  selected = NULL;

  //initialize all i hashtable entries to null
  for (i=0; i < HASHSIZE; i++){
    nameHashTab[i] = NULL;         
  }
  for (i=0; i < HASHSIZE; i++){
    idHashTab[i] = NULL;         
  }

  //get files and assign to file pointers
  sprintf(filename, "Master.csv");
  if ((masfp = fopen(filename, "r")) == NULL) {
    printf ("Can't find %s\n",filename);
    exit(0);
  }
  sprintf(filename, "Batting.csv");
  if ((batfp = fopen(filename, "r")) == NULL) {
    printf ("Can't find %s\n",filename);
    exit(0);
  }
  sprintf(filename, "Pitching.csv");
  if ((pitfp = fopen(filename, "r")) == NULL) {
    printf ("Can't find %s\n",filename);
    exit(0);
  }

  printf("\nPlease wait while your database is being built... thank you for your patience...\n");

  /********************Read-in data for Master********************/
  while( (play = read_mcsv()) )
  { //HASH HERE!!!
    //by playername (last)
    if(play->lastname){                 //if you have a playername available, then... go get a
      hashval = hashup(play->lastname); //hashval for the struct associated with that name
      if (!nameHashTab[hashval]){       //if the table at that value is empty, then...
        nameHashTab[hashval] = play;    //fill it with it with the current struct
      }
      else{                                 //otherwise... temp struct hashTemp gets whatever
        hashTemp = nameHashTab[hashval];    // is in idhashTable[hashval]
        while(hashTemp->name_hash_next){        //if anything got assigned (should always be 
          hashTemp = hashTemp->name_hash_next;  //the case), then...re-assign it to the next
        }                                       //spot in the list; this flag proceeds 
                                                //to the end of the list.
        hashTemp->name_hash_next = play;    //to get here means end of list has been reached,
      }                                     //so assign the last
    }
    //by playerID 
    if(play->pid){                  //if you have a playername available, then... go
      hashval = hashup(play->pid);  //get a hashval for the struct associated with that name
      if (!idHashTab[hashval]){     //if the table at that value is empty, then...
        idHashTab[hashval] = play;  //fill it with it with the current struct
      }
      else{                                 //otherwise... temp struct hashTemp gets whatever
        hashTemp = idHashTab[hashval];      // is in idhashTable[hashval]
        while(hashTemp->pid_hash_next){         //if anything got assigned (should always be
          hashTemp = hashTemp->pid_hash_next;   //the case), then... re-assign it to the next
        }                                       // spot in the list; this flag proceeds to the
                                                //end of the list.
        hashTemp->pid_hash_next = play;     //to get here means end of list has been reached,
      }                                     //so assign the last
    } 
#ifdef USE_LINKED_LIST
    if(!head){              //create a typical linked list; start at head
      tail = play;
      head = tail;          //put head and tail on same linked list
    }
    else{
      tail->next = play;    //self-reference to make linked list
      tail = play; 
    }
#endif
  }

  /********************Read-in data for Batting****************/
  while( (bplay = read_bcsv()) )
  {                             //go get me a batting record
    hashval = hashup(bplay->pid);  //hash it up

    i = 0;                        //'assigned or not?' count
    play = idHashTab[hashval];    //go grab the corresponding idHashTab row of master records
    while(play){
#ifdef USE_LINKED_LIST
      if(!(strcmp(bplay->pid, play->pid))){   //if a match
        play->batter = bplay;
        i++;
      }
#endif
      if(!(strcmp(bplay->pid, play->pid))){     //if 0, we have our matching master record
        if(play->batter){                       //if there is something in play.batter
          bat_temp = play->batter;              //store in temp struct for batter
          while(bat_temp->next){                //while there is another account in the list,
            bat_temp = bat_temp->next;          //cycle through the list until you reach the 
          }                                     //end--i.e., null.
          bat_temp->next = bplay;   //nothing is in bat_temp.next, so store bplay there
          i++;
          break;                    //break because done; only one id match in each play var
        }
        else{       //this else statement is most handy for players with only 1 batting record
          play->batter = bplay;  //careful! This will make bplay HEAD of bat ll in master rec
          i++;
          break;    //break because done; only one match in each play var
        }
      }
      play = play->pid_hash_next;  //cycle thru the master hash linked list
    }

    if(!i){
      printf("\nUnknown batter record found; check player exists in Master database.\n");
      printf("Alien batting record is ID: %s\n", bplay->pid);
      exit(0);
    }

#ifdef USE_LINKED_LIST
    if(!bhead){          //create a typical linked list, start at head
      btail = bplay;
      bhead = btail;     //put head and tail on same linked list
    }
    else{
      btail->next = bplay;  //self-reference to make linked list
      btail = bplay; 
    }
#endif
  }

  /********************Read-in data for Pitching****************/
  while( (pplay = read_pcsv()) )
  {
    hashval = hashup(pplay->pid);

    i = 0;                        //'assigned or not?' count
    play = idHashTab[hashval];    //go grab the corresponding idHashTab row of master records
    while(play)
    {
#ifdef USE_LINKED_LIST
      if(!(strcmp(pplay->pid, play->pid))){   //if a match
        play->pitcher = pplay;
        i++;
      }
#endif
      if(!(strcmp(pplay->pid, play->pid))){     //if 0, we have our matching master record
        if(play->pitcher){                      //if there is something in play.pitcher
          pit_temp = play->pitcher;             //store in temp struct for pitcher
          while(pit_temp->next){                //while there is another account in the list,
            pit_temp = pit_temp->next;          //cycle through the list until you reach the 
          }                                     //end--i.e., null.
          pit_temp->next = pplay;   //nothing is in pit_temp.next, so store pplay there
          i++;
          break;                    //break because done; only one id match in each play var
        }
        else{  //this else statement is most handy for players with only 1 pitching record
          play->pitcher = pplay;  //careful! This will make pplay HEAD of pit ll in master rec
          i++;
          break;  //break because done; only one match in each play var
        }
      }
      play = play->pid_hash_next;  //cycle thru the master hash linked list
    }

    if(!i){
      printf("\nUnknown pitcher record found; check player exists in Master database.\n");
      printf("Alien pitching record is ID: %s\n", pplay->pid);
      exit(0);
    }

#ifdef USE_LINKED_LIST
    if(!phead){         //create the linked list, start at head
      ptail = pplay;
      phead = ptail;     //put head and tail on same linked list
    }
    else{
      ptail->next = pplay;  //self-reference to make linked list
      ptail = pplay; 
    }
#endif
  }

#if defined(USE_LINKED_LIST) && defined(DBG_LINKED_LIST)
  //now print Master linked list; begin at head and proceed thru *next until tail is reached
  play = head;
  while(play)
  {
    printf("%s, %s\n", play->lastname, play->firstname);
    play = play->next;
  }
  //now print out batter linked list
  bplay = bhead;
  while(bplay)
  {
    printf("%s, %s\n", bplay->pid, bplay->yid);
    bplay = bplay->next;
  }
  //now print out pitcher linked list
  pplay = phead;
  while(pplay)
  {
    printf("%s, %s\n", pplay->pid, pplay->yid);
    pplay = pplay->next;
  }
#endif

#if 0
  /***************Now print hash table*************************/
  printf("ready for printing nameHashTab\n\n");fflush(stdout);
    //nameHashTab
    for(i=0; i < HASHSIZE; i++){
      play = nameHashTab[i];
      while(play){
        print_struct(play);
        play = play->name_hash_next;
      }
      printf("ITERATION: %d\n\n",i);fflush(stdout);
    }
    //idHashTab
    printf("ready for printing idHashTab\n\n");fflush(stdout);
    for(i=0; i < HASHSIZE; i++){
      play = idHashTab[i];
      while(play){
        print_struct(play);     //prints master struct of player data
        if(play->batter){           //if play has a batter record
          bat_temp = play->batter;  //give it to bat_temp struct var
          while(bat_temp){           
            print_bat_struct(bat_temp); //print some fields of batter data
            bat_temp = bat_temp->next;  //now cycle through until end of ll of batting records
          }
        }
        if(play->pitcher){              //if play has a batter record
          pit_temp = play->pitcher;     //give it to bat_temp struct var
          while(pit_temp){           
            print_pit_struct(pit_temp); //print some fields of pitcher data
            pit_temp = pit_temp->next;  //now cycle through until end of ll of pitching recs
          }
        }
        play = play->pid_hash_next;
      }
      printf("ITERATION: %d\n\n",i);fflush(stdout);
    }
#endif

//exit(0); //temp exit for testing play->batter->next syntax


  /**********ENHANCED Interactive input routine for searching**************/

  while(opt)
  {
    //massive loop allows for continual player lookup
    printf("To Access database via PlayerID, PRESS 1\n");
    printf("To Access database via PlayerName, PRESS 2: ");
    choice = getchar();
    while(getchar() != '\n');  //get the newline out of the way (flush the line)
  
    if(choice == '1'){
       //enter playID# and search and print
       printf("\nYou have chosen to search by playID\n");
       printf("\nPlease enter the Player ID and press RETURN: ");
    
       i = 0;
       while((c = getchar()) != '\n'){   
         temp_id[i++] = c;
       }
       temp_id[i] = '\0';
       printf("\n");       //aesthetic print 

       hashval = hashup(temp_id);

       i = 0;
       play = idHashTab[hashval];
       while(play){
         if(!(strcasecmp(temp_id, play->pid))){   //if a match
           selected = play;             //this is the desired player account
           print_struct(selected);
           i++;
         }
         play = play->pid_hash_next;
       }
       if(!i){
         printf("\nNo match found... please check your entry and retry.\n");
         exit(0);
       }
#ifdef USE_LINKED_LIST
       //FOR USING A TYPICAL LINKED LIST:
       play = head;
       while(play){    //walk linked list
         if(!(strcasecmp(temp_id, play->pid))){   //if a match
           print_struct(play);             //print the stats for the match
         }
         play = play->next;
       }
       printf("\nAll Matches returned.\n");
#endif
    }

    else if(choice == '2'){
      //enter playerName and search and print
       printf("\nYou have chosen to search by Name\n");
       printf("\nPlease enter the LAST NAME of the player and press RETURN: ");
     
       i = 0;
       while((c = getchar()) != '\n'){   
         temp_id[i++] = c;
       }
       temp_id[i] = '\0';
       printf("\n");       //aesthetic print 

       hashval = hashup(temp_id);

       i = 0;
       play = nameHashTab[hashval];
       while(play)
       {
         if(!(strcasecmp(temp_id, play->lastname))){   //if a match
           printf("%d.  ", (i+1));   //there's your numbered list
           print_struct(play);
           selected = play;  //for a single match, selected will be assigned only once
           i++;
         }
         play = play->name_hash_next;
       }

       if(!i){
         printf("\nNo match found... please check your entry and retry.\n");
         exit(0);
       }

       //single out a single account from multiple matches based on last name
       if(i > 1){ 
         printf("\n!!!MORE THAN ONE MATCH HAS BEEN DETECTED!!!\n");
         printf("Please enter the FIRST name of the desired account: ");
         i = 0;
         while((c = getchar()) != '\n')
         {
           temp_id_2[i++] = c;
         }
         temp_id_2[i] = '\0';
         printf("\n");
     
         i = 0;
         play = nameHashTab[hashval];
         while(play)
         {
           if(!(strcasecmp(temp_id, play->lastname))){   //if a last name match
             if(!(strcasecmp(temp_id_2, play->firstname))){   //if a first name match
               selected = play;             //this is the desired player account
               print_struct(selected);
               i++;
             }
           }
           play = play->name_hash_next;
         }

         if(!i){  //entered first name couldn't be found
           printf("\nNo match found... please check your entry and retry.\n");
           exit(0);
         }

         if(i > 1){  //more than one match of last AND first name found
           printf("\n!!!MORE THAN ONE MATCH HAS BEEN DETECTED!!!\n");
           printf("Please enter the playerID of the desired account: ");
           i = 0;
           while((c = getchar()) != '\n'){   
             temp_id[i++] = c;
           }
           temp_id[i] = '\0';
           printf("\n");       //aesthetic print 
        
           i = 0;
           play = nameHashTab[hashval];
           while(play){
             if(!(strcasecmp(temp_id, play->pid))){   //if a playerID matches
               selected = play;             //this is the desired player account
               print_struct(selected);
               i++;
             }
             play = play->name_hash_next;
           }
         }

         if(!i){
           printf("\nNo match found... please check your entry and retry.\n");
           exit(0);
         }
       }

#ifdef USE_LINKED_LIST
       //FOR USING A TYPICAL LINKED LIST:
       play = head;
       while(play){    //walk linked list
         if(!(strcasecmp(temp_id, play->lastname))){   //if a match
           print_struct(play);             //print the stats for the match
         }
         play = play->next;
       }
       printf("\nAll Matches returned.\n");
#endif
    }
    else{
      printf("ERROR: Restart and enter only a 1 or 2.\n");
      exit(0);
    }

    //time to manipulate some output!
    printf("Time to manipulate some output!\n");
    while(whapow(selected));

    printf("\nEnter 0 to terminate program, 1 to choose another player\n");
    opt = getchar();
    while(getchar() != '\n');  //get the newline out of the way (flush the line)
    opt = opt - 48;
    //printf("option is %d\n",opt);
  }

  printf("\nPROGRAM TERMINATED\n");

  free(hashTemp);
  free(selected);

  return (0);
}

/****************************************************
 *FUNCTION   : whapow
 *DESCRIPTION: manipulates a player master record for output 
 *ARGUMENTS  : MASTER *selected -- a master record struct 
 *RETURNS    : 1 for normal op, 0 for termination
 ***************************************************/
int
whapow(MASTER *selected)
{
  char choice;
  float er = 0;
  float hits = 0;
  float atbats = 0;
  float highest = 0;
  BAT *temp_bat;
  PITCH *temp_pit;

  temp_pit = (PITCH *) malloc(sizeof(PITCH));
  temp_pit->next = NULL;

  temp_bat = (BAT *) malloc(sizeof(BAT));
  temp_bat->next = NULL;

  printf("To see specific statistics for %s %s, please enter:\n",
        selected->firstname,selected->lastname);
  printf("1 - Career Batting Average\n2 - Highest Single-Season Batting Average\n");
  printf(
    "3 - Career Earned Run Average\n4 - Lowest Single Season ERA\n0 - Quit Statistics Mode\n"
        );
  printf("\n");
  choice = getchar();
  while(getchar() != '\n');  //get the newline out of the way (flush the line)

  switch(choice)
  {
    case '0': 
      printf("\nExiting Statistics Mode...\n");
      return (0);

    case '1':    //compute Career Batting Avg (Hits / At Bats)
      if(selected->batter){
        temp_bat = selected->batter;
        while(temp_bat){
          hits = hits + temp_bat->h;
          atbats = atbats + temp_bat->ab; 
          temp_bat = temp_bat->next;     //now cycle through until end of ll of batting recs
        }
        printf("\nCareer Batting Average was %f\n\n", (hits/atbats));
      }
      else{
        printf("\nPlayer never stepped to the plate (no records found).\n");
      }
      break;

    case '2':  //find highest single season batting average
      if(selected->batter){
        temp_bat = selected->batter;
        while(temp_bat){
          if(!highest){
            hits = temp_bat->h;
            atbats = temp_bat->ab; 
            highest = hits / atbats;
            //printf("\nHighest was 0, now is %f\n\n", highest);
          }
          else{
            hits = temp_bat->h;
            atbats = temp_bat->ab; 
            if(highest < (hits / atbats)){
              highest = (hits / atbats);
              //printf("\nHighest is now %f\n\n", highest);
            }
          }
          temp_bat = temp_bat->next;     //now cycle through until end of ll of batting recs
        }
        printf("\nHighest Single-Season Batting Average was %f\n\n", highest);
      }
      else{
        printf("\nPlayer never stepped to the plate (no records found).\n\n");
      }
      break;

    case '3':  //pitchers: Career Earned Run Avg (Earned runs / 9)
      if(selected->pitcher){
        temp_pit = selected->pitcher;
        while(temp_pit){
          er = er + temp_pit->er;
          temp_pit = temp_pit->next;     //now cycle through until end of ll of pitching recs
        }
        printf("\nCareer Earned Run Average was %f\n\n", (er/9));
      }
      else{
        printf("\nPlayer never stepped on the mound (no records found).\n\n");
      }
      break;

    case '4':
      if(selected->pitcher){
        temp_pit = selected->pitcher;
        er = temp_pit->era;
        while(temp_pit){
          if((temp_pit->era) < er){
            er = temp_pit->era;
          }
          temp_pit = temp_pit->next;     //now cycle through until end of ll of batting recs
        }
        printf("\nLowest Single-Season Earned Run Average was %f\n\n", er);
      }
      else{
        printf("\nPlayer never stepped on the mound (no records found).\n\n");
      }
      break;
  }

  free(temp_pit);
  free(temp_bat);

  return (1);
}

/****************************************************
 *FUNCTION   : hashup
 *DESCRIPTION: assigns hash values 
 *ARGUMENTS  : 
 *RETURNS    : (hashval % HASHSHIZE) -- hash val for player
 ***************************************************/
unsigned int
hashup(char *s)
{
  unsigned int hashval;
  hashval = 0;

  //printf("In hashup\n");fflush(stdout);
  while(*s != '\0'){
    hashval = *s + 31 * hashval;
    s++;
  }

  //printf("leaving hashup\n");fflush(stdout);
  return(hashval % HASHSIZE);

}

/****************************************************
 *FUNCTION   : read_mcsv 
 *DESCRIPTION: reads in a csv table 
 *ARGUMENTS  : 
 *RETURNS    : pointer to struct 
 ***************************************************/
MASTER *
read_mcsv()
{
  char *field;
  int i = 0;
  MASTER *play; //pointer to master struct 

  play = (MASTER *) malloc(sizeof(MASTER));
  play->next           = NULL;
  play->name_hash_next = NULL;
  play->pid_hash_next  = NULL;
  play->batter         = NULL;
  play->pitcher        = NULL;

  while( (field = get_field(masfp)) )
  {
    //printf("%s",field);        //print for testing
    switch(i++){
      case 0: play->lid = field;   //lahmanID
              break;
      case 1: play->pid = field;   //playerID
              break;
      case 2: play->mgr = field;
              break;
      case 3: play->hof = field;
              break;
      case 4: play->byear = field; 
              break;
      case 5: play->bmonth = field;
              break;
      case 6: play->bday = field;
              break;
      case 7: play->bcountry = field;
              break;
      case 8: play->bstate = field;
              break;
      case 9: play->bcity = field; 
              break;
      case 10: play->dyear = field; 
              break;
      case 11: play->dmonth = field;
              break;
      case 12: play->dday = field;
              break;
      case 13: play->dcountry = field; 
              break;
      case 14: play->dstate = field;
              break;
      case 15: play->dcity = field; 
              break;
      case 16: play->firstname = field; 
               //printf("first name is %s\n", play->firstname);
              break;
      case 17: play->lastname = field; 
               //printf("last name is %s\n", play->lastname);
              break;
      case 18: play->notename = field;
              break;
      case 19: play->gvnname = field; 
              break;
      case 20: play->nickname = field; 
              break;
       //here begins two ints
      case 21: play->wt = atoi(field);
              break;
      case 22: play->ht = atoi(field);
              break;
      case 23: play->bats = field; 
              break;
      case 24: play->throws = field;
              break;
      case 25: play->debut = field;  
              break;
      case 26: play->final = field; 
              break;
      case 27: play->college = field; 
              break;
      case 28: play->l40id = field;
              break;
      case 29: play->l45id = field; 
              break;
      case 30: play->retid = field; 
              break;
      case 31: play->hid = field;
              break;
      case 32: play->bbrefid = field; 
              break;
    }
    if(i > 32){        //kick-out
      break;
    }
  }

  if (!i){
    return NULL;    //occurs on EOF
  }
  
  return(play);

}

/****************************************************
 *FUNCTION   : read_pcsv 
 *DESCRIPTION: reads in a csv table 
 *ARGUMENTS  : 
 *RETURNS    : pointer to struct 
 ***************************************************/
PITCH *
read_pcsv()
{
  char *field;
  int i = 0;
  PITCH *play; //pointer struct 

  play = (PITCH *) malloc(sizeof(PITCH));
  play->next = NULL;

  while( (field = get_field(pitfp)) )
  {
    //printf("%s",field);        //print for testing
    switch(i++){
      case 0: play->pid = field;   //lahmanID
              break;
      case 1: play->yid = field;   //playerID
              break;
      case 2: play->stint = field;
              break;
      case 3: play->tid = field;
              break;
      case 4: play->lgid = field; 
              break;
      case 5: play->w = atoi(field);
              break;
      case 6: play->l = atoi(field);
              break;
      case 7: play->g = atoi(field);
              break;
      case 8: play->gs = atoi(field);
              break;
      case 9: play->cg = atoi(field); 
              break;
      case 10: play->sho = atoi(field); 
              break;
      case 11: play->sv = atoi(field);
              break;
      case 12: play->ipout = atoi(field);
              break;
      case 13: play->h = atoi(field); 
              break;
      case 14: play->er = atoi(field);
              break;
      case 15: play->hr = atoi(field); 
              break;
      case 16: play->bb = atoi(field); 
              break;
      case 17: play->so = atoi(field); 
              break;
      case 18: play->bao = atoi(field);
              break;
      case 19: play->era = atoi(field); 
              break;
      case 20: play->ibb = atoi(field); 
              break;
      case 21: play->wp = atoi(field);
              break;
      case 22: play->hbp = atoi(field);
              break;
      case 23: play->bk = atoi(field); 
              break;
      case 24: play->bfp = atoi(field);
              break;
      case 25: play->gf = atoi(field);  
              break;
      case 26: play->r = atoi(field); 
              break;
    }
    if(i > 26){
      break;
    }
  }

  if (!i){
    return NULL;    //occurs on EOF
  }
  
  return(play);

}

/****************************************************
 *FUNCTION   : read_bcsv 
 *DESCRIPTION: reads in a csv table 
 *ARGUMENTS  : 
 *RETURNS    : pointer to struct 
 ***************************************************/
BAT *
read_bcsv()
{
  char *field;
  int i = 0;
  BAT *play; //pointer to master struct 

  play = (BAT *) malloc(sizeof(BAT));
  play->next = NULL;

  while( (field = get_field(batfp)) )
  {
    //printf("%s",field);        //print for testing
    switch(i++){
      case 0: play->pid = field;   //playerID
              break;
      case 1: play->yid = field;   //yearID
              break;
      case 2: play->stint = field;
              break;
      case 3: play->tid = field;
              break;
      case 4: play->lgid = field; 
              break;
      case 5: play->g = atoi(field);
              break;
      case 6: play->ab = atoi(field);
              break;
      case 7: play->r = atoi(field);
              break;
      case 8: play->h = atoi(field);
              break;
      case 9: play->secondbase = atoi(field); 
              break;
      case 10: play->thirdbase = atoi(field); 
              break;
      case 11: play->hr = atoi(field);
              break;
      case 12: play->rbi = atoi(field);
              break;
      case 13: play->sb = atoi(field); 
              break;
      case 14: play->cs = atoi(field);
              break;
      case 15: play->bb = atoi(field); 
              break;
      case 16: play->so = atoi(field); 
              break;
      case 17: play->ibb = atoi(field); 
              break;
      case 18: play->hbp = atoi(field);
              break;
      case 19: play->sh = atoi(field); 
              break;
      case 20: play->sf = atoi(field); 
              break;
      case 21: play->gidp = atoi(field);
              break;
    }
    if(i > 21){
      break;
    }
  }

  if (!i){
    return NULL;    //occurs on EOF
  }
  
  return(play);
}

/***************************************************
 *FUNCTION   : get_field
 *DESCRIPTION: gets one field at a time from a line 
 *ARGUMENTS  : *file -- file pointer to desired file 
 *RETURNS    : field -- pointer to a field
               0 for EOF
 **************************************************/
char *
get_field(FILE *file)
{
  char *field;
  char c; 
  char tempfield[255]; 
  int len; 
  int i=0, j;

  while(((c=fgetc(file)) != ',') && (c != '\n') && (c != EOF))
  {
    if(c == '"'){       //if data is in double quotes
      while((c=fgetc(file)) != '"'){
        tempfield[i++] = c;
      }
      if((c=fgetc(file)) == ','){   //in this scenario, should trigger every time
        break;                      //break out with tempfield done populating
      }
      else{
        printf("UNKNOWN ERROR HAS OCCURED: VERIFY DATA AND READ-IN AGREE\n");
        exit(0);
      }
    }
    tempfield[i++] = c;     //routine population of tempfield
  }

  if (c == EOF) {           
    if (!i) return NULL;    //occurs when EOF is in first col of stdin
  }
 
  tempfield[i] = '\0';      //this takes care of an empty field
  len = strlen(tempfield);  //find length for malloc purposes

  field = (char *) malloc((len+1) * sizeof(char));  //malloc w/room for NULL

  for(j=0; j <= len; j++)
  {
    *(field + j) = tempfield[j];       //populate pointers list
  }

  
  return (field);
}

/****************************************************
 *FUNCTION   : print_struct
 *DESCRIPTION: prints a struct from a pointer 
 *ARGUMENTS  : *play -- pointer to struct
 *RETURNS    : void 
****************************************************/
void
print_struct(MASTER *play)
{
   
  printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s \n\n", play->lid, play->pid, play->mgr, play->hof, play->byear, play->bmonth, play->bday, play->bcountry, play->bstate, play->bcity, play->dyear, play->dmonth, play->dday, play->dcountry, play->dstate, play->dcity, play->firstname, play->lastname, play->notename, play->gvnname, play->nickname, play->wt, play->ht, play->bats, play->throws, play->debut, play->final, play->college, play->l40id, play->l45id, play->retid, play->hid, play->bbrefid);

}

//print bat structs:
void
print_bat_struct(BAT *play)
{
  printf("%s,%s,%s,%s,%s \n\n", play->pid, play->yid, play->stint, play->tid, play->lgid);
}

//print pit structs:
void
print_pit_struct(PITCH *play)
{
  printf("%s,%s,%s,%s,%s \n\n", play->pid, play->yid, play->stint, play->tid, play->lgid);
}

/****************************************************
 *FUNCTION   : print_line
 *DESCRIPTION: prints a line from a pointer 
 *ARGUMENTS  : *line -- pointer holding string of chars 
 *RETURNS    : void 
****************************************************/
void
print_line(char *line)
{
  printf("%s",line);
}

/*********************************************************/
