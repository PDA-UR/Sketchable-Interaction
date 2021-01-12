

#include "SIGRunExternalApplicationManagerTest.hpp"
#include <sigrun/context/managers/ExternalApplicationManager.hpp>

TEST_F(SIGRunExternalApplicationManagerTest, construction_default)
{
    ExternalApplicationManager eam;
    ASSERT_EQ(DEFAULT_PROCESS_WINID_FETCH_SLEEP_TIME_MS, eam.process_winid_fetch_sleep_tie_ms());
    ASSERT_EQ(DEFAULT_PROCESS_WINID_FETCH_TIMEOUT_MS, eam.process_winid_fetch_timeout_ms());
    ASSERT_EQ(DEFAULT_PROCESS_WINID_FETCH_TIMEOUT_MS / DEFAULT_PROCESS_WINID_FETCH_SLEEP_TIME_MS / 1000, eam.process_winid_fetch_iterations());

}

TEST_F(SIGRunExternalApplicationManagerTest, construction_custom)
{
    ExternalApplicationManager eam(1.0 / 30.0, 1000.0);
    ASSERT_EQ(1.0 / 30.0, eam.process_winid_fetch_sleep_time_ms());
    ASSERT_EQ(1000.0, eam.process_winid_fetch_timeout_ms());
    ASSERT_EQ(30, eam.process_winid_fetch_iterations());
}

TEST_F(SIGRunExternalApplicationManagerTest, launch_application)
{

}

TEST_F(SIGRunExternalApplicationManagerTest, launch_standard_application)
{
    ExternalApplicationManager eam;


}

TEST_F(SIGRunExternalApplicationManagerTest, terminate_application)
{
    ExternalApplicationManager eam;


}

TEST_F(SIGRunExternalApplicationManagerTest, set_process_winid_fetch_sleep_time_ms)
{
    ExternalApplicationManager eam;

    EXPECT_NO_FATAL_FAILURE(eam.set_process_winid_fetch_sleep_time_ms(1.0 / 30));

    ASSERT_EQ(30, eam.process_winid_fetch_iterations());
}

TEST_F(SIGRunExternalApplicationManagerTest, set_process_winid_fetch_timeout_ms)
{
    ExternalApplicationManager eam;

    EXPECT_NO_FATAL_FAILURE(eam.set_process_winid_fetch_timeout_ms(1000.0));
    ASSERT_EQ(60, eam.process_winid_fetch_iterations());
}

TEST_F(SIGRunExternalApplicationManagerTest, process_winid_fetch_sleep_time_ms)
{
    ExternalApplicationManager eam;

    ASSERT_EQ(DEFAULT_PROCESS_WINID_FETCH_SLEEP_TIME_MS, eam.process_winid_fetch_sleep_time_ms());
}

TEST_F(SIGRunExternalApplicationManagerTest, process_winid_fetch_timeout_ms)
{
    ExternalApplicationManager eam;

    ASSERT_EQ(DEFAULT_PROCESS_WINID_FETCH_TIMEOUT_MS, eam.process_winid_fetch_timeout_ms());
}

TEST_F(SIGRunExternalApplicationManagerTest, process_winid_fetch_iterations)
{
    ExternalApplicationManager eam;

    ASSERT_EQ(60, eam.process_winid_fetch_iterations());
}