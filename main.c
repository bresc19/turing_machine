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




typedef struct tuple_s  {
    int curr_state;
    char toGet;
    char toSet;
    int move;
    int next_state;
    int passed;
    struct tuple_s *next_bro;
    struct tuple_s *f_child;
    struct tuple_s *first_bro;
}   tuple_t;


int  check(int acc[], int i);
void result(int i);
void clean(tuple_t * root);
void compute(tuple_t * tmp, char tape[], int i, int acc[]);
tuple_t * insert_tail(tuple_t *head, tuple_t new);
tuple_t *insert_tuple(tuple_t *tmp, tuple_t *pmt, tuple_t *a);

int count=0;
int max_state = 0;
int acc[5];
int max;
int res = 0;
tuple_t *root = NULL;

int main(int argc, const char *argv[]) {


    tuple_t aaa;
    int z;
    char *a;
    tuple_t *list_state = NULL;
    char k;
    char tape[50];
    char blank[30];
    char input[1024];
    tuple_t *tmp = NULL;
    int i = 0;

    a = fgets(input, 1024, stdin);

    for (int i = 0; i < 9; i++)
        blank[i] = '_';

    while (a != NULL) {
        strtok(input, "\n");

        tmp = ALLOC_TUPLE;
        tmp->f_child = NULL;
        tmp->next_bro = NULL;
        tmp->first_bro = NULL;

        if (strcmp(input, "tr") == 0) {
            scanf("%s", input);


            while (strcmp(input, "acc") != 0) {

                tmp->curr_state = atoi(input);
                if (max_state < i)
                    max_state = i;
                scanf("%s %s %s %d", &tmp->toGet, &tmp->toSet, &k, &tmp->next_state);

                if (k == 'R')
                    tmp->move = 1;
                else if (k == 'L')
                    tmp->move = -1;
                else if (k == 'S')
                    tmp->move = 0;

                aaa.curr_state = tmp->curr_state;
                aaa.next_state = tmp->next_state;
                aaa.toGet = tmp->toGet;
                aaa.toSet = tmp->toSet;
                aaa.move = tmp->move;
                aaa.next_bro = NULL;
                aaa.f_child = NULL;
                aaa.first_bro = NULL;
                 list_state = insert_tail(list_state, aaa);

                root = insert_tuple(tmp, root, root);
                scanf("%s", input);
                z++;


            }
            scanf("%s", input);
            for (i = 0; strcmp(input, "max") != 0; i++) {
                acc[i] = atoi(input);
                scanf("%s", input);

            }
            scanf("%d", &max);
            scanf("%s", input);

        } else if (strcmp(input, "run") == 0) {

            for (i = 0; i < 10; i++)
                tape[i] = '_';
            a = fgets(blank, 1024, stdin);
            a = fgets(blank, 1024, stdin);

            strtok(blank, "\n");
            for (int j = 0; blank[j] != '\0'; j++, i++)
                tape[i] = blank[j];

            for (int j = 0; j < 10; j++, i++)
                tape[i] = '_';

            for (tmp = list_state; tmp != NULL; tmp = tmp->next_bro) {
                root = insert_tuple(tmp, root, root);
            }
            res = 0;
            compute(root, tape, 10, acc);
            clean(root);

            result(res);
            fgets(input, 1024, stdin);
            strtok(input, "\n");
            res = 0;
            count = 0;

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
            res = 0;
            compute(root, tape, 10, acc);
            clean(root);
            result(res);


            res = 0;
            count = 0;


            a = fgets(input, 1024, stdin);
            strtok(input, "\n");
        }

        free(tmp);
    }


    return 0;
}

