extern "C" {
#include "rauco.h"
#include <errno.h>
}

#include "gtest.h"
#include "fff.h"

DEFINE_FFF_GLOBALS;

class EntryPointIntTests : public testing::Test
{
	public:
		void SetUp() {
		}
};


TEST_F(EntryPointIntTests, create_invalid_parameter_return_null)
{
	ASSERT_EQ(NULL, netcaller_create(NULL));
}

TEST_F(EntryPointIntTests, create_and_destroy)
{
	netcaller_setup_t h;
	void *ret;
	ret = netcaller_create(&h);
	ASSERT_NE((void *) NULL, ret);
	netcaller_destroy(ret);
}

TEST_F(EntryPointIntTests, create_and_destroy_with_one_element)
{
	netcaller_setup_t h;
	void *ret;
	ret = netcaller_create(&h);
	ASSERT_NE((void *) NULL, ret);
	netcaller_register(ret, 4);
	netcaller_destroy(ret);
}

TEST_F(EntryPointIntTests, create_and_destroy_insert_and_erase_element)
{
	netcaller_setup_t h;
	void *ret;
	ret = netcaller_create(&h);
	ASSERT_NE((void *) NULL, ret);
	ASSERT_EQ(0, netcaller_register(ret, 4));
	ASSERT_EQ(0, netcaller_deregister(ret, 4));
	netcaller_destroy(ret);
}

TEST_F(EntryPointIntTests, create_and_destroy_insert_twice_and_erase_twice\
same_element)
{
	netcaller_setup_t h;
	void *ret;
	ret = netcaller_create(&h);
	ASSERT_NE((void *) NULL, ret);
	ASSERT_EQ(0, netcaller_register(ret, 4));
	ASSERT_EQ(0, errno);
	ASSERT_EQ(-1, netcaller_register(ret, 4));
	ASSERT_EQ(EALREADY, errno);
	ASSERT_EQ(0, netcaller_deregister(ret, 4));
	ASSERT_EQ(0, errno);
	ASSERT_EQ(-1, netcaller_deregister(ret, 4));
	ASSERT_EQ(ENOENT, errno);
	netcaller_destroy(ret);
}

TEST_F(EntryPointIntTests, create_and_destroy_insert_twice_and_erase_twice)
{
	netcaller_setup_t h;
	void *ret;
	ret = netcaller_create(&h);
	ASSERT_NE((void *) NULL, ret);
	ASSERT_EQ(0, netcaller_register(ret, 4));
	ASSERT_EQ(0, netcaller_register(ret, 14));
	ASSERT_EQ(0, netcaller_deregister(ret, 4));
	ASSERT_EQ(0, netcaller_deregister(ret, 14));
	netcaller_destroy(ret);
}
