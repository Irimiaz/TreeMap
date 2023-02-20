#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeMap.h"

#define MAX(a, b) (((a) >= (b))?(a):(b))


/* Creeaza un arbore cu o serie de metode asociate
 *
 * return: arborele creat
 */
TTree* createTree(void* (*createElement)(void*),
				  void (*destroyElement)(void*),
				  void* (*createInfo)(void*),
				  void (*destroyInfo)(void*),
				  int compare(void*, void*)) {
	TTree* copac = malloc(sizeof(TTree)); ///alocam memorie pentru arbore
	copac->compare = compare;
	copac->createElement = createElement; /// ii dam functiile pe care le vom folosi ulterior
	copac->createInfo = createInfo;
	copac->destroyElement = destroyElement;
	copac->destroyInfo = destroyInfo;
	copac->size = 0;	///size ul initial este 0
	copac->root = NULL;
	return copac;
}


/* Verifica daca un arbore este gol (vid)
 * 		1 - daca arborele este gol
 * 		0 - in caz contrar
 */
int isEmpty(TTree* tree) {
	if(tree->root == NULL)
		return 1;
	return 0;
}


/* Cauta un anumit element in interiorul unui arbore
 *
 * tree: structura cu metodele asociate arborelui
 *   !folosit pentru a putea compara elementul curent cu cel cautat
 *
 * x: radacina arborelui curent (in care se face cautarea)
 * elem: elementul ce trebuie cautat
 */
TreeNode* search(TTree* tree, TreeNode* x, void* elem) {
	
	if(x == NULL)
		return NULL;
	TreeNode* nod = x;
	while(nod != NULL && tree->compare(nod->elem,elem) != 0 ) { ///cauta in tot arborele un anumit nod
		if(tree->compare(elem,nod->elem) > 0)
			nod = nod->right;
		else
			nod = nod->left;
	}
	if(nod == NULL)
		return NULL; 
	if(tree->compare(elem, nod->elem) == 0) ///daca gaseste un nod duplicat, va returna acel nod
		return nod;
	return NULL;
}


/* Gaseste nodul cu elementul minim dintr-un arbore
 * avand radacina in x
 */
TreeNode* minimum(TreeNode* x) {
	if(x == NULL)
		return NULL;
	TreeNode* nod = x;
	while(nod != NULL && nod->left != NULL) /// merge pana in extremitatea din stanga
		nod = nod->left;
	return nod;
}

/* Gaseste nodul cu elementul maxim dintr-un arbore
 * avand radacina in x
 */
TreeNode* maximum(TreeNode* x) {
	if(x == NULL)
		return NULL;
	TreeNode* nod = x;
	while(nod != NULL && nod->right != NULL) /// merge pana in extremitatea din dreapta
		nod = nod->right;
	return nod;
}


/* Functie pentru gasirea succesorului unui nod
 * (succesorul in inordine)
 */
TreeNode* successor(TreeNode* x) {
	if(x == NULL)
		return NULL;
	TreeNode* nod = x;
	
	if(nod->right != NULL) {
		nod = nod->right;
		return minimum(nod); /// succesorul este minimul din arborele drept
	}
	else {
		while(nod->parent != NULL && nod->parent->right == nod) ///daca nu are fiu drept, va urca din parinte in parinte pana gaseste succesorul
			nod = nod->parent;
		return nod->parent;
	}
	return NULL;
}


/* Functie pentru gasirea predecesorului unui nod
 * (predecesorul in inordine)
 */
TreeNode* predecessor(TreeNode* x) {
	if(x == NULL)
		return NULL;
	TreeNode* nod = x;
	if(nod->left != NULL) {
		nod = nod->left;
		return maximum(nod); /// succesorul este maximul din arborele drept
	}
	else {
		while(nod->parent != NULL && nod->parent->left == nod)///daca nu are fiu drept, va urca din parinte in parinte pana gaseste succesorul
			nod = nod->parent;
		return nod->parent;
	}
	return NULL;
}


/* Actualizeaza inaltimea unui nod din arbore
 */