tuple_t *insert_tuple(tuple_t *tmp, tuple_t *pmt, tuple_t *a) {

    int flag = 0;
    tuple_t * first;
    if (pmt == NULL) {
        pmt = ALLOC_TUPLE;
        pmt->next_state = tmp->next_state;
        pmt->toGet = tmp->toGet;
        pmt->toSet = tmp->toSet;
        pmt->move = tmp->move;
        pmt->curr_state = tmp->curr_state;
        pmt->next_bro = NULL;
        pmt->f_child = NULL;
        pmt -> first_bro = pmt;
        return pmt;
    } else {
        if (a->curr_state == tmp->curr_state) {
            if (tmp->curr_state != tmp->next_state) {
                if (tmp->curr_state == a->curr_state && tmp->next_state == a->next_state && tmp->move == a->move &&
                    tmp->toSet == a->toSet && tmp->toGet == a->toGet) {
                    flag = 1;
                }
                first = a;
                while (a->next_bro != NULL) {
                    a = a->next_bro;
                    if (tmp->curr_state == a->curr_state && tmp->next_state == a->next_state &&
                        tmp->move == a->move &&
                        tmp->toSet == a->toSet && tmp->toGet == a->toGet) {
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    a->next_bro = ALLOC_TUPLE;
                    a->next_bro->next_state = tmp->next_state;
                    a->next_bro->toGet = tmp->toGet;
                    a->next_bro->toSet = tmp->toSet;
                    a->next_bro->move = tmp->move;
                    a->next_bro->curr_state = tmp->curr_state;
                    a->next_bro->next_bro = NULL;
                    a->next_bro->f_child = NULL;
                    a->next_bro ->first_bro = first;
                    a = a->next_bro;
                }

            } else {
                if (tmp->curr_state == a->curr_state && tmp->next_state == a->next_state && tmp->move == a->move &&
                    tmp->toSet == a->toSet && tmp->toGet == a->toGet)
                    flag = 1;

                while (a->next_bro != NULL) {
                    if (tmp->curr_state == a->curr_state && tmp->next_state == a->next_state &&
                        tmp->move == a->move &&
                        tmp->toSet == a->toSet && tmp->toGet == a->toGet)
                        flag = 1;

                    a = a->next_bro;
                    if (tmp->curr_state == a->curr_state && tmp->next_state == a->next_state &&
                        tmp->move == a->move &&
                        tmp->toSet == a->toSet && tmp->toGet == a->toGet)
                        flag = 1;
                }
                if (flag == 0) {
                    a->next_bro = ALLOC_TUPLE;
                    a->next_bro->next_state = tmp->next_state;
                    a->next_bro->toGet = tmp->toGet;
                    a->next_bro->toSet = tmp->toSet;
                    a->next_bro->move = tmp->move;
                    a->next_bro->curr_state = tmp->curr_state;
                    a->next_bro->next_bro = NULL;
                    a->next_bro->f_child = NULL;
                    a = a->next_bro;
                }

            }

        }


        if (a->next_state == tmp->curr_state) {
            if (a->f_child == NULL) {

                a->f_child = ALLOC_TUPLE;
                a->f_child->next_state = tmp->next_state;
                a->f_child->toGet = tmp->toGet;
                a->f_child->toSet = tmp->toSet;
                a->f_child->move = tmp->move;
                a->f_child->curr_state = tmp->curr_state;
                a->f_child->next_bro = NULL;
                a->f_child->f_child = NULL;
                a -> f_child ->first_bro = a ->f_child;
                a = a->f_child;
            } else {
                a = a->f_child;
                if (tmp->curr_state != tmp->next_state) {
                    if (tmp->curr_state == a->curr_state && tmp->next_state == a->next_state &&
                        tmp->move == a->move &&
                        tmp->toSet == a->toSet && tmp->toGet == a->toGet)
                        flag = 1;
                    first = a;
                    while (a->next_bro != NULL) {
                        a = a->next_bro;
                        if (tmp->curr_state == a->curr_state && tmp->next_state == a->next_state &&
                            tmp->move == a->move &&
                            tmp->toSet == a->toSet && tmp->toGet == a->toGet)
                            flag = 1;
                    }
                    if (flag == 0) {
                        a->next_bro = ALLOC_TUPLE;
                        a->next_bro->next_state = tmp->next_state;
                        a->next_bro->toGet = tmp->toGet;
                        a->next_bro->toSet = tmp->toSet;
                        a->next_bro->move = tmp->move;
                        a->next_bro->curr_state = tmp->curr_state;
                        a->next_bro->next_bro = NULL;
                        a->next_bro->f_child = NULL;
                        a->next_bro ->first_bro = first;
                        a = a->next_bro;
                    }

                } else {
                    if (tmp->curr_state == a->curr_state && tmp->next_state == a->next_state &&
                        tmp->move == a->move &&
                        tmp->toSet == a->toSet && tmp->toGet == a->toGet)
                        flag = 1;
                    first = a;
                    while (a->next_bro != NULL) {
                        if (tmp->curr_state == a->curr_state && tmp->next_state == a->next_state &&
                            tmp->move == a->move &&
                            tmp->toSet == a->toSet && tmp->toGet == a->toGet)
                            flag = 1;
                        a = a->next_bro;

                    }
                    if (flag == 0) {
                        a->next_bro = ALLOC_TUPLE;
                        a->next_bro->next_state = tmp->next_state;
                        a->next_bro->toGet = tmp->toGet;
                        a->next_bro->toSet = tmp->toSet;
                        a->next_bro->move = tmp->move;
                        a->next_bro->curr_state = tmp->curr_state;
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

tuple_t * search(tuple_t *head, int state) {
    tuple_t *tmp;
    if (state == head->next_state && head->f_child != NULL)
        return head->f_child;
    if (head->f_child != NULL) {
        tmp = search(head->f_child, state);
        if (tmp != NULL)
            return tmp;

    }

    if (head->next_bro != NULL) {
        tmp = search(head->next_bro, state);
        if (tmp != NULL)
            return tmp;
    }

    return NULL;
}

void compute(tuple_t *tmp, char tape[], int i, int acc[]) {

    char tape_2[50];
    tuple_t *a;
    for (int j = 0; tape[j] != '\0'; j++)
        tape_2[j] = tape[j];

    if (res == 1 || res == 2 || res == 4)
        return;
    if (tmp == NULL) {
        res = 0;
        return;
    }

    if (count >= max) {
        res = 3;
    }

    if (count >= max * 10) {
        res = 4;
        return;
    }


    a = tmp;
    while (a->next_bro != NULL) {
        compute(a->next_bro, tape, i, acc);
        a = a->next_bro;
    }


    if (tape_2[i] == tmp->toGet) {
        tape_2[i] = tmp->toSet;

        i = i + tmp->move;
        count++;
        if (check(acc, tmp->next_state) == 1) {
            if (res == 0) {
                res = 1;
                return;
            } else if (res == 3)
                res = 2;
        }
        if (tmp->curr_state == tmp->next_state) {
            compute(tmp->first_bro, tape_2, i, acc);

        } else if (tmp->f_child != NULL)
            compute(tmp->f_child, tape_2, i, acc);

        else if (res != 1) {
            a = search(root, tmp->next_state);
            if (a != NULL) {
                if (a->passed <= 5) {
                    a->passed++;
                    compute(a, tape_2, i, acc);
                }
            }
        }

    } else


        return;


}


void result(int i) {
    if (i == 1)
        printf("1\n");
    else if (i == 4 || i == 0)
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

    tuple_t * insert_tail(tuple_t *head, tuple_t new) {

        tuple_t *tmp;
        if (head == NULL) {
            head = ALLOC_TUPLE;
            head->curr_state = new.curr_state;
            head->next_state = new.next_state;
            head->move = new.move;
            head->toSet = new.toSet;
            head->toGet = new.toGet;
            head -> passed = 0;
            head->next_bro = NULL;
            head->f_child = NULL;
            head -> first_bro = NULL;
            return head;
        } else {
            tmp = head;
            while (tmp->next_bro != NULL)
                tmp = tmp->next_bro;

            tmp->next_bro = ALLOC_TUPLE;
            tmp->next_bro->curr_state = new.curr_state;
            tmp->next_bro->next_state = new.next_state;
            tmp->next_bro->move = new.move;
            tmp->next_bro->toSet = new.toSet;
            tmp->next_bro->toGet = new.toGet;
            tmp->next_bro->next_bro = NULL;
            tmp->next_bro ->passed = 0;
            tmp->next_bro->f_child = NULL;
            tmp->next_bro ->first_bro = NULL;
            return head;
        }
    }


void clean(tuple_t * root) {
    if (root != NULL && root->passed > 0)
        root->passed = 0;

    if (root->f_child == NULL && root->next_bro == NULL)
        return;
    if (root->next_bro != NULL) {
        clean(root->next_bro);
    }
    if (root->f_child != NULL)
        clean(root->f_child);

    return;
}




