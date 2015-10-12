extern "C" {
	#include "rauco_internal.h"
}

#include "gtest.h"
#include "fff.h"

DEFINE_FFF_GLOBALS;

/* rauco.h */
FAKE_VALUE_FUNC(void *, netcaller_create, const netcaller_setup_t *);


#define FFF_FAKE_LIST(FAKE)					\
			FAKE(netcaller_create)

class EntryPointUnitTests : public testing::Test
{
	public:
		void SetUp() {
			FFF_FAKE_LIST(RESET_FAKE);
			FFF_RESET_HISTORY();
		}
};


TEST_F(EntryPointUnitTests, nulled_parameter)
{
}
