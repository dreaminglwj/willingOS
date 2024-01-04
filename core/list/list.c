#include "lsit.h"


void initWillingList( List_t * const list ) {
    if ( list == NULL ) {
        return;
    }

    list->itemNum = 0;
    list->head = NULL;
    list->tail = NULL;
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
        list->tail = item;
        item->next = NULL;
        item->previous = NULL;
        item->listWith = list;

        list->itemNum++;
        return list->itemNum;
    }

    /* 查找tickCountSession */
    if ( currentItem->tickCountSession != item->tickCountSession ) {
        while ( currentItem->tickCountSession == item->tickCountSession ) {
            if ( currentItem->next != NULL ) {
                currentItem = currentItem->next;
            } else {
                break;
            }
        }
    }

    /* 查找插入位置 */
    while ( currentItem->next == NULL ) {
        nextItem = currentItem->next;
        if ( item->sortValue >= currentItem->sortValue && item->sortValue < nextItem->sortValue ) {
            break;
        } else {
            currentItem->next = nextItem;
        }
    }
    
    /* 插入 */
    itemNums = insertWillingList_Hehind( list, currentItem, item );
    return itemNums;
}


// UBase_t insertWillingList_End( List_t * const list, ListItem_t * const item ) {
//     if ( list == NULL || item == NULL ) {
//         return 0;
//     }


// }


// UBase_t insertWillingList_Head( List_t * const list, ListItem_t * const item );
UBase_t insertWillingList_Behind( List_t * const list, ListItem_t * const currentItem, ListItem_t * const item ) {
    if ( currentItem == NULL || item == NULL || list == NULL ) {
        return 0;
    }

    if ( currentItem->listWith != list ) {
        return 0;
    }

    item->next = currentItem->next;
    item->previous = currentItem;
    item->listWith = list;

    if ( item->next == NULL ) {
        list->tail = item;
    }

    list->itemNum++;
    return list>itemNum;
}

UBase_t getWillingListItemNum( List_t * const list ) {
    if ( list == NULL ) {
        return 0;
    }

    return list->itemNum;
}


ListItem_t * getWillingListCurrentItem( List_t * const list ) {
    if ( list == NULL ) {
        return 0;
    }

    if ( list->current == NULL ) {
        list->current = list->head;
    }

    return list->current;
}

ListItem_t * getWillingListNextItem( List_t * const list ) {
    ListItem_t * currentItem = getWillingListCurrentItem(list);
    if ( currentItem == NULL ) {
        return NULL;
    }

    return currentItem->next;

    // list->current = currentItem->next;
    // return list->current;
}

ListItem_t * getWillingListPreviousItem( List_t * const list ) {
    ListItem_t * currentItem = getWillingListCurrentItem(list);
    if ( currentItem == NULL ) {
        return NULL;
    }

    return currentItem->previous;

    // list->current = currentItem->previous;
    // return list->current;
}

ListItem_t * getWillingListHeadItem( List_t * const list ) {
    if ( list == NULL ) {
        return NULL;
    }

    return list->head;
}

ListItem_t * getWillingListTailItem( List_t * const list ) {
    if ( list == NULL ) {
        return NULL;
    }

    return list->tail;
}

ListItem_t * moveWillingListCursorNext( List_t * const list ) {

}

ListItem_t * moveWillingListCursorPrevious( List_t * const list ) {

}

ListItem_t * moveWillingListCursorTo( List_t * const list, ListItem_t * const item ) {

}

UBase_t removeWillingListItem( List_t * const list, ListItem_t * const item ) {

}
