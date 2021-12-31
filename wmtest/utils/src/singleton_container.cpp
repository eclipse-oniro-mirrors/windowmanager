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

#include "singleton_container.h"
#include <mutex>
#include "window_manager_hilog.h"

namespace OHOS {
namespace Rosen {
namespace {
constexpr HiviewDFX::HiLogLabel LABEL = {LOG_CORE, 0, "WindowManager_SingletonContainer"};
} // namespace

sptr<SingletonContainer> SingletonContainer::GetInstance()
{
    if (instance == nullptr) {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr) {
            instance = new SingletonContainer();
        }
    }
    return instance;
}

SingletonContainer::~SingletonContainer()
{
    while (singletonMap.empty() == false) {
        auto it = singletonMap.begin();
        while (it != singletonMap.end()) {
            if (it->second.refCount > 0) {
                it++;
                continue;
            }

            if (dependencySetMap.find(it->first) != dependencySetMap.end()) {
                for (auto mid : dependencySetMap[it->first]) {
                    singletonMap[mid].refCount--;
                }
                dependencySetMap.erase(it->first);
            }

            for (const auto &[k, v] : stringMap) {
                if (v == it->first) {
                    WLOGFD("remove %{public}s", k.c_str());
                    break;
                }
            }
            singletonMap.erase(it++);
        }
    }
}

void SingletonContainer::AddSingleton(const std::string &name, const std::any &instance)
{
    if (stringMap.find(name) == stringMap.end()) {
        static int32_t nextId = 0;
        singletonMap[nextId].value = instance;
        singletonMap[nextId].refCount = 0;
        WLOGFD("add %{public}s", name.c_str());
        stringMap[name] = nextId++;
    } else {
        WLOGFE("add failed: %{public}s", name.c_str());
    }
}

void SingletonContainer::SetSingleton(const std::string &name, const std::any &instance)
{
    if (stringMap.find(name) == stringMap.end()) {
        AddSingleton(name, instance);
    } else {
        WLOGFD("set %{public}s", name.c_str());
        singletonMap[stringMap[name]].value = instance;
    }
}

const std::any &SingletonContainer::GetSingleton(const std::string &name)
{
    if (stringMap.find(name) == stringMap.end()) {
        WLOGFD("cannot get %{public}s", name.c_str());
        static std::any voidAny;
        return voidAny;
    }
    return singletonMap[stringMap[name]].value;
}

const std::any &SingletonContainer::DependOn(const std::string &instance, const std::string &name)
{
    auto &instanceDependencySet = dependencySetMap[stringMap[instance]];
    if (instanceDependencySet.find(stringMap[name]) == instanceDependencySet.end()) {
        WLOGFD("%{public}s dependon %{public}s", instance.c_str(), name.c_str());
        instanceDependencySet.insert(stringMap[name]);
        singletonMap[stringMap[name]].refCount++;
    }
    return GetSingleton(name);
}
} // namespace Rosen
} // namespace OHOS
