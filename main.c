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
#include <time.h>

#define ALLOC_ELEMENT (list_tuple_t *)malloc(sizeof(list_tuple_t))
#define ALLOC_TUPLE   (tuple_t *)malloc(sizeof(tuple_t))

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
    int i;
    int count;
    int time;

} ;


typedef struct tapes_s{
    char *tape;
    int len;
}tapes_t;

typedef struct list_tuple_s{
    tuple_t info;
    struct list_tuple_s *next;

} list_tuple_t;


int  check(const int acc[], int j);
void compute(tuple_t *tmp, char tape[]);
void Enqueue(tuple_t *tmp, struct queue_s head[], struct tapes_s *list, int count, int i, const char string[], int len, int index, char toWrite, int num);
void insert_on_tail(tuple_t *head, tuple_t new);
void find_child(list_tuple_t *head, tuple_t *elem);
void Enqueue_first(tuple_t *tmp, struct queue_s head[], struct tapes_s *list, unsigned int count, int i, char *string, int num, int len);
void insert_order(tuple_t tmp, list_tuple_t ** head);
int num_tuple(tuple_t *a, char ch);

int N =0;
int acc[10];
long int max = 0;
int main(int argc, const char *argv[]) {

    int z = 0;
    char * a;
    list_tuple_t *p;
    tuple_t *aaa;
    list_tuple_t *list_state = NULL;
    char k;
    char tape[100000];
    char input[100000];
    tuple_t tmp;
    int i = 0;
    int j = 0;
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

            while(p!= NULL){
                aaa = &p->info;
                while(aaa != NULL) {

                    find_child(list_state, aaa);
                    aaa = aaa->next_bro;
                }
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


            a =  fgets(input, 100000, stdin);
            a =  fgets(input, 100000, stdin);

            strtok(input, "\n");
            N = (int) strlen(input);

            memset(tape, '_', N);

            for ( j = 0, i = N; input[j] != '\0'; j++, i++)
                tape[i] = input[j];

            for (int j = 0; j < N; j++, i++)
                tape[i] = '_';

            tape[i]= '\0';
            compute(&list_state->info, tape);


            fgets(input, 100000, stdin);
            strtok(input, "\n");

        } else {

            N = (int) (  strlen(input));
            memset(tape, '_', N);

            for ( j = 0, i = N; input[j] != '\0'; j++, i++) {
                if (input[j] == '\n')
                    break;
                tape[i] = input[j];
            }

            for ( j = 0; j < N; j++, i++)
                tape[i] = '_';
            tape[i]= '\0';
            compute(&list_state->info, tape);


            a = fgets(input, 100000, stdin);
            strtok(input, "\n");
        }


    }


    return 0;
}

