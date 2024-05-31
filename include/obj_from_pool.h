#pragma once
#include "objpool.h"
#include <memory>
#include <cassert>

namespace netco
{
    template<class T>
    class ObjFromPool
    {
    public:
        template<typename... Args>
        ObjFromPool(objPool<T>* pool, Args&&... args)
            : myPool_(pool), myObj_(nullptr)
        {
            if (myPool_) {
                myObj_ = myPool_->new_obj(std::forward<Args>(args)...);
            }
        }
        ~ObjFromPool()
        {
            if( nullptr != myObj_)
            {
                myPool_->delete_obj(myObj_);
            }
        }
        // 提供对底层对象的访问
        T* get() const {
            return myObj_;
        }

        T* operator->() const {
            return myObj_;
        }

        T& operator*() const {
            return *myObj_;
        }
    private:
        objPool<T>* myPool_;
        T* myObj_;
    };
}