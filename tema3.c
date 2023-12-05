/* Tanase Elena-Ramona 313CC */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <limits.h>

// declarare fisiere
FILE * fin, * fout;

// definire structura pentru nodurile din lista de adiacenta in care
// vom retine graful
typedef struct Node {
  // numarul componentei conexe din care face partul nodul (muchia)
  int no_comp;
  // nodul din care pleaca muchia
  int start;
  // nodul in care se termina muchia
  int end;
  // costul muchiei
  int cost;
  // legatura cu urmatorul nod
  struct Node * next;
} Node;

// functie care verifica daca un string se afla sau nu in matricea
// de cuvinte
int is_word(char matrix[][100], int dim, char name[]) {
  int i;
  if (dim <= -1) return 0;
  for (i = 0; i < dim; i++) {
    if (strcmp(matrix[i], name) == 0) {
      return 1;
    }
  }
  return 0;
}

// functie care retuneaza linia pe care se afla un string in matricea
// de cuvinte
int index_char(char matrix[][100], int dim, char name[]) {
  int i;
  for (i = 0; i < dim; i++) {
    if (strcmp(matrix[i], name) == 0) {
      return i;
    }
  }
  return 0;
}

// functie de creare a unui nod
Node * create_node(int s, int e, int c) {
  Node * new_node = malloc(1 * sizeof(Node));
  new_node -> start = s;
  new_node -> end = e;
  new_node -> cost = c;
  new_node -> no_comp = 0;
  new_node -> next = NULL;
  return new_node;
}

// functie de adaugare a unui nod in lista care retine graful neorientat
void add_node(Node ** list, int source, int destination, int cost) {
  Node * new_node;
  // facem legatura intre nodul destinatie si sursa
  new_node = create_node(source, destination, cost);
  new_node -> next = list[source] -> next;
  list[source] -> next = new_node;
  // facem legatura intre nodul sursa si destinatie
  new_node = create_node(destination, source, cost);
  new_node -> next = list[destination] -> next;
  list[destination] -> next = new_node;
}

// functie de adaugare a unui nod in lista care retine graful orientat
void add_node_2(Node ** list, int source, int destination, int cost) {
  Node * new_node;
  new_node = create_node(source, destination, cost);
  new_node -> next = list[source] -> next;
  list[source] -> next = new_node;
}

// functie care parcurge graful in adancime si actualizeaza campul no_como
// al nodurilor in fucntie de componenta conexa in care se afla
void DFS(Node ** list, int visited[], int index, int n) {
  visited[index] = 1;
  list[index] -> no_comp = n - 1;
  Node * new_node = list[index];
  int neighbor;
  while (new_node != NULL) {
    neighbor = new_node -> end;
    new_node -> no_comp = n - 1;
    if (visited[neighbor] == 0) {
      list[neighbor] -> no_comp = n - 1;
      DFS(list, visited, neighbor, n);
    }
    new_node = new_node -> next;
  }
}

// functie care retuneaza numarul de componente conexe
int conex_components(int N, Node ** list) {
  int * visited = calloc(N, sizeof(int));
  // no = numarul de componente conexe
  // nc = indicele componentei conexe curente
  int i, no = 0, nc = 1;
  for (i = 0; i < N; i++) {
    if (visited[i] == 0) {
      DFS(list, visited, i, nc);
      nc++;
      no++;
    }
  }
  free(visited);
  return no;
}

// functie care parcurge graful in adancime si verifica dace exista drum intre
// 2 noduri date ca argument
int DFS_2(Node ** list, int visited[], int index_s, int index_d) {
  visited[index_s] = 1;
  Node * new_node = list[index_s];
  int neighbor;
  while (new_node != NULL) {
    neighbor = new_node -> end;
    // daca unul dintre vecini este nodul cautat retuneaza 1 = gasit
    if (neighbor == index_d) {
      return 1;
    }
    if (visited[neighbor] == 0) {
      if (DFS_2(list, visited, neighbor, index_d) == 1) {
        return 1;
      }
    }
    new_node = new_node -> next;
  }
  return 0;
}

