#ifndef __memcont_h__
#define __memcont_h__
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
template <class Type>
class MemoryController{
    int RealSize;
    int Size;
    void *Ptr;
    public:
    int resize(int new_size);
    int free_all();
    Type *getPtr(){return (Type *)Ptr;}
	Type *pointer_to(int i){return &(((Type *)Ptr)[i]);}
    MemoryController();
    virtual ~MemoryController();
    int size(){return Size;}
    int length(){return Size;}
    int getRealSize(){return RealSize;}
	//void set_access_slice(int _start=0,int _step=1);
	//void set_step(int _step){step = _step;}
	Type & operator[](int index);
	void set(int x){memset(Ptr,x,RealSize*sizeof(Type));}
	//Type & slice(int index);
    //template <class T> MemoryController<Type> & operator=(const MemoryController<T> & src );
};
/*
template <class Type> Type & MemoryController::access_slice(int index){
	int new_index = start + index*step;
	return Ptr[new_index];
}

template <class Type,class T> MemoryController<Type> & operator=(const MemoryController<T> & src ){
	if(this == &src)return *this;
	resize(Src.Size);
	start = Src.start;
	step = Src.step;
	for(int i = 0;i < Size;i++)Ptr[i] = Src.Ptr[i];
	return *this;
}
*/
template <class Type> MemoryController<Type>::MemoryController(){
    RealSize = Size = 0;
    Ptr = NULL;
}
template <class Type> int MemoryController<Type>::free_all(){
    free(Ptr);
    RealSize = 0;
    Size = 0;
    Ptr = NULL;
	return 0;
}
template <class Type> MemoryController<Type>::~MemoryController(){free_all();}
template <class Type> int  MemoryController<Type>::resize(int new_size){
	
    if(new_size > RealSize){
        Ptr = (Type *)realloc((void *)Ptr, new_size*sizeof(Type) );
        RealSize = new_size;
        Size = new_size;
    }else{
        Size = new_size;
    }
	if(!Ptr)RealSize = 0;
	return RealSize;
}
//template <class Type> Type *MemoryController<Type>::getPtr(){return (Size >0)? Ptr : NULL;}

template <class Type> Type & MemoryController<Type>::operator[](int index) {
	if(index < Size)return ((Type *)Ptr)[index];
	return ((Type *)Ptr)[(index+index/Size)%Size];
}

#endif
