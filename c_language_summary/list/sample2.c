/*
 * @file sample.c
 * @author Akagi201
 * @date 2015/03/18
 *
 * sample for list.h
 */

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

static LIST_HEAD(class_list);

struct class{
    int num; //班号
    /* linked to class_list 全局变量*/
	struct list_head clist;
    /* links all students (student->list) */
	struct list_head stu_list;
    //other
};

struct student{
    int num; //学号
    struct list_head list;
	struct class *class; //指向所属的班级
    //other
};

int main(int argc, char **argv){

    struct class class1={num:1};
    struct class *class2;
    class2 = (struct class *)malloc(sizeof(struct class));
	class2->num=2;
    struct list_head *pos;
    unsigned int i;

    INIT_LIST_HEAD(&class1.clist);
    list_add(&class1.clist,&class_list);
    
	INIT_LIST_HEAD(&class2->clist);
    list_add(&class2->clist,&class_list);

    

    INIT_LIST_HEAD(&class1.stu_list);
	INIT_LIST_HEAD(&class2->stu_list);
   

    for(i=1; i<=5; i++){
        struct student * stu;
        stu= (struct student *)malloc(sizeof(struct student));
		stu->num=i;
		stu->class = &class1;
		list_add_tail(&stu->list,&class1.stu_list); 
    }

    for(i=11; i<=15; i++){
        struct student * stu;
        stu= (struct student *)malloc(sizeof(struct student));
		stu->num=i;
		stu->class = class2;
		list_add_tail(&stu->list,&class2->stu_list); 
    }
  
    printf("traversing the list using list_for_each()\n");
    list_for_each(pos, &class1.stu_list){
        struct student * tmp;
        tmp= list_entry(pos, struct student, list);

        printf("student %d \n", tmp->num);

    }
   printf("traversing the list using list_for_each_prev()\n");
   list_for_each_prev(pos, &class1.stu_list){
        struct student * tmp;
        tmp= list_entry(pos, struct student, list);

        printf("student %d \n", tmp->num);

    }
    printf("\n");


    return 0;
}