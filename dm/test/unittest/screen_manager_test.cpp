/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "screen_manager_test.h"
#include "mock_display_manager_adapter.h"
#include "singleton_mocker.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Rosen {
using Mocker = SingletonMocker<DisplayManagerAdapter, MockDisplayManagerAdapter>;

sptr<Display> ScreenManagerTest::defaultDisplay_ = nullptr;
DisplayId ScreenManagerTest::defaultDisplayId_ = DISPLAY_ID_INVALD;
uint32_t ScreenManagerTest::defaultWidth_ = 480;
uint32_t ScreenManagerTest::defaultHeight_ = 320;

void ScreenManagerTest::SetUpTestCase()
{
    defaultDisplay_ = DisplayManager::GetInstance().GetDefaultDisplay();
    defaultDisplayId_ = defaultDisplay_->GetId();
    defaultWidth_ = defaultDisplay_->GetWidth();
    defaultHeight_ = defaultDisplay_->GetHeight();
}

void ScreenManagerTest::TearDownTestCase()
{
}

void ScreenManagerTest::SetUp()
{
}

void ScreenManagerTest::TearDown()
{
}

namespace {
/**
 * @tc.name: CreateAndDestory01
 * @tc.desc: CreateVirtualScreen with invalid option and return invalid screen id
 * @tc.type: FUNC
 */
HWTEST_F(ScreenManagerTest, CreateAndDestory01, Function | SmallTest | Level1)
{
    VirtualScreenOption wrongOption = {defaultName_, defaultWidth_, defaultHeight_,
                                       defaultDensity_, nullptr, defaultFlags_};
    std::unique_ptr<Mocker> m = std::make_unique<Mocker>();
    EXPECT_CALL(m->Mock(), CreateVirtualScreen(_)).Times(1).WillOnce(Return(SCREEN_ID_INVALD));
    EXPECT_CALL(m->Mock(), DestroyVirtualScreen(_)).Times(1).WillOnce(Return(DMError::DM_ERROR_INVALID_PARAM));
    ScreenId id = ScreenManager::GetInstance().CreateVirtualScreen(wrongOption);
    DMError ret = ScreenManager::GetInstance().DestroyVirtualScreen(id);
    ASSERT_EQ(SCREEN_ID_INVALD, id);
    ASSERT_EQ(DMError::DM_ERROR_INVALID_PARAM, ret);
}

/**
 * @tc.name: CreateAndDestory02
 * @tc.desc: CreateVirtualScreen with valid option and return valid screen id
 * @tc.type: FUNC
 */
HWTEST_F(ScreenManagerTest, CreateAndDestory02, Function | SmallTest | Level1)
{
    ScreenManagerUtils utils;
    ASSERT_TRUE(utils.CreateSurface());
    VirtualScreenOption defaultOption = {defaultName_, defaultWidth_, defaultHeight_,
                                         defaultDensity_, utils.psurface_, defaultFlags_};
    ScreenId validId = 0;
    std::unique_ptr<Mocker> m = std::make_unique<Mocker>();
    EXPECT_CALL(m->Mock(), CreateVirtualScreen(_)).Times(1).WillOnce(Return(validId));
    EXPECT_CALL(m->Mock(), DestroyVirtualScreen(_)).Times(1).WillOnce(Return(DMError::DM_OK));
    ScreenId id = ScreenManager::GetInstance().CreateVirtualScreen(defaultOption);
    DMError ret = ScreenManager::GetInstance().DestroyVirtualScreen(id);
    ASSERT_EQ(validId, id);
    ASSERT_EQ(DMError::DM_OK, ret);
}
}
} // namespace Rosen
} // namespace OHOS