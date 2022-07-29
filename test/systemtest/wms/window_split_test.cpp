/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#include "window_test_utils.h"
using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Rosen {
using Utils = WindowTestUtils;
class WindowSplitTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() override;
    virtual void TearDown() override;
    std::vector<sptr<Window>> activeWindows_;
    Utils::TestWindowInfo fullInfo_;
    Utils::TestWindowInfo splitInfo_;

private:
    static constexpr uint32_t SPLIT_TEST_SLEEP_S = 1; // split test sleep time
};

void WindowSplitTest::SetUpTestCase()
{
}

void WindowSplitTest::TearDownTestCase()
{
}

void WindowSplitTest::SetUp()
{
    fullInfo_ = {
        .name = "",
        .rect = Utils::customAppRect_,
        .type = WindowType::WINDOW_TYPE_APP_MAIN_WINDOW,
        .mode = WindowMode::WINDOW_MODE_SPLIT_PRIMARY,
        .needAvoid = true,
        .parentLimit = false,
        .parentName = "",
    };

    splitInfo_ = {
        .name = "",
        .rect = Utils::customAppRect_,
        .type = WindowType::WINDOW_TYPE_APP_MAIN_WINDOW,
        .mode = WindowMode::WINDOW_MODE_FULLSCREEN,
        .needAvoid = true,
        .parentLimit = false,
        .parentName = "",
    };

    activeWindows_.clear();
}

void WindowSplitTest::TearDown()
{
    while (!activeWindows_.empty()) {
        ASSERT_EQ(WMError::WM_OK, activeWindows_.back()->Destroy());
        activeWindows_.pop_back();
    }
}

