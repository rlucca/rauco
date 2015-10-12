#include "gtest.h"
#include "fff.h"

DEFINE_FFF_GLOBALS;

class EntryPointIntTests : public testing::Test
{
	public:
		void SetUp() {
			//FFF_FAKE_LIST(RESET_FAKE);
			FFF_RESET_HISTORY();
		}
};


TEST_F(EntryPointIntTests, nulled_parameter)
{
}
