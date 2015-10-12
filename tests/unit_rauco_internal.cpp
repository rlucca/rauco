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
			errno = 0;
		}
};

TEST_F(EntryPointUnitTests, add_fd_without_memory)
{
	struct internal_handler ih;
	int pi[] = { -1, -1 };
	ih.fds = pi;

	realloc_fake.return_val = 0;

	ASSERT_EQ(-1, ih_add_fd(&ih, 4));
	ASSERT_EQ(ENOMEM, errno);
	ASSERT_EQ(1u, realloc_fake.call_count);
	ASSERT_EQ(pi, realloc_fake.arg0_history[0]);
	ASSERT_EQ(sizeof(int) * 2, realloc_fake.arg1_history[0]);
}

TEST_F(EntryPointUnitTests, add_already_inserted_fd)
{
	struct internal_handler ih;
	int value = 5;
	int pi[] = { value, -1, -1 };
	ih.fds = pi;

	ASSERT_EQ(-1, ih_add_fd(&ih, value));
	ASSERT_EQ(EALREADY, errno);
	ASSERT_EQ(0u, realloc_fake.call_count);
}

TEST_F(EntryPointUnitTests, add_new_fd)
{
	struct internal_handler ih;
	int value = __LINE__;
	int pi[] = { -1, -1 };

	ih.fds = pi;
	realloc_fake.return_val = &pi;

	ASSERT_EQ(0, ih_add_fd(&ih, value));
	ASSERT_EQ(0, errno);
	ASSERT_EQ(1u, realloc_fake.call_count);
	ASSERT_EQ(pi, realloc_fake.arg0_history[0]);
	ASSERT_EQ(sizeof(int) * 2, realloc_fake.arg1_history[0]);
	ASSERT_EQ(value, pi[0]);
	ASSERT_EQ(-1, pi[1]);
}

TEST_F(EntryPointUnitTests, del_first_fd)
{
	struct internal_handler ih;
	int k;
	int pi[] = { 0, 1, 2, 3, 4, -1 };

	ih.fds = pi;
	realloc_fake.return_val = &pi;

	ASSERT_EQ(0, ih_del_fd(&ih, 0));
	ASSERT_EQ(0, errno);
	ASSERT_EQ(1u, realloc_fake.call_count);
	ASSERT_EQ(pi, realloc_fake.arg0_history[0]);
	ASSERT_EQ(sizeof(int) * 5, realloc_fake.arg1_history[0]);

	k = 1;
	for (unsigned idx = 0; idx < 4; idx++, k++)
		ASSERT_EQ(k, pi[idx]);
	ASSERT_EQ(-1, pi[4]);
}

TEST_F(EntryPointUnitTests, del_last_fd)
{
	struct internal_handler ih;
	int k;
	int pi[] = { 0, 1, 2, 3, 4, -1 };

	ih.fds = pi;
	realloc_fake.return_val = &pi;

	ASSERT_EQ(0, ih_del_fd(&ih, 4));
	ASSERT_EQ(0, errno);
	ASSERT_EQ(1u, realloc_fake.call_count);
	ASSERT_EQ(pi, realloc_fake.arg0_history[0]);
	ASSERT_EQ(sizeof(int) * 5, realloc_fake.arg1_history[0]);

	k = 0;
	for (unsigned idx = 0; idx < 4; idx++, k++)
		ASSERT_EQ(k, pi[idx]);
	ASSERT_EQ(-1, pi[4]);
}

TEST_F(EntryPointUnitTests, del_middle_fd)
{
	struct internal_handler ih;
	int k;
	int pi[] = { 0, 1, 2, 3, 4, -1 };

	ih.fds = pi;
	realloc_fake.return_val = &pi;

	ASSERT_EQ(0, ih_del_fd(&ih, 2));
	ASSERT_EQ(0, errno);
	ASSERT_EQ(1u, realloc_fake.call_count);
	ASSERT_EQ(pi, realloc_fake.arg0_history[0]);
	ASSERT_EQ(sizeof(int) * 5, realloc_fake.arg1_history[0]);

	k = 0;
	for (unsigned idx = 0; idx < 4; idx++, k++)
	{
		if (idx == 2) k++;
		ASSERT_EQ(k, pi[idx]);
	}
	ASSERT_EQ(-1, pi[4]);
}

TEST_F(EntryPointUnitTests, del_fd_empty_list)
{
	struct internal_handler ih;
	int pi[] = { -1 };

	ih.fds = pi;
	realloc_fake.return_val = &pi;

	ASSERT_EQ(-1, ih_del_fd(&ih, 0));
	ASSERT_EQ(ENOENT, errno);
	ASSERT_EQ(0u, realloc_fake.call_count);
	ASSERT_EQ(-1, pi[0]);
}

TEST_F(EntryPointUnitTests, del_fd_not_inserted)
{
	struct internal_handler ih;
	int pi[] = { 2, -1 };

	ih.fds = pi;
	realloc_fake.return_val = &pi;

	ASSERT_EQ(-1, ih_del_fd(&ih, 5));
	ASSERT_EQ(ENOENT, errno);
	ASSERT_EQ(0u, realloc_fake.call_count);
	ASSERT_EQ(2, pi[0]);
	ASSERT_EQ(-1, pi[1]);
}

TEST_F(EntryPointUnitTests, del_fd_without_memory) // !?!
{
	struct internal_handler ih;
	int pi[] = { 2, -1 };

	ih.fds = pi;
	realloc_fake.return_val = 0;

	ASSERT_EQ(-1, ih_del_fd(&ih, 2));
	ASSERT_EQ(ENOMEM, errno);
	ASSERT_EQ(1u, realloc_fake.call_count);
	ASSERT_EQ(2, pi[0]);
	ASSERT_EQ(-1, pi[1]);
}