void compute(tuple_t *tmp, char *tape) {

    struct queue_s open[1000];
    struct tapes_s list[1000];
    int z;
    int j;
    tuple_t *a = tmp;
    int new_count;
    unsigned int x1 = 0;
    unsigned int x2 = 0;
    int undecidable = 0;

    Enqueue_first(a, open, &list[0], 0, N, tape, 0, (int) strlen(tape));
    x2++;
    while (x1 != x2) {
        if (list[x1].tape[open[x1].i] == open[x1].info->toGet) {
            if ((open[x1].info->curr_state == open[x1].info->next_state && open[x1].info->move == -1 &&
                 list[x1].tape[open[x1].i] == '_' &&
                 (open[x1].i == 0) )|| ( (open[x1].info->curr_state == open[x1].info->next_state
                                          && open[x1].info->move == 0  && open[x1].info->toGet == open[x1].info->toSet))) {
                undecidable = 1;

                free(list[x1].tape);


                if (x1 != 999)
                    x1++;
                else {
                    x1 = 0;
                }

            } else {

                if (open[x1].time != 1) {
                    new_count = open[x1].count + 1;
                    if (new_count > max) {
                        for (; x1 != x2;) {
                            if (x1 != x2)
                                free(list[x1].tape);
                            if (x1 != 999)
                                x1++;
                            else {
                                x1 = 0;
                            }
                        }
                        printf("U\n");
                        return;
                    }
                    if (open[x1].info->f_child == NULL) {
                        if (check(acc, open[x1].info->next_state) == 1) {
                            for (; x1 != x2;) {
                                if (x1 != x2)
                                    free(list[x1].tape);
                                if (x1 != 999)
                                    x1++;
                                else {
                                    x1 = 0;
                                }
                            }
                            printf("1\n");
                            return;
                        }
                    }
                    j = list[x1].len;
                    if (open[x1].info->f_child != NULL) {
                        Enqueue(open[x1].info->f_child, open, &list[x2], new_count, open[x1].info->move + open[x1].i,
                                list[x1].tape, j,
                                open[x1].i, open[x1].info->toSet, x2);
                        open[x1].time--;

                        if (x2 != 999)
                            x2++;
                        else {
                            x2 = 0;
                        }


                    }

                    open[x1].info = open[x1].info->next_bro;

                } else {
                    list[x1].tape[open[x1].i] = open[x1].info->toSet;
                    j = list[x1].len;
                    open[x2].i = open[x1].i + open[x1].info->move;
                    list[x2].len = j;

                    if (list[x1].tape[0] != '_') {
                        list[x2].len = list[x2].len + N;
                        list[x1].tape = (char *) realloc(list[x1].tape, (j + 1 + N) * sizeof(char));
                        open[x2].i = open[x1].i + open[x1].info->move + N;

                        for (z = list[x2].len - 2; z != N - 1; z--)
                            list[x1].tape[z] = list[x1].tape[z - N];
                        memset(list[x1].tape, '_', N);
                        list[x1].tape[list[x2].len] = '\0';
                    } else if (list[x1].tape[j - 1] != '_') {
                        list[x2].len = list[x2].len + N;
                        list[x1].tape = (char *) realloc(list[x1].tape, (j + 1 + N) * sizeof(char));
                        for (z = j; z < list[x2].len; z++)
                            list[x1].tape[z] = '_';
                        list[x1].tape[z] = '\0';
                        open[x2].i = open[x1].i + open[x1].info->move;

                    }

                    if (open[x1].count > max) {
                        for (; x1 != x2;) {
                            if (x1 != x2)
                                free(list[x1].tape);
                            if (x1 != 999)
                                x1++;
                            else {
                                x1 = 0;
                            }
                        }
                        printf("U\n");
                        return;
                    }
                    if (open[x1].info->f_child == NULL) {
                        if (check(acc, open[x1].info->next_state) == 1) {
                            for (; x1 != x2;) {
                                if (x1 != x2)
                                    free(list[x1].tape);
                                if (x1 != 999)
                                    x1++;
                                else {
                                    x1 = 0;
                                }
                            }
                            printf("1\n");

                            return;
                        } else {

                            if (x1 != x2)
                                free(list[x1].tape);
                            if (x1 != 999)
                                x1++;
                            else {
                                x1 = 0;
                            }
                        }
                    } else {

                        open[x2].count = open[x1].count + 1;
                        open[x2].info = open[x1].info->f_child;
                        open[x2].time = num_tuple(open[x2].info, list[x1].tape[open[x2].i]);
                        list[x2].tape = list[x1].tape;
                        if (x2 != 999)
                            x2++;
                        else {
                            x2 = 0;
                        }
                        list[x1].tape = NULL;


                        if (x1 != 999)
                            x1++;
                        else {
                            x1 = 0;
                        }
                    }

                }
            }
        } else if (open[x1].time == 0) {

            free(list[x1].tape);


            if (x1 != 999)
                x1++;
            else {
                x1 = 0;
            }
        } else {
            open[x1].info = open[x1].info->next_bro;
        }

    }
    if (undecidable == 0)
        printf("0\n");
    else printf("U\n");

}

int check(const int acc[], int j) {
    for (int i = 0; i < sizeof(acc); i++) {
        if (acc[i] == 0)
            break;
        if (acc[i] == j)
            return 1;
    }

    return 0;
}



