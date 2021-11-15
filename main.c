#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define COLUMNS 2

/////////////// NODE /////////////// 
struct node
{
	int vertex;
	struct node *next;
};

struct node *create_node(int); // constructor

/////////////// GRAPH /////////////// 
struct Graph
{
	struct node **adj_list;
	int *visited;
	int edge_count;
};

struct Graph *create_graph(int); // constructor

// Graph methods - Implementing Undirected Graph
void add_edge(struct Graph*, int, int);
void DFS(struct Graph*, int, int*);

//////////////////////////////////////////////////

int main(int argc, char **argv)
{
	// Check for input-dataset
	if (argc <= 1)
	{
		fprintf(stderr, "[Usage]: ./main <filename>\n");
		return EXIT_FAILURE;
	}

	// Fetch filename
	char *file_name = argv[1];
	char *file_mode = "r";

	// Open file in read-mode
	FILE *fptr = fopen(file_name, file_mode);

	// File open - error handling
	if (fptr == NULL)
	{
		fprintf(stderr, "[ERROR]: Unable to read file - %s\n", file_name);
		return EXIT_FAILURE;
	}

	// Print dataset filename
	printf("Dataset: %s\n", file_name);

	// Buffer-memory to parse and store data from datasets
	char line[BUFFER_SIZE], *token, *delim = " ";
	int node[COLUMNS], i = 0, max_vertex = 0, edge_count = 0;

	// Read file-contents - To set init-parameters
	while (fgets(line, BUFFER_SIZE, fptr) != NULL)
	{
		line[strlen(line) - 1] = '\0'; // \n -> \0
		
		i = 0;
		token = strtok(line, delim);
		do {
			node[i++] = atoi(token);
		} while((token = strtok(NULL, delim)) != NULL);

		++edge_count; // Count number of edges

		// According to brain-network dataset
		// vertex count is exactly equal to the
		// vertex with max value
		for (int j = 0; j < COLUMNS; ++j)
			if (max_vertex < node[j])
				max_vertex = node[j]; 

		// DEBUG
		// printf("[%d]\t->\t[%d]\n", node[0], node[1]);
	}

	printf("\n");
	printf("|V| = %d\n", max_vertex);
	printf("|E| = %d\n", edge_count);
	printf("\n");

	//////////////////////////////////////////////////

	rewind(fptr); // fseek(fptr, 0, SEEK_SET);

	// According to our brain-network dataset:
	// ---
	// Max vertex = Vertex count
	struct Graph *graph = create_graph(edge_count);
	while (fgets(line, BUFFER_SIZE, fptr) != NULL)
	{
		line[strlen(line) - 1] = '\0'; // \n -> \0
		
		i = 0;
		token = strtok(line, delim);
		do {
			node[i++] = atoi(token);
		} while((token = strtok(NULL, delim)) != NULL);
		
		// Add edge to the <graph>
		add_edge(graph, node[0], node[1]);
	}

	// Depth first search for the max number in graph
	int depth = 0;
	DFS(graph, max_vertex, &depth);

	printf("Depth = %d edges\n", depth);

	// Close file-pointer
	fclose(fptr);
	
	return EXIT_SUCCESS;
}

// Node constructor
struct node *create_node(int vertex)
{
	struct node *new_node = malloc(sizeof(struct node));
	new_node->vertex = vertex;
	new_node->next = NULL;
	return new_node;
}

// Graph constructor
struct Graph *create_graph(int ec)
{
	struct Graph *graph = malloc(sizeof(struct Graph));
	graph->adj_list = malloc(ec * sizeof(struct node*));
	graph->visited = malloc(ec * sizeof(int *));
	graph->edge_count = ec;

	for (int i = 0; i < ec; ++i)
	{
		graph->adj_list[i] = NULL;
		graph->visited[i] = 0;
	}

	return graph;
}

// Graph method - Add an (undirected) edge between two nodes
void add_edge(struct Graph *graph, int src, int dst)
{
	// Add edge: src -> dst
	struct node *new_node = create_node(dst);
	new_node->next = graph->adj_list[src];
	graph->adj_list[src] = new_node;

	// Add edge: dst -> src
	new_node = create_node(src);
	new_node->next = graph->adj_list[dst];
	graph->adj_list[dst] = new_node;

	return;
}

void DFS(struct Graph *graph, int vertex, int *depth)
{
	struct node *adj_list = graph->adj_list[vertex];
	struct node *buff_node = adj_list;

	graph->visited[vertex] = 1;
	printf("Visited node: [%d]\n", vertex);

	while(buff_node != NULL)
	{
		int adj_vertex = buff_node->vertex;

		if (graph->visited[adj_vertex] == 0)
		{
			*depth += 1;
			DFS(graph, adj_vertex, depth);
		}

		buff_node = buff_node->next;
	}

	return;
}
