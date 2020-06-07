#ifndef __C2_C__
#define __C2_C__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree {
	char player[5];
	int value;
	struct childList *list;
} tree;

typedef struct childList {
	struct tree *node;
	struct childList *next;
} childList;

int minORmax(childList *list, char player[5]) {
	int min = list->node->value;
	int max = list->node->value;

	while (list) {
		if (max < list->node->value)
			max = list->node->value;
		if (min > list->node->value)
			min = list->node->value;
		list = list->next;
	}
	if (strcmp(player, "Min") == 0)
		return min;
	else
		return max;
}

void values(int levels, char **lines, int **values, int **leaves) {

	int level;

	for (level = 2; level <= levels; level++) {

		int l = strlen(lines[level]);
		int p = 0;
		int c = 0;

		while (p < l) {
			int add = 1;
			int val = 0;
			int sign = 1;

			while (lines[level][p+add] != 41 && lines[level][p+add] != 93) {
				if (lines[level][p+add] == 45) {
					sign = -1;
					add = add + 1;
				}
				else {
					val = val*10 + (lines[level][p+add] - 48);
					add = add + 1;
				}
			}	
			val = val * sign;

			values[level][c] = val;

			if (lines[level][p] == 40)
				leaves[level][c] = 0; 
			else
				leaves[level][c] = 1;

			c = c + 1;
			p = p + add + 2;
		}
	}
}

tree *addNode(int value, int level) {
	tree *node = (tree *)malloc(sizeof(tree));
	if (level % 2)
		strcpy(node->player, "Max");
	else
		strcpy(node->player, "Min");
	node->value = value;
	node->list = NULL;
	return node;
}

void allGame(tree *gameNode, int max, int level, int *pos, int **val, int **leaves) {
	if (level <= max) {
		int i;
		int nr = gameNode->value;
		for (i = 0; i < nr; i++) {
			if (gameNode->list == NULL) {
					gameNode->list = (childList *)malloc(sizeof(childList));
					gameNode->list->node = addNode(val[level][pos[level]], level);
					gameNode->list->next = NULL;
					if (leaves[level][pos[level]] == 0)
						allGame(gameNode->list->node, max, level + 1, pos, val, leaves);
				}
			else {
				childList *aux = gameNode->list;
				while (gameNode->list->next)
					gameNode->list = gameNode->list->next;
				childList *auxList = (childList *)malloc(sizeof(childList));
				auxList->node = addNode(val[level][pos[level]], level);
				auxList->next = NULL;
				gameNode->list->next = auxList;
				if (leaves[level][pos[level]] == 0)
					allGame(gameNode->list->next->node, max, level + 1, pos, val, leaves);
				gameNode->list = aux;
			}
			pos[level] = pos[level] + 1;	
		}
		childList *aux = gameNode->list;
		gameNode->value = minORmax(aux, gameNode->player);
	}
}

void printMinimax(tree *root, int tab, FILE *file) {
	childList *aux = root->list;
	while (aux) {	
		int i;
		for (i = 0; i < tab; i++)
			fputs("\t",file);
		fprintf(file, "%d", aux->node->value);
		fputs("\n",file);
		printMinimax(aux->node, tab+1, file);
		aux = aux->next;
	}
}

void freeMinimax(tree *root) {
	while (root->list) {
		childList *aux = root->list;
		freeMinimax(root->list->node);
		root->list = root->list->next;
		free(aux->node);
		free(aux);
	}
}

void c2(FILE *input, FILE *output) {

	char first_line[10];

	fgets(first_line, 10, input);

	int levels = 0, lenght = strlen(first_line);
	int j;
	for (j = 0; j < lenght - 1; j++)
		levels = levels * 10 + (first_line[j] - 48);
	printf("%d\n",levels );



	char **lines = (char **)calloc((levels + 1), sizeof(char *));
	int i;

	for (i = 1; i <= levels; i++) {
		lines[i] = (char *)calloc((500000), sizeof(char));
		fgets(lines[i], 500000, input);
	}


	int p = 1;
	int l = strlen(lines[1]);
	int val = 0;
	while (p < l-2) {
		val = val*10 + (lines[1][p] - 48);
		p = p + 1;
	}

	tree *root = addNode(val, 1);
	int **valMatrix = (int **)calloc((levels + 1), sizeof(int *));
	int **leaves = (int **)calloc((levels + 1), sizeof(int *));
	for (i = 1; i <= levels; i++) {
		valMatrix[i] = (int *)calloc(strlen(lines[i]), sizeof(int));
		leaves[i] = (int *)calloc(strlen(lines[i]), sizeof(int));
	}
	values(levels, lines, valMatrix, leaves); 

	int *pos = (int *)calloc(500000, sizeof(int));

	allGame(root, levels, 2, pos, valMatrix, leaves);

	for (i = 0; i <= levels; i++) {
		free(valMatrix[i]);
		free(leaves[i]);
		free(lines[i]);
	}
	free(lines);
	free(valMatrix);
	free(leaves);
	free(pos);

	fprintf(output, "%d", root->value);
	fputs("\n", output);
	printMinimax(root, 1, output);


	freeMinimax(root);
	free(root);
}

#endif
