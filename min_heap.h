typedef struct node
{
  char data;
  int freq;
  int type;
  struct node *left;
  struct node *right;
}node;

void pqinsert(node*h[],node*,int*);
node* pqdelete(node*h[], int*);
void display(node*h[],int);
void heapify(node*h[] , int);

node* pqdelete(node*h[], int *count)
{
  node *key;
  key=h[0];
  h[0]=h[*count-1];
  --*count;
 heapify(h,*count);
 return key;
}
//recreate minimum heap
void heapify(node *h[], int count)
{
  int i,j;
  node *key;
  j=0;
  key=h[j];

  i=2*j+1;

  //repeat until left child exists
  while(i<count)
  {
    if((i+1)<count)//check if the right child exists
    {
      if(h[i+1]->freq <h[i]->freq)
        i++;//find the smallest child
     }
    if(key->freq > h[i]->freq)
    {
       h[j]=h[i];//move the child up
       j=i;
       i=2*j+1;//find the left child
    }
   else
    break;
}
h[j]=key;//insert key at its proper position
}

void pqinsert(node *h[],node *key,int* count)
{
  int i,j;
  i=*count;
  h[i]=key;          //initial position of the key
  (*count)++;
  j=(i-1)/2;

  while((i>0)&&(h[j]->freq > key->freq))
  {
    h[i]=h[j];
    i=j;
    j=(i-1)/2;
  }
  h[i]=key;
}

void display(node *h[], int count)
{
  int i;
  printf("\nThe elements in the priority queue are..\n");

  for(i=0;i<count;i++)
     printf("Element= %c , prority = %d\n",h[i]->data,h[i]->freq);
  printf("\n");
}