void updateHeight(TreeNode* x) {

	int leftHeight = 0;
	int rightHeight = 0;

	if (x != NULL) {
		if (x->left != NULL)  leftHeight  = x->left->height;
		if (x->right != NULL) rightHeight = x->right->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la stanga
 * a subarborelui care are varful in x
 *
 *   (x)	 Rotatie    (y)
 *	 / \     stanga     / \
 *  a  (y)    ---->   (x)  c
 *	   / \			  / \
 *    b   c          a   b
 */
void avlRotateLeft(TTree* tree, TreeNode* x) {
	if(tree == NULL || tree->root == NULL) 
		return;
	TreeNode* y = x->right;
	if (x == tree->root){ //in cazul in care este radacina trebuie sa modificam acest lucru
		tree->root = y;
	}
	x->right = y->left;///mutam nodul B la locul lui
	if(x->right != NULL)
		x->right->parent = x; 

	y->parent = x->parent;
	if(y->parent != NULL && tree->compare(y->elem,y->parent->elem) > 0)///legam nodul x de noul sau parinte
		y->parent->right = y;
	else if (y->parent != NULL && tree->compare(y->elem, y->parent->elem) < 0)
		y->parent->left = y;

	y->left = x;//legam x de y
	x->parent = y;

	updateHeight(x);///modificarea inaltimilor
	updateHeight(y);
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la dreapta
 * a subarborelui care are varful in y
 *
 *     (y)	 Rotatie    (x)
 *	   / \   dreapta    / \
 *   (x)  c   ---->    a  (y)
 *	 / \			      / \
 *  a   b                b   c
 */
void avlRotateRight(TTree* tree, TreeNode* y) {
	TreeNode* x = y->left;
	if (y == tree->root){ ///in cazul in care este radacina trebuie sa modificam acest lucru
		tree->root = x;
	}

	y->left = x->right;  ///mutam nodul B la locul lui
	if(y->left != NULL)
		y->left->parent = y;

	x->parent = y->parent;///legam nodul x de noul sau parinte
	if(x->parent != NULL && tree->compare(x->elem,x->parent->elem) > 0) 
		x->parent->right = x;
	else if (x->parent != NULL && tree->compare(x->elem,x->parent->elem) < 0)
		x->parent->left = x;
	
	x->right = y; //legam x de y
	y->parent = x;

	updateHeight(x); ///modificarea inaltimilor
	updateHeight(y);	
}


/* Calculeaza factorul de echilibrare pentru un nod x
 * (AVL balance factor)
*/
int avlGetBalance(TreeNode *x) {
	if (x == NULL)
		return 0;
	if(x->left != NULL && x->right != NULL) ///luam fiecare caz in parte pentru a nu fi posibilitatea de un segmentation fault
		return x->left->height - x->right->height;
	if(x->left == NULL && x->right != NULL)
		return -x->right->height;
	if(x->left != NULL && x->right == NULL)
		return x->left->height;
	return 0;
	
}


/* Functie pentru reechilibrarea unui arbore AVL
 * in urma unei inserari prin rotatii simple sau duble
 * si recalcularea inaltimii fiecarui nod intalnit parcurgand
 * arborele de jos in sus, spre radacina
 *
 */
void avlFixUp(TTree* tree, TreeNode* y) {
	TreeNode* nod = y;
	while(nod != NULL) {
		nod->height = avlGetBalance(nod);
		nod = nod->parent;
	}
}


/* Functie pentru crearea unui nod
 *
 * value: valoarea/cheia din cadrul arborelui
 * info: informatia/valoarea din dictionar
 */
TreeNode* createTreeNode(TTree *tree, void* value, void* info) {

	if (tree == NULL)
		return NULL;

	// Alocarea memoriei
	TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));

	// Setarea elementului si a informatiei
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);

	// Initializarea legaturilor din cadrul arborelui
	node->parent = node->right = node->left = NULL;

	// Initializarea legaturilor din cadrul listei dublu inlantuite
	node->next = node->prev = node->end = NULL;


	// Inaltimea unui nod nou este 1
	// Inaltimea lui NULL este 0
	node->height = 1;
	return node;
}


