/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// gtest
#include <gtest/gtest.h>
#include "window_manager.h"
#include "window_test_utils.h"
using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Rosen {
using utils = WindowTestUtils;
class WindowModeSupportInfoTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() override;
    virtual void TearDown() override;
    utils::TestWindowInfo fullScreenAppInfo_;
    utils::TestWindowInfo floatingScreenAppInfo_;
private:
    static constexpr uint32_t WAIT_SYANC_US = 100000;
};

void WindowModeSupportInfoTest::SetUpTestCase()
{
    auto display = DisplayManager::GetInstance().GetDisplayById(0);
    ASSERT_TRUE((display != nullptr));
    Rect displayRect = {0, 0, display->GetWidth(), display->GetHeight()};
    utils::InitByDisplayRect(displayRect);
}

void WindowModeSupportInfoTest::TearDownTestCase()
{
}

void WindowModeSupportInfoTest::SetUp()
{
    fullScreenAppInfo_ = {
            .name = "FullWindow",
            .rect = utils::customAppRect_,
            .type = WindowType::WINDOW_TYPE_APP_MAIN_WINDOW,
            .mode = WindowMode::WINDOW_MODE_FULLSCREEN,
            .needAvoid = false,
            .parentLimit = false,
            .parentName = "",
    };
    floatingScreenAppInfo_ = {
            .name = "FloatingWindow",
            .rect = utils::customAppRect_,
            .type = WindowType::WINDOW_TYPE_APP_MAIN_WINDOW,
            .mode = WindowMode::WINDOW_MODE_FULLSCREEN,
            .needAvoid = false,
            .parentLimit = false,
            .parentName = "",
    };
}

void WindowModeSupportInfoTest::TearDown()
{
}

