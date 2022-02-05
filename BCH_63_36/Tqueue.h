//-----------------------------------------------------------------------
// File Name:TQueue.h  
// Class Name:TQueue.h
// Base Class:no
// Aggregate Classes : struct Item
// Depend on files : no
//                  
// Implementation File: no
// Designer: A. Zhdanov
//
// Purpose and Rationale:
// Queue template class 
//-----------------------------------------------------------------------  
// 
// Attributes:
//  int CashSize; - complete numbebr of elements in list
//  int Length; - current quequ length, number of engaged elements
//    
//    struct Item{ - element of one-link list
//    Attributes:
//        Type Data; - data container
//        Item *Next; - pointer to next element
//        Methods:
//        Item(){Next = NULL;}
//        Item(Item *ptr){Next = ptr;}  - constructors
//        };
//    Item *UpperEnd,*LowEnd; - the first and the last element of one-link list
//    Item *CurrentPtr; - current element of one-link list
//   
//
// Methods:
// TQueue(int cashsize = 1); - constructor with pre-set buffer
//    TQueue& operator<<(Type x); - operating analog of insert() 
//    TQueue& operator=(TQueue& ); - operator  = for queues
//    Type remove(); - take element from queue
//    int getItemNumber() - return CashSize
//    int getCurrentLength() - return Length
//    void insert(Type x);- add element to queue
//    virtual ~TQueue(); - destructor, the operator delete is called in destructor only
//
//NOTE:
//Template class, implementing the queue algorithm
//Main methods:
//     void insert(Type x);
//     Type remove();
//The constructor argument is an initial number of free items
//In the course of the operation insert a free item is declared to be 
//occupied,if no free item, a new item is created.
//In the course of the operation remove, an occupied item is declared to 
//be free and the destructor is not called.     
//-----------------------------------------------------------------------
// Ver 1.0             Mar 30/98            A.Zhdanov
//-----------------------------------------------------------------------
#ifndef __tqueue_h__
#define __tqueue_h__
#include <stdlib.h>
#include <fstream>
using namespace std;
template <class Type>
class TQueue{
    protected:
    int CashSize;
    int Length;
    
    struct Item{
        Type Data;
        Item *Next;
        
        Item(){Next = NULL;}
        Item(Item *ptr){Next = ptr;}
    };
    Item *UpperEnd,*LowEnd;
    Item *CurrentPtr;
    public:
    TQueue(int cashsize = 1);
    TQueue& operator<<(Type x);
    TQueue& operator=(TQueue& );
    virtual Type remove();
    int getItemNumber(){return CashSize;}
    int getCurrentLength(){return Length;}
    int length(){return getCurrentLength();}
    virtual void insert(Type x);
	virtual void release();
    int ReadFromFile32(const char *fname);
    int ReadFromFileULL(const char *fname);
    virtual ~TQueue();
};
template <class Type>
 TQueue<Type>&  TQueue<Type>::operator=(TQueue<Type> &src){
    for(;;){
       CurrentPtr = UpperEnd;
       if(CurrentPtr->Next == NULL)break;
        UpperEnd = CurrentPtr->Next;
        delete CurrentPtr;
    }
        // set queue to zero length
    //LowEnd->Data = src.LowEnd->Data;
    int i;
    for( i = 0;i < src.CashSize;i++){
        UpperEnd = new Item(UpperEnd);
    }
        // create the queue with the same length as src
    Item *tmpptrmy = UpperEnd;
    Item *tmpptrhis = src.UpperEnd;
    for(i = 0;i < src.CashSize;i++){
        tmpptrmy->Data = tmpptrhis->Data;
        if(tmpptrhis == src.CurrentPtr)CurrentPtr = tmpptrmy;
        tmpptrmy = tmpptrmy->Next;
        tmpptrhis = tmpptrhis->Next;
        
    }
        // data attributing
    CashSize = src.CashSize;
        Length = src.Length;
    return *this;
}
    
    
template <class Type> TQueue<Type>::~TQueue(){
    for(;;){
       CurrentPtr = UpperEnd;
       if(CurrentPtr->Next == NULL)break;
        UpperEnd = CurrentPtr->Next;
        delete CurrentPtr;
    }
    delete LowEnd;
}
template <class Type> TQueue<Type>::TQueue(int cashsize){
    
    if(cashsize < 0)cashsize = 0;
    CashSize = cashsize;
    UpperEnd = new Item;
    CurrentPtr = UpperEnd;
    LowEnd = CurrentPtr;
    while(cashsize-- > 0){
        CurrentPtr->Next = new Item;
        LowEnd = CurrentPtr;
        CurrentPtr = CurrentPtr->Next;
    }
    CurrentPtr = UpperEnd;
    Length = 0;
}
template <class Type> void TQueue<Type>::insert(Type x){
    CurrentPtr->Data = x;Length++;
    if(!CurrentPtr->Next){
        CurrentPtr->Next = new Item;CashSize++;
        LowEnd = CurrentPtr = CurrentPtr->Next;
    }
        //  create new object if insufficient memory in buffer
    else{
        CurrentPtr = CurrentPtr->Next;
    }
    // move pointer if sufficient memory in buffer
}
template <class Type> void TQueue<Type>::release(){
	CurrentPtr=UpperEnd;Length = 0;
}
template <class Type>Type TQueue<Type>::remove(){
    Type out = UpperEnd->Data;Length--;
    if(UpperEnd == CurrentPtr){Length = 0;return (out = 0);}
        // if more elements than available are attempted to be taken, return zero
    Item *tmp = UpperEnd->Next;
    LowEnd->Next = UpperEnd;
    UpperEnd->Next = NULL;
    LowEnd = UpperEnd;
    UpperEnd = tmp; 
    // released element is moved to the end of queue
    return out;
}
template <class Type>
 TQueue<Type>&  TQueue<Type>::operator<<(Type input){
    insert(input);
    return *this;
}
template <class Type> int TQueue<Type>::ReadFromFile32(const char *fname){  
    //Open the file and check for its existance
    int count = 0;
    ifstream file(fname, ios::in); if(!file.is_open()) { return -1;}
    //File reading circle 
    Type Buffer;
    while(1) {
        
       file>>Buffer;if(file.eof())break; 
       insert(Buffer);count++; 
     
    }
    return count;
}
template <class Type> int TQueue<Type>::ReadFromFileULL(const char *fname){  
    FILE *fp=fopen(fname,"r");
    int count=0;
    unsigned long long Buffer=0;
    
    while(1){
      int num=fscanf(fp,"%llu\n",&Buffer);
      if(num!=1)break;
      insert(Buffer);count++;
    }
    
    fclose(fp);
    return count;
}
#endif
