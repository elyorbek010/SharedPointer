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
	class shared_ptr
	{
	private:
		cb<T>* cb_ptr;

	public:
		shared_ptr(T* ptr)
			: cb_ptr(new cb(ptr))
		{ }

		shared_ptr(const shared_ptr<T>& orig)
		{
			orig.cb_ptr->increment_shared();
			cb_ptr = orig.cb_ptr;
		}

		shared_ptr(cb<T>* cb_ptr)
			: cb_ptr(cb_ptr)
		{ }

		~shared_ptr()
		{
			cb_ptr->decrement_shared();
		}
		
		size_t use_count() const
		{
			return cb_ptr->get_count();
		}

		size_t unique() const
		{
			return use_count() == 1;
		}

		T* get() const
		{
			return cb_ptr->get_obj();
		}
		
		shared_ptr<T>& operator=(const shared_ptr<T>& rhs)
		{
			rhs.cb_ptr->increment_shared();
			this->cb_ptr->decrement_shared();

			cb_ptr = rhs.cb_ptr;

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
		}

	public:

		cb(T* ptr, bool mk_sh = false)
			: obj_ptr(ptr)
			, count(1)
			, weak_count(0)
			, expired(0)
			, mk_sh(mk_sh)
		{}

		~cb()
		{ }

		bool increment_shared()
		{
			if (!expired)
				count++;
			else
				return false;

			return true;
		}

		void decrement_shared()
		{
			if (--count == 0) {
				expired = true;
				delete_obj();

				if (weak_count == 0) {
					delete this;
				}
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

		void decrement_weak() {
			if (--weak_count == 0) {
				if (expired)
					delete this;
			}
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