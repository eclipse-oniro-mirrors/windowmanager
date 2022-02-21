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

#include "screen.h"

#include "display_manager_adapter.h"
#include "screen_info.h"
#include "window_manager_hilog.h"

namespace OHOS::Rosen {
namespace {
    constexpr HiviewDFX::HiLogLabel LABEL = {LOG_CORE, 0, "Screen"};
}
class Screen::Impl : public RefBase {
public:
    Impl(sptr<ScreenInfo> info)
    {
        if (info == nullptr) {
            WLOGFE("ScreenInfo is nullptr.");
        }
        screenInfo_ = info;
    }
    ~Impl() = default;
    DEFINE_VAR_FUNC_GET_SET(sptr<ScreenInfo>, ScreenInfo, screenInfo);
};

Screen::Screen(sptr<ScreenInfo> info)
    : pImpl_(new Impl(info))
{
}

Screen::~Screen()
{
}

bool Screen::IsGroup() const
{
    SingletonContainer::Get<ScreenManagerAdapter>().UpdateScreenInfo(GetId());
    return pImpl_->GetScreenInfo()->GetCanHasChild();
}

ScreenId Screen::GetId() const
{
    return pImpl_->GetScreenInfo()->GetScreenId();
}

uint32_t Screen::GetWidth() const
{
    SingletonContainer::Get<ScreenManagerAdapter>().UpdateScreenInfo(GetId());
    auto modeId = GetModeId();
    auto modes = GetSupportedModes();
    if (modeId < 0 || modeId >= modes.size()) {
        return 0;
    }
    return modes[modeId]->width_;
}

uint32_t Screen::GetHeight() const
{
    SingletonContainer::Get<ScreenManagerAdapter>().UpdateScreenInfo(GetId());
    auto modeId = GetModeId();
    auto modes = GetSupportedModes();
    if (modeId < 0 || modeId >= modes.size()) {
        return 0;
    }
    return modes[modeId]->height_;
}

uint32_t Screen::GetVirtualWidth() const
{
    SingletonContainer::Get<ScreenManagerAdapter>().UpdateScreenInfo(GetId());
    return pImpl_->GetScreenInfo()->GetVirtualWidth();
}

uint32_t Screen::GetVirtualHeight() const
{
    SingletonContainer::Get<ScreenManagerAdapter>().UpdateScreenInfo(GetId());
    return pImpl_->GetScreenInfo()->GetVirtualHeight();
}

float Screen::GetVirtualPixelRatio() const
{
    SingletonContainer::Get<ScreenManagerAdapter>().UpdateScreenInfo(GetId());
    return pImpl_->GetScreenInfo()->GetVirtualPixelRatio();
}

Rotation Screen::GetRotation()
{
    SingletonContainer::Get<ScreenManagerAdapter>().UpdateScreenInfo(GetId());
    return pImpl_->GetScreenInfo()->GetRotation();
}

bool Screen::RequestRotation(Rotation rotation)
{
    WLOGFD("rotation the screen");
    return SingletonContainer::Get<ScreenManagerAdapter>().RequestRotation(GetId(), rotation);
}

DMError Screen::GetScreenSupportedColorGamuts(std::vector<ScreenColorGamut>& colorGamuts) const
{
    return SingletonContainer::Get<ScreenManagerAdapter>().GetScreenSupportedColorGamuts(GetId(), colorGamuts);
}

DMError Screen::GetScreenColorGamut(ScreenColorGamut& colorGamut) const
{
    return SingletonContainer::Get<ScreenManagerAdapter>().GetScreenColorGamut(GetId(), colorGamut);
}

DMError Screen::SetScreenColorGamut(int32_t colorGamutIdx)
{
    return SingletonContainer::Get<ScreenManagerAdapter>().SetScreenColorGamut(GetId(), colorGamutIdx);
}

DMError Screen::GetScreenGamutMap(ScreenGamutMap& gamutMap) const
{
    return SingletonContainer::Get<ScreenManagerAdapter>().GetScreenGamutMap(GetId(), gamutMap);
}

DMError Screen::SetScreenGamutMap(ScreenGamutMap gamutMap)
{
    return SingletonContainer::Get<ScreenManagerAdapter>().SetScreenGamutMap(GetId(), gamutMap);
}

DMError Screen::SetScreenColorTransform()
{
    return SingletonContainer::Get<ScreenManagerAdapter>().SetScreenColorTransform(GetId());
}

ScreenId Screen::GetParentId() const
{
    return pImpl_->GetScreenInfo()->GetParentId();
}

uint32_t Screen::GetModeId() const
{
    return pImpl_->GetScreenInfo()->GetModeId();
}

std::vector<sptr<SupportedScreenModes>> Screen::GetSupportedModes() const
{
    return pImpl_->GetScreenInfo()->GetModes();
}

bool Screen::SetScreenActiveMode(uint32_t modeId)
{
    ScreenId screenId = GetId();
    if (modeId < 0 || modeId >= GetSupportedModes().size()) {
        return false;
    }
    if (SingletonContainer::Get<ScreenManagerAdapter>().SetScreenActiveMode(screenId, modeId)) {
        pImpl_->GetScreenInfo()->SetModeId(modeId);
        return true;
    }
    return false;
}

void Screen::UpdateScreenInfo(sptr<ScreenInfo> screenInfo)
{
    if (screenInfo == nullptr) {
        WLOGFE("ScreenInfo is invalid");
        return;
    }
    pImpl_->SetScreenInfo(screenInfo);
}
} // namespace OHOS::Rosen