/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */
void insert(TTree* tree, void* elem, void* info) {
	
	TreeNode* nod = createTreeNode(tree, elem, info);
	if(tree->root == NULL) {
		tree->root = nod;
		tree->size = 1;
		tree->root->prev = NULL;
		tree->root->next = NULL;
		tree->root->end = tree->root;
		return;	///punem la inceput, daca nu exista niciun nod
	}
	TreeNode* min = minimum(tree->root);
	TreeNode* max = maximum(tree->root);
	
	TreeNode* aux = tree->root;
	TreeNode* p = tree->root;
	while(aux != NULL && tree->compare(aux->elem, nod->elem) != 0) { // parcurgem arborele pana ajungem la pozitia in care trebuie inserat nodul
		p = aux;
		if(tree->compare(aux->elem, nod->elem) > 0)
			aux = aux->left;
		else if (tree->compare(aux->elem, nod->elem) < 0)
			aux = aux->right; 
	}
	if(aux == NULL) { ///este cazul in care nu exista un nod duplicat
		nod->end = nod;
		aux = nod;///inserare normala in arbore binar de cautare
		aux->parent = p;
		if(tree->compare(p->elem, aux->elem) > 0) 
			p->left = aux;
		else
			p->right = aux;

		
		while(p != NULL) {	///reparam height pentru fiecare nod
			updateHeight(p);
			p = p->parent;
			
		}
		while(aux != NULL ) { ///verificam din parinte in parinte daca subarborele cu radacina in acel parinte este echilibrat
			
			TreeNode* reset = aux;
			int index = avlGetBalance(aux);
			if(index >1 && aux->left->left != NULL)
				avlRotateRight(tree,aux);
			else if(index < -1 && aux->right->right != NULL)
				avlRotateLeft(tree,aux);
			else if(index > 1 && aux->left->elem < aux->left->right->elem){
				avlRotateLeft(tree,aux->left);
				avlRotateRight(tree,aux);
			}
			else if(index < -1 && aux->right->elem < aux->right->left->elem) {
				avlRotateRight(tree,aux->right);
				avlRotateLeft(tree,aux);
			}
			
			while(reset->parent != NULL) {
				updateHeight(reset);
				reset = reset->parent;
			}
			aux = aux->parent;
    }
	
		if(tree->compare(nod->elem, min->elem) < 0) { ///facem si legaturile pentru lista dublu inlantuita
			nod->prev = NULL;
			nod->next = min;
			min->prev = nod;///in acest caz, se va insera la inceputul listei
			//printf("1");
		}
		else if(tree->compare(nod->elem, max->elem) > 0) {
			nod->next = NULL;
			nod->prev = max; ///in acest caz, se va insera la sfarsitul listei
			max->next = nod;
			//printf("2");
		}
		else {
			while(tree->compare(nod->elem, min->elem) >0 && min != NULL)
				min = min->next;
			min->prev->next = nod;
			nod->prev = min->prev; /// se va insera in interiorul listei, dupa ce se gaseste pozitia potrivita
			min->prev = nod;
			nod->next = min;
		}
	}
	else { /// este cazul in care exista deja nodul in arbore
		
		if(tree->compare(nod->elem, max->elem) == 0) {
			max->end->next = nod; ///este cazul in care trebuie inserat la finalul listei
			nod->prev = max->end;
			nod->next = NULL;
			max->end = nod;
		}
		else {
			while(tree->compare(nod->elem, min->elem) != 0 && min != NULL) { ///este inserat in interiorul listei, pozitionand corespunzator si nod->nd
				min = min->next;
			}
			nod->next = min->end->next;
			min->end->next->prev = nod;
			min->end->next = nod;
			nod->prev = min->end;
			min->end = nod;
		}
		
	}
	tree->size++;
	
}

/* Eliminarea unui nod dintr-un arbore
 *
 * ! tree trebuie folosit pentru eliberarea
 *   campurilor `elem` si `info`
 * */
void destroyTreeNode(TTree *tree, TreeNode* node){

	// Verificarea argumentelor functiei
	if(tree == NULL || node == NULL) return;

	// Folosirea metodelor arborelui
	// pentru de-alocarea campurilor nodului
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// Eliberarea memoriei nodului
	free(node);
}


/* Eliminarea unui nod din arbore
 *
 * elem: cheia nodului ce trebuie sters
 * 	! In cazul in care exista chei duplicate
 *	  se va sterge ultimul nod din lista de duplicate
 */
