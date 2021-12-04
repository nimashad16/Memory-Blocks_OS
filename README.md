### Method Descriptions:

int stringToNum(char *task) : This task will use if conditions to determine what type of fit to use and will return a number for the algorithm called

int getTaskCount(char *fileName) : This will find the total number of tasks in the array list

void parseCommands(char *file, Task *taskArr) : Will format the inputs read in from the file and will assign their positions to the array

Process createBlock(int data, int index) : Creates the blocks in our array so that we can go through them and store sizes

void decreaseArraySize(Memory *mem, int avoid). :Will decrease the array size when an element is removed

void storeInBlocks(Memory *mem) {   : Stores our data in the blocks that needs to be assigned

void swap(Process *a, Process *b)   : Swaps two variables around

void sort(Memory *mem) {      //Uses a data structure algorithm to sort our data


void bestfit(char taskID[], int requiredMem, Memory *mem)     //Does the best fit algorithm


void firstfit(char taskID[], int requiredMem, Memory *mem)  //Does the first fit algorithm

void request(char taskID[], int requiredMem, Memory *mem)   //GOes through the array and if request function is called it will call this method 
{

void release(char taskID[], Memory *mem).   //GOes through the array and if release function is called it will call this method 
{

void listAvailable(Memory *mem)               //Outputs the list in the correct format

void listAssigned(Memory *mem)               //Again outputs the list in the specified format in the file document

void find(char taskID[], Memory *mem)       //Calls the find function that the document needs



### What my program is:
Although the initial document said it would be naive to use arrays, I decided that between linked list and arrays, I would have a better chance at using arrays. Especially because my knowledge of linked list in C was very limited. In my program, I basically focus on using a variety of condition statements to go through my arrays and figure out where things should be inputted or not. Figuring this part out proved to be the most challenging because it was where I struggled in finding how to sort my data. Other parts of this program included creating sub methods to help compliment certain functions. Since much of it was highly cooperable with other tasks that were to be called, creating these methods allowed for simpler implementation. I used structs to be able to store all my data into much larger arrays and then using the structs specify where they would be located. I originrally had trouble with storing my data into these structs to characters and strings not working together but was able to figure that part out. At the time of riding this, I was able to get my program working for much of my data until I ran into a segmentation fault that I ahve not been able to figure out why. Unfortunatle I cannot visit the kerber scholars because it is past that time but if I were able to get a few a extra points if you knew hwhy I was having a segmentation fault that would be highly appreciated.
  


### Known issues I faced and bugs I came across:
One of the first issues I came across was facing the task of receiving the right input into my arrays. When initially storing values into my arrays, I used a variety of different struct variables to use one array and store strings into these structs. However, due to the syntax of C, I had trouble storing whole words into it and instead would only put the first character of the tasks. This was solved using strcpy. Next, I was quite naive as the project description described and went with using arrays as nearly everyone else I talked to used Linked list. A problem I soon came to realize that was quite a big issue was the fact that while initialization was simpler using arrays, resizing and doing memory allocation was not. I realized that figuring out how to allocate the proper amounts of memory and sorting through the array to allow for those big blocks of memory when tasks and process IDs were released stumped me for quite a long time. Lastly, one of my next major issues, excluding minor ones like tracing through the arrays and such was next fit. I didn't quite understand how to go about doing this because Next fit was overall just too confusing me and its a shame it was the one worth the most points too.

