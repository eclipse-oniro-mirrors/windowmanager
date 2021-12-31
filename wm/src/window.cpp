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

#include "window.h"
#include "window_impl.h"
#include "window_manager_hilog.h"

namespace OHOS {
namespace Rosen {
namespace {
    constexpr HiviewDFX::HiLogLabel LABEL = {LOG_CORE, 0, "WindowImpl"};
}
sptr<Window> Window::Create(const std::string& windowName, sptr<WindowOption>& option,
    const sptr<IRemoteObject>& abilityToken)
{
    if (windowName.empty()) {
        WLOGFE("window name is empty");
        return nullptr;
    }
    if (option == nullptr) {
        option = new WindowOption();
    }
    option->SetWindowName(windowName);
    sptr<WindowImpl> windowImpl = new WindowImpl(option);
    WMError error = windowImpl->Create(option->GetParentName(), abilityToken);
    if (error != WMError::WM_OK) {
        return nullptr;
    }
    return windowImpl;
}

sptr<Window> Window::Find(const std::string& windowName)
{
    return WindowImpl::Find(windowName);
}
}
}
