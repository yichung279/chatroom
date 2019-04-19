#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int conn;
    struct node *next;
}n_conn;

typedef struct {
  n_conn *head;
}q_conn;

q_conn *q_conn_new();

void q_conn_free(q_conn *q);

bool q_conn_search(q_conn *q, int conn);

void q_conn_insert(q_conn *q, int conn);

bool q_conn_remove(q_conn *q, int conn);
