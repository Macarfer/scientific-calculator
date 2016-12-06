/*
*	This tray Table is gonna be implemented as a forest of trees
*	it'll be more efficient to use a hash table but,the additional memory usage
*	and development complexity of a hash table isn't worth it for this approach
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

#define MIN_INT -2147483648
#define MAX_INT 2147483647

/*Each node contains a symbol and a pointer to the left and right node*/
struct tray{
	struct symbol lexicalComponent;
	struct tray * left;
	struct tray * right;
} tray;

/*Level nodes are useful for managing different blocks of code (they are not used in this approach)*/
struct levelNode{
	short level;
	struct levelNode * nextNode;
	struct tray * firstSymbolOfLevel;
}levelNode;

/*The symbolTable itself, just contains a pointer to the first levelNode*/
struct symbolTable{
	struct levelNode * first;
}symbolTable;

/*Just two functions that allow more modularity for the ones that summon them*/
symbol * insertSymbol(struct tray **,char * lexeme,int type);
void inorder(struct tray *firstSymbolOfLevel);
void finalize(struct tray *firstSymbolOfLevel);

/*Auxiliar nodes and trays for moving through the tree*/
struct levelNode * workingNode, * auxiliarNode;
struct tray * workingTray, * auxiliarTray;

/*if the working node isn't initializated this function initializates it*/
void  initializeSymbolTable(){
	if(workingNode==NULL)
		workingNode= malloc(sizeof(levelNode));
}

symbol * insertOnSymbolTable(char * lexeme, int type, short level){
	/*
	*
	*	If the table is empty, the first node is created
	*/
	if(symbolTable.first==NULL){
		symbolTable.first=malloc(sizeof(levelNode));
		symbolTable.first->level = 1;
		symbolTable.first->nextNode = NULL;
		symbolTable.first->firstSymbolOfLevel=NULL;
		return insertSymbol(&(symbolTable.first->firstSymbolOfLevel),lexeme,type);
	}
	/*
	*
	*	If the table isn't empty we must check the
	* 	linked list of table nodes
	*/

	workingNode = symbolTable.first;
	while(workingNode!=NULL){
		/*If the level of the symbol matches the level we're working with then we insert it*/
		if(workingNode->level==level){
			return insertSymbol(&workingNode->firstSymbolOfLevel,lexeme, type);
		}
			workingNode=workingNode->nextNode;
	}
	return NULL;
 }

/*Search if a symbol is in the tree and, if it is, it is returned*/
symbol * searchSymbol(char * lexeme){
	int index=1;
	auxiliarNode=symbolTable.first;
	/*Starting on the initial node, we go over the tree*/
	for(;auxiliarNode!=NULL;){

		index=1;
		auxiliarTray=auxiliarNode->firstSymbolOfLevel;
		for(;index>0;){
			/*We compare the lexeme we're searching for with the one present in the actual node*/
			switch(strcmp(auxiliarTray->lexicalComponent.lexeme,lexeme)){
				/*If the lexeme is smaller than the actual, when if it has been inserted it has been on the left*/
				case MIN_INT ... -1:
					if(auxiliarTray->right!=NULL){
					/*If the symbol has a left symbol then we repeat the proccess with it if not, the symbol is not in the table*/
						auxiliarTray=auxiliarTray->right;
					}else{
						index=0;
					}
				break;
				/*If the lexeme is bigger than the actual, when if it has been inserted it has been on the right*/
				case 1 ... MAX_INT :
					/*If the symbol has a right symbol then we repeat the proccess with it if not, the symbol is not in the table*/
					if(auxiliarTray->left!=NULL){
						auxiliarTray=auxiliarTray->left;
					}else{
						index=0;
					}
				break;
				/*If the lexemes are equal then we've found the lexeme we were looking for and we return it*/
				default:
					return &auxiliarTray->lexicalComponent;
			}
		}
		if(auxiliarNode->nextNode==NULL)
			return NULL;
		else
			auxiliarNode=auxiliarNode->nextNode;
	}
	return NULL;
}