void Enqueue(tuple_t *tmp, struct queue_s head[], struct tapes_s *list, int count, int i, const char string[], int len, int index, char toWrite, int num) {

    int j = 0;
    int y = 0;
    head[num].info = tmp;
    head[num].i = i;
    head[num].count = count;
    if (index == 0) {
        len = len + N;
        list->tape = (char *) malloc((len + 1) * sizeof(char));
        memset(list->tape, '_', N);
        for (j = 1, y = 0; string[y] != '\0'; j++, y++)
            list->tape[j] = string[y];
        list->tape[index + N] = toWrite;
        list->len = len + 1;
        list->tape[len] = '\0';
        if (i < 0)
            head[num].i = 0;
        head[num].time = num_tuple(head[num].info, list->tape[i]);


    } else if (string[len - 1] != '_') {
        len = len + N;

        list->tape = (char *) malloc((len + 1) * sizeof(char));

        for (j = 0; string[j] != '\0'; j++)
            list->tape[j] = string[j];

        for (; j < len; j++)
            list->tape[j] = '_';
        list->len = len + 1;

        list->tape[index] = toWrite;
        list->tape[len] = '\0';
        head[num].time = num_tuple(head[num].info, list->tape[i]);


    } else {

        list->tape = (char *) malloc((len + 1) * sizeof(char));
        memmove(list->tape, string, len);

        list->tape[index] = toWrite;

        list->tape[len] = '\0';
        list->len = len;

    }
    head[num].time = num_tuple(head[num].info, list->tape[i]);

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


    if((*head)->info.curr_state > tmp.curr_state){
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

    if(tmp.curr_state == b->info.curr_state){
        insert_on_tail(&b->info, tmp);
        return;
    }

    while(b->next != NULL ){
        if (tmp.curr_state == b->info.curr_state && tmp.next_state == b->info.next_state &&
            tmp.move == b->info.move &&
            tmp.toSet == b->info.toSet && tmp.toGet == b->info.toGet)
            return;

        if(tmp.curr_state == b->info.curr_state){
            insert_on_tail(&b->info, tmp);
            return;
        }
        if(b->next->info.curr_state > tmp.curr_state){
            a = b->next;
            b->next = ALLOC_ELEMENT;
            b->next->info = tmp;
            b->next->next = a;
            return;
        }
        b = b->next;
    }

    if(tmp.curr_state == b->info.curr_state){
        insert_on_tail(&b->info, tmp);
        return;
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



void find_child(list_tuple_t *head, tuple_t *elem){
    if(head == NULL)
        return;
    if(head->info.curr_state == elem->next_state) {
        elem->f_child = &head->info;
        return;
    }   else
        return find_child(head->next, elem);
}








void Enqueue_first(tuple_t *tmp, struct queue_s head[], struct tapes_s *list, unsigned int count, int i, char *string, int num, int len) {

    char *new_tape= NULL;
    head[num].info = tmp;
    head[num].i = i;
    head[num].count = count;
    new_tape = (char *) malloc((len + 1) * sizeof(char));
    list->len = (int) strlen(string);
    memmove(new_tape, string, len);
    list->tape = new_tape;
    new_tape[len]='\0';
    head[num].time = num_tuple(head[num].info, string[N]);

}
int num_tuple(tuple_t *a, char ch){
    int num = 0;
    while(a!= NULL){
        if(a->toGet == ch)
            num++;
        a = a->next_bro;
    }
    return num;
}

/*void link_bro(list_tuple_t *p){
    if(p->next == NULL)
        return;

    if (p->info.curr_state == p->next->info.curr_state) {
        p->info->next_bro = p->next->info;
    }

    return link_bro(p->next);
}*/


void insert_on_tail(tuple_t *head, tuple_t new){
    tuple_t *tmp;
    tmp = head;


    while(tmp->next_bro != NULL){
        tmp = tmp->next_bro;
    }

    tmp->next_bro = ALLOC_TUPLE;

    tmp->next_bro->curr_state = new.curr_state;
    tmp->next_bro->next_state = new.next_state;
    tmp->next_bro->move = new.move;
    tmp->next_bro->toGet = new.toGet;
    tmp->next_bro->toSet = new.toSet;
    tmp->next_bro->next_bro = NULL;
    tmp->next_bro ->f_child = NULL;
}