// functie care retuneaza nodul de cost minim, adiacent cu un nod dat
Node * min_list(int viz[], Node ** list, int node) {
  int min_cost = INT_MAX;
  Node * aux = list[node], * aux_min = NULL;
  // parcurge lista de vecini a unui nod "parinte" si retuneaza
  // nodul vecin de cost minim
  while (aux -> next) {
    aux = aux -> next;
    if (aux -> cost < min_cost && viz[aux -> end] == 0) {
      min_cost = aux -> cost;
      aux_min = aux;
    }
  }
  return aux_min;
}

// functie care calculeaza pentru un graf, suma muchiilor din arborele
// de cost minim
int prim(Node ** list, Node * node, int N_comp) {
  int s = 0;
  int viz[1000] = {0};
  viz[node -> start] = 1;
  // vectorul "tree" retine indicii nodurilor care fac parte din arborele
  // de cost minim
  int tree[1000], dim_tree = -1;
  tree[++dim_tree] = node -> start;
  int i;
  Node * min_node, * aux;
  int min;
  while (dim_tree < N_comp - 1) {
    min = INT_MAX;
    // calculam pentru fiecare nod din arbore, vecinul de cost minim
    for (i = 0; i < dim_tree + 1; i++) {
      aux = min_list(viz, list, tree[i]);
      if (aux != NULL) {
        if (aux -> cost < min) {
          min = aux -> cost;
          min_node = aux;
        }
      }
    }
    // adaugam in arbore vecinul minim, dintre toti vecinii nodurilor
    // deja existente in arbore
    viz[min_node -> end] = 1;
    tree[++dim_tree] = min_node -> end;
    s += min_node -> cost;
  }
  return s;
}

