#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <stddef.h>
#include <stdint.h>

#include "my_atomic.hpp"

namespace smart_pointer 
{
template<typename T>
class cb;

template<typename T>
class weak_ptr;

template<typename T>
class shared_ptr
{
	friend class weak_ptr<T>;

private:
	cb<T>* cb_ptr;

public:
	shared_ptr()
		: cb_ptr(nullptr)
	{ }

	shared_ptr(T* ptr)
		: cb_ptr(new cb(ptr))
	{ }

	shared_ptr(const shared_ptr& orig)
	{
		orig.cb_ptr->increment_shared();
		cb_ptr = orig.cb_ptr;
	}

	shared_ptr(cb<T>* cb_ptr)
		: cb_ptr(cb_ptr)
	{ }

	~shared_ptr()
	{
		if (cb_ptr != nullptr)
			cb_ptr->decrement_shared();
	}
		
	size_t use_count() const
	{
		if (cb_ptr == nullptr)
			return 0;
		return cb_ptr->get_count();
	}

	size_t weak_count()
	{
		if (cb_ptr == nullptr)
			return 0;

		return cb_ptr->get_weak_count() - 1;
	}

	size_t unique() const
	{
		if (cb_ptr == nullptr)
			return 0;

		return use_count() == 1;
	}

	T* get() const
	{
		if (cb_ptr == nullptr)
			return nullptr;
		return cb_ptr->get_obj();
	}
		
	shared_ptr& operator=(const shared_ptr& rhs)
	{
		shared_ptr tmp(rhs);
		tmp.swap(*this);

		return *this;
	}

	T& operator*()
	{
		return *(cb_ptr->get_obj());
	}

	T* operator->()
	{
		return cb_ptr->get_obj();
	}

	const T& operator*() const
	{
		return *(cb_ptr->get_obj());
	}

	const T* operator->() const
	{
		return cb_ptr->get_obj();
	}

	void swap(shared_ptr& obj)
	{
		std::swap(this->cb_ptr, obj.cb_ptr);
	}
};

template<typename T>
class weak_ptr
{
private:
	cb<T>* cb_ptr;

public:
	weak_ptr()
		: cb_ptr(nullptr)
	{ }

	weak_ptr(const shared_ptr<T>& obj)
		: cb_ptr(obj.cb_ptr)
	{ 
		if(cb_ptr != nullptr)
			cb_ptr->increment_weak();
	}

	weak_ptr(const weak_ptr& obj)
		: cb_ptr(obj.cb_ptr)
	{
		if(cb_ptr != nullptr)
			cb_ptr->increment_weak();
	}

	~weak_ptr()
	{
		if (cb_ptr != nullptr) 
			cb_ptr->decrement_weak();
	}

	size_t use_count()
	{
		if (cb_ptr == nullptr)
			return 0;

		return cb_ptr->get_count();
	}

	size_t weak_count()
	{
		if (cb_ptr == nullptr)
			return 0;

		return cb_ptr->get_weak_count() - 1;
	}

	bool is_valid()
	{
		if (cb_ptr == nullptr || cb_ptr->is_expired())
			return false;

		return true;
	}

	weak_ptr& operator=(const shared_ptr<T>& rhs)
	{
		weak_ptr tmp(rhs);
		tmp.swap(*this);

		return *this;
	}

	weak_ptr& operator=(const weak_ptr& rhs)
	{
		weak_ptr tmp(rhs);
		tmp.swap(*this);

		return *this;
	}

	shared_ptr<T> lock()
	{
		if(!cb_ptr->is_expired())
			cb_ptr->increment_shared();
		return shared_ptr<T>(cb_ptr);
	}

	void swap(weak_ptr& obj)
	{
		std::swap(this->cb_ptr, obj.cb_ptr);
	}
};

template<typename T>
class cb
{
private:
	T* obj_ptr;
	my_atomic::atomic<size_t> count;
	my_atomic::atomic<size_t> weak_count;
	my_atomic::atomic<size_t> expired;
	const bool mk_sh; // using make_shared

