//////////////////////Master Struct/////////////////////////
typedef struct master_struct{

    char * lid;   //lahmanID
    char * pid;   //playerID
    char * mgr;
    char * hof;
    char * byear;
    char * bmonth;
    char * bday;
    char * bcountry;
    char * bstate;
    char * bcity;
    char * dyear;
    char * dmonth;
    char * dday;
    char * dcountry;
    char * dstate;
    char * dcity;
    char * firstname;
    char * lastname;   //use as player name var
    char * notename;
    char * gvnname;
    char * nickname;
  //ints shown below occur here in the csv sequence
    char * bats;
    char * throws;
    char * debut;
    char * final;
    char * college;
    char * l40id;
    char * l45id;
    char * retid;
    char * hid;
    char * bbrefid;

    int wt;
    int ht;

  struct master_struct * next;
    struct master_struct * name_hash_next;
    struct master_struct * pid_hash_next;
    struct bat_struct    * batter;
    struct pitch_struct  * pitcher;

} MASTER;

//////////////////////Batter Struct/////////////////////////
typedef struct bat_struct{

    char * pid;  //playerID
    char * yid;  //yearID
    char * stint;
    char * tid;  //teamID
    char * lgid; //lgID

    int g;
    int ab;  //at bats
    int r;   //runs
    int h;   //hits
    int secondbase;
    int thirdbase;
    int hr;
    int rbi;   //runs batted in (RBI)
    int sb;
    int cs;
    int bb;
    int so;
    int ibb;
    int hbp;
    int sh;
    int sf;
    int gidp;

    struct bat_struct    *next;

} BAT;

//////////////////////Pitcher Struct/////////////////////////

typedef struct pitch_struct{

    char * pid; //playerID
    char * yid; //yearID
    char * stint;
    char * tid;  //teamID
    char * lgid; //lgID

    int w;
    int l;
    int g;
    int gs;
    int cg;
    int sho;
    int sv;
    int ipout;
    int h;
    int er;     //earned runs
    int hr;
    int bb;
    int so;
    int bao;
    int era;   //earned run average (ERA)
    int ibb;
    int wp;    
    int hbp;
    int bk;
    int bfp;
    int gf;
    int r;

    struct pitch_struct  *next;

} PITCH;

