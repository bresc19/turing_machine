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
    struct tuple_s *first_bro;
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
void compute(tuple_t **tmp, char tape[]);
tuple_t *insert_tuple(tuple_t tmp, tuple_t *pmt, tuple_t *a);
void Dequeue(queue_t ** head);
void Enqueue(tuple_t *tmp, queue_t **head, int count, int i, const char string[], int len);

void re_insert_tuple(tuple_t tmp, tuple_t **pmt, tuple_t *a);


int length(char string[]);

void insert_order(tuple_t tmp, list_tuple_t ** head);

int tot = 0;
int max_state = 0;
int acc[20];
int max = 0 ;
tuple_t *root = NULL;
queue_t * b= NULL;

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
    tmp.first_bro = NULL;

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
            for(p = list_state; p != NULL; p = p->next) {
                root = insert_tuple(p->info, root, root);
            }

            while(list_state != NULL){
                p = list_state;
                list_state = list_state ->next;
                free(p);
            }

            fscanf(stdin, "%s", input);
            for (i = 0; strcmp(input, "max") != 0; i++) {
                acc[i] = atoi(input);
                fscanf(stdin, "%s", input);

            }
            fscanf(stdin, "%d", &max);
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

            compute(&root, tape);

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

            compute(&root, tape);
            tot=0;
            memset(tape, 0, sizeof(tape));


            a = fgets(input, 2048, stdin);
            strtok(input, "\n");
        }


    }


    return 0;
}

