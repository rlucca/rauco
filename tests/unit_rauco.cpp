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
/* rauco_internal.h */
FAKE_VALUE_FUNC(unsigned int, ih_calculate_checksum, struct internal_handler *);
FAKE_VALUE_FUNC(int, ih_checksum_valid, struct internal_handler *);
FAKE_VALUE_FUNC(int, ih_add_fd, struct internal_handler *, int);



#define FFF_FAKE_LIST(FAKE)					\
			FAKE(malloc)					\
			FAKE(free)						\
			FAKE(ih_calculate_checksum)		\
			FAKE(ih_checksum_valid)			\
			FAKE(ih_add_fd)

class EntryPointUnitTests : public testing::Test
{
	public:
		void SetUp() {
			FFF_FAKE_LIST(RESET_FAKE);
			FFF_RESET_HISTORY();
		}
};

static int helper_ih_add_fd_fake_already(struct internal_handler *, int)
{
	errno = EALREADY;
	return -1;
}

static int helper_ih_add_fd_fake_nomem(struct internal_handler *, int)
{
	errno = ENOMEM;
	return -1;
}

TEST_F(EntryPointUnitTests, create_nulled_parameter)
{
	struct internal_handler h;
	malloc_fake.return_val = &h;

	ASSERT_EQ(NULL, netcaller_create(NULL));
	ASSERT_EQ(0u, malloc_fake.call_count);
	ASSERT_EQ(0u, ih_calculate_checksum_fake.call_count);
}

TEST_F(EntryPointUnitTests, create_valid_parameter_without_memory_first_allocation)
{
	netcaller_setup_t param;

	ASSERT_EQ(NULL, netcaller_create(&param));
	ASSERT_EQ(1u, malloc_fake.call_count);
	ASSERT_EQ(0u, free_fake.call_count);
	ASSERT_EQ(0u, ih_calculate_checksum_fake.call_count);
}

TEST_F(EntryPointUnitTests, create_valid_parameter_without_memory_second_allocation)
{
	netcaller_setup_t param;
	struct internal_handler h;
	void *rets[2] = { &h, NULL };

	SET_RETURN_SEQ(malloc, rets, 2);
	ASSERT_EQ(NULL, netcaller_create(&param));
	ASSERT_EQ(2u, malloc_fake.call_count);
	ASSERT_EQ(1u, free_fake.call_count);
	ASSERT_EQ(0u, ih_calculate_checksum_fake.call_count);
}

TEST_F(EntryPointUnitTests, create_valid_parameter_returns_ok)
{
	netcaller_setup_t param;
	struct internal_handler h;
	unsigned int any = __LINE__;
	int i;
	void *rets[3] = { &h, &i, NULL };

	SET_RETURN_SEQ(malloc, rets, 3);
	ih_calculate_checksum_fake.return_val = any;

	ASSERT_EQ(&h, netcaller_create(&param));
	ASSERT_EQ(2u, malloc_fake.call_count);
	ASSERT_EQ(0u, free_fake.call_count);
	ASSERT_EQ(1u, ih_calculate_checksum_fake.call_count);
	ASSERT_EQ(any, h.checksum);
}

TEST_F(EntryPointUnitTests, destroy_invalid_parameter)
{
	struct internal_handler h;
	unsigned int any = __LINE__;

	h.checksum = any;
	ih_checksum_valid_fake.return_val = 1;

	netcaller_destroy(&h);

	ASSERT_EQ(0u, free_fake.call_count);
	ASSERT_EQ(1u, ih_checksum_valid_fake.call_count);
	ASSERT_EQ(any, h.checksum);
}

TEST_F(EntryPointUnitTests, destroy_ok)
{
	struct internal_handler h;

	netcaller_destroy(&h);
	ASSERT_EQ(3u, free_fake.call_count);
	ASSERT_EQ(1u, ih_checksum_valid_fake.call_count);
	ASSERT_EQ(&h, ih_checksum_valid_fake.arg0_val);
	ASSERT_EQ(0xDEADu, h.checksum);
}

TEST_F(EntryPointUnitTests, register_invalid_first_parm)
{
	ih_checksum_valid_fake.return_val = 1;
	ASSERT_EQ(-1, netcaller_register(NULL, 0));
	ASSERT_EQ(EINVAL, errno);
}

TEST_F(EntryPointUnitTests, register_invalid_second_parm)
{
	ASSERT_EQ(-1, netcaller_register(NULL, -1));
	ASSERT_EQ(EINVAL, errno);
}

TEST_F(EntryPointUnitTests, register_ok)
{
	ASSERT_EQ(0, netcaller_register(NULL, 1));
	ASSERT_EQ(0, errno);
}

TEST_F(EntryPointUnitTests, register_ok_second_time)
{
	ASSERT_EQ(0, netcaller_register(NULL, 1));
	ASSERT_EQ(0, errno);

	ih_add_fd_fake.custom_fake = helper_ih_add_fd_fake_already;
	ASSERT_EQ(-1, netcaller_register(NULL, 1));
	ASSERT_EQ(EALREADY, errno);

	ih_add_fd_fake.custom_fake = helper_ih_add_fd_fake_nomem;
	ASSERT_EQ(-1, netcaller_register(NULL, 1));
	ASSERT_EQ(ENOMEM, errno);
}
