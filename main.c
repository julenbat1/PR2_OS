#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <wait.h>

typedef struct t_process {
	int writepipe;
	int readpipe;
	int pid;
	int status;

	
	void (*writeInt) (int, struct t_process*);
	int (*readInt) (struct  t_process*);
	void (*delMyself) (struct t_process*);

}t_process;

typedef struct  nodeProcess {
	t_process *currentNode;
	struct nodeProcess *parentNode;
	struct nodeProcess *nextNode;

	int (*hasNext) (struct nodeProcess*);
	struct nodeProcess* (*next) (struct nodeProcess*);
	struct nodeProcess* (*prev) (struct nodeProcess*);
}nodeProcess;

typedef  struct linkedList {
	nodeProcess *head;
	nodeProcess *tail;
	int size;

	void (*addNode)(struct linkedList*, nodeProcess*);
	void (*removeNode)(struct linkedList*, int);
}linkedList;

void addNodeProcess(linkedList *, nodeProcess*);
void removeNodeProcess(linkedList *, int);
nodeProcess *init_nodeProcess(t_process *);
nodeProcess *getNode(linkedList *, int);




float calculateMean(int, int);
void replaceStd(int, int);
int readProcessInt(t_process*);
void writeProcessInt(int, t_process*);
void printMainOptions(int, char**);
void deleteMe(t_process*);
int readOptionThree();

void calculateProcesses(int);
int getIntInput();
void createProcesses(int);
void clear();

void deleteAll();


linkedList *processList;
linkedList *init_processList();
t_process* init_t_process();



int main(int argc, char *argv[])
{

	processList = init_processList();

	char *options[4] = {"1 -Create Calculator process\n", "2 -Eliminate Calculator process\n", "3 -Calculate result\n", "4 -End all applications\n"};
	printMainOptions(4, options);
}

void createProcesses(int amount)
{
	for(int i = 0; i < amount; i++)
	{
		t_process *process = init_t_process();
		nodeProcess *node = init_nodeProcess(process);
		processList->addNode(processList, node);
	}
}

int getIntInput()
{
	char strnumber[40];
	int n = 0;

	while((read(0, &strnumber[n] , 1) > 0) && (strnumber[n] != '\n') && ( n++ < 40))printf("%c \n", strnumber[n]);

	n = atoi(strnumber);

	return  n;
}

nodeProcess *init_nodeProcess(t_process *process)
{
	nodeProcess *node = (nodeProcess *) malloc(sizeof(nodeProcess));
	node->currentNode = process;

	return node;
}

void addNodeProcess(linkedList *list, nodeProcess *node)
{

	if(list->head == (void *) 0) list->head = node;
	else
	{
		nodeProcess *tail = list->tail;
		tail->nextNode = node;
		node->parentNode = tail;
	}

	list->tail = node;
	list->size += 1;
	printf("size %d", list->size);
}

void clear()
{
	char t = 'a';
	while( t != '\n' && (read(0, &t, sizeof(t))> 0));
}

void removeNodeProcess(linkedList *list, int pid)
{
	nodeProcess *tmp;
	nodeProcess* nodeProcess = getNode(list, pid);
	assert(nodeProcess != (void *) 0);
	if(nodeProcess == (void *) 0 || list->size <= 1) printf("process size is less or equal to 1: size %d", list->size);
	else
	{
		tmp = nodeProcess->nextNode;

		if(nodeProcess->parentNode == (void *) 0)
		{
			list->head = tmp;
			tmp->parentNode = (void *) 0;
		}
		else
		{
			nodeProcess->parentNode->nextNode = nodeProcess->nextNode;
			nodeProcess->nextNode->parentNode = nodeProcess->parentNode;
		}

		if(tmp->nextNode == (void *)0) list->tail = tmp;
		list->size--;

	}
		
}

nodeProcess *getNode(linkedList *list, int pid)
{

	assert(list != (void *) 0);
	nodeProcess *current = list->head;
	int n = list->size;
	while(n-- > 0)
	{
		assert(current != (void *) 0 && current->currentNode != (void *) 0);
		if(current->currentNode->pid == pid)
		{
			printf("process %d found \n", current->currentNode->pid );
			return current;
		}
		current = current->nextNode;
	}
	
	printf(" this process with pid: %d was found in the process list", pid);
	return (void *) 0;
	

}

