#ifndef MY_ATOMIC_H
#define MY_ATOMIC_H

namespace my_atomic 
{
	template<typename T>
	class atomic {
	private:
		T value;

	public:
		atomic()
			: value(0)
		{ }

		atomic(const atomic<const T>& orig)
			: value(orig.value)
		{ }

		atomic(const T& value)
			: value(value)
		{ }

		~atomic() 
		{ }

		atomic<T>& operator++()
		{
			__atomic_fetch_add(&value, 1, __ATOMIC_SEQ_CST);
			return *this;
		}

		atomic<T> operator++(int) 
		{
			__atomic_fetch_add(&value, 1, __ATOMIC_SEQ_CST);
			return *this;
		}

		atomic<T>& operator--()
		{
			__atomic_fetch_add(&value, -1, __ATOMIC_SEQ_CST);
			return *this;
		}

		atomic<T> operator--(int)
		{
			__atomic_fetch_add(&value, -1, __ATOMIC_SEQ_CST);
			return *this;
		}

		bool operator==(const atomic<T>& rhs) const
		{
			return value == rhs.value;
		}

		bool operator!=(const atomic<T>& rhs) const
		{
			return !(this == rhs);
		}

		bool operator==(const int& rhs) const
		{
			return __atomic_load_n(&value, __ATOMIC_ACQUIRE) == __atomic_load_n(&rhs, __ATOMIC_ACQUIRE);
		}

		bool operator!=(const int& rhs) const
		{
			return !(*this == rhs);
		}

		atomic<T>& operator=(const atomic<T>& rhs)
		{
			T tmp = __atomic_load_n(&rhs.value, __ATOMIC_ACQUIRE);
			__atomic_store_n(&value, tmp, __ATOMIC_RELEASE);
			return *this;
		}

		atomic<T> operator+=(const atomic<T>& rhs)
		{
			T tmp = __atomic_load_n(&rhs.value, __ATOMIC_ACQUIRE);
			__atomic_fetch_add(&value, tmp, __ATOMIC_SEQ_CST);
			return *this;
		}

		atomic<T> operator-=(const atomic<T>& rhs)
		{
			T tmp = -__atomic_load_n(&rhs.value, __ATOMIC_ACQUIRE);
			__atomic_fetch_add(&value, tmp, __ATOMIC_SEQ_CST);
			return *this;
		}

		operator int const() 
		{
			return __atomic_load_n(&value, __ATOMIC_ACQUIRE);
		}
	};
};

#endif // MY_ATOMIC_H