tuple_t *insert_tuple(tuple_t tmp, tuple_t *pmt, tuple_t *a) {

    int flag = 0;
    tuple_t * first;
    if (pmt == NULL ) {
        if(tmp.curr_state != 0)
            return NULL;
        pmt = ALLOC_TUPLE;
        pmt->next_state = tmp.next_state;
        pmt->toGet = tmp.toGet;
        pmt->toSet = tmp.toSet;
        pmt->move = tmp.move;
        pmt->curr_state = tmp.curr_state;
        pmt->next_bro = NULL;
        pmt->f_child = NULL;
        pmt -> first_bro = pmt;
        return pmt;
    } else {
        if (a->curr_state == tmp.curr_state) {
            if (tmp.curr_state != tmp.next_state) {
                if (tmp.curr_state == a->curr_state && tmp.next_state == a->next_state && tmp.move == a->move &&
                    tmp.toSet == a->toSet && tmp.toGet == a->toGet) {
                    flag = 1;
                }
                first = a;
                while (a->next_bro != NULL) {
                    a = a->next_bro;
                    if (tmp.curr_state == a->curr_state && tmp.next_state == a->next_state &&
                        tmp.move == a->move &&
                        tmp.toSet == a->toSet && tmp.toGet == a->toGet) {
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    a->next_bro = ALLOC_TUPLE;
                    a->next_bro->next_state = tmp.next_state;
                    a->next_bro->toGet = tmp.toGet;
                    a->next_bro->toSet = tmp.toSet;
                    a->next_bro->move = tmp.move;
                    a->next_bro->curr_state = tmp.curr_state;
                    a->next_bro->next_bro = NULL;
                    a->next_bro->f_child = NULL;
                    a->next_bro ->first_bro = first;

                    a = a->next_bro;

                }

            } else {
                if (tmp.curr_state == a->curr_state && tmp.next_state == a->next_state && tmp.move == a->move &&
                    tmp.toSet == a->toSet && tmp.toGet == a->toGet)
                    flag = 1;

                first = a;

                while (a->next_bro != NULL) {
                    if (tmp.curr_state == a->curr_state && tmp.next_state == a->next_state &&
                        tmp.move == a->move &&
                        tmp.toSet == a->toSet && tmp.toGet == a->toGet)
                        flag = 1;

                    a = a->next_bro;
                    if (tmp.curr_state == a->curr_state && tmp.next_state == a->next_state &&
                        tmp.move == a->move &&
                        tmp.toSet == a->toSet && tmp.toGet == a->toGet)
                        flag = 1;
                }
                if (flag == 0) {
                    a->next_bro = ALLOC_TUPLE;
                    a->next_bro ->first_bro = first;
                    a->next_bro->next_state = tmp.next_state;
                    a->next_bro->toGet = tmp.toGet;
                    a->next_bro->toSet = tmp.toSet;
                    a->next_bro->move = tmp.move;
                    a->next_bro->curr_state = tmp.curr_state;
                    a->next_bro->next_bro = NULL;
                    a->next_bro->f_child = NULL;
                    a = a->next_bro;

                }

            }

        }


        if (a->next_state == tmp.curr_state) {
            if (a->f_child == NULL) {

                a->f_child = ALLOC_TUPLE;
                a->f_child->next_state = tmp.next_state;
                a->f_child->toGet = tmp.toGet;
                a->f_child->toSet = tmp.toSet;
                a->f_child->move = tmp.move;
                a->f_child->curr_state = tmp.curr_state;
                a->f_child->next_bro = NULL;
                a->f_child->f_child = NULL;
                a -> f_child ->first_bro = a ->f_child;
                a = a->f_child;

            } else {
                a = a->f_child;
                if (tmp.curr_state != tmp.next_state) {
                    if (tmp.curr_state == a->curr_state && tmp.next_state == a->next_state &&
                        tmp.move == a->move &&
                        tmp.toSet == a->toSet && tmp.toGet == a->toGet)
                        flag = 1;
                    first = a;
                    while (a->next_bro != NULL) {
                        a = a->next_bro;
                        if (tmp.curr_state == a->curr_state && tmp.next_state == a->next_state &&
                            tmp.move == a->move &&
                            tmp.toSet == a->toSet && tmp.toGet == a->toGet)
                            flag = 1;
                    }
                    if (flag == 0) {
                        a->next_bro = ALLOC_TUPLE;
                        a->next_bro->next_state = tmp.next_state;
                        a->next_bro->toGet = tmp.toGet;
                        a->next_bro->toSet = tmp.toSet;
                        a->next_bro->move = tmp.move;
                        a->next_bro->curr_state = tmp.curr_state;
                        a->next_bro->next_bro = NULL;
                        a->next_bro->f_child = NULL;
                        a->next_bro ->first_bro = first;
                        a = a->next_bro;

                    }

                } else {
                    if (tmp.curr_state == a->curr_state && tmp.next_state == a->next_state &&
                        tmp.move == a->move &&
                        tmp.toSet == a->toSet && tmp.toGet == a->toGet)
                        flag = 1;
                    first = a;
                    while (a->next_bro != NULL) {
                        if (tmp.curr_state == a->curr_state && tmp.next_state == a->next_state &&
                            tmp.move == a->move &&
                            tmp.toSet == a->toSet && tmp.toGet == a->toGet)
                            flag = 1;
                        a = a->next_bro;

                    }
                    if (flag == 0) {
                        a->next_bro = ALLOC_TUPLE;
                        a->next_bro->next_state = tmp.next_state;
                        a->next_bro->toGet = tmp.toGet;
                        a->next_bro->toSet = tmp.toSet;
                        a->next_bro->move = tmp.move;
                        a->next_bro->curr_state = tmp.curr_state;
                        a->next_bro->next_bro = NULL;
                        a->next_bro->f_child = NULL;
                        a->next_bro ->first_bro = first;

                        a = a->next_bro;
                    }


                }
            }
        }
    }

    if (a->next_bro == NULL && a->f_child == NULL)
        return pmt;


    if (a->next_bro != NULL) {
        pmt = insert_tuple(tmp, pmt, a->next_bro);
    }

    if (a->f_child != NULL) {
        pmt = insert_tuple(tmp, pmt, a->f_child);

    }


    return pmt;
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


void compute(tuple_t **tmp, char tape[]) {


    struct queue_s *open = NULL;
    int j;
    queue_t * q;
    tuple_t *a = *tmp;
    tuple_t *b;


    while (a != NULL) {
        if (tape[1] == a->toGet)
            Enqueue(a, &open, 0, 1, tape, length(tape));
        a = a->next_bro;
    }

    while (open != NULL) {


        if (open->count > max ) {
            while(open != NULL) {
                q = open;
                open = open ->next;
                free(q);
            }
            printf("U\n");
            return;
        }

        if (open->tape[open->i] == open->info->toGet) {
            open->tape[open->i] = open->info->toSet;
            open->count++;
            tot++;
            open->i = open->info->move + open->i;


            if (check(acc, open->info->next_state) == 1) {
                while(open != NULL) {
                    q = open;
                    open = open ->next;
                    free(q);
                }
                printf("1\n");
                return;
            }
            if (open->info->next_state == open->info->curr_state)
                b = open->info->first_bro;
            else {
                b = open->info->f_child;
                if (b == NULL) {
                    b = search(root, open->info->next_state);
                }
            }
            j = length(open->tape);
            while (b != NULL) {
                if (open->tape[open->i] == b->toGet|| open->tape[0] != '_' || open->tape[j-1] != '_' )
                    Enqueue(b, &open, open->count, open->i, open->tape, j);
                b = b->next_bro;
            }

        }
        Dequeue(&open);

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
        new ->first_bro = NULL;
        new ->f_child = NULL;
        a->f_child = new;
        a->f_child->first_bro = a->f_child;
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
    new ->first_bro = NULL;
    new ->f_child = NULL;
    b->next_bro = new;
    b->next_bro ->first_bro = a->f_child;
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


void Enqueue(tuple_t *tmp, queue_t **head, int count, int i, const char string[], int len) {


    queue_t *a = *head;
    int j = 0;
    int y= 0;

    if (a == NULL ) {
        a = ALLOC_QUEUE;

        a->info = tmp;
        a->i = i;
        a->count = count;
        a->next = NULL;
        a->tape = NULL;
        if (string[0] != '_') {
            len = len + 1;
            a->tape = (char *) malloc((len+1) * sizeof(char));

            a->tape[0] = '_';

            for (j = 1; string[y]!= '\0' ; j++, y++)
                a->tape[j] = string[y];
            a->tape[j] = '\0';
            a ->i = 0;

        } else if (string[len - 1] != '_') {
            len = len + 1;
            a->tape = (char *) malloc((len+1) * sizeof(char));

            for (j = 0; string[j] != '\0'; j++)
                a->tape[j] = string[j];
            a->tape[j] = '_';
            a->tape[j+1] = '\0';

        } else {
            a->tape = (char *) malloc((len+1) * sizeof(char));

            for (j = 0; string[j] != '\0'; j++) {
                a->tape[j] = string[j];

            }
            a->tape[len]= '\0';
        }

        *head = a;
        b = a;
        return;
    }


    b->next = ALLOC_QUEUE;
    b->next->info = tmp;
    b->next->tape = NULL;
    b->next->count = count;
    b->next->i = i;
    b->next->next = NULL;
    if (string[0] != '_') {
        len = len + 1;
        b->next->tape = (char *) malloc((len+1) * sizeof(char));

        b->next->tape[0] = '_';

        for (j = 1, y = 0; string[y]!= '\0' ; j++, y++)
            b->next->tape[j] = string[y];
        b->next->tape[j] = '\0';
        if(i <0)
            b->next ->i = 0;
    } else if (string[len - 1] != '_') {
        len = len + 1;
        b->next->tape = (char *) malloc((len+1) * sizeof(char));
        for (j = 0; string[j] != '\0'; j++)
            b->next->tape[j] = string[j];
        b->next->tape[j] = '_';
        b->next->tape[j+1] = '\0';

    }
    else {
        b->next->tape = (char *) malloc((len+1) * sizeof(char));

        for (j = 0; string[j] != '\0'; j++) {
            b->next->tape[j] = string[j];

        }
        b->next->tape[len]= '\0';
    }
    b = b->next;
}

void Dequeue(queue_t ** head) {

    queue_t *b = *head;
    queue_t *a;

    a = b;
    *head = b->next;
    free(a->tape);
    free(a);

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