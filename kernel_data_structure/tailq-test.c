//#include <memory.h>
#include <stdio.h>
#include "queue.h"

// gcc -E -P  tailq-test.c ���滻

// �¼�����
struct event {
 TAILQ_ENTRY(event)   ev_next;
 int  iItem;
};
/*
struct event {
 struct { 
	struct event *tqe_next; 
	struct event **tqe_prev; //ΪʲôҪ�ö���ָ��
 } ev_next;
 int iItem;
};
*/

// ������һ���¼��ṹ˫������ event_list(first, last).
TAILQ_HEAD (event_list, event);
/*
���滻��
struct event_list { 
	struct event *tqh_first; 
	struct event **tqh_last;   //ΪʲôҪ�ö���ָ��  β������ʱ�ȽϷ���
};

*/

/*
 * main����
 */
int main(int argc, char* argv[])
{ 
 // ��ʼ��
 struct event_list evlist;
 TAILQ_INIT(&evlist);
 /*
 do { (&evlist)->tqh_first = ((void *)0); (&evlist)->tqh_last = &(&evlist)->tqh_first; } while (0);
 */

 // ����ͷ�� 
 struct event event0;
 event0.iItem = 0;
 TAILQ_INSERT_HEAD(&evlist, &event0, ev_next);
/*
do { if (((&event0)->ev_next.tqe_next = (&evlist)->tqh_first) != ((void *)0)) (&evlist)->tqh_first->ev_next.tqe_prev = &(&event0)->ev_next.tqe_next; else (&evlist)->tqh_last = &(&event0)->ev_next.tqe_next; (&evlist)->tqh_first = (
&event0); (&event0)->ev_next.tqe_prev = &(&evlist)->tqh_first; } while (0);
*/
 //
 struct event event1;
 event1.iItem = 1;
 TAILQ_INSERT_TAIL(&evlist, &event1, ev_next);
/*
do { (&event1)->ev_next.tqe_next = ((void *)0); (&event1)->ev_next.tqe_prev = (&evlist)->tqh_last; *(&evlist)->tqh_last = (&event1); (&evlist)->tqh_last = &(&event1)->ev_next.tqe_next; } while (0);
*/
 //
 struct event event2;
 event2.iItem = 2;
 TAILQ_INSERT_TAIL(&evlist, &event2, ev_next);

 //
 struct event event3;
 event3.iItem = 3;
 TAILQ_INSERT_TAIL(&evlist, &event3, ev_next);

 struct event* pEvent;
 TAILQ_FOREACH(pEvent, &evlist, ev_next) {
  printf("item:%d\n", pEvent->iItem);
 }
 /*
 for((pEvent) = ((&evlist)->tqh_first); (pEvent) != ((void *)0); (pEvent) = ((pEvent)->ev_next.tqe_next)) {
  printf("item:%d\n", pEvent->iItem);
 }

 */

 getchar();
 return 0;
}