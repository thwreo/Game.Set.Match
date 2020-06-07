#ifndef __C1_C__
#define __C1_C__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct gameMoment {
	char table[20][20];
	char player;
	struct listMoments *nextMoments;
} gameMoment;

typedef struct listMoments {
	struct gameMoment *child;
	struct listMoments *next;
} listMoments;


void copyTable(char table[20][20], char copy[20][20],int l) {
	int i;
	for (i = 0; i < l; i++)
		strcpy(copy[i], table[i]);
}

gameMoment *initGame(char table[20][20], char player, gameMoment *start, int nrLines) {
	start = (gameMoment *)malloc(sizeof(gameMoment ));
	copyTable(table,start->table,nrLines);
	int l = strlen(start->table[nrLines - 1]);
	if (start->table[nrLines - 1][l-1]  != '\n')
		strcat(start->table[nrLines - 1], "\n");
	start->player = player;
	start->nextMoments = NULL;
	return start;
}

gameMoment *addMove(gameMoment *moment, int lineIdx, int rowIdx, int l, gameMoment *newMoment) {
	newMoment = (gameMoment *)malloc(sizeof(gameMoment));
	copyTable(moment->table, newMoment->table, l);
	newMoment->table[lineIdx][rowIdx] = moment->player;
	if (moment->player == 'R')
		newMoment->player = 'B';
	else
		newMoment->player = 'R';
	
	newMoment->nextMoments = NULL;

	return newMoment;
}

int winCheck(gameMoment *moment, int nrLines, int nrRows) {
	int i, j, k;
	int sum = 0;
	for (i = nrLines - 1; i >= 0; i--) {
		for (j = 0; j < nrRows; j++) {
			int check = 0;
			char val = moment->table[i][j*2];
			if (moment->table[i][j*2] != '-') {
				if (j*2+6 < nrRows*2) {
					check = 1;
					for (k = 1; k < 4; k++) {
						if (val != moment->table[i][j*2+k*2])
							check = 0;
					}
				}	
				if (i-3 >= 0 && check == 0) {
					check = 1;
					for (k = 1; k < 4; k++) {
						if (val != moment->table[i-k][j*2])
							check = 0;
					}
				}
				if (i-3 >= 0 && j*2+6 < nrRows*2 && check == 0) {
					check = 1;
					for (k = 1; k < 4; k++) {
						if (val != moment->table[i-k][j*2+k*2])
							check = 0;
					}
				}
				if (i-3 >= 0 && j*2-6 >= 0 && check == 0) {
					check = 1;
					for (k = 1; k < 4; k++) {
						if (val != moment->table[i-k][j*2-k*2])
							check = 0;
					}
				}
			}
			sum = sum + check;
		}
	}
	return sum;
}

int finalCheck(gameMoment *moment, int nrRows) {
	int check = 1;
	int i;
	for (i = 0; i < nrRows; i++) {
		if (moment->table[0][i*2] == '-')
			check = 0;
	}
	return check;
}

void allMoves(gameMoment *moment, int nrLines, int nrRows) {
	if (finalCheck(moment,nrRows) == 0 && winCheck(moment,nrLines,nrRows) == 0) {
		int i, j;
		for (j = 0; j < nrRows; j++) {
			for (i = 0; i < nrLines; i++) {
				int v = 0;
				if (i != nrLines - 1) {
					if (moment->table[i][j*2] == '-' && moment->table[i+1][j*2] != '-')
						v = 1;
				}
				else {
					if (moment->table[i][j*2] == '-')
						v = 1;
				}
				if (v == 1) {
					if (moment->nextMoments == NULL) {
						moment->nextMoments = (listMoments *)malloc(sizeof(listMoments));
						moment->nextMoments->child = addMove(moment, i, j*2, nrLines, moment->nextMoments->child);
						moment->nextMoments->next = NULL;
						allMoves(moment->nextMoments->child, nrLines, nrRows);
					}
					else {
						listMoments *aux = moment->nextMoments;
						while (moment->nextMoments->next)
							moment->nextMoments = moment->nextMoments->next;
						listMoments *newSibling = (listMoments *)malloc(sizeof(listMoments));
						newSibling->child = addMove(moment, i, j*2, nrLines, newSibling->child);
						newSibling->next = NULL;
						moment->nextMoments->next = newSibling;
						allMoves(moment->nextMoments->next->child, nrLines, nrRows);
						moment->nextMoments = aux;
					}
				}	
			}
		}
	}
}

void printGame(gameMoment *moment, int nrLines, int tab, FILE *file) {
	int i;
	listMoments *aux = moment->nextMoments;
	while (aux) {
		for (i = 0; i < nrLines; i++) {
			int j;
			for (j = 0; j < tab; j++)
				fputs("\t",file);
			fputs(aux->child->table[i], file);
		}
		fputs("\n",file);
		printGame(aux->child, nrLines, tab+1, file);
		aux = aux->next;
	}

}

void freeTree(gameMoment *moment) {
	while (moment->nextMoments) {
		listMoments *aux = moment->nextMoments;
		freeTree(moment->nextMoments->child);
		moment->nextMoments = moment->nextMoments->next;
		free(aux->child);
		free(aux);
	}
}

void c1(FILE *input, FILE *output) {
	gameMoment *start = NULL;

	char first_line[20];
	char lines[20];

	fgets(first_line,20,input);

	char table[20][20];
	int nrLines = first_line[0] - 48; 
	int nrRows = first_line[2] - 48; 
	char player = first_line[4]; 

	int i;

	for (i = 0; i < nrLines; i++) {
		strcpy(table[i], fgets(lines,20,input));
	}

	start = initGame(table,player,start,nrLines);
	
	allMoves(start,nrLines,nrRows);
	
	for (i = 0; i < nrLines; i++)
		fputs(start->table[i],output);
	fputs("\n",output);
	
	printGame(start,nrLines,1,output);

	

	freeTree(start);
	free(start);
}

#endif
