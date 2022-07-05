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

#include "surface_draw.h"

#include <algorithm>
#include <surface.h>
#include <ui/rs_surface_extractor.h>
#include "window_manager_hilog.h"

#include "image/bitmap.h"
#include "image_source.h"
#include "image_type.h"
#include "image_utils.h"
#include "pixel_map.h"

namespace OHOS {
namespace Rosen {
namespace {
    constexpr HiviewDFX::HiLogLabel LABEL = {LOG_CORE, HILOG_DOMAIN_WINDOW, "SurfaceDraw"};
} // namespace

bool SurfaceDraw::DrawImage(std::shared_ptr<RSSurfaceNode> surfaceNode, int32_t bufferWidth,
    int32_t bufferHeight, const std::string& imagePath)
{
    sptr<OHOS::Surface> layer = GetLayer(surfaceNode);
    if (layer == nullptr) {
        WLOGFE("layer is nullptr");
        return false;
    }
    sptr<OHOS::SurfaceBuffer> buffer = GetSurfaceBuffer(layer, bufferWidth, bufferHeight);
    if (buffer == nullptr || buffer->GetVirAddr() == nullptr) {
        return false;
    }
    auto addr = static_cast<uint8_t *>(buffer->GetVirAddr());
    if (!DoDraw(addr, buffer->GetWidth(), buffer->GetHeight(), imagePath)) {
        WLOGE("draw window pixel failed");
        return false;
    }
    OHOS::BufferFlushConfig flushConfig = {
        .damage = {
            .w = buffer->GetWidth(),
            .h = buffer->GetHeight(),
        },
    };
    OHOS::SurfaceError ret = layer->FlushBuffer(buffer, -1, flushConfig);
    if (ret != OHOS::SurfaceError::SURFACE_ERROR_OK) {
        WLOGFE("draw pointer FlushBuffer ret:%{public}s", SurfaceErrorStr(ret).c_str());
        return false;
    }
    return true;
}

bool SurfaceDraw::DrawImage(std::shared_ptr<RSSurfaceNode> surfaceNode, int32_t bufferWidth,
    int32_t bufferHeight, std::shared_ptr<Media::PixelMap> pixelMap)
{
    sptr<OHOS::Surface> layer = GetLayer(surfaceNode);
    if (layer == nullptr) {
        WLOGFE("layer is nullptr");
        return false;
    }
    sptr<OHOS::SurfaceBuffer> buffer = GetSurfaceBuffer(layer, bufferWidth, bufferHeight);
    if (buffer == nullptr || buffer->GetVirAddr() == nullptr) {
        return false;
    }
    auto addr = static_cast<uint8_t *>(buffer->GetVirAddr());
    if (!DoDraw(addr, buffer->GetWidth(), buffer->GetHeight(), pixelMap)) {
        WLOGE("draw window pixel failed");
        return false;
    }
    OHOS::BufferFlushConfig flushConfig = {
        .damage = {
            .w = buffer->GetWidth(),
            .h = buffer->GetHeight(),
        },
    };
    OHOS::SurfaceError ret = layer->FlushBuffer(buffer, -1, flushConfig);
    if (ret != OHOS::SurfaceError::SURFACE_ERROR_OK) {
        WLOGFE("draw pointer FlushBuffer ret:%{public}s", SurfaceErrorStr(ret).c_str());
        return false;
    }
    return true;
}

bool SurfaceDraw::DrawColor(std::shared_ptr<RSSurfaceNode> surfaceNode, int32_t bufferWidth,
    int32_t bufferHeight, uint32_t color)
{
    sptr<OHOS::Surface> layer = GetLayer(surfaceNode);
    if (layer == nullptr) {
        WLOGFE("layer is nullptr");
        return false;
    }
    sptr<OHOS::SurfaceBuffer> buffer = GetSurfaceBuffer(layer, bufferWidth, bufferHeight);
    if (buffer == nullptr || buffer->GetVirAddr() == nullptr) {
        return false;
    }
    auto addr = static_cast<uint8_t *>(buffer->GetVirAddr());
    if (!DoDraw(addr, buffer->GetWidth(), buffer->GetHeight(), color)) {
        WLOGE("draw window color failed");
        return false;
    }
    OHOS::BufferFlushConfig flushConfig = {
        .damage = {
            .w = buffer->GetWidth(),
            .h = buffer->GetHeight(),
        },
    };
    OHOS::SurfaceError ret = layer->FlushBuffer(buffer, -1, flushConfig);
    if (ret != OHOS::SurfaceError::SURFACE_ERROR_OK) {
        WLOGFE("draw pointer FlushBuffer ret:%{public}s", SurfaceErrorStr(ret).c_str());
        return false;
    }
    return true;
}

sptr<OHOS::Surface> SurfaceDraw::GetLayer(std::shared_ptr<RSSurfaceNode> surfaceNode)
{
    if (surfaceNode == nullptr) {
        return nullptr;
    }
    return surfaceNode->GetSurface();
}

sptr<OHOS::SurfaceBuffer> SurfaceDraw::GetSurfaceBuffer(sptr<OHOS::Surface> layer,
    int32_t bufferWidth, int32_t bufferHeight)
{
    sptr<OHOS::SurfaceBuffer> buffer;
    int32_t releaseFence = 0;
    OHOS::BufferRequestConfig config = {
        .width = bufferWidth,
        .height = bufferHeight,
        .strideAlignment = 0x8,
        .format = PIXEL_FMT_RGBA_8888,
        .usage = HBM_USE_CPU_READ | HBM_USE_CPU_WRITE | HBM_USE_MEM_DMA,
    };

    OHOS::SurfaceError ret = layer->RequestBuffer(buffer, releaseFence, config);
    if (ret != OHOS::SURFACE_ERROR_OK) {
        WLOGFE("request buffer ret:%{public}s", SurfaceErrorStr(ret).c_str());
        return nullptr;
    }
    return buffer;
}

std::unique_ptr<OHOS::Media::PixelMap> SurfaceDraw::DecodeImageToPixelMap(const std::string &imagePath)
{
    OHOS::Media::SourceOptions opts;
    opts.formatHint = "image/png";
    uint32_t ret = 0;
    auto imageSource = OHOS::Media::ImageSource::CreateImageSource(imagePath, opts, ret);
    if (imageSource == nullptr) {
        WLOGFE("invalid image path.");
        return nullptr;
    }
    std::set<std::string> formats;
    ret = imageSource->GetSupportedFormats(formats);
    WLOGFD("get supported format ret:%{public}u", ret);

    OHOS::Media::DecodeOptions decodeOpts;
    std::unique_ptr<OHOS::Media::PixelMap> pixelMap = imageSource->CreatePixelMap(decodeOpts, ret);
    if (pixelMap == nullptr) {
        WLOGFE("pixelMap is nullptr");
    }
    return pixelMap;
}

void SurfaceDraw::DrawPixelmap(Drawing::Canvas &canvas, const std::string& imagePath)
{
    std::unique_ptr<OHOS::Media::PixelMap> pixelmap = DecodeImageToPixelMap(imagePath);
    if (pixelmap == nullptr) {
        WLOGFE("drawing pixel map is nullptr");
        return;
    }
    Drawing::Pen pen;
    pen.SetAntiAlias(true);
    pen.SetColor(Drawing::Color::COLOR_BLUE);
    Drawing::scalar penWidth = 1;
    pen.SetWidth(penWidth);
    canvas.AttachPen(pen);
    canvas.DrawBitmap(*pixelmap, 0, 0);
}

bool SurfaceDraw::DoDraw(uint8_t *addr, uint32_t width, uint32_t height, const std::string& imagePath)
{
    Drawing::Bitmap bitmap;
    Drawing::BitmapFormat format { Drawing::ColorType::COLORTYPE_RGBA_8888,
        Drawing::AlphaType::ALPHATYPE_OPAQUE };
    bitmap.Build(width, height, format);
    Drawing::Canvas canvas;
    canvas.Bind(bitmap);
    canvas.Clear(Drawing::Color::COLOR_TRANSPARENT);
    DrawPixelmap(canvas, imagePath);
    static constexpr uint32_t stride = 4;
    uint32_t addrSize = width * height * stride;
    errno_t ret = memcpy_s(addr, addrSize, bitmap.GetPixels(), addrSize);
    if (ret != EOK) {
        WLOGFE("draw failed");
        return false;
    }
    return true;
}

bool SurfaceDraw::DoDraw(uint8_t *addr, uint32_t width, uint32_t height, std::shared_ptr<Media::PixelMap> pixelMap)
{
    Drawing::Bitmap bitmap;
    Drawing::Canvas canvas;
    Drawing::BitmapFormat format { Drawing::ColorType::COLORTYPE_RGBA_8888, Drawing::AlphaType::ALPHATYPE_OPAQUE };
    bitmap.Build(width, height, format);
    canvas.Bind(bitmap);
    canvas.Clear(Drawing::Color::COLOR_TRANSPARENT);

    Drawing::Image image;
    Drawing::Bitmap imageBitmap;
    Drawing::SamplingOptions sampling = Drawing::SamplingOptions(Drawing::FilterMode::NEAREST,
    Drawing::MipmapMode::NEAREST);
    imageBitmap.Build(pixelMap->GetWidth(), pixelMap->GetHeight(), format);
    imageBitmap.SetPixels(const_cast<uint8_t*>(pixelMap->GetPixels()));
    image.BuildFromBitmap(imageBitmap);

    Drawing::Rect dst(0, 0, width, height);
    Drawing::Rect src(0, 0, pixelMap->GetWidth(), pixelMap->GetHeight());
    canvas.DrawImageRect(image, src, dst, sampling);

    static constexpr uint32_t stride = 4;
    uint32_t addrSize = width * height * stride;
    errno_t ret = memcpy_s(addr, addrSize, bitmap.GetPixels(), addrSize);
    if (ret != EOK) {
        WLOGFE("draw failed");
        return false;
    }
    return true;
}

bool SurfaceDraw::DoDraw(uint8_t *addr, uint32_t width, uint32_t height, uint32_t color)
{
    Drawing::Bitmap bitmap;
    Drawing::BitmapFormat format { Drawing::ColorType::COLORTYPE_RGBA_8888,
        Drawing::AlphaType::ALPHATYPE_OPAQUE };
    bitmap.Build(width, height, format);
    Drawing::Canvas canvas;
    canvas.Bind(bitmap);
    canvas.Clear(color);

    static constexpr uint32_t stride = 4;
    uint32_t addrSize = width * height * stride;
    errno_t ret = memcpy_s(addr, addrSize, bitmap.GetPixels(), addrSize);
    if (ret != EOK) {
        WLOGFE("draw failed");
        return false;
    }
    return true;
}

bool SurfaceDraw::DrawImageRect(std::shared_ptr<RSSurfaceNode> surfaceNode, Rect rect,
    sptr<Media::PixelMap> pixelMap, uint32_t color)
{
    int32_t winHeight = static_cast<int32_t>(rect.height_);
    int32_t winWidth = static_cast<int32_t>(rect.width_);
    sptr<OHOS::Surface> layer = GetLayer(surfaceNode);
    if (layer == nullptr) {
        WLOGFE("layer is nullptr");
        return false;
    }
    sptr<OHOS::SurfaceBuffer> buffer = GetSurfaceBuffer(layer, winWidth, winHeight);
    if (buffer == nullptr || buffer->GetVirAddr() == nullptr) {
        return false;
    }
    auto addr = static_cast<uint8_t *>(buffer->GetVirAddr());
    if (!DoDrawImageRect(addr, winWidth, winHeight, pixelMap, color)) {
        WLOGE("draw image rect failed.");
        return false;
    }
    OHOS::BufferFlushConfig flushConfig = {
        .damage = {
            .w = buffer->GetWidth(),
            .h = buffer->GetHeight(),
        },
    };
    OHOS::SurfaceError surfaceRet = layer->FlushBuffer(buffer, -1, flushConfig);
    if (surfaceRet != OHOS::SurfaceError::SURFACE_ERROR_OK) {
        WLOGFE("draw pointer FlushBuffer ret:%{public}s", SurfaceErrorStr(surfaceRet).c_str());
        return false;
    }
    return true;
}

bool SurfaceDraw::DoDrawImageRect(uint8_t *addr, int32_t winWidth, int32_t winHeight,
    sptr<Media::PixelMap> pixelMap, uint32_t color)
{
    Drawing::Bitmap bitmap;
    Drawing::BitmapFormat format { Drawing::ColorType::COLORTYPE_RGBA_8888,
        Drawing::AlphaType::ALPHATYPE_OPAQUE };
    bitmap.Build(winWidth, winHeight, format);
    Drawing::Canvas canvas;
    canvas.Bind(bitmap);
    canvas.Clear(color);
    if (pixelMap == nullptr) {
        WLOGFE("drawing pixel map is nullptr");
        return false;
    }
    uint32_t iconHeight = pixelMap->GetHeight();
    uint32_t iconWidth = pixelMap->GetWidth();
    Drawing::Image image;
    Drawing::Bitmap iconBitmap;
    iconBitmap.Build(iconWidth, iconHeight, format);
    iconBitmap.SetPixels(const_cast<uint8_t*>(pixelMap->GetPixels()));
    image.BuildFromBitmap(iconBitmap);
    Drawing::SamplingOptions sampling = Drawing::SamplingOptions(Drawing::FilterMode::NEAREST,
        Drawing::MipmapMode::NEAREST);

    int realHeight = std::min(winHeight, pixelMap->GetHeight()); // need to scale
    int realWidth = std::min(winWidth, pixelMap->GetWidth());
    int pointX = (winWidth - realWidth) / 2; // 2 is mid point
    int pointY = (winHeight - realHeight) / 2; // 2 is mid point
    Drawing::Rect dst(pointX, pointY, pointX + realWidth, pointY + realHeight);
    canvas.DrawImageRect(image, dst, sampling);

    uint32_t stride = 4;
    uint32_t addrSize = winWidth * winHeight * stride;
    errno_t ret = memcpy_s(addr, addrSize, bitmap.GetPixels(), addrSize);
    if (ret != EOK) {
        WLOGFE("draw failed");
        return false;
    }
    return true;
}
} // Rosen
} // OHOS