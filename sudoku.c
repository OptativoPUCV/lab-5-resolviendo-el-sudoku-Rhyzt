#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <sys/time.h>


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

int enFila(int sudo[9][9], int x, int cand, int posy) {// 0 no existe, 1 existe
    int array[10] = {0};
    for (int n = 0 ; n < 9 ; n++) {
        array[sudo[x][n]]++;
        if (array[sudo[x][n]] > 1 && sudo[x][n] != 0)
            return 1;
    }
    return 0;
}

int enColumna(int sudo[9][9], int y, int cand, int posx) {// 0 no existe, 1 existe
    int array[10] = {0};
    for (int n = 0 ; n < 9 ; n++) {
        array[sudo[n][y]]++;
        if (array[sudo[n][y]] > 1 && sudo[n][y] != 0)
            return 1;
    }
    return 0;
}

int enSubMatriz(int sudo[9][9], int x, int y, int cand) { // 0 no existe, 1 existe
    int k = 3 * (y/3) + (x/3), p;
    int array[10] = {0};
    for(p=0;p<9;p++) {
        int i=3*(k/3) + (p/3) ;
        int j=3*(k%3) + (p%3) ;
        array[sudo[i][j]]++;
        if(array[sudo[i][j]] > 1 && sudo[i][j] != 0) {
            return 1;
        }
    }
    return 0;
}

int is_valid(Node* n){
    for (int i = 0 ; i < 9 ; i++) {
        for (int k = 0 ; k < 9 ; k++) { // Recorrer todo el sudoku
            if (n -> sudo[i][k] != 0) { // Para cada elemento distinto de 0
                if (enFila(n -> sudo, i, n -> sudo[i][k], k))
                    return 0;
                if (enColumna(n -> sudo, k, n -> sudo[i][k], i))
                    return 0;
                if (enSubMatriz(n -> sudo, i, k, n -> sudo[i][k]))
                    return 0;
            }
        }
    }
    return 1;
}

List* get_adj_nodes(Node* n) {
    List *list = createList();
    int flag = 1;
    for (int i = 0 ; i < 9 ; i++) {
        for (int k = 0 ; k < 9 ; k++) { // Recorrer todo el sudoku
            if (n -> sudo[i][k] == 0) { // Encontrar casilla vacia
                for (int cand = 1 ; cand <= 9 ; cand++) { // Itera cada posible estado
                    n -> sudo[i][k] = cand;
                    if (is_valid(n)) {
                        Node *copia = copy(n);
                        n -> sudo[i][k] = 0;
                        pushBack(list, copia);
                        flag = 0;
                    }
                    else
                        n -> sudo[i][k] = 0;
                }
                break;
            }
        }
        if (!flag) break;
    }
    return list;
}


int is_final(Node* n) {
    List *lista = get_adj_nodes(n);
    if (first(lista) == NULL) {
        for (int i = 0 ; i < 9 ; i++)
            for (int k = 0 ; k < 9 ; k++)
                if (n -> sudo[i][k] == 0) {
                    clean(lista);
                    free(lista);
                    return 0;
                }
        clean(lista);
        free(lista);
        return 1;
    }
    clean(lista);
    free(lista);
    return 0;
}

Node *DFS(Node* initial, int* cont) {
    if (!is_valid(initial))
        return NULL;
    Stack *pila = createStack();
    push(pila, initial);
    while (top(pila) != NULL) {
        Node *aux = top(pila);
        pop(pila);

        if (is_final(aux)) {
            clean(pila);
            free(pila);
            return aux;
        }

        List *adj = get_adj_nodes(aux);
        Node *node = first(adj);

        while (node) {
            push(pila, node);
            node = next(adj);
        }
        free(aux);
        (*cont)++;
    }
    free(pila);
    return NULL;
}


/*
int main( int argc, char *argv[] ){
    struct timeval inicio, fin;
    long segundos, microsegundos;
    double tiempoTranscurrido;
    

    Node* initial= read_file("s12a.txt");;

    int cont=0;
    gettimeofday(&inicio, NULL);
    Node* final = DFS(initial, &cont);
    gettimeofday(&fin, NULL);
    segundos = fin.tv_sec - inicio.tv_sec;
    microsegundos = fin.tv_usec - inicio.tv_usec;
    tiempoTranscurrido = segundos + microsegundos*1e-6;
    printf("iterations:%d\n",cont);
    printf("Tiempo transcurrido: %.6f segundos\n", tiempoTranscurrido);
    if (final)
        print_node(final);

    return 0;
} 
*/