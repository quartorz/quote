
/************************************************************************
 *
 * File: CustomTextRenderer.h
 *
 * Description: 
 * 
 * 
 *  This file is part of the Microsoft Windows SDK Code Samples.
 * 
 *  Copyright (C) Microsoft Corporation.  All rights reserved.
 * 
 * This source code is intended only as a supplement to Microsoft
 * Development Tools and/or on-line documentation.  See these other
 * materials for detailed information regarding Microsoft code samples.
 * 
 * THIS CODE AND INFORMATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 * 
 ************************************************************************/

#pragma once

/******************************************************************
*                                                                 *
*  CustomTextRenderer                                             *
*                                                                 *
*  The IDWriteTextRenderer interface is an input parameter to     *
*  IDWriteTextLayout::Draw.  This interfaces defines a number of  *
*  callback functions that the client application implements for  *
*  custom text rendering.  This sample renderer implementation    *
*  renders text using text outlines and Direct2D.                 *
*  A more sophisticated client would also support bitmap          *
*  renderings.                                                    *
*                                                                 *
******************************************************************/

namespace quote{ namespace direct2d{ namespace detail{

class CustomTextRenderer : public IDWriteTextRenderer
{
public:
    CustomTextRenderer(
        ID2D1Factory* pD2DFactory, 
        ID2D1RenderTarget* pRT
        );

    ~CustomTextRenderer();

    STDMETHOD(IsPixelSnappingDisabled)(
        __maybenull void* clientDrawingContext,
        __out BOOL* isDisabled
        );

    STDMETHOD(GetCurrentTransform)(
        __maybenull void* clientDrawingContext,
        __out DWRITE_MATRIX* transform
        );

    STDMETHOD(GetPixelsPerDip)(
        __maybenull void* clientDrawingContext,
        __out FLOAT* pixelsPerDip
        );

    STDMETHOD(DrawGlyphRun)(
        __maybenull void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        DWRITE_MEASURING_MODE measuringMode,
        __in DWRITE_GLYPH_RUN const* glyphRun,
        __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
        IUnknown* clientDrawingEffect
        );

    STDMETHOD(DrawUnderline)(
        __maybenull void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        __in DWRITE_UNDERLINE const* underline,
        IUnknown* clientDrawingEffect
        );

    STDMETHOD(DrawStrikethrough)(
        __maybenull void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        __in DWRITE_STRIKETHROUGH const* strikethrough,
        IUnknown* clientDrawingEffect
        );

    STDMETHOD(DrawInlineObject)(
        __maybenull void* clientDrawingContext,
        FLOAT originX,
        FLOAT originY,
        IDWriteInlineObject* inlineObject,
        BOOL isSideways,
        BOOL isRightToLeft,
        IUnknown* clientDrawingEffect
        );

public:
    unsigned long STDMETHODCALLTYPE AddRef();
    unsigned long STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(
        IID const& riid,
        void** ppvObject
    );

private:
    unsigned long cRefCount_;
    ID2D1Factory* pD2DFactory_;
    ID2D1RenderTarget* pRT_;
};

} } }

#include "CustomTextRenderer.cpp"