namespace {
/**
 * @tc.name: WindowModeSupportInfo01
 * @tc.desc: SetModeSupportInfo | GetModeSupportInfo
 * @tc.type: FUNC
 */
HWTEST_F(WindowModeSupportInfoTest, WindowModeSupportInfo01, Function | MediumTest | Level3)
{
    const sptr<Window>& window = utils::CreateTestWindow(fullScreenAppInfo_);
    
    window->SetModeSupportInfo(WindowModeSupport::WINDOW_MODE_SUPPORT_FULLSCREEN);
    ASSERT_EQ(WindowModeSupport::WINDOW_MODE_SUPPORT_FULLSCREEN, window->GetModeSupportInfo());
    window->Destroy();
}

/**
 * @tc.name: WindowModeSupportInfo02
 * @tc.desc: modeSupportInfo test for single window
 * @tc.type: FUNC
 */
HWTEST_F(WindowModeSupportInfoTest, WindowModeSupportInfo02, Function | MediumTest | Level3)
{
    const sptr<Window>& window = utils::CreateTestWindow(fullScreenAppInfo_);

    window->SetModeSupportInfo(WindowModeSupport::WINDOW_MODE_SUPPORT_FULLSCREEN |
        WindowModeSupport::WINDOW_MODE_SUPPORT_FLOATING);
    ASSERT_EQ(WMError::WM_OK, window->Show());
    ASSERT_EQ(WindowMode::WINDOW_MODE_FULLSCREEN, window->GetMode());
    ASSERT_EQ(WMError::WM_OK, window->Hide());

    window->SetWindowMode(WindowMode::WINDOW_MODE_FLOATING);
    ASSERT_EQ(WMError::WM_OK, window->Show());
    ASSERT_EQ(WindowMode::WINDOW_MODE_FLOATING, window->GetMode());
    ASSERT_EQ(WMError::WM_OK, window->Hide());

    window->SetWindowMode(WindowMode::WINDOW_MODE_FULLSCREEN);
    ASSERT_EQ(WMError::WM_OK, window->Show());
    ASSERT_EQ(WindowMode::WINDOW_MODE_FULLSCREEN, window->GetMode());
    ASSERT_EQ(WMError::WM_OK, window->Hide());

    window->SetWindowMode(WindowMode::WINDOW_MODE_SPLIT_PRIMARY);
    ASSERT_EQ(WMError::WM_OK, window->Show());
    ASSERT_EQ(WindowMode::WINDOW_MODE_FULLSCREEN, window->GetMode());
    ASSERT_EQ(WMError::WM_OK, window->Hide());

    window->SetWindowMode(WindowMode::WINDOW_MODE_SPLIT_SECONDARY);
    ASSERT_EQ(WMError::WM_OK, window->Show());
    ASSERT_EQ(WindowMode::WINDOW_MODE_FULLSCREEN, window->GetMode());
    ASSERT_EQ(WMError::WM_OK, window->Hide());

    window->Destroy();
}

/**
 * @tc.name: WindowModeSupportInfo03
 * @tc.desc: modeSupportInfo test for single window in case current window mode is not supported.
 * @tc.type: FUNC
 */
HWTEST_F(WindowModeSupportInfoTest, WindowModeSupportInfo03, Function | MediumTest | Level3)
{
    const sptr<Window>& window = utils::CreateTestWindow(fullScreenAppInfo_);
    window->SetModeSupportInfo(WindowModeSupport::WINDOW_MODE_SUPPORT_FLOATING |
    WindowModeSupport::WINDOW_MODE_SUPPORT_SPLIT_PRIMARY |
    WindowModeSupport::WINDOW_MODE_SUPPORT_SPLIT_SECONDARY);
    ASSERT_EQ(WMError::WM_OK, window->Show());
    ASSERT_EQ(WindowMode::WINDOW_MODE_FLOATING, window->GetMode());
    ASSERT_EQ(WMError::WM_OK, window->Hide());

    window->SetModeSupportInfo(WindowModeSupport::WINDOW_MODE_SUPPORT_SPLIT_PRIMARY |
    WindowModeSupport::WINDOW_MODE_SUPPORT_SPLIT_SECONDARY);
    ASSERT_EQ(WMError::WM_OK, window->Show());
    ASSERT_EQ(WindowMode::WINDOW_MODE_SPLIT_PRIMARY, window->GetMode());
    ASSERT_EQ(WMError::WM_OK, window->Hide());

    window->Destroy();
}

/**
 * @tc.name: WindowModeSupportInfo04
 * @tc.desc: modeSupportInfo test for layout cascade
 * @tc.type: FUNC
 */
HWTEST_F(WindowModeSupportInfoTest, WindowModeSupportInfo04, Function | MediumTest | Level3)
{
    const sptr<Window>& window1 = utils::CreateTestWindow(fullScreenAppInfo_);
    window1->SetModeSupportInfo(WindowModeSupport::WINDOW_MODE_SUPPORT_FULLSCREEN);
    const sptr<Window>& window2 = utils::CreateTestWindow(utils::TestWindowInfo {
            .name = "FullWindow2",
            .rect = utils::customAppRect_,
            .type = WindowType::WINDOW_TYPE_APP_MAIN_WINDOW,
            .mode = WindowMode::WINDOW_MODE_FULLSCREEN,
            .needAvoid = false,
            .parentLimit = false,
            .parentName = "",
    });
    window2->SetModeSupportInfo(WindowModeSupport::WINDOW_MODE_SUPPORT_ALL);
    ASSERT_EQ(WMError::WM_OK, window1->Show());
    ASSERT_EQ(WMError::WM_OK, window2->Show());
    WindowManager::GetInstance().SetWindowLayoutMode(WindowLayoutMode::CASCADE);
    usleep(WAIT_SYANC_US);

    ASSERT_EQ(WindowMode::WINDOW_MODE_FULLSCREEN, window1->GetMode());
    ASSERT_EQ(WindowMode::WINDOW_MODE_FLOATING, window2->GetMode());

    window1->Destroy();
    window2->Destroy();
}

/**
 * @tc.name: WindowModeSupportInfo05
 * @tc.desc: modeSupportInfo test for layout tile
 * @tc.type: FUNC
 */
HWTEST_F(WindowModeSupportInfoTest, WindowModeSupportInfo05, Function | MediumTest | Level3)
{
    const sptr<Window>& window = utils::CreateTestWindow(fullScreenAppInfo_);
    window->SetModeSupportInfo(WindowModeSupport::WINDOW_MODE_SUPPORT_FULLSCREEN);
    ASSERT_EQ(WMError::WM_OK, window->Show());
    WindowManager::GetInstance().SetWindowLayoutMode(WindowLayoutMode::TILE);
    usleep(WAIT_SYANC_US);

    ASSERT_EQ(WindowMode::WINDOW_MODE_FULLSCREEN, window->GetMode());

    window->Destroy();
    WindowManager::GetInstance().SetWindowLayoutMode(WindowLayoutMode::CASCADE);
    usleep(WAIT_SYANC_US);
}

/**
 * @tc.name: WindowModeSupportInfo06
 * @tc.desc: modeSupportInfo test for split
 * @tc.type: FUNC
 */
HWTEST_F(WindowModeSupportInfoTest, WindowModeSupportInfo06, Function | MediumTest | Level3)
{
    const sptr<Window>& window1 = utils::CreateTestWindow(fullScreenAppInfo_);
    window1->SetModeSupportInfo(WindowModeSupport::WINDOW_MODE_SUPPORT_ALL);
    const sptr<Window>& window2 = utils::CreateTestWindow(utils::TestWindowInfo {
            .name = "FullWindow2",
            .rect = utils::customAppRect_,
            .type = WindowType::WINDOW_TYPE_APP_MAIN_WINDOW,
            .mode = WindowMode::WINDOW_MODE_FULLSCREEN,
            .needAvoid = false,
            .parentLimit = false,
            .parentName = "",
    });
    window2->SetModeSupportInfo(WindowModeSupport::WINDOW_MODE_SUPPORT_FULLSCREEN);
    ASSERT_EQ(WMError::WM_OK, window1->Show());
    ASSERT_EQ(WMError::WM_OK, window2->Show());
    usleep(WAIT_SYANC_US);

    window1->SetWindowMode(WindowMode::WINDOW_MODE_SPLIT_PRIMARY);

    ASSERT_EQ(WindowMode::WINDOW_MODE_SPLIT_PRIMARY, window1->GetMode());
    ASSERT_EQ(WindowMode::WINDOW_MODE_FULLSCREEN, window2->GetMode());

    window1->Destroy();
    window2->Destroy();
}
} // namespace
} // namespace Rosen
} // namespace OHOS