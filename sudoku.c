#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;



Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int enColumna(int sudo[9][9], int x, int cand, int posy) {// 0 no existe, 1 existe
    for (int n = 0 ; n < 9 ; n++) {
        if (sudo[x][n] == cand && n != posy)
            return 1;
    }
    return 0;
}

int enFila(int sudo[9][9], int y, int cand, int posx) {// 0 no existe, 1 existe
    for (int n = 0 ; n < 9 ; n++) {
        if (sudo[n][y] == cand && n != posx)
            return 1;
    }
    return 0;
}

int enSubMatriz(int sudo[9][9], int x, int y, int cand) { // 0 no existe, 1 existe
    int k = 3 * (x/3) + (y/3), p;
    for(p=0;p<9;p++) {
        int i=3*(k/3) + (p/3) ;
        int j=3*(k%3) + (p%3) ;
        if(sudo[i][j] == cand && !(i == x && j == y)) {
            return 1;
        }
    }
    return 0;
}

int is_valid(Node* n){
    for (int i = 0 ; i < 9 ; i++) {
        for (int k = 0 ; k < 9 ; k++) { // Recorrer todo el sudoku
            if (n -> sudo[i][k] != 0) {
                if (enColumna(n -> sudo, i, n -> sudo[i][k], k))
                    return 0;
                if (enFila(n -> sudo, k, n -> sudo[i][k], i))
                    return 0;
                if (enSubMatriz(n -> sudo, i, k, n -> sudo[i][k]))
                    return 0;
            }
        }
    }
    return 1;
}

List* get_adj_nodes(Node* n){
    List *list = createList();
    int flag = 1;
    for (int i = 0 ; i < 9 ; i++) {
        for (int k = 0 ; k < 9 ; k++) { // Recorrer todo el sudoku
            if (n -> sudo[i][k] == 0) {
                for (int cand = 1 ; cand <= 9 ; cand++) { // Itera cada posible estado
                    Node *copia = copy(n);
                    copia -> sudo[i][k] = cand;
                    if (is_valid(copia)) {
                        pushBack(list, copia);
                        flag = 0;
                    }
                } 
            }
            if (!flag) break;
        }
        if (!flag) break;
    }
    return list;
}


int is_final(Node* n) {
    List *lista = get_adj_nodes(n);
    int res = (first(lista) == NULL) ? 1 : 0;
    free(lista);
    return res;
}

Node* DFS(Node* initial, int* cont){
    Stack *pila = createStack();
    push(pila, initial);
    while (top(pila) != NULL) {
        Node *aux = top(pila);
        pop(pila);

        if (is_final(aux)) return aux;

        List *lista = get_adj_nodes(aux);
        for (aux = first(lista) ; aux != NULL ; aux = next(lista)) {
            push(pila, aux);
        }
        free(lista);
    }
    
    return NULL;
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/