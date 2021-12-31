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

#include "window_option.h"

namespace OHOS {
namespace Rosen {
WindowOption::WindowOption()
{
    AddWindowFlag(WindowFlag::WINDOW_FLAG_NEED_AVOID);
}

void WindowOption::SetWindowRect(const struct Rect& rect)
{
    windowRect_ = rect;
}

void WindowOption::SetWindowType(WindowType type)
{
    type_ = type;
}

void WindowOption::SetWindowMode(WindowMode mode)
{
    mode_ = mode;
}

void WindowOption::SetFocusable(bool isFocusable)
{
    focusable_ = isFocusable;
}

void WindowOption::SetTouchable(bool isTouchable)
{
    touchable_ = isTouchable;
}

void WindowOption::SetDisplayId(int32_t displayId)
{
    displayId_ = displayId;
}

void WindowOption::SetParentName(const std::string& parentName)
{
    parentName_ = parentName;
}

void WindowOption::SetWindowName(const std::string& windowName)
{
    windowName_ = windowName;
}

void WindowOption::AddWindowFlag(WindowFlag flag)
{
    flags_ |= static_cast<uint32_t>(flag);
}

void WindowOption::RemoveWindowFlag(WindowFlag flag)
{
    flags_ &= ~(static_cast<uint32_t>(flag));
}

void WindowOption::SetWindowFlags(uint32_t flags)
{
    flags_ = flags;
}

Rect WindowOption::GetWindowRect() const
{
    return windowRect_;
}

WindowType WindowOption::GetWindowType() const
{
    return type_;
}

WindowMode WindowOption::GetWindowMode() const
{
    return mode_;
}

bool WindowOption::GetFocusable() const
{
    return focusable_;
}

bool WindowOption::GetTouchable() const
{
    return touchable_;
}

int32_t WindowOption::GetDisplayId() const
{
    return displayId_;
}

const std::string& WindowOption::GetParentName() const
{
    return parentName_;
}

const std::string& WindowOption::GetWindowName() const
{
    return windowName_;
}

uint32_t WindowOption::GetWindowFlags() const
{
    return flags_;
}
}
}

