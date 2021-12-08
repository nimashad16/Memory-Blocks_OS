#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define TOTAL_TASKS 50
#define REQUEST 0
#define RELEASE 1
#define LIST_AVAILABLE 2
#define LIST_ASSIGNED 3
#define FIND 4

#define BESTFIT 0
#define FIRSTFIT 1
#define NEXTFIT 2
#define WORSTFIT 3

typedef struct
{
    char taskName[10];
    int allocatedMemory;
    int index;
} Process;

typedef struct
{
    char taskName[10];
    int task;
    int memoryNeeded;
} Task;

typedef struct
{
    Process processes[TOTAL_TASKS];
    int spaceCombined;
    int nextAddress;
    int unusedSpace;
    int task;
    int next;
} Memory;

Process createBlock(int data, int index)
{
    Process hole;
    memcpy(hole.taskName, "HOLE", sizeof(hole.taskName));
    hole.allocatedMemory = data;
    hole.index = index;
    return hole;
}

int getTaskCount(char *fileName)
{
    int counter = 0;
    char task[10];

    FILE *filePointer;
    filePointer = fopen(fileName, "r");

   while(fscanf(filePointer, "%s ", task) != EOF)
    {
        if(strcmp(task, "REQUEST") == 0){
            counter++;
            fscanf(filePointer, "%s ", task);
            fscanf(filePointer, "%s ", task);
        }
        else if(strcmp(task, "RELEASE") == 0 || strcmp(task, "LIST") == 0 || strcmp(task, "FIND") == 0){
            counter++;
            fscanf(filePointer, "%s ", task);
        }
    }
    fclose(filePointer);
    return counter;
}
int stringToNum(char *task)
{
    if(strcmp(task, "BESTFIT") == 0)
    {
        return 0;
    }
    else if(strcmp(task, "FIRSTFIT") == 0)
    {
        return 1;
    }
    else if(strcmp(task, "NEXTFIT") == 0)
    {
        return 2;
    }
    else if(strcmp(task, "WORSTFIT") == 0)
    {
        return 3;
    }

    return -1;
}

void storeInBlocks(Memory *mem);

void parsing(char *file, Task *taskArr)
{
    char task[10];
    int counter = 0;
    int x;
    char null[10] = "NULL";
    FILE *input = fopen(file, "r");

    while(fscanf(input, "%s ", task) != EOF)
    {
        if(strcmp(task, "REQUEST") == 0)
        {
            taskArr[counter].task = REQUEST;
            fscanf(input, "%s ", task);
            for(int i = 0; i < 10; i++)
                taskArr[counter].taskName[i] = task[i];
            fscanf(input, "%s ", task);
            taskArr[counter].memoryNeeded = (int)strtol(task, (char **)NULL, 10);
            counter++;
        }
        else if(strcmp(task, "RELEASE") == 0)
        {
            taskArr[counter].task = RELEASE;
            fscanf(input, "%s ", task);
            for(x = 0; x < 10; x++) {
                taskArr[counter].taskName[x] = task[x];
                taskArr[counter].memoryNeeded = 0;
                counter++;
            }
        }
        else if(strcmp(task, "LIST") == 0)
        {
            fscanf(input, "%s ", task);
            if(strcmp(task, "AVAILABLE") == 0)
            {
                taskArr[counter].task = LIST_AVAILABLE;
                for(x = 0; x < 10; x++) {
                    taskArr[counter].taskName[x] = null[x];
                    taskArr[counter].memoryNeeded = 0;
                }
            }
            else
            {
                taskArr[counter].task = LIST_ASSIGNED;
                for( x = 0; x < 10; x++) {
                    taskArr[counter].taskName[x] = null[x];
                    taskArr[counter].memoryNeeded = 0;
                }
            }
            counter++;
        }
        else if(strcmp(task, "FIND") == 0)
        {
            taskArr[counter].task = FIND;
            fscanf(input, "%s ", task);
            for(x = 0; x < 10; x++)
                taskArr[counter].taskName[x] = task[x];
            taskArr[counter].memoryNeeded = 0;
            counter++;

        }
    }
    fclose(input);
}

void decreaseArraySize(Memory *mem, int avoid)
{
    Process newTask[TOTAL_TASKS];
    int idx = 0;

    for(int i = 0; i < mem->next; i++){
        if(i != avoid){
            newTask[idx] = mem->processes[i];
            idx++;
        }
    }
    memcpy(mem->processes, &newTask, sizeof(mem->processes));
    mem->next = idx;
    storeInBlocks(mem);
}

void storeInBlocks(Memory *mem) {
    for(int a = 0; a < mem->next; a++)
    {
        if(strcmp(mem->processes[a].taskName, "HOLE") == 0 && strcmp(mem->processes[a+1].taskName, "HOLE") == 0)
        {
            mem->processes[a].allocatedMemory = mem->processes[a].allocatedMemory + mem->processes[a+1].allocatedMemory;
            decreaseArraySize(mem, a+1);
        }
    }
}


void swap(Process *a, Process *b)
{
    Process temp = *a;
    *a = *b;
    *b = temp;
}


void sort(Memory *mem) {
    int a,x;
    for(a = 0; a < mem->next -1; a++)
    {
        for(x= 0; x < mem->next - a -1; x++)
        {
            if(mem->processes[x].index >  mem->processes[x+1].index)
            {
                swap(&mem->processes[x], &mem->processes[x+1]);
            }
        }
    }
}

