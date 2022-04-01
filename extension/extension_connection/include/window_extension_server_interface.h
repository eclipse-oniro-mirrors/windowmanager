/*
 * Copyright (c) 2022-2022 Huawei Device Co., Ltd.
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

#ifndef WINDOW_EXTENSION_SERVER_INTERFACE
#define WINDOW_EXTENSION_SERVER_INTERFACE

#include "iremote_broker.h"

#include <i_input_event_consumer.h>
#include <key_event.h>
#include <ui/rs_surface_node.h>

namespace OHOS {
namespace Rosen {
class IWindowExtensionServer : public IRemoteBroker {
public:
    DECLARE_INTERFACE_DESCRIPTOR(u"OHOS.IWindowExtensionServer");
    enum {
        TRANS_ID_ON_WINDOW_READY,
        TRANS_ID_ON_BACK_PRESS,
        TRANS_ID_ON_KEY_EVENT,
        TRANS_ID_ON_POINTER_EVENT,
    };

    virtual void OnWindowReady(const std::shared_ptr<RSSurfaceNode>& surfaceNode) = 0;
    virtual void OnBackPress() = 0;
    virtual void OnKeyEvent(std::shared_ptr<MMI::KeyEvent>& keyEvent) = 0;
    virtual void OnPointerEvent(std::shared_ptr<MMI::PointerEvent>& pointerEvent) = 0;
};
} // namespace Rosen
} // namespace OHOS
#endif // WINDOW_EXTENSION_SERVER_INTERFACE