int readOptionTwo()
{
	char r[20];
	int res = 10;
	
	return res;

}

int readOptionThree()
{
	char r[4];
	int res;
	read(1, r, 4);

	res = atoi(r);
	if(res < 1 || res > 1000) 
	{
		printf("Number must be [1-1000] please, try again");
		readOptionThree();
	}

	return res;
}


void calculateProcesses(int number)
{
	printf("joined calculateProcesses \n");
	int total = 0;
	int pAmount = 0;
	nodeProcess *process = processList->head;

	while(process != (void *) 0 && process->currentNode != (void *) 0 )
	{
		t_process *current= process->currentNode;
		current->writeInt( number, current);
		total += current->readInt(current); 
		pAmount++;
		printf("n: %d, * pid: %d , total %d, with %d porcess \n", number, current->pid, total, pAmount);
		if((process = process->nextNode) == (void *) 0) break;
	}

	float res = calculateMean(pAmount, total);
	printf("The mean result is: %f", res);
}

float calculateMean(int pAmount, int number)
{
	return ((float) number / (float) pAmount);

}
void printMainOptions(int optSize, char *options[])
{
	int selectedOption;
	for(int i = 0; i < optSize; i++) write(1, options[i], strlen(options[i]));
	

	scanf("%d", &selectedOption);


	switch(selectedOption)
	{
		case 1:
			printf("indica la cantidad de procesos [1-5] que quieras crear \n");
			int amnt;
			scanf("%d", &amnt);
			createProcesses(amnt);
			break;
		case 2:
			printf("introduce el pid del proceso que quieres eliminar \n");
			int ppid;
			scanf("%d", &ppid);
			processList->removeNode(processList, ppid);
			break;
		case 3:
			printf("Indica un número [1-1000] \n");
			int n;
			scanf("%d", &n);
			calculateProcesses(n);
			break;
		case 4:
			printf("Are you sure ? y/n \n");
			char r;
			scanf(" %c", &r);
			printf("selected option is %c", r);
			r == 'y' ? deleteAll(): printf("aborting deletion all processes\n");
			break;

		default:
			printf("No has introducido una opción válida \n");
			break;
	}

	printMainOptions(optSize, options);


}

void deleteAll()
{
	printf("\n joined deleteAll function \n");
	nodeProcess * current = processList->head;
	int n = processList->size;

	while(n-- > 0) 
	{
		nodeProcess * tmp = current->nextNode;
		current->currentNode->delMyself(current->currentNode);
		free(current);
		current = tmp;

	}

	free(processList);
	processList = init_processList();


}



t_process* init_t_process()
{
	int pid;
	int inpip[2];
	int outpip[2];

	pipe(inpip);
	pipe(outpip);
	t_process *process  =  (t_process *) malloc(sizeof(t_process));

	if((pid = fork()) > 0)
	{
		process->pid = pid;
		process->writepipe =  inpip[1];
		process->readpipe = inpip[0];
		process->writeInt = writeProcessInt;
		process->readInt = readProcessInt;
		process->delMyself = deleteMe;

	}
	else
	{
		replaceStd(0, inpip[0]);
		replaceStd(1, outpip[1]);
		execlp("./calculator", "calculator", (void *) 0);
	}

	return process;

}

void deleteMe(t_process *process)
{
	kill(process->pid, SIGTERM);
	waitpid(process->pid, &(process->status), 0 );
	if(WIFSIGNALED(process->pid))	
	{
			char message[200];
			sprintf(message, "process pid: %d has been killed with signal %d \n", process->pid, WTERMSIG(process->status));
			write(1, message, strlen(message));
	}	
}

int readProcessInt(t_process *self)
{
	int number;
	read(self->writepipe, &number, sizeof(int));

	return number;

}

void writeProcessInt(int value, t_process *self)
{
	write(self->readpipe, &value, sizeof(int));

}


void replaceStd(int replace, int with)
{
	close(replace);
	dup(with);
	close(with);
}

linkedList *init_processList()
{
	linkedList *n = (linkedList *) malloc(sizeof(linkedList));
	n->size = 0;
	n->removeNode = removeNodeProcess;
	n->addNode = addNodeProcess;
	return n;
}

