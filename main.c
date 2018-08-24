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
#define ALLOC_TAPE ((list_tape_t *)malloc(sizeof(list_tape_t)))


typedef struct list_tape_s{
    char *tape;
    struct list_tape_s *next;
    int i;
    int count;
} list_tape_t;

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
void Enqueue(tuple_t *tmp, queue_t **head);
void insert_step(list_tape_t ** head, int i, int count, char toWrite, int index, char *tape);

void re_insert_tuple(tuple_t tmp, tuple_t **pmt, tuple_t *a);


int length(char string[]);

void insert_order(tuple_t tmp, list_tuple_t ** head);

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
    int qqqq = 0;
    queue_t *open = NULL;
    list_tape_t *tapes = NULL;
    int j;
    char new_ch;
    int new_pos;
    queue_t * q;
    list_tape_t * p;
    tuple_t *a = *tmp;
    tuple_t *b;

    insert_step(&tapes, 1, 0, 'c', -1, tape);
    while (a != NULL) {
        if (tape[1] == a->toGet)
            Enqueue(a, &open);
        a = a->next_bro;
    }

    while (open != NULL) {
        if (tapes->tape[tapes->i] == open->info->toGet) {
            new_pos = open->info->move + tapes->i;
            new_ch = open->info->toSet;

            if (tapes->count + 1 > max ) {
                while (open != NULL) {
                    q = open;
                    open = open->next;
                    free(q);
                }
                while (tapes != NULL) {
                    p = tapes;
                    tapes = tapes->next;
                    free(p->tape);
                }
                printf("U\n");
                return;
            }



            if (open->info->next_state == open->info->curr_state)
                b = open->info->first_bro;
            else {
                b = open->info->f_child;
                if (b == NULL) {

                    if (check(acc, open->info->next_state) == 1) {
                        while (open != NULL) {
                            q = open;
                            open = open->next;
                            free(q);
                        }
                        while (tapes != NULL) {
                            p = tapes;
                            tapes = tapes->next;
                            free(p->tape);
                            free(p);

                        }
                        printf("1\n");

                        return;
                    }

                    b = search(root, open->info->next_state);



                }
            }
            if(b != NULL)
            insert_step(&tapes, new_pos, tapes->count+1, new_ch, tapes->i, tapes->tape);



            while (b != NULL) {
                Enqueue(b, &open);

                b = b->next_bro;
            }
            if(open->info->next_bro == NULL){
                p = tapes;
                tapes = tapes->next;
                free(p->tape);
                free(p);
            }

        }
        else if (open->info->next_bro == NULL) {
                p = tapes;
                tapes = tapes->next;
                free(p->tape);
                free(p);
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


void Enqueue(tuple_t *tmp, queue_t **head) {

    queue_t *c;
    queue_t *a = *head;

    if (a == NULL) {


        a = ALLOC_QUEUE;

        a->info = tmp;


        *head = a;
        b = a;
        return;
    }

    if (used == NULL) {
        b->next = ALLOC_QUEUE;
        b->next->info = tmp;
        b->next->next = NULL;
    }
    else {
        c = used;
        used = used->next;
        b->next = c;
        b->next->info = tmp;

        b->next->next = NULL;

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


void insert_step(list_tape_t ** head, int i, int count, char toWrite, int index, char *tape) {


    list_tape_t *tmp = NULL;
    int j = 0;
    int y = 0;
    int len = length(tape);
    if (*head == NULL) {
        *head = ALLOC_TAPE;
        (*head)->count = count;
        (*head)->i = i;
        (*head)->next = NULL;

        (*head)->tape = (char *) malloc((len + 1) * sizeof(char));

        for (y = 0; tape[y] != '\0'; y++) {
            (*head)->tape[y] = tape[y];
        }


        (*head)->tape[y] = '\0';

    } else {
        tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = ALLOC_TAPE;
        tmp->next->i = i;
        tmp->next->count = count;
        tmp->next->next = NULL;
        if (index == 0) {
            len = len + 1;
            tmp->next->tape = (char *) malloc((len + 1) * sizeof(char));
            tmp->next->tape[0] = '_';
            for (j = 1, y = 0; tape[y] != '\0'; j++, y++) {
                tmp->next->tape[j] = tape[y];
            }
            tmp->next->tape[index + 1] = toWrite;

            tmp->next->tape[j] = '\0';
            if (i == -1)
                tmp->next->i = 0;
            else tmp->next->i++;


        } else if (index >= len - 1) {
            len = len + 1;
            tmp->next->tape = (char *) malloc((len + 1) * sizeof(char));
            for (y = 0; tape[y] != '\0'; y++) {
                tmp->next->tape[y] = tape[y];
            }
            tmp->next->tape[index] = toWrite;
            tmp->next->tape[y] = '_';
            tmp->next->tape[y + 1] = '\0';


        } else {
            tmp->next->tape = (char *) malloc((len + 1) * sizeof(char));

            for (y = 0; tape[y] != '\0'; y++) {
                tmp->next->tape[y] = tape[y];
            }
            tmp->next->tape[index] = toWrite;

            tmp->next->tape[y] = '\0';

        }


    }


}






