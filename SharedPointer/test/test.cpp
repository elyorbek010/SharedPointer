#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <thread>

#include "shared_pointer.hpp"

class test_class
{
public:
	int arg1;
	float arg2;
	long long arg3;
	char arg4;

	test_class() 
		: arg1(0)
		, arg2(0)
		, arg3(0)
		, arg4(0)
	{ }

	test_class(int arg1)
		: arg1(arg1)
		, arg2(0)
		, arg3(0)
		, arg4(0)
	{ }

	test_class(int arg1, float arg2)
		: arg1(arg1)
		, arg2(arg2)
		, arg3(0)
		, arg4(0)
	{ }

	test_class(int arg1, float arg2, long long arg3)
		: arg1(arg1)
		, arg2(arg2)
		, arg3(arg3)
		, arg4(0)
	{ }

	test_class(int arg1, float arg2, long long arg3, char arg4)
		: arg1(arg1)
		, arg2(arg2)
		, arg3(arg3)
		, arg4(arg4)
	{ }



	~test_class()
	{ }
};

TEST_CASE("unique", "shared_ptr")
{
	smart_pointer::shared_ptr<int> int1_ptr(new int(10));
	REQUIRE(int1_ptr.unique() == true);
	{
		smart_pointer::shared_ptr<int> int1_ptr_cpy(int1_ptr);
		REQUIRE(int1_ptr_cpy.unique() == false);
	}
	REQUIRE(int1_ptr.unique() == true);
}

TEST_CASE("use count", "shared_ptr")
{
	smart_pointer::shared_ptr<int> int1_ptr(new int(10));
	smart_pointer::shared_ptr<int> int2_ptr(new int(20));

	REQUIRE(int1_ptr.use_count() == 1);
	REQUIRE(int2_ptr.use_count() == 1);

	{
		smart_pointer::shared_ptr<int> int_ptr_cpy(int1_ptr);
		REQUIRE(int1_ptr.use_count() == 2);
		REQUIRE(int2_ptr.use_count() == 1);

		int_ptr_cpy = int2_ptr;
		REQUIRE(int1_ptr.use_count() == 1);
		REQUIRE(int2_ptr.use_count() == 2);
	}

	REQUIRE(int1_ptr.use_count() == 1);
	REQUIRE(int2_ptr.use_count() == 1);
}

TEST_CASE("value check", "shared_ptr")
{
	smart_pointer::shared_ptr<int> int1_ptr(new int(10));
	smart_pointer::shared_ptr<int> int2_ptr(new int(20));

	REQUIRE(int1_ptr.use_count() == 1);
	REQUIRE(int2_ptr.use_count() == 1);

	{
		smart_pointer::shared_ptr<int> int_ptr_cpy(int1_ptr);
		REQUIRE(int1_ptr.use_count() == 2);
		REQUIRE(int2_ptr.use_count() == 1);
		REQUIRE(*int_ptr_cpy == *int1_ptr);

		int_ptr_cpy = int2_ptr;
		REQUIRE(int1_ptr.use_count() == 1);
		REQUIRE(int2_ptr.use_count() == 2);
		REQUIRE(*int_ptr_cpy == *int2_ptr);
	}

	REQUIRE(int1_ptr.use_count() == 1);
	REQUIRE(int2_ptr.use_count() == 1);
}

TEST_CASE("no argument", "make_shared")
{
	smart_pointer::shared_ptr<test_class> ptr1 = smart_pointer::make_shared<test_class>();
	smart_pointer::shared_ptr<test_class> ptr2 = smart_pointer::make_shared<test_class>();
	{
		smart_pointer::shared_ptr<test_class> ptr_cpy(ptr1);
		REQUIRE((ptr_cpy->arg1 == 0 && ptr_cpy->arg2 == 0 && ptr_cpy->arg3 == 0 && ptr_cpy->arg4 == 0));
		REQUIRE(ptr1.use_count() == 2);
		REQUIRE(ptr2.use_count() == 1);

		ptr_cpy = ptr2;
		REQUIRE((ptr_cpy->arg1 == 0 && ptr_cpy->arg2 == 0 && ptr_cpy->arg3 == 0 && ptr_cpy->arg4 == 0));
		REQUIRE(ptr1.use_count() == 1);
		REQUIRE(ptr2.use_count() == 2);
	}

	REQUIRE(ptr1.use_count() == 1);
	REQUIRE(ptr2.use_count() == 1);
}

TEST_CASE("one argument", "make_shared")
{
	smart_pointer::shared_ptr<test_class> ptr1 = smart_pointer::make_shared<test_class>(1);
	smart_pointer::shared_ptr<test_class> ptr2 = smart_pointer::make_shared<test_class>(2);
	{
		smart_pointer::shared_ptr<test_class> ptr_cpy(ptr1);
		REQUIRE((ptr_cpy->arg1 == 1 && ptr_cpy->arg2 == 0 && ptr_cpy->arg3 == 0 && ptr_cpy->arg4 == 0));
		REQUIRE(ptr1.use_count() == 2);
		REQUIRE(ptr2.use_count() == 1);

		ptr_cpy = ptr2;
		REQUIRE((ptr_cpy->arg1 == 2 && ptr_cpy->arg2 == 0 && ptr_cpy->arg3 == 0 && ptr_cpy->arg4 == 0));
		REQUIRE(ptr1.use_count() == 1);
		REQUIRE(ptr2.use_count() == 2);
	}

	REQUIRE(ptr1.use_count() == 1);
	REQUIRE(ptr2.use_count() == 1);
}

