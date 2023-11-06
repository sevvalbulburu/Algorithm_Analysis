#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING 50
#define STACK_SIZE 40


// A structure to represent an adjacency list node
struct AdjListNode {
	int dest;
	struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList {
	int isdeleted;
	int indegree;
	int* visited;
	char name[50];
	struct AdjListNode* head;
};

struct Graph {
	int V;
	int* visitedCount;
	struct AdjList* array;
};

struct stack {
    struct AdjList arr[STACK_SIZE];
    int top;
};

// steak 
struct stack* createStack() {
    struct stack* s = (struct stack*) malloc(sizeof(struct stack));
    s->top = -1;
    return s;
}

struct AdjList peek(struct stack* s) {
	
    return s->arr[s->top];
}

int isFull(struct stack* s) {
    return STACK_SIZE == s->top+1 ? 1 : 0;
}

int isEmpty(struct stack* s) {
    return -1 == s->top ? 1 : 0;
}

struct AdjList pop(struct stack* s) {
	struct AdjList node;
    if (isEmpty(s))
        printf("Stack is empty can't pop!\n");
    else
    {	
    	node = peek(s);
        s->top --;
        return node;
    }
}

void push(struct stack* s, struct AdjList data) {
    if (isFull(s))
        printf("Stack is full can't push!\n");
    else
    {
        s->top ++;
        s->arr[s->top] = data;
    }
}

// create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest) {
	struct AdjListNode* newNode	= (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

// creates a graph of V vertices
struct Graph* createGraph(int V) {
	struct Graph* graph
		= (struct Graph*)malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists.
	graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));
	graph->visitedCount = (int*)calloc(V , sizeof(int));

	// Initialize each adjacency list as empty
	int i;
	for (i = 0; i < V; ++i){
		graph->array[i].head = NULL;
		graph->array[i].indegree = 0;
		graph->array[i].isdeleted = 0;
		graph->array[i].visited = (int*)calloc(V , sizeof(int));
	}
	return graph;
}

void addEdge(struct Graph* graph, int src, int dest, char *name) {
 
	struct AdjListNode* check = NULL;
	struct AdjListNode* newNode = newAdjListNode(dest);
	

	if (graph->array[src].head == NULL) {
		newNode->next = graph->array[src].head;
		graph->array[src].head = newNode;
	}
	else {

		check = graph->array[src].head;
		while (check->next != NULL) {
			check = check->next;
		}
		check->next = newNode;
	}
	strcpy(graph->array[src].name, name);
}

// finds indegree values of each node
void indegrees(struct Graph* graph) {
	int i;
	for(i = 1; i < graph->V; i++) {
		struct AdjListNode* node = graph->array[i].head;
		while(node) {
			graph->array[node->dest].indegree++;
			node = node->next;
		}
	}
}

void printGraph(struct Graph* graph) {
    int i;
    for (i = 1; i < graph->V; i++) {
        struct AdjListNode* node = graph->array[i].head;
        if(graph->array[i].isdeleted == 0){	
		    printf("\nnum: %d\nname: %s\nindegree: %d\n", i, graph->array[i].name, graph->array[i].indegree);
		}
    }
}

void printInfluencer(struct Graph* graph, struct AdjList node, int index) {
	printf("\nnumber: %d\nname: %s\nindegree: %d\nconnection number: %d\n", index, node.name, node.indegree, graph->visitedCount[index]);
}

struct Graph* removeNode(struct Graph* graph, int index) {
	int i;
	for(i = 1; i < graph->V; i++) {
		struct AdjListNode* node = graph->array[i].head;
		if(i != index && graph->array[i].isdeleted == 0) {
			while(node != NULL && node->dest != index && (node->next != NULL && node->next->dest != index))
				node = node->next;
				
		    if(node != NULL && node->dest == index ) { // Node is on the top of the list	
				if(node->next != NULL)
					graph->array[i].head = node->next;
				else {
					node = NULL;
					graph->array[i].head = NULL; 
				}
			}
			else if(node != NULL && node->next != NULL && node->next->dest == index) { // Node is not the top one 
				node->next = node->next->next;
			}
		}
		else if(i == index) { // decreases indegree's of nodes that connected the deleted one.
			while(node != NULL)  {
				if( graph->array[node->dest].isdeleted == 0) 
					graph->array[node->dest].indegree = graph->array[node->dest].indegree - 1;
				node = node->next;
			}			
		}
	}
	return graph;
}

// Eliminate the nodes if their indegree's lower than M
struct Graph* eliminationOfNodes(struct Graph* graph, int M) {
	int i;
	for(i = 1; i < graph->V; i++) {
		if(graph->array[i].isdeleted == 0 && graph->array[i].indegree < M) {
			graph = removeNode(graph, i); // deleted the connection of the other nodes
			graph->array[i].isdeleted = 1;	
		}
	}
	return graph;
}

