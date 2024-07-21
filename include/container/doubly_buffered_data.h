#pragma once
#include <pthread.h>
#include <atomic>
#include <memory>
#include <map>
#include <cassert>
#include "../mutex_guard.h"

namespace netco{
    extern __thread int threadIdx;
    /**
     * @brief A thread-safe data structure that provides two views of the same data, 
     * allowing concurrent reads and writes, 
     * based on the threadlocal mutex locks.
     * use ScopePtr and Read(ScopePtr&) to read
     * use Modify(Fn&& fn) to write
     * @tparam T The type of the data to be stored.
     */
    template<typename T>
    class DoublyBufferedData{
    public:
        class Wrapper;
        class ScopePtr;
        using UniqueScopePtr = std::unique_ptr<ScopePtr>;
        DoublyBufferedData() : _index(0) {
            _data[0] = T();
            _data[1] = T();
        }
        ~DoublyBufferedData(){
        }
        UniqueScopePtr GetDataPtr() {
            UniqueScopePtr s_ptr(new ScopePtr());
            Read(*s_ptr);
            return s_ptr;
        }
        // 读取数据，在不需要使用时及时释放ScopePtr，ScopePtr会占用thread local锁
        void Read(ScopePtr& s_ptr);
        // 写数据，调用fn(T& data)函数进行对应写操作
        template<typename Fn>
        void Modify(Fn&& fn);
        // 写数据，用Closure包装带参数的函数
        template<typename Fn, typename Arg1>
        void Modify(Fn&& fn, Arg1&& arg1);
        // 写数据，用Closure包装带参数的函数
        template<typename Fn, typename Arg1, typename Arg2>
        void Modify(Fn&& fn, Arg1&& arg1, Arg2&& arg2);
        // 写数据，调用fn(T& bg, const T& fg)函数进行对应写操作
        template<typename Fn>
        void ModifyWithForeground(Fn&& fn);
        // 写数据，用WithFg包装带参数的函数
        template<typename Fn, typename Arg1>
        void ModifyWithForeground(Fn&& fn, Arg1&& arg1);
        // 写数据，用WithFg包装带参数的函数
        template<typename Fn, typename Arg1, typename Arg2>
        void ModifyWithForeground(Fn&& fn, Arg1&& arg1, Arg2&& arg2);
        

    private:
        template<typename Fn, typename Arg1>
        struct Closure1{
            Closure1(Fn&& fn, Arg1&& arg1) : _fn(std::forward<Fn>(fn)), _arg1(std::forward<Arg1>(arg1)){}
            size_t operator()(T& data){
                return _fn(data, _arg1);
            }
        private:
            Fn _fn;
            Arg1 _arg1;
        };
        template<typename Fn, typename Arg1, typename Arg2>
        struct Closure2{
            Closure2(Fn&& fn, Arg1&& arg1, Arg2&& arg2) : _fn(std::forward<Fn>(fn)), _arg1(std::forward<Arg1>(arg1)), _arg2(std::forward<Arg2>(arg2)){}
            size_t operator()(T& data){
                return _fn(data, _arg1, _arg2);
            }
        private:
            Fn _fn;
            Arg1 _arg1;
            Arg2 _arg2;
        };
        template<typename Fn>
        struct WithFg0{
            WithFg0(Fn&& fn, T* data) : _fn(std::forward<Fn>(fn)), _data(data){}
            size_t operator()(T& bg){
                return _fn(bg, (const T&)_data[&bg != _data]);
            }
        private:
            Fn _fn;
            T* _data;
        };
        template<typename Fn, typename Arg1>
        struct WithFg1{
            WithFg1(Fn&& fn, T* data, Arg1&& arg1) : _fn(std::forward<Fn>(fn)), _data(data), _arg1(std::forward<Arg1>(arg1)){}
            size_t operator()(T& bg){
                return _fn(bg, (const T&)_data[&bg != _data], _arg1);
            }
        private:
            Fn _fn;
            T* _data;
            Arg1 _arg1;
        };
        template<typename Fn, typename Arg1, typename Arg2>
        struct WithFg2{
            WithFg2(Fn&& fn, T* data, Arg1&& arg1, Arg2&& arg2) : _fn(std::forward<Fn>(fn)), _data(data), _arg1(std::forward<Arg1>(arg1)), _arg2(std::forward<Arg2>(arg2)){}
            size_t operator()(T& bg){
                return _fn(bg, (const T&)_data[&bg == _data], _arg1, _arg2);
            }
        private:
            Fn _fn;
            T* _data;
            Arg1 _arg1;
            Arg2 _arg2;
        };
        const T* UnsafeRead();
        std::shared_ptr<Wrapper> create_or_get_wrapper();
        void RemoveWrapper(std::shared_ptr<Wrapper>);

    private:
        T _data[2];
        std::atomic<int> _index;
        std::map<int, std::shared_ptr<Wrapper>> _wrappers_map;
        MutexLock _wrappers_mutex;
        MutexLock _modify_mutex;

    };