	void delete_obj()
	{
		if (mk_sh)
			obj_ptr->~T();
		else
			delete obj_ptr;

		decrement_weak();
	}

public:

	cb(T* ptr, bool mk_sh = false)
		: obj_ptr(ptr)
		, count(1)
		, weak_count(1)
		, expired(0)
		, mk_sh(mk_sh)
	{}

	~cb()
	{ }

	void increment_shared()
	{
		count++;
	}

	void decrement_shared()
	{
		if (--count == 0) 
		{
			expired = true;
			delete_obj();
		}
	}

	bool increment_weak()
	{
		if (!expired)
			weak_count++;
		else
			return false;

		return true;
	}

	void decrement_weak() 
	{
		if (--weak_count == 0) 
			delete this;
	}

	T* get_obj()
	{
		return obj_ptr;
	}

	size_t get_count()
	{
		return count;
	}

	size_t get_weak_count()
	{
		return weak_count;
	}

	bool is_expired()
	{
		return expired;
	}
};

void allocate_aligned(size_t sizeof_cb, size_t sizeof_obj, size_t alignof_obj, void*& cb_begin, void*& obj_begin)
{
	cb_begin = ::operator new (sizeof_cb + sizeof_obj + alignof_obj);
	obj_begin = (cb_begin + sizeof_cb + alignof_obj) - (reinterpret_cast<uintptr_t>(obj_begin)) % alignof_obj;
}

template<typename T>
shared_ptr<T> make_shared()
{
	void* cb_begin = nullptr;
	void* obj_begin = nullptr;

	allocate_aligned(sizeof(cb<T>), sizeof(T), alignof(T), cb_begin, obj_begin);

	T* obj = new(obj_begin) T();
	cb<T>* cb_ptr = new(cb_begin) cb<T>(obj, true);

	return shared_ptr<T>(cb_ptr);
}

template<typename T, typename Arg1>
shared_ptr<T> make_shared(Arg1 arg1)
{
	void* cb_begin = nullptr;
	void* obj_begin = nullptr;

	allocate_aligned(sizeof(cb<T>), sizeof(T), alignof(T), cb_begin, obj_begin);

	T* obj = new(obj_begin) T(arg1);
	cb<T>* cb_ptr = new(cb_begin) cb<T>(obj, true);

	return shared_ptr<T>(cb_ptr);
}

template<typename T, typename Arg1, typename Arg2>
shared_ptr<T> make_shared(Arg1 arg1, Arg2 arg2)
{
	void* cb_begin = nullptr;
	void* obj_begin = nullptr;

	allocate_aligned(sizeof(cb<T>), sizeof(T), alignof(T), cb_begin, obj_begin);

	T* obj = new(obj_begin) T(arg1, arg2);
	cb<T>* cb_ptr = new(cb_begin) cb<T>(obj, true);

	return shared_ptr<T>(cb_ptr);
}

template<typename T, typename Arg1, typename Arg2, typename Arg3>
shared_ptr<T> make_shared(Arg1 arg1, Arg2 arg2, Arg3 arg3)
{
	void* cb_begin = nullptr;
	void* obj_begin = nullptr;

	allocate_aligned(sizeof(cb<T>), sizeof(T), alignof(T), cb_begin, obj_begin);

	T* obj = new(obj_begin) T(arg1, arg2, arg3);
	cb<T>* cb_ptr = new(cb_begin) cb<T>(obj, true);

	return shared_ptr<T>(cb_ptr);
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
shared_ptr<T> make_shared(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
{
	void* cb_begin = nullptr;
	void* obj_begin = nullptr;

	allocate_aligned(sizeof(cb<T>), sizeof(T), alignof(T), cb_begin, obj_begin);

	T* obj = new(obj_begin) T(arg1, arg2, arg3, arg4);
	cb<T>* cb_ptr = new(cb_begin) cb<T>(obj, true);

	return shared_ptr<T>(cb_ptr);
}
};

#endif // SHARED_PTR_H