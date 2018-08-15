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
    char tape[200];
    int i;
    int count;

} queue_t;


int  check(int acc[], int j);
void compute(tuple_t **tmp, char tape[]);
tuple_t *insert_tuple(tuple_t tmp, tuple_t *pmt, tuple_t *a);
void Dequeue(tuple_t *tmp, queue_t head[]);
void Enqueue(tuple_t *tmp, queue_t head[], int count, int i, char string[]);
void insertionSort(tuple_t arr[]);
void re_insert_tuple(tuple_t tmp, tuple_t **pmt, tuple_t *a);

int tot = 0;
int max_state = 0;
int acc[5];
int max = 0 ;
int res = 0;
tuple_t *root = NULL;

int main(int argc, const char *argv[]) {

    int f = 0;
    int z = 0;
    char * a;
    struct tuple_s list_state[200];
    char k;
    char tape[200];
    char blank[30];
    char input[200];
    tuple_t tmp;
    int i = 0;
    tmp.f_child = NULL;
    tmp.next_bro = NULL;
    tmp.first_bro = NULL;

    a = fgets(input, 512, stdin);

    for (int i = 0; i < 9; i++)
        blank[i] = '_';

    while (a != NULL) {
        strtok(input, "\n");

        if (strcmp(input, "tr") == 0) {
            scanf( "%s", input);


            while (strcmp(input, "acc") != 0) {

                tmp.curr_state = atoi(input);
                if (max_state < i)
                    max_state = i;
                fscanf(stdin,  " %c %c %c %d", &tmp.toGet, &tmp.toSet, &k, &tmp.next_state);

                if (k == 'R')
                    tmp.move = 1;
                else if (k == 'L')
                    tmp.move = -1;
                else if (k == 'S')
                    tmp.move = 0;


                list_state[z].curr_state = tmp.curr_state;
                list_state[z].next_state = tmp.next_state;
                list_state[z].toGet = tmp.toGet;
                list_state[z].toSet = tmp.toSet;
                list_state[z].move = tmp.move;
                list_state[z].next_bro = NULL;
                list_state[z].f_child = NULL;
                list_state[z].first_bro = NULL;
                //  list_state = insert_tail(list_state, tmp);


                root = insert_tuple(tmp, root, root);

                fscanf(stdin, "%s", input);
                z++;


            }
            fscanf(stdin, "%s", input);
            for (i = 0; strcmp(input, "max") != 0; i++) {
                acc[i] = atoi(input);
                fscanf(stdin, "%s", input);

            }
            fscanf(stdin, "%d", &max);
            fscanf(stdin, "%s", input);

        } else if (strcmp(input, "run") == 0) {
            insertionSort(list_state);

            while(f < 7){
                for(int j = 0; j <z; j++){
                    re_insert_tuple(list_state[j], &root, root);

                }

                f++;
            }


            for (i = 0; i < 10; i++)
                tape[i] = '_';
            a =  fgets(blank, 512, stdin);
            a =  fgets(blank, 512, stdin);

            strtok(blank, "\n");
            for (int j = 0; blank[j] != '\0'; j++, i++)
                tape[i] = blank[j];

            for (int j = 0; j < 10; j++, i++)
                tape[i] = '_';


            compute(&root, tape);
            tot=0;

            fgets(input, 512, stdin);
            strtok(input, "\n");
            res = 0;

        } else {
            for (i = 0; i < 10; i++)
                tape[i] = '_';
            for (int j = 0; input[j] != '\0'; j++, i++) {
                if (input[j] == '\n')
                    break;
                tape[i] = input[j];
            }

            for (int j = 0; j < 10; j++, i++)
                tape[i] = '_';

            compute(&root, tape);
            tot=0;



            res = 0;


            a = fgets(input, 512, stdin);
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

    struct queue_s open[200];
    int j;
    tuple_t * a = *tmp;
    tuple_t *b;

    for(j = 0; j< 200; j++) {
        open[j].i = 0;
        open[j].count = 0;
    }
    while(a!= NULL){
        Enqueue(a, open, 0 , 10, tape);
        a = a->next_bro;
    }

    while(open[0].i != 0){
        if(open[0].count >max || tot > 70000) {
            for(j = 0; j< 200; j++) {
                open[j].i = 0;
                open[j].count = 0;

            }
            printf("U\n");
            return;
        }
        if(open[0].tape[open[0].i] == open[0].info->toGet){


            open[0].tape[open[0].i] = open[0].info->toSet;
            tot++;
            open[0].count++;
            open[0].i = open[0].info->move + open[0].i;


            if (check(acc, open[0].info->next_state) == 1) {
                for(j = 0; j< 200; j++) {
                    open[j].i = 0;
                    open[j].count = 0;
                }
                    printf("1\n");
                return;
            }
            if(open[0].info->next_state == open[0].info->curr_state)
                b = open[0].info->first_bro;
            else {
               b = open[0].info->f_child;
               if(b == NULL)
                   b = search(root, open[0].info->next_state);

            }
            while(b!= NULL){
                Enqueue(b, open, open[0].count, open[0].i, open[0].tape);
                b = b->next_bro;
            }
        }
        Dequeue(open[0].info, open);



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


    if (a->next_state == tmp.curr_state && a->curr_state != a->next_state)
        insert_on_tail(tmp, &a);

    if (a->next_bro == NULL && a->f_child == NULL)
        return;

    if (a->next_bro != NULL) {
        re_insert_tuple(tmp, pmt, a->next_bro);
    }

    if (a->f_child != NULL) {
       re_insert_tuple(tmp, pmt, a->f_child);

    }

}


void insertionSort(tuple_t arr[])
{
    int i, j;
    tuple_t key;
    for (i = 1; i < sizeof(arr); i++)
    {
        key = arr[i];
        j = i-1;


        while (j >= 0 && arr[j].curr_state > key.curr_state)
        {
            arr[j+1] = arr[j];
            j = j-1;
        }
        arr[j+1] = key;
    }
}

void Enqueue(tuple_t *tmp, queue_t head[], int count, int i, char string[]) {

    int j=0;

    while(head[j].i !=0)
        j++;
    head[j].i = i;
    head[j].count = count;
    head[j].info = tmp;
    for( int k = 0; string[k] != '\0'; k++)
        head[j].tape[k] = string[k];

    return;


}


void Dequeue(tuple_t *tmp, queue_t head[]){

    int k;

    for(k = 0; head[k].i != 0; k++) {
        head[k] = head[k + 1];
    }
}


void insert_middle(tuple_t *tmp, queue_t head[], int count, int i, char string[]) {

    int j=0;

    queue_t elem;
    head[1].i = i;
    head[j].count = count;
    head[j].info = tmp;
    for( int k = 0; string[k] != '\0'; k++)
        head[j].tape[k] = string[k];

    

    return;


}