//
//  main.c
//  API Project
//
//  Created by Matteo Bresciani on 17/07/18.
//  Copyright © 2018 Matteo Bresciani. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include <time.h>

#define ALLOC_TUPLE (tuple_t *)malloc(sizeof(tuple_t))
#define ALLOC_ELEMENT (list_tuple_t *)malloc(sizeof(list_tuple_t))

#define ALLOC_QUEUE (queue_t *) malloc(sizeof(queue_t))



typedef struct tuple_s  {
    int curr_state;
    char toGet;
    char toSet;
    int move;
    int next_state;
    struct tuple_s *next_bro;
    struct tuple_s *f_child;
}   tuple_t;


typedef struct queue_s{
    tuple_t *info;
    char *tape;
    int i;
    int count;

    struct queue_s *next;

} queue_t;

typedef struct list_tuple_s{
    tuple_t info;
    struct list_tuple_s *next;

} list_tuple_t;


int  check(int acc[], int j);
void compute(tuple_t *tmp, char tape[]);
void Dequeue(queue_t ** head);
void Enqueue(tuple_t *tmp, queue_t **head, int count, int i, const char string[], int len, int index, char toWrite);

void re_insert_tuple(tuple_t tmp, tuple_t **pmt, tuple_t *a);


int length(char string[]);

void insert_order(tuple_t tmp, list_tuple_t ** head);

void find_child(list_tuple_t *head, list_tuple_t *elem);

int zz =0;
int tot = 0;
int acc[10];
long int max = 0;
tuple_t *root = NULL;
queue_t * b= NULL;
queue_t *used = NULL;

int main(int argc, const char *argv[]) {

    int z = 0;
    char * a;
    list_tuple_t *p;
    list_tuple_t *list_state = NULL;
    char k;
    char tape[1024];
    char input[2048];
    tuple_t tmp;
    int i = 0;
    tmp.f_child = NULL;
    tmp.next_bro = NULL;

    a = fgets(input, 512, stdin);


    while (a != NULL) {
        strtok(input, "\n");

        if (strcmp(input, "tr") == 0) {
            scanf( "%s", input);


            while (strcmp(input, "acc") != 0) {

                tmp.curr_state = atoi(input);

                fscanf(stdin,  " %c %c %c %d", &tmp.toGet, &tmp.toSet, &k, &tmp.next_state);

                if (k == 'R')
                    tmp.move = 1;
                else if (k == 'L')
                    tmp.move = -1;
                else if (k == 'S')
                    tmp.move = 0;



                insert_order(tmp, &list_state);

                fscanf(stdin, "%s", input);
                z++;


            }


            p = list_state;

            while(p->next!= NULL) {
                if (p->info.curr_state == p->next->info.curr_state) {
                    p->info.next_bro = &(p->next->info);
                }
                p = p->next;

            }

            p = list_state;
            while(p!=NULL){
                find_child(list_state, p);
                p = p->next;
            }

            fscanf(stdin, "%s", input);
            for (i = 0; strcmp(input, "max") != 0; i++) {
                acc[i] = atoi(input);
                fscanf(stdin, "%s", input);

            }
            fscanf(stdin, "%li", &max);
            fscanf(stdin, "%s", input);

        } else if (strcmp(input, "run") == 0) {

            for (i = 0; i < 1; i++)
                tape[i] = '_';
            a =  fgets(input, 2048, stdin);
            a =  fgets(input, 2048, stdin);

            strtok(input, "\n");
            for (int j = 0; input[j] != '\0'; j++, i++)
                tape[i] = input[j];

            for (int j = 0; j <1; j++, i++)
                tape[i] = '_';

            tape[i]= '\0';
            compute(&list_state->info, tape);

            tot=0;

            fgets(input, 2048, stdin);
            strtok(input, "\n");

        } else {
            for (i = 0; i < 1; i++)
                tape[i] = '_';
            for (int j = 0; input[j] != '\0'; j++, i++) {
                if (input[j] == '\n')
                    break;
                tape[i] = input[j];
            }

            for (int j = 0; j < 1; j++, i++)
                tape[i] = '_';

            compute(&list_state->info, tape);
            tot=0;
            memset(tape, 0, sizeof(tape));


            a = fgets(input, 2048, stdin);
            strtok(input, "\n");
        }


    }


    return 0;
}



