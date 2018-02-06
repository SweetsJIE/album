#ifndef __LIST_H
#define __LIST_H

#include <time.h>
#include "common.h"
#include "jpg.h"


typedef struct node
{
	char data[20];
	struct RGB rgb;
	
	struct node *prev;
	struct node *next;

}listnode, *linklist;

linklist init_list(void);
linklist new_node(char *data);
void list_add_tail(linklist new, linklist head);
void show(linklist head);

#endif
