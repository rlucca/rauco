extern "C" {
#include "rauco.h"
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
