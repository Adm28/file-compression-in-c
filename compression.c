#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "min_heap.h"
#define internal 0
#define leaf 1

typedef struct freqtable
{
  int freq;
  char ch;
}ftable;

typedef struct codetable
{
  char data;
  char arr[28];
}codetable;
codetable carr[128];
int codetable_top = 0;

node *root;
ftable freqtable[128];
node *hpq[128];      // MAX_no. of elements in min_heap

 void addcharacter(char);    //read character from file and store its frequency in frequency table
 void makemin_heap();        //inserted the elements whose frquency is more than 0 to the min heap tree
 node* newNode(char,int);    //creates a new node
 void make_huffman_tree();   //make the huffman tree
 void display_freqtable();
 void display_tree(node*);
 char* getCode(char ch);

int count=0;
int *length = &count;
int main()
{
  printf("\n1.Compress \n");
  char choice;
  scanf("%c",&choice);
  switch(choice)
{
 case '1':
            printf("\nEnter the file name to be compressed : ");
            char filename[20];
            scanf("%s",filename);
            FILE *fp1,*fp2;
            char ch;
            for(int i=0;i<128;i++)
               freqtable[i].freq=0;
            fp1 = fopen(filename,"r");
            while(fread(&ch,sizeof(char),1,fp1)!=NULL)
              addcharacter(ch);
            fclose(fp1);
            display_freqtable();
            printf("\npass1");
            makemin_heap();
            printf("\npass2");
            printf("making the huffman tree.....");
            make_huffman_tree();
            printf("\npass3");
            int arr[128], top = 0;
            storeCodes(root, arr, top);
            printf("\npass4");
            printf("\nWriting the compressed contents....");
            fp2=fopen("compressed.txt","wb");
            fp1 = fopen(filename,"r");
            while(fread(&ch,sizeof(char),1,fp1)!=0)
          	{
          		//printf("\n%c replaced with ",ch);
          			 writeCode(ch,fp2);//write corresponding code into fp2
          	}
          fclose(fp1);
          fclose(fp2);
          printf("\nFile succesfully compressed...!\n");

            break;



  default: break;
}

  return 0;
}


void addcharacter(char ch)
{
freqtable[ch].freq+=1;
}
void display_freqtable()
{
  for(int i=0;i<128;i++)
    printf("\n%c %d",i ,freqtable[i].freq);
}


void makemin_heap()
{

  for(int i=0;i<128;i++)
    {
      if(freqtable[i].freq>0)
      {
        node *temp = (node*)malloc(sizeof(node));
        temp->freq= freqtable[i].freq;
        temp->data = i;
        temp->type=leaf;
        temp->left=NULL;
        temp->right=NULL;
        pqinsert(hpq,temp,length);
      }

    }
  display(hpq,*length);
}

node* newNode(char ch,int freq)
{
  node *q = (node*)malloc(sizeof(node));
  q->data=ch;
  q->type=internal;
  q->freq=freq;
  q->left=NULL;
  q->right=NULL;
  return q;
}

void make_huffman_tree()
{
 while((*length)>=2)
 {
   node *leaf1 = pqdelete(hpq,length);
   node *leaf2 = pqdelete(hpq,length);
   node *q = newNode('@',leaf1->freq+leaf2->freq);
   q->left = leaf1;
   q->right = leaf2;
   root = q;
   pqinsert(hpq,q,length);
 }
}
void display_tree(node *p)
{
  if(p == NULL)
     return;
  else
  {
    display_tree(p->left);
    printf("\n%c %d %d ",p->data,p->freq,p->type);
    display_tree(p->right);
  }

}
void storeCodes(node*root, char arr[], int top)
{
    if (root->left)
    {
        arr[top] = '0';
        storeCodes(root->left, arr, top + 1);
    }
    if (root->right)
    {
        arr[top] = '1';
        storeCodes(root->right, arr, top + 1);
    }
    if (root->type==leaf)
    {
        storearr(arr,top,root->data);
    }
}
void storearr(char arr[], int n ,char key)
{
    int i;
    arr[n] = '\0';
    carr[codetable_top].data = key;
    strcpy(carr[codetable_top].arr , arr);
    codetable_top++;

}
void display_codetable()
{
  for(int i=0;i<codetable_top;i++)
  {
    printf("%s ",carr[i].arr);
  }
}
void writeCode(char ch,FILE *f)
{
  char *code;
  code=getCode(ch);
  //printf("\n%s\n",code);
  	while(*code!='\0')
  	{
  		if(*code=='1')
  			writeBit(1,f); //write bit 1 into file f
  		else
  			writeBit(0,f);
  	code++;
  	}
  	return;
}
char* getCode(char ch)
{
  int i;
  for(i = 0;i<codetable_top;i++)
  {
    if(carr[i].data == ch)
    {
      return carr[i].arr;
    }
  }

}
void writeBit(int b,FILE *f)
{          //My Logic: Maintain static buffer, if it is full, write into file
	static char byte;
	static int cnt;
	char temp;
	    //printf("\nSetting %dth bit = %d of %d ",cnt,b,byte);
	if(b==1)
	{	temp=1;
		temp=temp<<(7-cnt);		//right shift bits
		byte=byte | temp;
	}
	cnt++;

	if(cnt==8)	//buffer full
	{
//		printf("[%s]",bitsInChar(byte));
		fwrite(&byte,sizeof(char),1,f);
		cnt=0;
    byte=0;	//reset buffer
		return;// buffer written to file
	}
	return;
}
