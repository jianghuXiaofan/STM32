#include "bsp_link.h"
#include "bsp_malloc.h"	
#include <string.h>
#include <stdlib.h>

typedef struct Link
{
    char name[20];
    struct Link * next;
}my_link;

//creat node
my_link * Creat_Node(char * name)
{
    if(NULL == name)
    {
        return NULL;
    }
    my_link * head = mymalloc(sizeof(my_link));
    //my_link * head = malloc(32);
    strcpy(head->name,name);
    head->next = NULL;
    return head;
}    
//add node

//del node
//change node
//find node 
//sort node
//free link