namespace {
/**
 * @tc.name: SplitWindow01
 * @tc.desc: first create a secondary window, then create a primary window, test mode change
 * @tc.type: FUNC
 */
HWTEST_F(WindowSplitTest, SplitWindow01, Function | MediumTest | Level3)
{
    fullInfo_.name  = "fullscreen.1";
    fullInfo_.mode = WindowMode::WINDOW_MODE_SPLIT_SECONDARY;
    splitInfo_.name = "primary.1";
    splitInfo_.mode = WindowMode::WINDOW_MODE_SPLIT_PRIMARY;

    const sptr<Window>& fullWindow = Utils::CreateTestWindow(fullInfo_);
    activeWindows_.push_back(fullWindow);
    ASSERT_EQ(WMError::WM_OK, fullWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    const sptr<Window>& priWindow = Utils::CreateTestWindow(splitInfo_);
    activeWindows_.push_back(priWindow);
    ASSERT_EQ(WMError::WM_OK, priWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    ASSERT_EQ(WindowMode::WINDOW_MODE_SPLIT_PRIMARY, priWindow->GetMode());
    ASSERT_EQ(WindowMode::WINDOW_MODE_SPLIT_SECONDARY, fullWindow->GetMode());

    ASSERT_EQ(WMError::WM_OK, priWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
    ASSERT_EQ(WindowMode::WINDOW_MODE_FULLSCREEN, fullWindow->GetMode());
    ASSERT_EQ(WMError::WM_OK, fullWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
}

/**
 * @tc.name: SplitWindow02
 * @tc.desc: first create a primary window, then create a secondary window, test mode change
 * @tc.type: FUNC
 */
HWTEST_F(WindowSplitTest, SplitWindow02, Function | MediumTest | Level3)
{
    fullInfo_.name  = "fullscreen.2";
    fullInfo_.mode = WindowMode::WINDOW_MODE_SPLIT_PRIMARY;
    splitInfo_.name = "secondary.2";
    splitInfo_.mode = WindowMode::WINDOW_MODE_SPLIT_SECONDARY;

    const sptr<Window>& fullWindow = Utils::CreateTestWindow(fullInfo_);
    activeWindows_.push_back(fullWindow);
    ASSERT_EQ(WMError::WM_OK, fullWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);
    const sptr<Window>& secWindow = Utils::CreateTestWindow(splitInfo_);
    activeWindows_.push_back(secWindow);
    ASSERT_EQ(WMError::WM_OK, secWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    ASSERT_EQ(WindowMode::WINDOW_MODE_SPLIT_SECONDARY, secWindow->GetMode());
    ASSERT_EQ(WindowMode::WINDOW_MODE_SPLIT_PRIMARY, fullWindow->GetMode());

    ASSERT_EQ(WMError::WM_OK, fullWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
    ASSERT_EQ(WindowMode::WINDOW_MODE_FULLSCREEN, secWindow->GetMode());
    ASSERT_EQ(WMError::WM_OK, secWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
}

/**
 * @tc.name: SplitScreen03
 * @tc.desc: first create a secondary window, then create a primary window, test rects
 * @tc.type: FUNC
 */
HWTEST_F(WindowSplitTest, SplitScreen03, Function | MediumTest | Level3)
{
    fullInfo_.name  = "fullscreen.3";
    fullInfo_.mode = WindowMode::WINDOW_MODE_SPLIT_SECONDARY;
    splitInfo_.name = "primary.3";
    splitInfo_.mode = WindowMode::WINDOW_MODE_SPLIT_PRIMARY;

    ASSERT_TRUE(Utils::InitSplitRects());

    const sptr<Window>& fullWindow = Utils::CreateTestWindow(fullInfo_);
    activeWindows_.push_back(fullWindow);
    ASSERT_EQ(WMError::WM_OK, fullWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);
    const sptr<Window>& priWindow = Utils::CreateTestWindow(splitInfo_);
    activeWindows_.push_back(priWindow);
    ASSERT_EQ(WMError::WM_OK, priWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    Utils::UpdateSplitRects(fullWindow);

    ASSERT_TRUE(Utils::RectEqualTo(fullWindow, Utils::splitRects_.secondaryRect));
    ASSERT_TRUE(Utils::RectEqualTo(priWindow, Utils::splitRects_.primaryRect));

    ASSERT_EQ(WMError::WM_OK, fullWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
    ASSERT_EQ(WMError::WM_OK, priWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
}

/**
 * @tc.name: SplitScreen04
 * @tc.desc: first create a primary window, then create a secondary window, test rects
 * @tc.type: FUNC
 */
HWTEST_F(WindowSplitTest, SplitScreen04, Function | MediumTest | Level3)
{
    fullInfo_.name  = "fullscreen.4";
    splitInfo_.name = "secondary.4";
    splitInfo_.mode = WindowMode::WINDOW_MODE_SPLIT_SECONDARY;

    ASSERT_TRUE(Utils::InitSplitRects());

    const sptr<Window>& fullWindow = Utils::CreateTestWindow(fullInfo_);
    activeWindows_.push_back(fullWindow);
    ASSERT_EQ(WMError::WM_OK, fullWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);
    const sptr<Window>& secWindow = Utils::CreateTestWindow(splitInfo_);
    activeWindows_.push_back(secWindow);
    ASSERT_EQ(WMError::WM_OK, secWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    Utils::UpdateSplitRects(fullWindow);

    ASSERT_TRUE(Utils::RectEqualTo(fullWindow, Utils::splitRects_.primaryRect));
    ASSERT_TRUE(Utils::RectEqualTo(secWindow, Utils::splitRects_.secondaryRect));

    ASSERT_EQ(WMError::WM_OK, fullWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
    ASSERT_EQ(WMError::WM_OK, secWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
}

/**
 * @tc.name: SplitScreen05
 * @tc.desc: forbid dock slice to move test
 * @tc.type: FUNC
 */
HWTEST_F(WindowSplitTest, SplitScreen05, Function | MediumTest | Level3)
{
    fullInfo_.name  = "fullscreen.5";
    splitInfo_.name = "primary.5";
    splitInfo_.mode = WindowMode::WINDOW_MODE_SPLIT_SECONDARY;

    const sptr<Window>& fullWindow = Utils::CreateTestWindow(fullInfo_);
    activeWindows_.push_back(fullWindow);
    ASSERT_EQ(WMError::WM_OK, fullWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    const sptr<Window>& priWindow = Utils::CreateTestWindow(splitInfo_);
    activeWindows_.push_back(priWindow);
    ASSERT_EQ(WMError::WM_OK, priWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    ASSERT_EQ(WindowMode::WINDOW_MODE_SPLIT_SECONDARY, priWindow->GetMode());
    ASSERT_EQ(WindowMode::WINDOW_MODE_SPLIT_PRIMARY, fullWindow->GetMode());

    Utils::TestWindowInfo dividerInfo;
    dividerInfo.name = "divider0";
    dividerInfo.type = WindowType::WINDOW_TYPE_DOCK_SLICE;
    dividerInfo.mode = WindowMode::WINDOW_MODE_FLOATING;
    dividerInfo.focusable_ = false;

    const sptr<Window>& divider = Utils::CreateTestWindow(dividerInfo);
    activeWindows_.push_back(divider);
    ASSERT_EQ(WMError::WM_OK, divider->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    Rect lastRect = divider->GetRect();
    WMError ret = divider->MoveTo(lastRect.posX_ + 10, lastRect.posY_ + 10);
    sleep(SPLIT_TEST_SLEEP_S);
    Rect rect = divider->GetRect();
    ASSERT_TRUE(lastRect.posX_ != rect.posX_ || lastRect.posY_ != rect.posY_);

    ret = fullWindow->AddWindowFlag(WindowFlag::WINDOW_FLAG_FORBID_SPLIT_MOVE);
    sleep(SPLIT_TEST_SLEEP_S);
    ret = divider->MoveTo(rect.posX_ + 10, rect.posY_ + 10);
    sleep(SPLIT_TEST_SLEEP_S);
    Rect NewRect = divider->GetRect();
    ASSERT_TRUE(rect.posX_ == NewRect.posX_ && rect.posY_ == NewRect.posY_);

    ASSERT_EQ(WMError::WM_OK, priWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
    ASSERT_EQ(WMError::WM_OK, fullWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
}

/**
 * @tc.name: SplitScreen06
 * @tc.desc: split ratio test
 * @tc.type: FUNC
 */
HWTEST_F(WindowSplitTest, SplitScreen06, Function | MediumTest | Level3)
{
    fullInfo_.name  = "fullscreen.6";
    splitInfo_.name = "primary.6";
    splitInfo_.mode = WindowMode::WINDOW_MODE_SPLIT_SECONDARY;

    const sptr<Window>& fullWindow = Utils::CreateTestWindow(fullInfo_);
    activeWindows_.push_back(fullWindow);
    ASSERT_EQ(WMError::WM_OK, fullWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    const sptr<Window>& priWindow = Utils::CreateTestWindow(splitInfo_);
    activeWindows_.push_back(priWindow);
    ASSERT_EQ(WMError::WM_OK, priWindow->Show());
    sleep(SPLIT_TEST_SLEEP_S);

    Utils::TestWindowInfo dividerInfo;
    dividerInfo.name = "divider0";
    dividerInfo.type = WindowType::WINDOW_TYPE_DOCK_SLICE;
    dividerInfo.mode = WindowMode::WINDOW_MODE_FLOATING;
    dividerInfo.focusable_ = false;

    const sptr<Window>& divider = Utils::CreateTestWindow(dividerInfo);
    activeWindows_.push_back(divider);
    ASSERT_EQ(WMError::WM_OK, divider->Show());
    sleep(SPLIT_TEST_SLEEP_S);
    Rect rect = divider->GetRect();

    uint32_t pointerId = 0;
    int32_t posX, posY;
    posX = posY = 0;
    std::shared_ptr<MMI::PointerEvent> pointerEvent =
        Utils::CreatePointerEvent(posX, posY, pointerId, MMI::PointerEvent::POINTER_ACTION_DOWN);
    divider->ConsumePointerEvent(pointerEvent);
    sleep(SPLIT_TEST_SLEEP_S);

    if (rect.width_ < rect.height_) {
        posX += 10;
    } else {
        posY += 10;
    }
    pointerEvent = Utils::CreatePointerEvent(posX, posY, pointerId, MMI::PointerEvent::POINTER_ACTION_MOVE);
    divider->ConsumePointerEvent(pointerEvent);
    sleep(SPLIT_TEST_SLEEP_S);
    Rect moveRect = divider->GetRect();
    ASSERT_TRUE(rect.posX_ != moveRect.posX_ || rect.posY_ != moveRect.posY_);

    pointerEvent = Utils::CreatePointerEvent(posX, posY, pointerId, MMI::PointerEvent::POINTER_ACTION_UP);
    divider->ConsumePointerEvent(pointerEvent);
    sleep(SPLIT_TEST_SLEEP_S);
    Rect newRect = divider->GetRect();
    ASSERT_TRUE(rect.posX_ == newRect.posX_ && rect.posY_ == newRect.posY_);

    ASSERT_EQ(WMError::WM_OK, priWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
    ASSERT_EQ(WMError::WM_OK, fullWindow->Hide());
    sleep(SPLIT_TEST_SLEEP_S);
}
}
} // namespace Rosen
} // namespace OHOS