#include "list.h"


void initWillingList( List_t * const list ) {
    if ( list == NULL ) {
        return;
    }

    list->itemNum = 0;
    list->head = NULL;
    list->tail = NULL;
    // list->current = NULL;
}


UBase_t insertWillingList_Sort(  List_t * const list, ListItem_t * const item  ){
    UBase_t itemNums = 0;
	  ListItem_t * nextItem = NULL;
	ListItem_t * currentItem = NULL;
    /* 判断参数的代码在这个级别上来说不一定有用，一是降低了效率，
    二是系统级别的代码如果参数都存在问题，是不可想象的
    三是变量在分配内存后不必要立即进行初始化，因此tcbWith可能是无效的任意值不一定为NULL */
    if ( list == NULL  || item == NULL || item->tcbWith == NULL ) {
        return 0;
    }

    currentItem = list->head;
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
    itemNums = insertWillingList_Behind( list, currentItem, item );
    return itemNums;
}


UBase_t insertWillingList_Tail( List_t * const list, ListItem_t * const item ) {
    if ( list == NULL || item == NULL ) {
        return 0;
    }
		
		item->listWith = list;

    if ( list->tail == NULL ) {
        list->head = item;
        item->previous = NULL;
    } else {
        list->tail->next = item;
    }

    list->tail = item;
    item->next = NULL;
    list->itemNum++;
    return list->itemNum;
}


UBase_t insertWillingList_Head( List_t * const list, ListItem_t * const item ) {
    if ( list == NULL || item == NULL ) {
        return 0;
    }
		item->listWith = list;

    if ( list->head == NULL ) {
        list->head = item;
        list->tail = item;
			  item->next = NULL;
    } else {
        item->next = list->head;
        list->head = item;
    }

    item->previous = NULL;
    list->itemNum++;
    return list->itemNum;
}


UBase_t insertWillingList_Behind( List_t * const list, ListItem_t * const currentItem, ListItem_t * const item ) {
    if ( item == NULL || list == NULL ) {
        return 0;
    }

     item->listWith = list;

    if ( currentItem == NULL ) {
			insertWillingList_Head( list, item );
//        item->next = list->head;
//        item->previous = NULL;
//        list->head = item;
    } else {
        if ( currentItem->listWith != list ) {
            return 0;
        }

        item->next = currentItem->next;
				currentItem->next = item;
        item->previous = currentItem;
    
        if ( item->next == NULL ) {
            list->tail = item;
        }
				
				list->itemNum++;
    }

    
    return list->itemNum;
}


UBase_t getWillingListItemNum( List_t * const list ) {
    if ( list == NULL ) {
        return 0;
    }

    return list->itemNum;
}


// ListItem_t * getWillingListCurrentItem( List_t * const list ) {
//     if ( list == NULL ) {
//         return 0;
//     }

//     if ( list->current == NULL ) {
//         list->current = list->head;
//     }

//     return list->current;
// }

ListItem_t * getWillingListNextItem( List_t * const list , ListItem_t * const item) {
    if ( list == NULL || item == NULL || item->listWith != list ) {
        return NULL;
    }

    return item->next;
}

ListItem_t * getWillingListNextItem_Circle( List_t * const list, ListItem_t * const item ) {
        if ( list == NULL || item == NULL || item->listWith != list ) {
        return NULL;
    }

    if ( item->next == NULL ) {
        return list->head;
    }

    return item->next;
}

ListItem_t * getWillingListPreviousItem( List_t * const list, ListItem_t * const item ) {
    if ( list == NULL || item == NULL || item->listWith != list ) {
        return NULL;
    }

    return item->previous;
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

// ListItem_t * moveWillingListCursorNext( List_t * const list ) {

// }

// ListItem_t * moveWillingListCursorPrevious( List_t * const list ) {

// }

// ListItem_t * moveWillingListCursorTo( List_t * const list, ListItem_t * const item ) {

// }

UBase_t removeWillingListItem( List_t * const list, ListItem_t * const item ) {
    if ( list == NULL || item == NULL || item->listWith != list ) {
        return 0;
    }

//  只有一个元素的情况
    if ( item->next == NULL && item->previous == NULL ) {
        list->head = NULL;
        list->tail = NULL;
        list->itemNum = 0;
					item->next = NULL;
		item->previous = NULL;
        return list->itemNum;
    }

// 删除tail
    if ( item->next == NULL ) {
        ListItem_t * previousItem = item->previous;
        previousItem->next = NULL;
        list->tail = previousItem;
        list->itemNum--;
					item->next = NULL;
		item->previous = NULL;
        return list->itemNum;
    }

// 删除head
    if ( item->previous == NULL ) {
        list->head = item->next;
        list->head->previous = NULL;
        list->itemNum--;
        item->next = NULL;
		item->previous = NULL;
       return list->itemNum;
    }
// 删除中间
    item->previous->next = item->next;
    item->next->previous = item->previous;
    list->itemNum--;
		item->next = NULL;
		item->previous = NULL;
    return list->itemNum;
}
