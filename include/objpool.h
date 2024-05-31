#pragma once
#include "mempool.h"
#include <type_traits>

namespace netco
{
    template <class T>
    class objPool
    {
    public:
        objPool()
        {}
        ~objPool() {}
        template<typename... Args>
        inline T* new_obj(Args... args);
        inline void delete_obj(void* obj);
    private:
        template<typename... Args>
		inline T* new_aux(std::true_type, Args... args);

		template<typename... Args>
		inline T* new_aux(std::false_type, Args... args);

		inline void delete_aux(std::true_type, void* obj);

		inline void delete_aux(std::false_type, void* obj);

		MemPool<sizeof(T)> memPool_;
    };
}

namespace netco
{
    template<class T>
    template<typename... Args>
    inline T* objPool<T>::new_aux(std::true_type, Args... args)
    {
        return static_cast<T*> ( memPool_.allocAMemBlock() );
    }

    template<class T>
    template<typename... Args>
    inline T* objPool<T>::new_aux(std::false_type, Args... args)
    {
        void* newPos = memPool_.allocAMemBlock();
        return new(newPos) T(args...);
    }

    template<class T>
    template<typename... Args>
    inline T* objPool<T>::new_obj(Args... args)
    {
        return new_aux(std::integral_constant<bool, std::is_trivially_constructible<T>::value>(), args...);
    }


    template<class T>
    inline void objPool<T>::delete_obj(void* obj)
    {
        if (!obj)
        {
            return;
        }
        delete_aux(std::integral_constant<bool, std::is_trivially_destructible<T>::value>(), obj);
    }

    template<class T>
    inline void objPool<T>::delete_aux(std::true_type, void* obj)
    {
        memPool_.freeAMemBlock(obj);
    }

    template<class T>
    inline void objPool<T>::delete_aux(std::false_type, void* obj)
    {
        (static_cast<T*>(obj))->~T();
        memPool_.freeAMemBlock(obj);
    }

}