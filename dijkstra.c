/*
 * My previous submit was losing time with sorting algorithms,
 * I saw this algorithm in websited and used it.
 */
//#include <assert.h>
//#include <limits.h>
//#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline(char *);
char** split_string(char*, char**);

typedef struct _node node;

typedef struct _edge{
	node* nodesedge[2];
	int weight;
} edge;

typedef struct _edgelist{
	edge *ed;
	struct _edgelist *next;
} edgelist;

typedef struct _node{
	int name;
	edgelist *fedge;
	edgelist *ledge;
	int sdistance;
} node;

typedef struct _queue_node {
	node* node;
	struct _queue_node* next;
} queue_node;

typedef struct _queue {
	queue_node* first;
	queue_node* last;
} queue;

edge * connect_nodes(node *first, node *second, int w){
	edge *e = (edge *)malloc(sizeof(edge));
	e->nodesedge[0] = first;
	e->nodesedge[1] = second;
	e->weight = w;

	node *undirected[2] = {first, second};

	for(int i=0; i<2; i++){
		edgelist * addtolist = (edgelist *)malloc(sizeof(edgelist));
		addtolist->ed = e;
		addtolist->next = NULL;

		if(undirected[i]->fedge == NULL){
			undirected[i]->fedge = undirected[i]->ledge = addtolist;
		}
		else{
			undirected[i]->ledge->next = addtolist;
			undirected[i]->ledge = addtolist;
		}
	}

	return e;
}

void push(queue *q, node *n){
	queue_node *same = q->first;
	while(same){
		if(same->node == n){
			return;
		}
		same = same->next;
	}

	queue_node *add = (queue_node *)malloc(sizeof(queue_node));
	add->node = n;
	add->next = NULL;

	if(!q->first){
		q->first = add;
		q->last = add;
	}
	else{
		q->last->next = add;
		q->last = q->last->next;
	}
}

node *pop(queue *q) {
	if (q->first == NULL) {
		return NULL;
	} else {
		queue_node* popped = q->first;
		if (q->first->next == NULL) {
			q->first = NULL;
			q->last = NULL;
		} else {
			q->first = q->first->next;
		}
		node* n = popped->node;
		free(popped);
		popped = NULL;
		return n;
	}
}

void shortestReach(node *snode){
	queue q;
	q.first = NULL;
	q.last = NULL;
	push(&q, snode);
	snode->sdistance = 0;
	node* n;
	while((n = pop(&q))) {
		edgelist* list_node = n->fedge;
		while (list_node != NULL) {
			edge *e = list_node->ed;
			node *adj = (e->nodesedge[0] == n) ? e->nodesedge[1] : e->nodesedge[0];

			int current_distance = n->sdistance + e->weight;
			if (adj->sdistance < 0 || adj->sdistance > current_distance) {
				adj->sdistance = current_distance;
				push(&q, adj);
			}
			list_node = list_node->next;
		}
	}
}

int main() {
	FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
	int t;
	scanf("%i", &t);

	for (int t_itr = 0; t_itr < t; t_itr++) {
		int n,m;
		scanf("%i %i", &n, &m);

		node **nodes = (node **)malloc(n*sizeof(node *));
		for(int i=0; i<n; i++){
			nodes[i] = (node *)malloc(sizeof(node));
			nodes[i]->fedge = NULL;
			nodes[i]->ledge = NULL;
			nodes[i]->sdistance = -1;
			nodes[i]->name = i+1;
		}

		edge **edges = (edge **)(malloc(m*sizeof(edge *)));

		for (int i = 0; i < m; i++) {
			int nums[3];
			scanf("%i %i %i", &nums[0], &nums[1], &nums[2]);
			edges[i] = connect_nodes(nodes[nums[0]-1], nodes[nums[1]-1], nums[2]);
		}

		int s;
		scanf("%i", &s);
		//MAIN FINDING FUNCTION
		shortestReach(nodes[s-1]);

		//Print RESULTS
		for(int i=0; i<n; i++){
			if(i == (s-1)){
				continue;
			}
			fprintf(fptr, "%d ", nodes[i]->sdistance);
		}
		fprintf(fptr, "\n");

		//Free memory
		for(int i=0; i<n; i++){
			edgelist *list = nodes[i]->fedge;
			while(list != NULL){
				edgelist *next = list->next;
				free(list);
				list = next;
			}
			free(nodes[i]);
		}
		free(nodes);

		for(int i=0; i<m; i++){
			free(edges[i]);
		}
		free(edges);
	}

	return 0;
}