    template<typename T>
    class DoublyBufferedData<T>::Wrapper : public std::enable_shared_from_this<typename DoublyBufferedData<T>::Wrapper> {
    friend class DoublyBufferedData;
    public:
        explicit Wrapper() : _control(nullptr) {
        }
        ~Wrapper(){
        }
        inline std::shared_ptr<Wrapper> get_shared_from_this() {
            return std::enable_shared_from_this<typename DoublyBufferedData<T>::Wrapper>::shared_from_this();
        }
        void StartRead() {
            _mutex.lock();
        }
        void EndRead() {
            _mutex.unlock();
        }
        inline void WaitReadDone() {
            MutexGuard guard(_mutex);
        }

    private:
        DoublyBufferedData* _control;        
        MutexLock _mutex;
    };

    template<typename T>
    class DoublyBufferedData<T>::ScopePtr
    {
    friend class DoublyBufferedData;
    public:
        ScopePtr() : _ptr(nullptr) {}
        ~ScopePtr(){
            if(_wrapper)
            {
                _wrapper->EndRead();
            }
        }
        const T* get() const { return _ptr; }
        const T& operator*() const { return *_ptr; }
        const T* operator->() const { return _ptr; }
    private:
        const T* _ptr;
        std::shared_ptr<Wrapper> _wrapper;
    };


    template<typename T>
    void DoublyBufferedData<T>::Read(ScopePtr& s_ptr) {
        std::shared_ptr<Wrapper> wrapper = create_or_get_wrapper();
        wrapper->StartRead();
        s_ptr._wrapper = wrapper;
        s_ptr._ptr = UnsafeRead();        
    }

    template<typename T>
    const T* DoublyBufferedData<T>::UnsafeRead() {
        int idx = _index.load(std::memory_order_relaxed);
        return &_data[idx];
    }

    template<typename T>
    std::shared_ptr<typename DoublyBufferedData<T>::Wrapper> DoublyBufferedData<T>::create_or_get_wrapper() {
        int idx = threadIdx;
        MutexGuard lock(_wrappers_mutex);
        auto it = _wrappers_map.find(idx);
        if(it != _wrappers_map.end())
            return it->second;
        std::shared_ptr<typename DoublyBufferedData<T>::Wrapper> wrapper = std::make_shared<typename DoublyBufferedData<T>::Wrapper>();
        wrapper->_control = this;
        _wrappers_map[idx] = wrapper;
        return wrapper;
    }

    template<typename T>
    void DoublyBufferedData<T>::RemoveWrapper(std::shared_ptr<Wrapper> wrapper) {
        for(auto it = _wrappers_map.begin(); it != _wrappers_map.end(); ++it){
            if(it->second == wrapper)
            {
                _wrappers_map.erase(it);
                return;
            }
        }
    }

    template<typename T>
    template<typename Fn>
    void DoublyBufferedData<T>::Modify(Fn&& fn) {
        MutexGuard lock(_modify_mutex);
        int bg_idx = !_index.load(std::memory_order_relaxed);
        size_t ret0 = fn(_data[bg_idx]);
        _index.store(bg_idx, std::memory_order_relaxed);
        {      
            MutexGuard lock(_wrappers_mutex);          
            for(auto it = _wrappers_map.begin(); it != _wrappers_map.end(); ++it){
                it->second->WaitReadDone();
            }
        }
        bg_idx = !bg_idx;
        size_t ret1 = fn(_data[bg_idx]);
        assert(ret0 == ret1);
    }

    template<typename T>
    template<typename Fn, typename Arg1>
    void DoublyBufferedData<T>::Modify(Fn&& fn, Arg1&& arg1) {
        Closure1<Fn, Arg1> closure(std::forward<Fn>(fn), std::forward<Arg1>(arg1));
        Modify(std::move(closure));
    }

    template<typename T>
    template<typename Fn, typename Arg1, typename Arg2>
    void DoublyBufferedData<T>::Modify(Fn&& fn, Arg1&& arg1, Arg2&& arg2) {
        Closure2<Fn, Arg1, Arg2> closure(std::forward<Fn>(fn), std::forward<Arg1>(arg1), std::forward<Arg2>(arg2));
        Modify(std::move(closure));
    }
    template<typename T>
    template<typename Fn>
    void DoublyBufferedData<T>::ModifyWithForeground(Fn&& fn) {
        WithFg0<Fn> closure(std::forward<Fn>(fn), _data);
        Modify(std::move(closure));
    }
    template<typename T>
    template<typename Fn, typename Arg1>
    void DoublyBufferedData<T>::ModifyWithForeground(Fn&& fn, Arg1&& arg1) {
        WithFg1<Fn, Arg1> closure(std::forward<Fn>(fn), _data, std::forward<Arg1>(arg1));
        Modify(std::move(closure));
    }
    template<typename T>
    template<typename Fn, typename Arg1, typename Arg2>
    void DoublyBufferedData<T>::ModifyWithForeground(Fn&& fn, Arg1&& arg1, Arg2&& arg2) {
        WithFg2<Fn, Arg1, Arg2> closure(std::forward<Fn>(fn), _data, std::forward<Arg1>(arg1), std::forward<Arg2>(arg2));
        Modify(std::move(closure));
    }
}