#include "list.h"

// 初始化一个带头结点的空链表
linklist init_list(void)
{
	linklist head = calloc(1, sizeof(listnode));
	if(head != NULL)
	{
		head->prev = head;
		head->next = head;
	}

	return head;
}

linklist new_node(char *data)
{
	linklist new = calloc(1, sizeof(listnode));
	if(new != NULL)
	{
		strcpy(new->data,data); //文件名字复制到链表

		new->prev = new;
		new->next = new;
		new->rgb  = NULL;
	}

	return new;
}

// 将节点new插入到以head为首的链表的末尾
void list_add_tail(linklist new, linklist head)
{
	new->prev = head->prev;
	new->next = head;

	head->prev->next = new;
	head->prev = new;
}

void show(linklist head)
{
	linklist tmp;

	for(tmp=head->next; tmp!=head; tmp=tmp->next)
	{
		printf("%s\t", tmp->data);
	}
	printf("\n");
}