tuple_t * search(tuple_t *head, int state){
    tuple_t *tmp;

    if(state == 0 && head ->curr_state == 0  && head -> f_child != NULL && head->next_bro != NULL)
        return head;
    if(state == head ->next_state && head -> f_child != NULL){
        return head->f_child;

    }
    if(head -> f_child != NULL) {
        tmp = search(head->f_child, state);
        if(tmp != NULL)
            return tmp;

    }

    if(head->next_bro != NULL) {
        tmp =  search(head->next_bro, state);
        if(tmp != NULL)
            return tmp;
    }

    return NULL;
}


void compute(tuple_t *tmp, char tape[]) {

    struct queue_s *open = NULL;
    int j;
    queue_t * q;
    tuple_t *a = tmp;
    tuple_t *b;
    char new_ch;
    int new_pos;
    int new_count;


    Enqueue(a, &open, 0, 1, tape, length(tape), -1, 'c');


    while (open != NULL) {

        if (open->tape[open->i] == open->info->toGet) {
            new_pos = open->info->move + open->i;
            new_ch = open->info->toSet;
            new_count = open->count + 1;

            if (new_count > max) {
                while (open != NULL) {
                    q = open;
                    open = open->next;
                    free(q);
                }
                printf("U\n");
                return;
            }


                b = open->info->f_child;
                if (b == NULL) {

                    if (check(acc, open->info->next_state) == 1) {


                        while (open != NULL) {
                            q = open;
                            open = open->next;
                            free(q);
                        }
                        printf("1\n");

                        return;
                    }



                }

            j = length(open->tape);
            if (b != NULL) {
                Enqueue(b, &open, new_count, new_pos, open->tape, j, open->i, new_ch);
            }
            if (open->info->next_bro == NULL) {
                Dequeue(&open);
            }
            else open ->info = open->info ->next_bro;
        }
        else if(open->info->next_bro == NULL)
            Dequeue(&open);
        else open->info = open->info ->next_bro;

    }

    printf("0\n");

}


int check(int acc[], int j) {
    for (int i = 0; i < sizeof(acc); i++) {
        if (acc[i] == 0)
            break;
        if (acc[i] == j)
            return 1;
    }

    return 0;
}

void insert_on_tail(tuple_t tmp, tuple_t ** head){
    tuple_t * new;

    tuple_t *b;

    tuple_t * a = *head;


    if(a->f_child == NULL) {
        new= ALLOC_TUPLE;
        new->curr_state = tmp.curr_state;
        new-> next_state = tmp.next_state;
        new->move= tmp.move;
        new -> toSet = tmp.toSet;
        new->toGet = tmp.toGet;
        new->next_bro = NULL;
        new ->f_child = NULL;
        a->f_child = new;
        return;
    }

    b = a->f_child;

    if (tmp.curr_state == b->curr_state && tmp.next_state == b->next_state &&
        tmp.move == b->move &&
        tmp.toSet == b->toSet && tmp.toGet == b->toGet)
        return;

    while(b->next_bro != NULL){
        if (tmp.curr_state == b->curr_state && tmp.next_state == b->next_state &&
            tmp.move == b->move &&
            tmp.toSet == b->toSet && tmp.toGet == b->toGet)
            return;
        b = b->next_bro;
    }

    if (tmp.curr_state == b->curr_state && tmp.next_state == b->next_state &&
        tmp.move == b->move &&
        tmp.toSet == b->toSet && tmp.toGet == b->toGet)
        return;
    new= ALLOC_TUPLE;
    new->curr_state = tmp.curr_state;
    new-> next_state = tmp.next_state;
    new->move= tmp.move;
    new -> toSet = tmp.toSet;
    new->toGet = tmp.toGet;
    new->next_bro = NULL;
    new ->f_child = NULL;
    b->next_bro = new;
}

void re_insert_tuple(tuple_t tmp, tuple_t **pmt, tuple_t *a) {


    if(tot > 3000)
        return;
    if (a->next_state == tmp.curr_state && a->curr_state != a->next_state)
        insert_on_tail(tmp, &a);

    if (a->next_bro == NULL && a->f_child == NULL)
        return;

    if (a->next_bro != NULL) {
        tot++;
        re_insert_tuple(tmp, pmt, a->next_bro);
    }

    if (a->f_child != NULL) {
        tot++;
        re_insert_tuple(tmp, pmt, a->f_child);

    }

}