/*Given a level, inserts a symbol on the right level*/
symbol * insertSymbol(struct tray ** firstSymbolOfLevel,char * lexeme,int type){
	/*If the first symbol doesn't exist, then we create it*/
	if(*firstSymbolOfLevel==NULL){
		/*The tray is allocated*/
		*firstSymbolOfLevel= malloc(sizeof(tray));
		/*We initializate the symbol itself, and we assign the right values to it's fields*/
		(*firstSymbolOfLevel)->lexicalComponent.lexeme = malloc(strlen(lexeme));
		strcpy((*firstSymbolOfLevel)->lexicalComponent.lexeme,lexeme);
		(*firstSymbolOfLevel)->lexicalComponent.type = type;
		(*firstSymbolOfLevel)->left = NULL;
		(*firstSymbolOfLevel)->right = NULL;

		/*A pointer to the created symbol is returned*/
		return &(*firstSymbolOfLevel)->lexicalComponent;
	}
	/*If the first symbol already exists*/
	else{
		workingTray=*firstSymbolOfLevel;
		/*We go over the tree searching for a place to store the new symbol*/
		for(;;){
			/*The element we compare to choose a direction is the lexeme of the symbol*/
			switch(strcmp(workingTray->lexicalComponent.lexeme,lexeme)){
				/*If the value is smaller,then we go left*/
				case MIN_INT ... -1:
				/*If the tray is empty, then we store the symbol there*/
				if(workingTray->right==NULL){
				/*The tray is allocated*/
					workingTray->right=malloc(sizeof(tray));
					workingTray->right->lexicalComponent.type = type;
					workingTray->right->lexicalComponent.lexeme = malloc(strlen(lexeme));
					strcpy(workingTray->right->lexicalComponent.lexeme,lexeme);
					workingTray->right->left=NULL;
					workingTray->right->right=NULL;
					/*The inserted symbol is returned*/
					return &workingTray->right->lexicalComponent;
				}
				/*If the tray isn't empty then we continue going over the tree*/
				else{
					workingTray=workingTray->right;
				}
				break;
				/*If the value is higher then we go right*/
				case 1 ... MAX_INT :
				/*If the tray is empty, then we store the symbol there*/
				if(workingTray->left==NULL){
					/*The tray is allocated*/
					workingTray->left=malloc(sizeof(tray));
					workingTray->left->lexicalComponent.type = type;
					workingTray->left->lexicalComponent.lexeme = malloc(strlen(lexeme));
					strcpy(workingTray->left->lexicalComponent.lexeme,lexeme);
					workingTray->left->left=NULL;
					workingTray->left->right=NULL;
					/*The inserted symbol is returned*/
					return &workingTray->left->lexicalComponent;
					}
					/*If the tray isn't empty then we continue going over the tree*/
					else{
						workingTray=workingTray->left;
				}
				break;
				/*If it is the same value, then we've found the element*/
				default:
					//workingTray->lexicalComponent.identifier=identifier;
					return &workingTray->lexicalComponent;
			}
		}

	}
}

/*This function iterates over the level nodes for printing the tree*/
void printSymbolTable(){
	workingNode = symbolTable.first;
	while(workingNode!=NULL){
		/*For each working node we print the tree in it in alphabetical order*/
		inorder(workingNode->firstSymbolOfLevel);
		workingNode=workingNode->nextNode;
	}

}

/*Just an inorder approach for printing the nodes of the tree*/
void inorder(struct tray *firstSymbolOfLevel){
	if(firstSymbolOfLevel== NULL)
		return;
	inorder(firstSymbolOfLevel->left);
	/*checks if the element is a value or a function*/
	if(firstSymbolOfLevel->lexicalComponent.type==IDENTIFIER)
		printf("%s =\t %g\n",firstSymbolOfLevel->lexicalComponent.lexeme,firstSymbolOfLevel->lexicalComponent.value.var);
	inorder(firstSymbolOfLevel->right);
 }

/*This function iterates over the level nodes for freeing the tree*/
void finalizeSymbolTable(){
	workingNode = symbolTable.first;
	while(workingNode!=NULL){
		/*For each working node we print the tree in it in alphabetical order*/
		finalize(workingNode->firstSymbolOfLevel);
		free(workingNode->firstSymbolOfLevel);
		workingNode=workingNode->nextNode;
	}
}

/*This iteration goes left,right, node is a postorder approach for deleting the tree*/
void finalize(struct tray *firstSymbolOfLevel){
	if(firstSymbolOfLevel== NULL)
		return;
	finalize(firstSymbolOfLevel->left);
	finalize(firstSymbolOfLevel->right);
	free(firstSymbolOfLevel->lexicalComponent.lexeme);
 }