int main(int argc, char const * argv[]) {
  // verificam cerinta care trebuie rezolvata
  if (argv[1][0] == '1') {
    // cerinta 1
    int N, M, i, dim = -1, cost;
    int index_d, index_s;
    char source[100], destination[100], matrix[100][100];
    // deschidem fisierele
    fin = fopen("tema3.in", "r");
    fout = fopen("tema3.out", "w");
    // verificam daca s-au deschis fisierele
    if (fin == NULL) {
      printf("Fin can not be opened\n");
    }
    if (fout == NULL) {
      printf("Fout can not be opened\n");
    }
    // citim  datele din fisier
    fscanf(fin, "%d %d", & N, & M);
    // alocam un vector de liste, unde retinem pentru fiecare nod
    // toate nodurile cu care se invecineaza intr-o lista
    Node ** list = (Node ** ) malloc(N * sizeof(Node * ));
    // primul element din fiecare lista o sa fie un nod care se leaga cu el
    // insusi cu costul 0
    for (i = 0; i < N; i++) {
      list[i] = create_node(i, i, 0);
    }
    // citim sirurile de caractere din fisier si le atribuim un index
    // in functie de linia din matricea de cuvinte unde le salvam
    for (i = 0; i < M; i++) {
      fscanf(fin, "%s %s %d", source, destination, & cost);
      if (is_word(matrix, dim + 1, source) == 0) {
        strcpy(matrix[++dim], source);
      }
      if (is_word(matrix, dim + 1, destination) == 0) {
        strcpy(matrix[++dim], destination);
      }
      index_d = index_char(matrix, dim + 1, destination);
      index_s = index_char(matrix, dim + 1, source);
      add_node(list, index_s, index_d, cost);
    }
    // afisam numarul de componente conexe
    fprintf(fout, "%d\n", conex_components(N, list));
    int cc = conex_components(N, list);
    int x; // nr comp conexe
    Node * aux;
    // calculam cate noduri sunt in fiecare componenta conexa si retinem
    // numarul in vector N_comp
    int N_comp[1000] = {
      0
    };
    for (i = 0; i < N; i++) {
      aux = list[i];
      x = aux -> no_comp;
      N_comp[x]++;
    }
    // calculam costul minim pentru fiecare componenta conexa si retinem
    // rezultatul in vectorul sum
    int sum[100], dim_s = -1, j;
    for (i = 0; i < cc; i++) {
      // parcurgem nodurile si alegem primul nod din componenta conexa i
      // si apelam functia prim pentru acesta
      for (j = 0; j < N; j++)
        if (list[j] -> no_comp == i) {
          sum[++dim_s] = prim(list, list[j], N_comp[i]);
          // iesim din parcurgerea nodurilor, pentru a trece la urmatoarea
          // componenta conexa
          break;
        }
    }
    // sortam vectorul de costuri
    int aux_swap;
    for (i = 0; i < dim_s; i++) {
      for (j = i + 1; j <= dim_s; j++) {
        if (sum[i] > sum[j]) {
          aux_swap = sum[i];
          sum[i] = sum[j];
          sum[j] = aux_swap;
        }
      }
    }
    // afisam vectorul de costuri
    for (i = 0; i <= dim_s; i++) {
      fprintf(fout, "%d\n", sum[i]);
    }
    // eliberare memorie pentru structurile alocate dinamic
    for (i = 0; i < N; i++) {
      Node * node = list[i];
      while (node != NULL) {
        Node * temp = node;
        node = node -> next;
        free(temp);
      }
    }
    free(list);
    fclose(fin);
    fclose(fout);
  } else {
    // cerinta 2
    int N, M, i, dim = -1, cost;
    int index_d, index_s, index_island, index_ship;
    char source[100], destination[100], matrix[100][100];
    // deschidem fisierele
    fin = fopen("tema3.in", "r");
    fout = fopen("tema3.out", "w");
    if (fin == NULL) {
      printf("Fin can not be opened\n");
    }
    if (fout == NULL) {
      printf("Fout can not be opened\n");
    }
    // citim  datele din fisier
    fscanf(fin, "%d %d", & N, & M);
    // alocam un vector de liste, unde retinem pentru fiecare nod
    // toate nodurile cu care se invecineaza
    Node ** list = (Node ** ) malloc(N * sizeof(Node * ));
    // primul element din fiecare lista o sa fie un nod care se leaga cu el
    // insusi cu costul 0
    for (i = 0; i < N; i++) {
      list[i] = create_node(i, i, 0);
    }
    // Node ** array = malloc(M * sizeof(Node));
    // citim sirurile de caractere din fisier si le atribuim un index
    // in functie de linia din matricea de cuvinte unde le salvam
    for (i = 0; i < M; i++) {
      fscanf(fin, "%s %s %d", source, destination, & cost);
      if (is_word(matrix, dim + 1, source) == 0) {
        strcpy(matrix[++dim], source);
      }
      if (is_word(matrix, dim + 1, destination) == 0) {
        strcpy(matrix[++dim], destination);
      }
      index_d = index_char(matrix, dim + 1, destination);
      index_s = index_char(matrix, dim + 1, source);
      // retinem indicele pentru "Corabie" si "Insula"
      if (strstr(source, "Corabie")) {
        index_ship = index_s;
      }
      if (strstr(destination, "Corabie")) {
        index_ship = index_d;
      }
      if (strstr(source, "Insula")) {
        index_island = index_s;
      }
      if (strstr(destination, "Insula")) {
        index_island = index_d;
      }
      add_node_2(list, index_s, index_d, cost);
      // array[++poz] = create_node(index_s, index_d, cost);
    }
    // alocam un vector in care marcam nodurile vizitate
    int * visited = calloc(N, sizeof(int));
    // verificam daca exista drum intre Corabie si Insula
    if (DFS_2(list, visited, index_ship, index_island) == 0) {
      fprintf(fout, "Echipajul nu poate ajunge la insula\n");
    }
    // reinitializam vectorul de vizitat cu 0
    for (i = 0; i < N; i++) {
      visited[i] = 0;
    }
    // verificam daca exista drum intre Insula si Corabie
    if (DFS_2(list, visited, index_island, index_ship) == 0) {
      fprintf(fout,
            "Echipajul nu poate transporta comoara inapoi la corabie\n");

    }
    // eliberare memorie pentru structurile alocate dinamic
    for (i = 0; i < N; i++) {
      Node * node = list[i];
      while (node != NULL) {
        Node * temp = node;
        node = node -> next;
        free(temp);
      }
    }
    free(list);
    // for (int i = 0; i < M; i++) {
    //   free(array[i]);
    // }
    // free(array);
    fclose(fin);
    fclose(fout);
    free(visited);
  }
}