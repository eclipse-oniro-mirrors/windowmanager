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

#include "display_manager_service.h"

#include <cinttypes>
#include <unistd.h>

#include <ipc_skeleton.h>
#include <iservice_registry.h>
#include <system_ability_definition.h>

#include "window_manager_hilog.h"

namespace OHOS::Rosen {
namespace {
    constexpr HiviewDFX::HiLogLabel LABEL = {LOG_CORE, 0, "DisplayManagerService"};
}

IMPLEMENT_SINGLE_INSTANCE(DisplayManagerService);

const bool REGISTER_RESULT =
    SystemAbility::MakeAndRegisterAbility(SingletonContainer::Get<DisplayManagerService>().GetRefPtr());

DisplayManagerService::DisplayManagerService() : SystemAbility(DISPLAY_MANAGER_SERVICE_SA_ID, true)
{
}

void DisplayManagerService::OnStart()
{
    WLOGFI("DisplayManagerService::OnStart start");
    if (!Init()) {
        return;
    }
}

bool DisplayManagerService::Init()
{
    WLOGFI("DisplayManagerService::Init start");
    bool ret = Publish(this);
    if (!ret) {
        WLOGFW("DisplayManagerService::Init failed");
        return false;
    }
    displayScreenManager_ = DisplayScreenManager::GetInstance();
    if (displayScreenManager_ == nullptr) {
        WLOGFW("Get DisplayScreenManager failed");
        return false;
    }
    WLOGFI("DisplayManagerService::Init success");
    return true;
}

const sptr<DisplayInfo>& DisplayManagerService::GetDisplayInfo(const DisplayType type)
{
    // TODO 从displayScreenMap_得到DisplayInfo
    WLOGFI("DisplayManagerService::GetDisplayInfo");
    return new DisplayInfo();
}

DisplayId DisplayManagerService::GetDisplayIdFromScreenId(ScreenId screenId)
{
    return (DisplayId)screenId;
}

ScreenId DisplayManagerService::GetScreenIdFromDisplayId(DisplayId displayId)
{
    return (ScreenId)displayId;
}

DisplayId DisplayManagerService::GetDefaultDisplayId()
{
    if (displayScreenManager_ == nullptr) {
        return DISPLAY_ID_INVALD;
    }
    ScreenId screenId = displayScreenManager_->GetDefaultScreenId();
    WLOGFI("GetDefaultDisplayId %{public}llu", screenId);
    return GetDisplayIdFromScreenId(screenId);
}

DisplayInfo DisplayManagerService::GetDisplayInfoById(DisplayId displayId)
{
    DisplayInfo displayInfo;
    if (displayScreenManager_ == nullptr) {
        return displayInfo;
    }
    ScreenId screenId = GetScreenIdFromDisplayId(displayId);
    auto screenModeInfo = displayScreenManager_->GetScreenActiveMode(screenId);
    displayInfo.id_ = displayId;
    displayInfo.width_ = screenModeInfo.GetScreenWidth();
    displayInfo.height_ = screenModeInfo.GetScreenHeight();
    displayInfo.freshRate_ = screenModeInfo.GetScreenFreshRate();
    return displayInfo;
}

void DisplayManagerService::OnStop()
{
    WLOGFI("ready to stop display service.");
    displayScreenManager_ = nullptr;
}
} // namespace OHOS::Rosen