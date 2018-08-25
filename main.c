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

#define ALLOC_ELEMENT (list_tuple_t *)malloc(sizeof(list_tuple_t))



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

void Enqueue(tuple_t *tmp, struct queue_s head[], int count, int i, const char string[], int len, int index, char toWrite, int num);

void insert_order(tuple_t tmp, list_tuple_t ** head);

void find_child(list_tuple_t *head, list_tuple_t *elem);

void Enqueue_first(tuple_t *tmp, struct queue_s head[], int count, int i, char *string, int num, int len);

int acc[10];
long int max = 0;

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
            memset(tape, 0, sizeof(tape));


            a = fgets(input, 2048, stdin);
            strtok(input, "\n");
        }


    }


    return 0;
}

void compute(tuple_t *tmp, char tape[]) {

    struct queue_s open[2500];
    int j;
    tuple_t *a = tmp;
    char new_ch;
    int new_pos;
    int new_count;
    int x1 = 0;
    int x2 = 0;

    Enqueue_first(a, open, 0, 1, tape, 0, (int) strlen(tape));
    x2++;


    while (x1 != x2) {

        if (open[x1].tape[open[x1].i] == open[x1].info->toGet) {
            new_pos = open[x1].info->move + open[x1].i;
            new_ch = open[x1].info->toSet;
            new_count = open[x1].count + 1;

            if (new_count > max) {
                for( j = x1; j<x2; j++)
                    if(open[j].tape != NULL)
                        free(open[j].tape);
                printf("U\n");
                return;
            }
            if (open[x1].info->f_child == NULL) {

                if (check(acc, open[x1].info->next_state) == 1) {
                    for( j = x1; j<x2; j++)
                        if(open[j].tape != NULL)
                            free(open[j].tape);
                    printf("1\n");

                    return;
                }
            }
            j = (int) strlen(open[x1].tape);





            if (open[x1].info->f_child != NULL) {
                Enqueue(open[x1].info->f_child, open, new_count, new_pos, open[x1].tape, j, open[x1].i, new_ch, x2);
                if(x2 != 2499)
                    x2++;
                else x2 = 0;
            }
            if(open[x1].info->next_bro == NULL) {
                if(open[x1].tape != NULL)
                    free(open[x1].tape);
                open[x1].tape = NULL;
                if(x1!=2499)
                    x1++;
                else x1 = 0;
            }
            else open[x1].info = open[x1].info ->next_bro;











        }








        else if(open[x1].info->next_bro == NULL) {
            if(open[x1].tape != NULL)
                free(open[x1].tape);
            open[x1].tape = NULL;

            if(x1!=2499)
                x1++;
            else x1 = 0;

        }
        else open[x1].info = open[x1].info ->next_bro;

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



void Enqueue(tuple_t *tmp, struct queue_s head[], int count, int i, const char string[], int len, int index, char toWrite, int num) {

    int j = 0;
    int y = 0;

    head[num].info = tmp;
    head[num].i = i;
    head[num].count = count;
    head[num].next = NULL;
    if (index == 0 && i == -1) {
        len = len + 1;
        head[num].tape = (char *) malloc((len + 1) * sizeof(char));

        head[num].tape[0] = '_';

        for (j = 1, y = 0; string[y] != '\0'; j++, y++)
            head[num].tape[j] = string[y];
            head[num].tape[index+1]= toWrite;


        head[num].tape[len] = '\0';
        if (i < 0)
            head[num].i = 0;

    } else if (string[len - 1] != '_') {
        len = len + 1;

        head[num].tape = (char *) malloc((len + 1) * sizeof(char));

        for (j = 0; string[j] != '\0'; j++)
            head[num].tape[j] = string[j];

        head[num].tape[j] = '_';

            head[num].tape[index]= toWrite;
        head[num].tape[len] = '\0';

    } else {

        head[num].tape = (char *) malloc((len + 1) * sizeof(char));
        memcpy(head[num].tape, string, len);

            head[num].tape[index]= toWrite;

        head[num].tape[len] = '\0';
    }
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

void Enqueue_last(tuple_t *tmp, struct queue_s head[], int count, int i,  char string[], int len, int index, char toWrite, int num) {
    if(index >1 && index < len-2){
        head[num].info = tmp;
        head[num].i = i;
        head[num].count = count;
        head[num].next = NULL;
        head[num].tape = string;
        head[num].tape[index]= toWrite;
        return;
    }
    else Enqueue(tmp, head, count, i, string, len, index, toWrite, num);



}



void Enqueue_first(tuple_t *tmp, struct queue_s head[], int count, int i, char *string, int num, int len) {

    head[num].info = tmp;
    head[num].i = i;
    head[num].count = count;
    head[num].next = NULL;
    head[num].tape = (char *) malloc((len + 1) * sizeof(char));
    memcpy(head[num].tape, string, len);
    head[num].tape[len]= '\0';


}