void delete(TTree* tree, void* elem) {
	if(tree == NULL || tree->root == NULL)
		return;
	if(search(tree,tree->root,elem) == NULL)
		return;
	TreeNode* nod = tree->root;
	while(tree->compare(nod->elem, elem) != 0) { ///parcurgem arborele pana ajungem la nodul ce trebuie sters
		if(tree->compare(nod->elem, elem) > 0)
			nod = nod->left;
		else if(tree->compare(nod->elem, elem) < 0)
			nod = nod->right;
	}
	TreeNode* minbun = minimum(tree->root);
	TreeNode* min = minimum(tree->root);
	TreeNode* max = maximum(tree->root);
	TreeNode* aux = NULL;

	while(tree->compare(min->elem, elem) != 0 && min != NULL)
		min = min->next;
	if(tree->compare(max->elem, elem) == 0 && max->end != max) { ///cazul in care exista nodul duplicat , si este si maximul listei
		aux = max->end;
		max->end->prev->next = NULL;
		max->end = max->end->prev;
		destroyTreeNode(tree,aux);
		tree->size--;
		aux = NULL;
		return;
	}
	else if(min != NULL && min != min->end) { /// cazul in care exista nodul duplicat, si este si minimul listei
		aux = min->end;
		min->end->next->prev = min->end->prev;
		min->end->prev->next = min->end->next;
		min->end = min->end->prev;
		destroyTreeNode(tree, aux);
		tree->size--;
		aux = NULL;
		return;
	}
	else {
		if(nod->right == NULL && nod->left == NULL) { /// cazul in care nu are fii
			if(nod == tree->root) { /// cazul in care trebuie sa eliminam ultimul nod din lista
				destroyTreeNode(tree,tree->root);
				tree->root = NULL;
			}
			else {
				aux = nod->parent; 
				if(tree->compare(nod->parent->elem, nod->elem) > 0)
					nod->parent->left = NULL;
				else
					nod->parent->right = NULL;
				if(nod == max) {
					nod->prev->next = NULL;
				}
				else if(nod == minbun){
					nod->next->prev = NULL;
				}
				else {
					nod->next->prev = nod->prev;
					nod->prev->next = nod->next;
				}
				destroyTreeNode(tree, nod);
				nod = NULL;
				
			}
		}
		else if(nod->right != NULL && nod->left == NULL) {// cazul in care are doar un fiu drept
			aux = nod->parent;
			nod->right->parent = nod->parent;
			if(tree->compare(nod->parent->elem, nod->elem) > 0) {
				nod->parent->left = nod->right;
			}
			else {
				nod->parent->right = nod->right;
			}
			nod->prev->next = nod->next;
			nod->next->prev = nod->prev;
			destroyTreeNode(tree, nod);
			nod = NULL;
		}
		else if(nod->right == NULL && nod->left != NULL) { ///cazul in care are doar un fiu stang
			aux = nod->parent;
			nod->left->parent = nod->parent;
			if(tree->compare(nod->parent->elem, nod->elem) > 0) {
				nod->parent->left = nod->right;
			}
			else {
				nod->parent->right = nod->right;
			}
			nod->prev->next = nod->next;
			nod->next->prev = nod->prev;
			destroyTreeNode(tree, nod);
			nod = NULL;
		}
		else if (nod->right != NULL && nod->left != NULL) { /// cazul in care are 2 fii
			min = minimum(nod->right);
			aux = min->parent;
			tree->destroyInfo(nod->info);
			tree->destroyElement(nod->elem);
			nod->elem = tree->createElement(min->elem);
			nod->info = tree->createInfo(min->info);
			if(min->right != NULL ) 
				min->right->parent = min->parent;
			if(min->parent != tree->root)
				min->parent->left = min->right;
			else
				min->parent->right = min->right;
			if(min->next != NULL)
				min->next->prev = min->prev;
			if(min->prev != NULL)
				min->prev->next = min->next;
			destroyTreeNode(tree,min);
			min = NULL;
		}
		
	}
	avlFixUp(tree,aux);
	TreeNode* reset = aux;
	while(aux != NULL ) { ///verificam din parinte in parinte daca subarborele cu radacina in acel parinte este echilibrat
		int index = avlGetBalance(aux);
		if(index >1 && aux->left->left != NULL){
			avlRotateRight(tree,aux);

		}
		else if(index < -1 && aux->right->right != NULL){
			avlRotateLeft(tree,aux);

		}
		else if(index > 1 && aux->left->elem < aux->left->right->elem){
			avlRotateLeft(tree,aux->left);
			avlRotateRight(tree,aux);
		}
		else if(index < -1 && aux->right->elem > aux->right->left->elem) {
			avlRotateRight(tree,aux->right);
			avlRotateLeft(tree,aux);
		}
		while(reset != NULL) {
			updateHeight(reset);
			reset = reset->parent;
		}
		aux = aux->parent;
	}
	tree->size--;
	
}


/* Eliberarea memoriei unui arbore
 */
void destroyTree(TTree* tree){

	/* Se poate folosi lista dublu intalntuita
	 * pentru eliberarea memoriei
	 */
	if (tree == NULL || tree->root == NULL)
		return;
	TreeNode* min = minimum(tree->root);
	TreeNode* aux = min;
	while(min != NULL) {
		aux = min;
		min = min->next;
		destroyTreeNode(tree, aux);
	}
	tree->size = 0;
	free(tree);
}