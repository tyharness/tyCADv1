struct tyPerson
{
  int key;			//record key
  int NIN;			//unique national id number
  double x;			//tracking position
  double y;			//tracking position  

}


struct tyPersonPtr
{
  struct tyLin *m;
  struct tyLine *next;		//linked list pointer
};


//global pointers for LL
struct tyPersonPtr *head;	//link pointer to head node
struct tyPersonPtr *z;		//link pointer to tail node
struct tyPersonPtr *t;		//link pointer to current node


//forward declarations
void createPeopleList ();
void freePeopleList ();
struct tyPersonPtr *insertPersonafter (struct tyPerson tm,
				       struct tyPersonPtr *t);

int NPeople ();