TEST_CASE("two arguments", "make_shared")
{
	smart_pointer::shared_ptr<test_class> ptr1 = smart_pointer::make_shared<test_class>(1, 1.5);
	smart_pointer::shared_ptr<test_class> ptr2 = smart_pointer::make_shared<test_class>(2, 2.5);
	{
		smart_pointer::shared_ptr<test_class> ptr_cpy(ptr1);
		REQUIRE((ptr_cpy->arg1 == 1 && ptr_cpy->arg2 == 1.5 && ptr_cpy->arg3 == 0 && ptr_cpy->arg4 == 0));
		REQUIRE(ptr1.use_count() == 2);
		REQUIRE(ptr2.use_count() == 1);

		ptr_cpy = ptr2;
		REQUIRE((ptr_cpy->arg1 == 2 && ptr_cpy->arg2 == 2.5 && ptr_cpy->arg3 == 0 && ptr_cpy->arg4 == 0));
		REQUIRE(ptr1.use_count() == 1);
		REQUIRE(ptr2.use_count() == 2);
	}

	REQUIRE(ptr1.use_count() == 1);
	REQUIRE(ptr2.use_count() == 1);
}

TEST_CASE("three arguments", "make_shared")
{
	smart_pointer::shared_ptr<test_class> ptr1 = smart_pointer::make_shared<test_class>(1, 1.5, 500000);
	smart_pointer::shared_ptr<test_class> ptr2 = smart_pointer::make_shared<test_class>(2, 2.5, 1000000);
	{
		smart_pointer::shared_ptr<test_class> ptr_cpy(ptr1);
		REQUIRE((ptr_cpy->arg1 == 1 && ptr_cpy->arg2 == 1.5 && ptr_cpy->arg3 == 500000 && ptr_cpy->arg4 == 0));
		REQUIRE(ptr1.use_count() == 2);
		REQUIRE(ptr2.use_count() == 1);

		ptr_cpy = ptr2;
		REQUIRE((ptr_cpy->arg1 == 2 && ptr_cpy->arg2 == 2.5 && ptr_cpy->arg3 == 1000000 && ptr_cpy->arg4 == 0));
		REQUIRE(ptr1.use_count() == 1);
		REQUIRE(ptr2.use_count() == 2);
	}

	REQUIRE(ptr1.use_count() == 1);
	REQUIRE(ptr2.use_count() == 1);
}

TEST_CASE("four arguments", "make_shared")
{
	smart_pointer::shared_ptr<test_class> ptr1 = smart_pointer::make_shared<test_class>(1, 1.5, 500000, 'a');
	smart_pointer::shared_ptr<test_class> ptr2 = smart_pointer::make_shared<test_class>(2, 2.5, 1000000, 'b');
	{
		smart_pointer::shared_ptr<test_class> ptr_cpy(ptr1);
		REQUIRE((ptr_cpy->arg1 == 1 && ptr_cpy->arg2 == 1.5 && ptr_cpy->arg3 == 500000 && ptr_cpy->arg4 == 'a'));
		REQUIRE(ptr1.use_count() == 2);
		REQUIRE(ptr2.use_count() == 1);

		ptr_cpy = ptr2;
		REQUIRE((ptr_cpy->arg1 == 2 && ptr_cpy->arg2 == 2.5 && ptr_cpy->arg3 == 1000000 && ptr_cpy->arg4 == 'b'));
		REQUIRE(ptr1.use_count() == 1);
		REQUIRE(ptr2.use_count() == 2);
	}

	REQUIRE(ptr1.use_count() == 1);
	REQUIRE(ptr2.use_count() == 1);
}

/*
TEST_CASE("use count multithreading", "shared_ptr")
{
	smart_pointer::shared_ptr<int> int1_ptr(new int(10));
	smart_pointer::shared_ptr<int> int2_ptr(new int(20));

	REQUIRE(int1_ptr.use_count() == 1);
	REQUIRE(int2_ptr.use_count() == 1);

	auto thread1 = std::thread([int1_ptr, int2_ptr]()
		{
			smart_pointer::shared_ptr<int> int_ptr_cpy(int1_ptr);
			REQUIRE(int1_ptr.use_count() == 2);
			REQUIRE(int2_ptr.use_count() == 1);

			int_ptr_cpy = int2_ptr;
			REQUIRE(int1_ptr.use_count() == 1);
			REQUIRE(int2_ptr.use_count() == 2);
		}
	);

	auto thread2 = std::thread([int1_ptr, int2_ptr]()
		{
			smart_pointer::shared_ptr<int> int_ptr_cpy(int1_ptr);
			REQUIRE(int1_ptr.use_count() == 2);
			REQUIRE(int2_ptr.use_count() == 1);

			int_ptr_cpy = int2_ptr;
			REQUIRE(int1_ptr.use_count() == 1);
			REQUIRE(int2_ptr.use_count() == 2);
		}
	);

	auto thread3 = std::thread([int1_ptr, int2_ptr]()
		{
			smart_pointer::shared_ptr<int> int_ptr_cpy(int1_ptr);
			REQUIRE(int1_ptr.use_count() == 2);
			REQUIRE(int2_ptr.use_count() == 1);

			int_ptr_cpy = int2_ptr;
			REQUIRE(int1_ptr.use_count() == 1);
			REQUIRE(int2_ptr.use_count() == 2);
		}
	);
	
	thread1.join();
	thread2.join();
	thread3.join();
	
	REQUIRE(int1_ptr.use_count() == 1);
	REQUIRE(int2_ptr.use_count() == 1);
}*/