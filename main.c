#include "GenericHashTable.c"
#include <sys/ioctl.h>



int main(){
   Table *t = createTable(4, 1,1);

  char *s = "aaa";
  char *s1 = "bbb";
  char *s2 = "ccc";
  char *s3 = "ddd";
   char *s4 = "eee";

  int x3 = add(t, s);   //a
  int x4 = add(t, s1);  //b
  int x5 = add(t, s2);  //c
  int x6 = add(t, s3);  //d
   int x7 = add(t, s4);  //e
   printTable(t);
   freeTable(t);
}