#include "lsit.h"


void initWillingList( List_t * const list ) {
    if ( list == NULL ) {
        return;
    }

    list->itemNum = 0;
    list->head = NULL;
    list->current = NULL;
}


UBase_t insertWillingList_Sort(  List_t * const list, ListItem_t * const item  ){
    UBase_t itemNums = 0;
    /* 判断参数的代码在这个级别上来说不一定有用，一是降低了效率，
    二是系统级别的代码如果参数都存在问题，是不可想象的
    三是变量在分配内存后不必要立即进行初始化，因此tcbWith可能是无效的任意值不一定为NULL */
    if ( list == NULL  || item == NULL || item->tcbWith == NULL ) {
        return 0;
    }

    ListItem_t * currentItem = list->head;
    if ( currentItem == NULL ) {
        list->head = item;
        item->next = NULL;
        item->previous = NULL;
        item->listWith = list;

        list->itemNum++;
        return list->itemNum;
    }

    /* 查找插入位置 */
    while (currentItem->next == NULL)
    {
        nextItem = currentItem->next;
        if ( item->sortValue >= currentItem->sortValue && item->sortValue < nextItem->sortValue ) {
            break;
        } else {
            currentItem->next = nextItem;
        }
    }
    
    /* 插入 */
    itemNums = insertWillingList_Hehind( currentItem, item );
    return itemNums;
}


UBase_t insertWillingList_End( List_t * const list, ListItem_t * const item );
UBase_t insertWillingList_Head( List_t * const list, ListItem_t * const item );
UBase_t insertWillingList_Hehind( ListItem_t * const currentItem, ListItem_t * const item );
UBase_t getWillingListItermNum( List_t * const list );
ListItem_t * getWillingListCurrentItem( List_t * const list );
ListItem_t * getWillingListNextItem( List_t * const list );
ListItem_t * getWillingListPreviousItem( List_t * const list );
ListItem_t * getWillingListHeadItem( List_t * const list );
ListItem_t * getWillingListTailItem( List_t * const list );
