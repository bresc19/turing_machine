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

#define ALLOC_TUPLE (tuple_t *)calloc(1, sizeof(tuple_t))
#define ALLOC_QUEUE (queue_t *) calloc(1, sizeof(queue_t))




typedef struct tuple_s  {
    int curr_state;
    char toGet;
    char toSet;
    int move;
    int next_state;
    char tape[100];
    int i;
    int count;
    struct tuple_s *next_bro;
    struct tuple_s *f_child;
    struct tuple_s *first_bro;
}   tuple_t;


typedef struct queue_s{
    tuple_t *info;
    struct queue_s *next;
} queue_t;


int  check(int acc[], int j);
void result(int i);
void clean(tuple_t * root);
void compute(tuple_t **tmp, char tape[]);
//tuple_t * insert_tail(tuple_t *head, tuple_t *new);
tuple_t *insert_tuple(tuple_t tmp, tuple_t *pmt, tuple_t *a);
void sort(tuple_t list[]);
void Dequeue(tuple_t *tmp, queue_t ** head);
void Enqueue(tuple_t *tmp, queue_t **head);
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
    struct tuple_s list_state[100];
    char k;
    char tape[50];
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

            while(f < 6){
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
            clean(root);

            result(res);
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
            clean(root);

            result(res);


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

void setTape(tuple_t *tmp, char string[]){
    for(int j = 0; string[j] != '\0'; j++)
        tmp->tape[j] = string[j];
}

void compute(tuple_t **tmp, char tape[]) {



    queue_t *open = NULL;
    tuple_t * a = *tmp;
    queue_t * q;
    tuple_t *b;

    while(a!= NULL){
        for(int j = 0; tape[j] != '\0'; j++)
            a->tape[j] = tape[j];
        Enqueue(a, &open);
        a = a->next_bro;
    }
    q = open;

    while(open != NULL){
        if(q->info->count >max) {
            printf("U\n");
            break;
        }
        if(q ->info->tape[q->info->i] == q->info->toGet){
            q ->info->tape[q->info->i] = q->info->toSet;
            q->info->count++;
            q->info ->i = q->info->move + q->info ->i;
            if (check(acc, q->info->next_state) == 1) {
                res = 1;
                return;
            }
            if(q->info->next_state == q->info->curr_state)
                b = q->info->first_bro;
            else  b = q->info->f_child;

            while(b!= NULL){
                setTape(b, q->info->tape);
                Enqueue(b, &open);
                b = b->next_bro;
            }
        }
        Dequeue(q->info, &open);

        q = open;

    }











    /*if (res == 1 || res == 2 || res == 4)
        return;
    char tape_2[50];
    tuple_t *a = NULL;
    tuple_t *b = NULL;
    int j;
    if (tmp == NULL) {
        res = 0;
        return;
    }

    for (j = 0; tape[j] != '\0'; j++)
        tape_2[j] = tape[j];



    if (tot > 150){
        res = 2;
    return;
        }

    if (count >= max) {
        res = 2;
        return;
    }


    a = *tmp;
    b = *tmp;

    if (a->next_bro != NULL) {
        compute(&a->next_bro, tape, i, count);
    }


    if (tape_2[i] == b->toGet) {
        tape_2[i] = b->toSet;

        i = i + b->move;
        count++;
        tot++;

        if (check(acc, b->next_state) == 1) {
                res = 1;
                return;

        }
       else if (b->curr_state == b->next_state) {
           return compute(&b->first_bro, tape_2, i, count);
        } else if (b->f_child != NULL)

       return compute(&b->f_child, tape_2, i, count);


    } else
        return;

    return; */

}


void result(int i) {
    if (i == 1)
        printf("1\n");
    else if (i == 4 || i == 0 )
        printf("0\n");
    else if(res == 2)
        printf("U\n");

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


void clean(tuple_t * root){
    if(root!= NULL);
    else return;

    if(root->f_child == NULL && root->next_bro == NULL)
        return;
    if(root->next_bro != NULL){
        clean(root->next_bro);
    }
    if(root -> f_child != NULL)
        clean(root->f_child);
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

    return;
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

    return;
}


void sort(tuple_t list[]){
    tuple_t tmp;
    for(int i = 0; i< sizeof(list); i++){
        for(int j= 1; j< sizeof(list)-1; j++){
            if(list[j].curr_state < list[i].curr_state) {
                tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }
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

void Enqueue(tuple_t *tmp, queue_t **head) {

    queue_t *new;

    queue_t *b;


    queue_t *a = *head;



    tmp->count = 0;
    tmp->i = 10;
    if (a == NULL) {
        a = ALLOC_QUEUE;
        a->info = tmp;
        a->next = NULL;
        *head = a;
        return;
    }

    b = *head;

    while (b->next != NULL)
        b = b->next;

    b->next = ALLOC_QUEUE;
    b->next->info = tmp;
    b->next->next = NULL;

    return;
}


void Dequeue(tuple_t *tmp, queue_t ** head){

    queue_t * b = *head;
    queue_t * a;

    if(tmp->curr_state == b->info->curr_state && tmp->next_state == b->info->next_state &&
          tmp->move == b->info->move &&
          tmp->toSet == b->info->toSet && tmp->toGet == b->info->toGet) {
        a = b;
        *head = b->next;
        free(a);
        return;
    }

    while(b->next!= NULL){
        if(tmp->curr_state == b->next->info->curr_state && tmp->next_state == b->next->info->next_state &&
           tmp->move == b->next->info->move &&
           tmp->toSet == b->next->info->toSet && tmp->toGet == b->next->info->toGet){

        }
        a = b->next;
        b->next = a ->next;
        free(a);
    }
}

