void
createPeopleList ()
{

  head = (struct tyPersonPtr *) malloc (sizeof (struct tyPersonPtr));	//will need freeing eventually
  z = NULL;

  head->m = NULL;		//perhaps wont need to free m
  head->next = z;


}


void
freePeopleListt ()
{

  struct tyLine *x;
  struct tyLin *xPtr;


  t = head->next;

  while (t != z)
    {
      x = t;
      xPtr = (*x).m;

      t = t->next;

      free (xPtr);
      free (x);

    }

  free (t);
  free (head);

}



struct tyPersonPtr *
insertLineafter (struct tyPerson tm, struct tyPersonPtr *t)
{


  struct tyLine *x;
  x = (struct tyLine *) malloc (sizeof (struct tyLine));	//create a new move x

  struct tyLin *xm;
  xm = (struct tyLin *) malloc (sizeof (struct tyLin));

  xm->key = tm.key;
  xm->x0 = tm.x0;
  xm->y0 = tm.y0;
  xm->x1 = tm.x1;
  xm->y1 = tm.y1;
  xm->colour = tm.colour;

  x->m = xm;
  x->next = t->next;		//copy t's pointer into new move
  t->next = x;			//set t next pointer to new move x



  return x;


}



int
NPeople ()
{
  int n = 0;
  t = head->next;		//assume head is a dummy node
  while (t != z)
    {
      n++;
      t = t->next;
    }

  return n;
}
