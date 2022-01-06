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

#ifndef OHOS_ROSEN_WINDOW_OPTION_H
#define OHOS_ROSEN_WINDOW_OPTION_H
#include <refbase.h>
#include <string>
#include <unordered_map>
#include "wm_common.h"

namespace OHOS {
namespace Rosen {
class WindowOption : public RefBase {
public:
    WindowOption();
    virtual ~WindowOption() = default;

    void SetWindowRect(const struct Rect& rect);
    void SetWindowType(WindowType type);
    void SetWindowMode(WindowMode mode);
    void SetFocusable(bool isFocusable);
    void SetTouchable(bool isTouchable);
    void SetDisplayId(int32_t displayId);
    void SetParentName(const std::string& parentName);
    void SetWindowName(const std::string& windowName);
    void AddWindowFlag(WindowFlag flag);
    void RemoveWindowFlag(WindowFlag flag);
    void SetWindowFlags(uint32_t flags);
    void SetSystemBarProperty(WindowType type, const SystemBarProperty& property);

    Rect GetWindowRect() const;
    WindowType GetWindowType() const;
    WindowMode GetWindowMode() const;
    bool GetFocusable() const;
    bool GetTouchable() const;
    int32_t GetDisplayId() const;
    const std::string& GetParentName() const;
    const std::string& GetWindowName() const;
    uint32_t GetWindowFlags() const;
    const std::unordered_map<WindowType, SystemBarProperty>& GetSystemBarProperty() const;
private:
    Rect windowRect_ { 0, 0, 0, 0 };
    WindowType type_ { WindowType::WINDOW_TYPE_APP_MAIN_WINDOW };
    WindowMode mode_ { WindowMode::WINDOW_MODE_FULLSCREEN };
    bool focusable_ { true };
    bool touchable_ { true };
    int32_t displayId_ { 0 };
    std::string parentName_ { "" };
    std::string windowName_ { "" };
    uint32_t flags_ { 0 };
    std::unordered_map<WindowType, SystemBarProperty> sysBarPropMap_ {
        { WindowType::WINDOW_TYPE_STATUS_BAR,     SystemBarProperty() },
        { WindowType::WINDOW_TYPE_NAVIGATION_BAR, SystemBarProperty() },
    };
};
}
}
#endif // OHOS_ROSEN_WINDOW_OPTION_H
