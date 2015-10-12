extern "C" {
	#include "rauco_internal.h"
}

#include "gtest.h"
#include "fff.h"

DEFINE_FFF_GLOBALS;

/* stdlib.h */
FAKE_VALUE_FUNC(void *, malloc, size_t);
FAKE_VALUE_FUNC(void *, realloc, void *, size_t);
FAKE_VOID_FUNC(free, void *);


#define FFF_FAKE_LIST(FAKE)					\
			FAKE(malloc)					\
			FAKE(realloc)					\
			FAKE(free)						\



class EntryPointUnitTests : public testing::Test
{
	public:
		void SetUp() {
			FFF_FAKE_LIST(RESET_FAKE);
			FFF_RESET_HISTORY();
		}
};
