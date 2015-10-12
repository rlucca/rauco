extern "C" {
	#include "rauco_internal.h"
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

TEST_F(EntryPointIntTests, add_and_remove_from_beggining_and_ending)
{
	struct internal_handler ih;
	ih.fds = (int *) malloc(sizeof(int));
	ih.fds[0] = -1;

	ASSERT_EQ(0, ih_add_fd(&ih, 0));
	ASSERT_EQ(0, ih_add_fd(&ih, 1));
	ASSERT_EQ(0, ih_del_fd(&ih, 0));
	ASSERT_EQ(0, ih_add_fd(&ih, 2));
	ASSERT_EQ(0, ih_add_fd(&ih, 3));
	ASSERT_EQ(0, ih_del_fd(&ih, 3));

	free(ih.fds);
}

TEST_F(EntryPointIntTests, add_and_remove_from_middle)
{
	struct internal_handler ih;
	ih.fds = (int *) malloc(sizeof(int));
	ih.fds[0] = -1;

	ASSERT_EQ(0, ih_add_fd(&ih, 0));
	ASSERT_EQ(0, ih_add_fd(&ih, 1));
	ASSERT_EQ(0, ih_add_fd(&ih, 2));
	ASSERT_EQ(0, ih_del_fd(&ih, 1));
	ASSERT_EQ(0, ih_del_fd(&ih, 2));
	ASSERT_EQ(0, ih_del_fd(&ih, 0));

	free(ih.fds);
}