void Enqueue(tuple_t *tmp, queue_t **head, int count, int i, const char string[], int len, int index, char toWrite) {

    queue_t *c;
    queue_t *a = *head;
    int j = 0;
    int y = 0;

    if (a == NULL) {


        a = ALLOC_QUEUE;

        a->info = tmp;
        a->i = i;
        a->count = count;
        a->next = NULL;
        a->tape = NULL;
        if (string[0] != '_') {
            len = len + 1;

            a->tape = (char *) malloc((len + 1) * sizeof(char));

            a->tape[0] = '_';

            for (j = 1; string[y] != '\0'; j++, y++)
                a->tape[j] = string[y];
            a->tape[j] = '\0';
            a->i = 0;

        } else if (string[len - 1] != '_') {
            len = len + 1;
            a->tape = (char *) malloc((len + 1) * sizeof(char));

            for (j = 0; string[j] != '\0'; j++)
                a->tape[j] = string[j];
            a->tape[j] = '_';
            a->tape[j + 1] = '\0';

        } else {
            a->tape = (char *) malloc((len + 1) * sizeof(char));


            for (j = 0; string[j] != '\0'; j++) {
                a->tape[j] = string[j];

            }
            a->tape[len] = '\0';
        }

        *head = a;
        b = a;
        return;
    }

    if (used == NULL) {
        b->next = ALLOC_QUEUE;
        b->next->info = tmp;
        b->next->tape = NULL;
        b->next->count = count;
        b->next->i = i;
        b->next->next = NULL;
    }
    else {
        c = used;
        used = used->next;
        b->next = c;
        b->next->info = tmp;
        b->next->count = count;
        b->next->i = i;
        b->next->next = NULL;
    }
    if (string[0] != '_') {
        len = len + 1;
        b->next->tape = (char *) malloc((len + 1) * sizeof(char));

        b->next->tape[0] = '_';

        for (j = 1, y = 0; string[y] != '\0'; j++, y++)
            b->next->tape[j] = string[y];
        b->next->tape[index+1]= toWrite;
        b->next->tape[j] = '\0';
        if (i < 0)
            b->next->i = 0;

    } else if (string[len - 1] != '_') {
        len = len + 1;

        b->next->tape = (char *) malloc((len + 1) * sizeof(char));

        for (j = 0; string[j] != '\0'; j++)
            b->next->tape[j] = string[j];
        b->next->tape[index]= toWrite;

        b->next->tape[j] = '_';
        b->next->tape[j + 1] = '\0';

    } else {

        b->next->tape = (char *) malloc((len + 1) * sizeof(char));
        for (j = 0; string[j] != '\0'; j++) {
            b->next->tape[j] = string[j];
            b->next->tape[index]= toWrite;


        }
        b->next->tape[len] = '\0';
    }
    b = b->next;
}
void Dequeue(queue_t ** head) {

    queue_t *c;
    if(*head != NULL) {

        queue_t *b = *head;
        queue_t *a;

        a = b;
        *head = b->next;
        free(a->tape);

        c = used;
        a ->next = c;
        used = a;

    }

}


int length(char string[]){
    int i = 0;
    while(string[i] != '\0'){
        i++;
    }
    return i;
}


void insert_order(tuple_t tmp, list_tuple_t ** head){
    list_tuple_t * new;

    list_tuple_t *b, *a;



    if(*head == NULL) {
        new= ALLOC_ELEMENT;
        new->info = tmp;
        new->next=NULL;
        *head = new;
        return;
    }


    if((*head)->info.curr_state >= tmp.curr_state){
        b = *head;
        new= ALLOC_ELEMENT;
        new->info = tmp;
        new->next=b;
        *head = new;

    }

    b = *head;

    if (tmp.curr_state == b->info.curr_state && tmp.next_state == b->info.next_state &&
        tmp.move == b->info.move &&
        tmp.toSet == b->info.toSet && tmp.toGet == b->info.toGet)
        return;

    while(b->next != NULL ){
        if (tmp.curr_state == b->info.curr_state && tmp.next_state == b->info.next_state &&
            tmp.move == b->info.move &&
            tmp.toSet == b->info.toSet && tmp.toGet == b->info.toGet)
            return;
        if(b->next->info.curr_state > tmp.curr_state){
            a = b->next;
            b->next = ALLOC_ELEMENT;
            b->next->info = tmp;
            b->next->next = a;
            return;
        }
        b = b->next;
    }

    if (tmp.curr_state == b->info.curr_state && tmp.next_state == b->info.next_state &&
        tmp.move == b->info.move &&
        tmp.toSet == b->info.toSet && tmp.toGet == b->info.toGet)
        return;
    new= ALLOC_ELEMENT;
    new->info = tmp;
    new->next = NULL;
    b->next = new;
}



void find_child(list_tuple_t *head, list_tuple_t *elem){
    list_tuple_t *p;
    p = head;
    while(p!= NULL) {
        if(p->info.curr_state == elem->info.next_state) {
            elem->info.f_child = &p->info;
            return;
        }   else p = p->next;
    }
}