void storingTheBlocks(char newTask[], int requiredMem, Memory *mem, int idx)
{
    int diffMem;
    diffMem = mem->processes[idx].allocatedMemory - requiredMem;

    memcpy(mem->processes[idx].taskName, newTask, sizeof(mem->processes[idx].taskName));
    mem->processes[idx].allocatedMemory = requiredMem;

    if(diffMem == 0){
        return;
    }
    mem->processes[mem->next] = createBlock(diffMem, mem->processes[idx].index + requiredMem);
    mem->next++;
    mem->unusedSpace = mem->unusedSpace - requiredMem;
}
void newMem(Memory *mem)
{
    mem->processes[0] = createBlock(mem->spaceCombined, 0);
    mem->next = 1;
    mem->unusedSpace = mem->spaceCombined;
    mem->nextAddress = 0;
}

void bestfit(char taskID[], int requiredMem, Memory *mem)
{
    int x;
    int size = mem->spaceCombined+1; // largest possible size
    int index = -1;

    for(x = 0; x < mem->next; x++)
    {
        if(strcmp(mem->processes[x].taskName, "HOLE") == 0)
        {
            if((mem->processes[x].allocatedMemory >= requiredMem) && (mem->processes[x].allocatedMemory < size))
            {
                size = mem->processes[x].allocatedMemory;
                index = x;
            }
        }
    }

    if(index > -1)
    {
        storingTheBlocks(taskID, requiredMem, mem, index);
        printf("ALLOCATED %s %d\n", taskID, mem->processes[index].index);
        return;
    }

    printf("FAIL REQUEST %s %d\n", taskID, requiredMem);
    return;
}


void firstfit(char taskID[], int requiredMem, Memory *mem)
{
    int a;
    for(a = 0; a < mem->next; a++)
    {
        if(strcmp(mem->processes[a].taskName, "HOLE") == 0) {
            if(mem->processes[a].allocatedMemory >= requiredMem){
                storingTheBlocks(taskID, requiredMem, mem, a);
                printf("ALLOCATED %s %d\n", taskID, mem->processes[a].index);
                return;
            }
        }
    }

    printf("FAIL REQUEST %s %d\n", taskID, requiredMem);
    return;
}


void worstfit(char taskID[], int requiredMem, Memory *mem)
{

}

void request(char taskID[], int requiredMem, Memory *mem)
{
    if(mem->unusedSpace < requiredMem)
    {
        printf("FAIL REQUEST %s %d\n", taskID, requiredMem);
        return;
    }

    switch(mem->task)
    {
        case BESTFIT:
            bestfit(taskID, requiredMem, mem);
            break;
        case FIRSTFIT:
            firstfit(taskID, requiredMem, mem);
            break;
        case NEXTFIT:
            break;
        case WORSTFIT:
            break;
    }
}

void release(char taskID[], Memory *mem)
{
    for(int i = 0; i < mem->next; i++)
    {
        if(strcmp(mem->processes[i].taskName, taskID) == 0)
        {
            memcpy(mem->processes[i].taskName, "HOLE", sizeof(mem->processes[i].taskName));
            mem->unusedSpace = mem->unusedSpace + mem->processes[i].allocatedMemory;
            printf("FREE %s %d %d\n", taskID, mem->processes[i].allocatedMemory, mem->processes[i].index);
            return;
        }
    }
    printf("FAIL RELEASE %s\n", taskID);
    return;
}

void listAvailable(Memory *mem)
{
    int available = 0;
    for(int i = 0; i < mem->next; i++)
    {
        if(strcmp(mem->processes[i].taskName, "HOLE") == 0)
        {
            printf("(%d, %d) ", mem->processes[i].allocatedMemory, mem->processes[i].index);
            available++;
        }
    }

    if(available > 0)
    {
        printf("\n");
        return;
    }
    else
    {
        printf("FULL\n");
        return;
    }
}

void listAssigned(Memory *mem)
{
    int counter,x;
    counter =0;
    for( x= 0; x < mem->next; x++)
    {
        if(!(strcmp(mem->processes[x].taskName, "HOLE") == 0))
        {
            printf("(%s, %d, %d) ", mem->processes[x].taskName, mem->processes[x].allocatedMemory, mem->processes[x].index);
            counter++;
        }
    }

    if(counter > 0)
    {
        printf("\n");
        return;
    }
    else
    {
        printf("NONE\n");
        return;
    }
}


void find(char taskID[], Memory *mem)
{
    for(int i = 0; i < mem->next; i++)
    {
        if(strcmp(mem->processes[i].taskName, taskID) == 0)
        {
            printf("(%s, %d, %d)\n", taskID, mem->processes[i].allocatedMemory, mem->processes[i].index);
            return;
        }
    }
    printf("FAULT\n");
    return;
}

void run(Task task, Memory *mem)
{
    switch(task.task)
    {
        case REQUEST:
            request(task.taskName, task.memoryNeeded, mem);
            break;
        case RELEASE:
            release(task.taskName, mem);
            break;
        case LIST_AVAILABLE:
            listAvailable(mem);
            break;
        case LIST_ASSIGNED:
            listAssigned(mem);
            break;
        case FIND:
            find(task.taskName, mem);
            break;
        default:
            printf("BAD COMMAND\n");
            break;
    }
    sort(mem);
    storeInBlocks(mem);
}


int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Arguments not inputted correctly\n");
    }

    Memory mem;
    mem.task = stringToNum(argv[1]);
    mem.spaceCombined = (int)strtol(argv[2], (char **)NULL, 10);
    newMem(&mem);

    char *fileName = argv[3];
    int numTasks;
    getTaskCount(fileName);

    Task taskArray[numTasks];
    parsing(fileName, taskArray);
    int x;

    for(x = 0; x < numTasks; x++)
    {
        run(taskArray[x], &mem);
    }
    return 0;
}
