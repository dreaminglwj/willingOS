#ifndef WILLING_LIST_H
#define WILLING_LIST_H

#include "willing.h"

#ifndef LIST_VOLATILE
    #define LIST_VOLATILE volatile
#endif

typedef struct  willingListItem {
    struct willingListItem * LIST_VOLATILE next; /* 下一个元素 */
    struct willingListItem * LIST_VOLATILE previous;/* 上一个元素 */
    void * tcbWith; /* 本item所属tcb */
    void * LIST_VOLATILE listWith; /*  */
    UBase_t sortValue; /* 用于排序 */
    uint8_t tickCountSession; /* 用于排序 */

}  ListItem_t;


typedef struct willingList {
    LIST_VOLATILE UBase_t itemNum; /* 元素个数 */
    ListItem_t * LIST_VOLATILE head; /* 链表头 */
    ListItem_t * LIST_VOLATILE tail; /* 链表尾 */
    //ListItem_t * LIST_VOLATILE current; /*当前指向的元素，用于遍历链表*/
}  List_t;


void initWillingList( List_t * const list );
UBase_t insertWillingList_Sort(  List_t * const list, ListItem_t * const item  );
UBase_t insertWillingList_Tail( List_t * const list, ListItem_t * const item );
UBase_t insertWillingList_Head( List_t * const list, ListItem_t * const item );
UBase_t getWillingListItemNum( List_t * const list );
UBase_t insertWillingList_Behind( List_t * const list, ListItem_t * const currentItem, ListItem_t * const item );
// ListItem_t * getWillingListCurrentItem( List_t * const list );
ListItem_t * getWillingListNextItem( List_t * const list, ListItem_t * const item );
ListItem_t * getWillingListNextItem_Circle( List_t * const list, ListItem_t * const item );
ListItem_t * getWillingListPreviousItem( List_t * const list, ListItem_t * const item );
ListItem_t * getWillingListHeadItem( List_t * const list );
ListItem_t * getWillingListTailItem( List_t * const list );
// ListItem_t * moveWillingListCursorNext( List_t * const list );
// ListItem_t * moveWillingListCursorPrevious( List_t * const list );
// ListItem_t * moveWillingListCursorTo( List_t * const list, ListItem_t * const item );
UBase_t removeWillingListItem( List_t * const list, ListItem_t * const item );

#endif
