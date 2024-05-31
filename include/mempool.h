#pragma once
#include <stddef.h>
#include <cstdlib>
#include "parameter.h"

namespace netco
{
    /* 内存块链表结点结构体 */
    struct MemBlockNode{
        union 
        {
            MemBlockNode* next;
            char data;
        };        
    };
    /* 内存池类, mallocListHead_存储了整块连续内存的首地址链表,freeListHead_链表存储了可用的objSize_大小的内存块的指针*/
    template<size_t objsize>
    class MemPool
    {
    public:
        MemPool()
        :freeListHead_(nullptr), mallocListHead_(nullptr), mallocTimes_(0)
        {
            if(objsize<sizeof(MemBlockNode))
            {    
                objSize_ = sizeof(MemBlockNode);
            }
            else
            {    
                objSize_ = objsize;
            }
        }        
        ~MemPool()
        {
            while(mallocListHead_){
                MemBlockNode* to_free = mallocListHead_;
                mallocListHead_ = mallocListHead_->next;
                free( static_cast<void*> (to_free) );
            }
        }
        void* allocAMemBlock();
        void freeAMemBlock(void* block);

    private:
        size_t objSize_;
        size_t mallocTimes_;
        MemBlockNode* freeListHead_;
        MemBlockNode* mallocListHead_;

    };
}

namespace netco
{
    template <size_t objsize>
    void* MemPool<objsize>::allocAMemBlock()
    {
        void* ret;
        // 没有空闲的内存块了，分配新的内存块
        if( nullptr == freeListHead_ )
        {
            int objCnt = parameter::memPoolMallocObjCnt + mallocTimes_;
            void* newMemBlock = malloc( objCnt * objSize_ + sizeof(MemBlockNode));
            // 连接新内存块和旧内存块链表,表头为mallocListHead_
            MemBlockNode* newMallocHead = static_cast<MemBlockNode*> (newMemBlock);
            newMallocHead->next = mallocListHead_;
            mallocListHead_ = newMallocHead;
            newMemBlock = static_cast<char*> (newMemBlock) + sizeof(MemBlockNode);        

            // 将新内存块划分为多个对象大小的内存，并以链表存于freeListHead_
            for(int i = 0; i < objCnt; i++)
            {
                MemBlockNode* newObjHead = static_cast<MemBlockNode*> (newMemBlock);
                newObjHead->next = freeListHead_;
                freeListHead_ = newObjHead;
                newMemBlock = static_cast<char*> (newMemBlock) + objSize_;
            }
            ++mallocTimes_;
        }
        ret = &freeListHead_->data;
        freeListHead_ = freeListHead_->next;
        return ret;
    }
    template <size_t objsize>
    void MemPool<objsize>::freeAMemBlock(void* block)
    {
        if( nullptr == block )
        {
            return;
        }
        MemBlockNode* freeMem = static_cast<MemBlockNode*> (block);
        freeMem->next = freeListHead_;
        freeListHead_ = freeMem;
    }
}