struct Graph* undirectedPaths(struct Graph* graph) {
	// for all nodes uses the DFS logic for traversal.
	// When pushed a node add it's visitedCount's index 1 for find number of directed and indirected paths
	struct stack* s = createStack();
	int i;
	
	for(i = 1; i < graph->V; i++) {
		if(graph->array[i].isdeleted == 0) {
			push(s, graph->array[i]);
			graph->array[i].visited[i] = 1;
			while(isEmpty(s) != 1) {
				struct AdjList v = pop(s);
				struct AdjListNode* node = v.head;
				if(node != NULL)
				while(node != NULL) {
					if(graph->array[i].visited[node->dest] != 1) {
						graph->visitedCount[node->dest]++;
						push(s, graph->array[node->dest]);
						graph->array[i].visited[node->dest] = 1;
					}
					node = node->next;
				}
			}
		}	
	}
	return graph;
}

struct Graph* isInfluencer(struct Graph* graph, int X, int Y) {
	int i, flag = 0;
	for(i = 1; i < graph->V; i++) {
		if(graph->array[i].isdeleted == 0 && graph->array[i].indegree >= X) {
			if(graph->visitedCount[i] >= Y) {
				printInfluencer(graph, graph->array[i], i);
				flag = 1;
			}
		}	
	}
	if(flag == 0) 
		printf("There is not any influencer in this people.\n");
	return graph;
}

struct Graph* readData(char *file_name) {
	FILE *ptr;
	char buff[250];
	char *token, *name, *surname;
	int flag = 0,  V = 0, src, dest;
	
	name = malloc(sizeof(STRING));
	surname = malloc(sizeof(STRING));

	ptr = fopen(file_name, "r");
	if(ptr == NULL){
		printf("File could not opened.\n");
		exit(0);
	}
	// finds the # of nodes in the given txt
	while(fgets(buff, 255, ptr) != NULL) {
		V++;
	}
	V = (V / 2) + 1; // # of nodes
	struct Graph* graph = createGraph(V);

	//reads txt and add the data to the graph
	rewind(ptr);
	while(fgets(buff, 255, ptr) != NULL) {
		if (buff[strlen(buff)-1] == '\n')
			buff[strlen(buff)-1] = '\0';
	// if the read line is person's line parse operations 
		if(flag == 0) {
			token = strtok(buff, ",");
			src = atoi(token); // source of the node
			token = strtok(NULL,",");
			strcpy(name,token);
			token = strtok(NULL,",");
			strcpy(surname,token);
			
	// union of the name and surname
			strcat(name," ");
			strcat(name, surname);
			flag = 1;
		}
	// if the read line is connection line parce operations
		else{
			token = strtok(buff, ",");
			while(token != NULL){
				dest = atoi(token);
				addEdge(graph, src, dest, name);
				token = strtok(NULL,",");
			}
			flag = 0;
		}
	}
	indegrees(graph);
	
	free(token);
	free(name);
	free(surname);
	fclose(ptr);
	return graph;
}

struct Graph* normalMode(struct Graph* graph, char *file_name, int M, int X, int Y ) {
	graph = readData(file_name);
	graph = eliminationOfNodes(graph, M);
	graph = undirectedPaths(graph);
	printf("\nINFLUENCERS:\n");
	isInfluencer(graph, X, Y);
	return graph;
}

struct Graph* detailedMode(struct Graph* graph, char *file_name, int M, int X, int Y ) {
	graph = readData(file_name);
	printf("\nGIVEN DATA:\n");
	printGraph(graph);
	graph = eliminationOfNodes(graph, M);
	printf("\nDATA AFTER ELIMINATION WITH M:\n");
	printGraph(graph);
	graph = undirectedPaths(graph);
	printf("\nINFLUENCERS:\n");
	isInfluencer(graph, X, Y);
	return graph;
}

int main()
{
	struct Graph* graph;
	int flag = 0, mode, M, X, Y;
	while(flag == 0) {
		printf("Enter the mode(0: normal mode / 1: detailed mode):\n");
		scanf("%d", &mode);
		if(mode != 0 && mode != 1)
			exit(1);
		printf("Enter the numbers: M - X - Y\n");
		scanf("%d%d%d", &M, &X, &Y);
		if(mode == 0) {
			graph = normalMode(graph, "socialNET.txt", M, X, Y);
		}
		else if(mode == 1) {
			graph = detailedMode(graph, "socialNET.txt", M, X, Y);
		}
	}
	return 0;
}

