#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "q_conn.h"

q_conn *q_conn_new(){
    q_conn *q = malloc(sizeof(q_conn));
    if (q == NULL)
        return NULL;

    q->head = NULL;

    return q;
}

void q_conn_free(q_conn *q){
    if (!q) return;

    n_conn *current = q->head;
    n_conn *tmp;
    while(current){
        tmp = current;
        current = current->next;
        free(tmp);
    };
    free(q);
}

bool q_conn_search(q_conn *q, int conn){
    if (!q) return 0;

    n_conn *current = q->head;
    n_conn *tmp;
    while(current){
        if (current->conn == conn) return 1;
        tmp = current;
        current = current->next;
    };
    return 0;
}

void q_conn_insert(q_conn *q, int conn){
    if (!q || !conn) return;

    if (q_conn_search(q, conn)) return;
    n_conn *n = malloc(sizeof(n_conn));
    if (!n) return;
    n->next = q->head;
    n->conn = conn;

    q->head = n;
}

bool q_conn_remove(q_conn *q, int conn){
    if (!q || !conn) return 0;

    n_conn *current = q->head;
    n_conn *last;
    n_conn *next;
    bool found = 0;
    while(current){
        if (current->conn == conn){
            next = current->next;
            found = 1;
            break;
        }

        last = current;
        current = current->next;
    }
    if (!found) return 0;

    if (last)
        last->next = next;
    else
        q->head = next;
    free(current);
    return 1;
}
