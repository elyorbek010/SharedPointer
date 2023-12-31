#ifndef MY_ATOMIC_H
#define MY_ATOMIC_H

namespace my_atomic 
{
template<typename T>
class atomic {
private:
	alignas(alignof(T)) T value;

public:
	atomic()
		: value(0)
	{ }

	atomic(const atomic& orig)
		: value(orig.value)
	{ }

	atomic(const T& value)
		: value(value)
	{ }

	~atomic() 
	{ }

	atomic& operator++()
	{
		__atomic_fetch_add(&value, 1, __ATOMIC_SEQ_CST);
		return *this;
	}

	atomic operator++(int) 
	{
		atomic ret(*this);
		++*this;
		return ret;
	}

	atomic& operator--()
	{
		__atomic_fetch_add(&value, -1, __ATOMIC_SEQ_CST);
		return *this;
	}

	atomic operator--(int)
	{
		atomic<T> ret(*this);
		--*this;
		return ret;
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

	atomic& operator=(const atomic& rhs)
	{
		T tmp = __atomic_load_n(&rhs.value, __ATOMIC_ACQUIRE);
		__atomic_store_n(&value, tmp, __ATOMIC_RELEASE);
		return *this;
	}

	atomic operator+=(const atomic& rhs)
	{
		T tmp = __atomic_load_n(&rhs.value, __ATOMIC_ACQUIRE);
		__atomic_fetch_add(&value, tmp, __ATOMIC_SEQ_CST);
		return *this;
	}

	atomic operator-=(const atomic& rhs)
	{
		T tmp = -__atomic_load_n(&rhs.value, __ATOMIC_ACQUIRE);
		__atomic_fetch_add(&value, tmp, __ATOMIC_SEQ_CST);
		return *this;
	}

	operator int const() 
	{
		return __atomic_load_n(&value, __ATOMIC_ACQUIRE);
	}

	bool cmpnxchg(T& expected, T desired)
	{
		return  __atomic_compare_exchange_n(&value, &expected, desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
	}
};
};

#endif // MY_ATOMIC_H