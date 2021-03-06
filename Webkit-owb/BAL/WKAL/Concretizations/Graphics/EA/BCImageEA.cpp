/*
Copyright (C) 2008-2011 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// BCImageEA.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "BitmapImage.h"

#include "AffineTransform.h"
#include "FloatRect.h"
#include "GraphicsContext.h"
#include "ImageObserver.h"
#include <math.h>
#include <EARaster/EARaster.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitConfig.h>
#include <EARaster/EARaster.h>
#include <EARaster/EARasterColor.h>
#include <EAWebKit/internal/EAWebKitAssert.h>
#include "BCImageCompressionEA.h"

// This function loads resources from WebKit.
Vector<char> loadResourceIntoArray(const char*);


namespace WKAL {


// Inline 16x16 "?" missing image icon binary. 
static const int kBrokenImageWidth = 16; 
static const int kBrokenImageHeight = 16; 
static const int brokenImageBin[] =
{
 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4, 0xff94b1d4
,0xff92afd4, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff92afd4
,0xff90aed3, 0xffffffff, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xff2f8cca, 0xffffffff, 0xff90aed3
,0xff8dacd2, 0xffffffff, 0xff2e8ac9, 0xff2e8ac9, 0xff2e8ac9, 0xff2c86c6, 0xff2c86c6, 0xff2c85c5, 0xff2c85c5, 0xff2c85c5, 0xff2c86c6, 0xff2e89c8, 0xff2e8ac9, 0xff2e8ac9, 0xffffffff, 0xff8dacd2
,0xff8aaad0, 0xffffffff, 0xff2d88c7, 0xff2d88c7, 0xff2b84c4, 0xff2a81c2, 0xff8ebde0, 0xff8ebde0, 0xffb9d6ec, 0xff9cc6e3, 0xff4692ca, 0xff2a81c2, 0xff2d88c7, 0xff2d88c7, 0xffffffff, 0xff8aaad0
,0xff87a7cf, 0xffffffff, 0xff2c86c5, 0xff2c86c5, 0xff2a83c2, 0xff8dbcde, 0xffe3eff7, 0xff2980c0, 0xff4691c8, 0xffffffff, 0xffe3eff7, 0xff297fc0, 0xff2c86c5, 0xff2c86c5, 0xffffffff, 0xff87a7cf
,0xff84a5ce, 0xffffffff, 0xff2b83c3, 0xff2b83c3, 0xff2a80c1, 0xff5498cc, 0xff8dbbdd, 0xff287dbf, 0xff3686c2, 0xffffffff, 0xfff1f7fb, 0xff287dbe, 0xff2b83c3, 0xff2b83c3, 0xffffffff, 0xff84a5ce
,0xff80a2cc, 0xffffffff, 0xff2a80c1, 0xff2a80c1, 0xff297fc0, 0xff297dbf, 0xff297dbf, 0xff287dbe, 0xff9bc2e1, 0xffc6ddee, 0xff5395ca, 0xff277abd, 0xff2a80c1, 0xff2a80c1, 0xffffffff, 0xff80a2cc
,0xff7da0cb, 0xffffffff, 0xff287dbf, 0xff287dbf, 0xff287dbf, 0xff287dbf, 0xff277cbe, 0xff5295ca, 0xff8cb8dc, 0xff2679bc, 0xff2679bc, 0xff277cbe, 0xff287dbf, 0xff287dbf, 0xffffffff, 0xff7da0cb
,0xff7a9eca, 0xffffffff, 0xff277abd, 0xff277abd, 0xff277abd, 0xff277abd, 0xff2576ba, 0xff347fbf, 0xff347fbf, 0xff2576ba, 0xff277abd, 0xff277abd, 0xff277abd, 0xff277abd, 0xffffffff, 0xff7a9eca
,0xff769bc8, 0xffffffff, 0xff2577ba, 0xff2577ba, 0xff2577ba, 0xff2577ba, 0xff2373b7, 0xffd3e4f1, 0xffd3e4f1, 0xff2373b7, 0xff2577ba, 0xff2577ba, 0xff2577ba, 0xff2577ba, 0xffffffff, 0xff769bc8
,0xff7399c7, 0xffffffff, 0xff2474b8, 0xff2474b8, 0xff2474b8, 0xff2474b8, 0xff2270b5, 0xffd3e3f1, 0xffd3e3f1, 0xff2270b5, 0xff2474b8, 0xff2474b8, 0xff2474b8, 0xff2474b8, 0xffffffff, 0xff7399c7
,0xff7096c6, 0xffffffff, 0xff2372b7, 0xff2372b7, 0xff2372b7, 0xff2372b7, 0xff216fb4, 0xff216fb4, 0xff216fb4, 0xff216fb4, 0xff2372b7, 0xff2372b7, 0xff2372b7, 0xff2372b7, 0xffffffff, 0xff7096c6
,0xff6d94c4, 0xffffffff, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xff2270b5, 0xffffffff, 0xff6d94c4
,0xff6a92c3, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff6a92c3
,0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2, 0xff6890c2
};

void FrameData::clear()
{
    if (m_frame)
    {
        EA::WebKit::GetEARasterInstance()->DestroySurface(m_frame);

        m_frame    = 0;
        m_duration = 0;
        m_hasAlpha = true;
    }
}


BitmapImage::BitmapImage(BalSurface* pSurface, ImageObserver* pObserver)
    : Image(pObserver)
    , m_currentFrame(0)
    , m_frames(0)
    , m_frameTimer(0)
    , m_repetitionCount(0)
    , m_repetitionsComplete(0)
    , m_isSolidColor(false)
    , m_animatingImageType(false)
    , m_animationFinished(true)
    , m_allDataReceived(true)
    , m_haveSize(true)
    , m_sizeAvailable(true)
    , m_decodedSize(0)
    , m_haveFrameCount(true)
    , m_frameCount(1)
{
    initPlatformData();

    int width  = 0;
    int height = 0;
	pSurface->GetDimensions(&width, &height);

    m_decodedSize = width * height * 4;
    m_size        = IntSize(width, height);

    m_frames.grow(1);
    m_frames[0].m_frame = pSurface;

    checkForSolidColor();
}


void BitmapImage::draw(GraphicsContext* context, const FloatRect& dst, const FloatRect& src, CompositeOperator op)
{
    // 11/09/09 CSidhall Added notify start of process to user
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImage, EA::WebKit::kVProcessStatusStarted);
	
    // CSidhall 1/14/09 Removed pImage as const pointer so it can be replace by a decompressed version when needed
    EA::Raster::ISurface* pImage = frameAtIndex(m_currentFrame);

    if (pImage) // If it's too early we won't have an image yet.
    {
        if (mayFillWithSolidColor())
            fillWithSolidColor(context, dst, solidColor(), op); // This will skip for a 0 alpha
        else
        {
            // Set the compositing operation.
            if (op == CompositeSourceOver && !frameHasAlphaAtIndex(m_currentFrame))
                context->setCompositeOperation(CompositeCopy);
            else
                context->setCompositeOperation(op);

            EA::Raster::ISurface* pDstSurface = context->platformContext();
            
             // Draw the image.
            EA::Raster::Rect srcRect, dstRect;

            srcRect.x = (int)src.x();
            srcRect.y = (int)src.y();

			int width = 0;
			int height = 0;
			pImage->GetDimensions(&width, &height);

            if (0 == src.width())
                srcRect.w = width;
            else
                srcRect.w = (int)src.width();

            if (0 == src.height())
                srcRect.h = height;
            else
                srcRect.h = (int)src.height();

            dstRect.x = (int)(dst.x() + context->origin().width());
            dstRect.y = (int)(dst.y() + context->origin().height());
            dstRect.w = (int)dst.width();
            dstRect.h = (int)dst.height();

            bool needScale = dst.width() != src.width() || dst.height() != src.height();

            float scaleX = needScale ? (dst.width() / src.width()) : 1.0f;
            float scaleY = needScale ? (dst.height() / src.height()) : 1.0f;
            EA::Raster::Matrix2D imageTransform(scaleX, 0.0f, 0.0f, scaleY, 0.0f, 0.0f);

            if(context->hasTransform())
            {
                if (needScale)
                {
                    // YChin: Not terribly efficient, maybe just pass the scale transform to context->transform to prevent pushing stack
                    context->save();
                    context->concatCTM(WKAL::AffineTransform(scaleX, 0, 0, scaleY, 0, 0));
                }
                context->transform(srcRect, &dstRect, &imageTransform);
                if (needScale)
                {
                    context->restore();
                }
            }

			EA::Raster::IEARaster* pRaster = EA::WebKit::GetEARasterInstance();
			pRaster->DrawSurface(pImage, srcRect, pDstSurface, dstRect, imageTransform, context->transparencyLayer());

            startAnimation();

            if (imageObserver())
                imageObserver()->didDraw(this);
        }
    }

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImage, EA::WebKit::kVProcessStatusEnded);
}


void Image::drawPattern(GraphicsContext* context, const FloatRect& tileRect, const AffineTransform& patternTransform,
                        const FloatPoint& phase, CompositeOperator op, const FloatRect& destRect)
{
    if (destRect.isEmpty())
        return;

    // Note: For drawPattern, 1x1 texel textures detected with mayFillWithSolidColor() are already handled by Image::drawTiled().

    NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusStarted);
	
    // CSidhall - Removed pImage as const pointer so it can be changed for decompression
    EA::Raster::ISurface* pImage = nativeImageForCurrentFrame();

    if (!pImage) // If it's too early we won't have an image yet.
	{
		//abaldeva -  Long term, we may want to revise the process status to be something inspired from RAII pattern. For now, this will work.
		NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusEnded);
		return;
	}

    EA::Raster::ISurface* const cr = context->platformContext();
    context->save();
    context->setCompositeOperation(op);

    EA::Raster::Rect clipRect = cr->GetClipRect();

    EA::Raster::Rect srcRect(0, 0, 0, 0);
    EA::Raster::Rect dstRect(0, 0, 0, 0);
    IntSize origin = context->origin();       

    srcRect.w = static_cast<int>(tileRect.width());
    srcRect.h = static_cast<int>(tileRect.height());

    // Adjust source rect if needed.
    int width = 0;
    int height = 0;
    pImage->GetDimensions(&width, &height);

    if (srcRect.w == 0) {
        srcRect.w = width;
    }

    if (srcRect.h == 0) {
        srcRect.h = height;
    }

    // Compute destination rect and adjust clipping.
    dstRect.x = static_cast<int>(phase.x() + origin.width());
    dstRect.y = static_cast<int>(phase.y() + origin.height());

    // Tiling needs to extend to the original destRect, so compute the width/height to that point.
    dstRect.w = static_cast<int>(destRect.x() + destRect.width()) - dstRect.x;
    dstRect.h = static_cast<int>(destRect.y() + destRect.height()) - dstRect.y;

	// Removing these, negative coordinates wont' render properly but it's non-fatal.
    //EAW_ASSERT(dstRect.w > 0);
    //EAW_ASSERT(dstRect.h > 0);

    // 2/9/09 CSidhall Height and width adjustments
    // This is to fix a background clip bug when scrolling
    FloatRect adjDestRect = destRect;
    float dy =  destRect.y() + origin.height();
    if(dy < 0.0f)
    {
        float heightAdj = adjDestRect.height() + dy;
        if(heightAdj <= 0.0f)
        {
            // Added to fix negative or 0 height clip bug 
            context->restore();
            //abaldeva -  Long term, we may want to revise the process status to be something inspired from RAII pattern. For now, this will work.
            NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusEnded);
            return;
        }
        adjDestRect.setHeight(heightAdj);
    }
    float dx =  destRect.x() + origin.width();
    if(dx < 0.0f)
    {
        float widthAdj = adjDestRect.width() + dx;
        if(widthAdj <= 0.0f)
        {
            context->restore();
            //abaldeva -  Long term, we may want to revise the process status to be something inspired from RAII pattern. For now, this will work.
            NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusEnded);
            return;
        }
        adjDestRect.setWidth(widthAdj);
    }

    context->clip(IntRect(adjDestRect));

    EA::Raster::Matrix2D imageTransform(patternTransform.a(), patternTransform.b(), patternTransform.c(), patternTransform.d(), patternTransform.e(), patternTransform.f());
    EA::Raster::IEARaster* pRaster = EA::WebKit::GetEARasterInstance();
    pRaster->DrawSurfaceTiled(pImage, srcRect, cr, dstRect, clipRect, imageTransform, context->transparencyLayer());
  
    context->restore();

    if (imageObserver())
        imageObserver()->didDraw(this);

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawImagePattern, EA::WebKit::kVProcessStatusEnded);
}


void BitmapImage::initPlatformData()
{
}


void BitmapImage::invalidatePlatformData()
{
}

Image* Image::loadPlatformResource(const char* name)
{
    BitmapImage* img=NULL; 
    const char* kMissingName="missingImage";  

   // 11/30/10 CSidhall - Added support for missing image by providing an inline error icon texture (brokenImageBin).
   if (strcmp(name,kMissingName) == 0)
   {
       // We create a surface that uses the inline image data directly
       EA::Raster::ISurface* pSurface =   EA::WebKit::GetEARasterInstance()->CreateSurface((void*) &brokenImageBin[0],
                                                                                            kBrokenImageWidth,kBrokenImageHeight, 
                                                                                            kBrokenImageWidth * 4,            
                                                                                            EA::Raster::kPixelFormatTypeARGB, false, EA::Raster::kSurfaceCategoryImage);  
       // This will also set up the first frame and the image size
       img = new BitmapImage(pSurface); 
   }
   else 
   {
        // CSidhall - Don't think this option actually would work with our images as the size and frame set up are private.
        // Also the buffer passed/shared here is a Vector.
        Vector<char> arr = loadResourceIntoArray(name);  // Note by Paul Pedriana: The current version of loadResourceIntoArray is a no-op.
        img = new BitmapImage;
        RefPtr<SharedBuffer> buffer = SharedBuffer::create(arr.data(), arr.size());
        img->setData(buffer, true);
   }
   return img;

}

// Checks to see if the image is of a solid color. We optimize these images and just do a fill rect instead.
void BitmapImage::checkForSolidColor()
{
    if (frameCount() == 1) // We could check multiple frames, but generally all that matters is the special case of a 1x1 single frame.
    {
        // CS - Removed pSurface pointer as const so can change it for decompressed versions
        EA::Raster::ISurface* pSurface = frameAtIndex(0);

		int width = 0;
		int height = 0;
		pSurface->GetDimensions(&width, &height);

        if(pSurface && (width == 1) && (height == 1))
        {
			void *pData = NULL;
			int stride = 0;
            pSurface->Lock(&pData, &stride);

			if (pData == NULL) {
				return;
			}

            int      bpp = pSurface->GetPixelFormat().mBytesPerPixel;
            uint8_t* p   = (uint8_t*)pData; //  + (0 * pSurface->pitch) + (0 * bpp);
            uint32_t color;

            switch (bpp)
            {
                case 3:
                    // Note by Paul Pedriana: I'm not sure this is correct.
                    color = (p[0] << 16) | (p[1] <<  8) | (p[0] << 0);  // ARGB
                    break;

                default:
                case 4:
                    color = *(uint32_t*)p; // This assumes p is 32 bit aligned.
                    break;
            }
            pSurface->Unlock();

			EA::Raster::IEARaster* pRaster = EA::WebKit::GetEARasterInstance();
			EA::Raster::Color translatedColor;
            pRaster->ConvertColor(color, pSurface->GetPixelFormat(), &translatedColor);
            int r = translatedColor.red();
			int g = translatedColor.green();
			int b = translatedColor.blue();
			int a = translatedColor.alpha();
			m_solidColor.setRGB(EA::Raster::makeRGBA(r, g, b, a));
            
			if((a >= 255) || (a == 0))
                m_isSolidColor = true;  // This will fill instead of texture draw to safe some time. In case of 0 alpha, will exit the fill.
        }
    }
}

// CSidhall 7/14/09 - Added image lock for prune from decoder (false is not locked)
 bool BitmapImage::imagePruneLockStatus() const
 {
     return m_source.imagePruneLockStatus();
 }


} // namespace

