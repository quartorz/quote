
/************************************************************************
*
* File: CustomTextRenderer.cpp
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

#include "DrawingEffect.h"
#include <numeric>

namespace quote{ namespace direct2d{ namespace detail{

/******************************************************************
*                                                                 *
*  CustomTextRenderer::CustomTextRenderer                         *
*                                                                 *
*  The constructor stores the Direct2D factory, the render        *
*  target.                                                        *
*                                                                 *
******************************************************************/

CustomTextRenderer::CustomTextRenderer(
	ID2D1Factory* pD2DFactory,
	ID2D1RenderTarget* pRT
	)
	:
	cRefCount_(0),
	pD2DFactory_(pD2DFactory),
	pRT_(pRT)
{
	pD2DFactory_->AddRef();
	pRT_->AddRef();
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::~CustomTextRenderer                        *
*                                                                 *
*  Destructor releases the factory and render target.             *
*                                                                 *
******************************************************************/

CustomTextRenderer::~CustomTextRenderer()
{
	SafeRelease(&pD2DFactory_);
	SafeRelease(&pRT_);
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::DrawGlyphRun                               *
*                                                                 *
*  Gets GlyphRun outlines via IDWriteFontFace::GetGlyphRunOutline *
*  and then draws and fills them using Direct2D path geometries   *
*                                                                 *
******************************************************************/

STDMETHODIMP CustomTextRenderer::DrawGlyphRun(
	__maybenull void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	DWRITE_MEASURING_MODE measuringMode,
	__in DWRITE_GLYPH_RUN const* glyphRun,
	__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
	IUnknown* clientDrawingEffect
	)
{
	HRESULT hr = S_OK;

	// Create the path geometry.
	ID2D1PathGeometry* pPathGeometry = nullptr;
	hr = pD2DFactory_->CreatePathGeometry(
		&pPathGeometry
		);

	// Write to the path geometry using the geometry sink.
	ID2D1GeometrySink* pSink = nullptr;
	if(SUCCEEDED(hr))
	{
		hr = pPathGeometry->Open(
			&pSink
			);
	}

	// Get the glyph run outline geometries back from DirectWrite and place them within the
	// geometry sink.
	if(SUCCEEDED(hr))
	{
		hr = glyphRun->fontFace->GetGlyphRunOutline(
			glyphRun->fontEmSize,
			glyphRun->glyphIndices,
			glyphRun->glyphAdvances,
			glyphRun->glyphOffsets,
			glyphRun->glyphCount,
			glyphRun->isSideways,
			glyphRun->bidiLevel % 2,
			pSink
			);
	}

	// Close the geometry sink
	if(SUCCEEDED(hr))
	{
		hr = pSink->Close();
	}

	// Initialize a matrix to translate the origin of the glyph run.
	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
		);

	// Create the transformed geometry
	ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
	if(SUCCEEDED(hr))
	{
		hr = pD2DFactory_->CreateTransformedGeometry(
			pPathGeometry,
			&matrix,
			&pTransformedGeometry
			);
	}

	ID2D1SolidColorBrush* pBrush = nullptr;

	// If there is a drawing effect create a color brush using it, otherwise create a black brush.
	if(clientDrawingEffect != nullptr)
	{
		// Go from IUnknown to TextSelection.
		TextSelection *textSelection = nullptr;

		D2D1_COLOR_F color;

		hr = clientDrawingEffect->QueryInterface(__uuidof(TextSelection), reinterpret_cast<void**>(&textSelection));
		if(SUCCEEDED(hr)){
			// Get the color from the TextSelection object.

			textSelection->GetColor(&color);

			// Create the brush using the color pecified by our TextSelection object.
			hr = pRT_->CreateSolidColorBrush(
				color,
				&pBrush);

			pRT_->FillRectangle(
				D2D1::RectF(
				baselineOriginX,
				baselineOriginY - glyphRun->fontEmSize,
				baselineOriginX + std::accumulate(glyphRun->glyphAdvances, glyphRun->glyphAdvances + glyphRun->glyphCount, 0.f),
				baselineOriginY + glyphRun->fontEmSize / 3
				),
				pBrush);

			textSelection->GetTextColor(&color);
			pBrush->SetColor(color);

			SafeRelease(&textSelection);
		} else{
			hr = clientDrawingEffect->QueryInterface(IID_PPV_ARGS(&pBrush));
		}
	} else
	{
		pBrush = reinterpret_cast<ID2D1SolidColorBrush*>(clientDrawingContext);
		pBrush->AddRef();

		/*// Create a black brush.
		if (SUCCEEDED(hr))
		{
		hr = pRT_->CreateSolidColorBrush(
		D2D1::ColorF(
		D2D1::ColorF::Black
		),
		&pBrush);
		}*/
	}

	if(SUCCEEDED(hr))
	{
		pBrush->SetOpacity(1.);
		//pBrush->SetOpacity(.9);
		// Draw the outline of the glyph run
		pRT_->DrawGeometry(
			pTransformedGeometry,
			pBrush,
			0.1f
			);

		// Fill in the glyph run
		pRT_->FillGeometry(
			pTransformedGeometry,
			pBrush
			);
	}

	SafeRelease(&pPathGeometry);
	SafeRelease(&pSink);
	SafeRelease(&pTransformedGeometry);
	SafeRelease(&pBrush);

	return hr;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::DrawUnderline                              *
*                                                                 *
*  Draws underlines below the text using a Direct2D recatangle    *
*  geometry                                                       *
*                                                                 *
******************************************************************/

STDMETHODIMP CustomTextRenderer::DrawUnderline(
	__maybenull void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	__in DWRITE_UNDERLINE const* underline,
	IUnknown* clientDrawingEffect
	)
{
	HRESULT hr;

	D2D1_RECT_F rect = D2D1::RectF(
		0,
		underline->offset,
		underline->width,
		underline->offset + underline->thickness
		);

	ID2D1RectangleGeometry* pRectangleGeometry = nullptr;
	hr = pD2DFactory_->CreateRectangleGeometry(
		&rect,
		&pRectangleGeometry
		);

	// Initialize a matrix to translate the origin of the underline
	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
		);

	ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
	if(SUCCEEDED(hr))
	{
		hr = pD2DFactory_->CreateTransformedGeometry(
			pRectangleGeometry,
			&matrix,
			&pTransformedGeometry
			);
	}

	ID2D1SolidColorBrush* pBrush = nullptr;

	// If there is a drawing effect create a color brush using it, otherwise create a black brush.
	if(clientDrawingEffect != nullptr)
	{
		// Go from IUnknown to TextSelection.
		TextSelection *textSelection;

		hr = clientDrawingEffect->QueryInterface(__uuidof(TextSelection), reinterpret_cast<void**>(&textSelection));
		if(SUCCEEDED(hr)){
			// Get the color from the TextSelection object.
			D2D1_COLOR_F color;

			textSelection->GetTextColor(&color);

			// Create the brush using the color pecified by our TextSelection object.
			if(SUCCEEDED(hr))
			{
				hr = pRT_->CreateSolidColorBrush(
					color,
					&pBrush);
			}

			SafeRelease(&textSelection);
		} else{
			hr = clientDrawingEffect->QueryInterface(IID_PPV_ARGS(&pBrush));
		}
	} else
	{
		pBrush = reinterpret_cast<ID2D1SolidColorBrush*>(clientDrawingContext);
		pBrush->AddRef();
		/*// Create a black brush.
		if (SUCCEEDED(hr))
		{
		hr = pRT_->CreateSolidColorBrush(
		D2D1::ColorF(
		D2D1::ColorF::Black
		),
		&pBrush);
		}*/
	}

	// Draw the outline of the rectangle
	/*   pRT_->DrawGeometry(
	pTransformedGeometry,
	pBrush
	);*/

	// Fill in the rectangle
	pRT_->FillGeometry(
		pTransformedGeometry,
		pBrush
		);

	SafeRelease(&pRectangleGeometry);
	SafeRelease(&pTransformedGeometry);
	SafeRelease(&pBrush);

	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::DrawStrikethrough                          *
*                                                                 *
*  Draws strikethroughs below the text using a Direct2D           *
*  recatangle geometry                                            *
*                                                                 *
******************************************************************/

STDMETHODIMP CustomTextRenderer::DrawStrikethrough(
	__maybenull void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	__in DWRITE_STRIKETHROUGH const* strikethrough,
	IUnknown* clientDrawingEffect
	)
{
	HRESULT hr;

	D2D1_RECT_F rect = D2D1::RectF(
		0,
		strikethrough->offset,
		strikethrough->width,
		strikethrough->offset + strikethrough->thickness
		);

	ID2D1RectangleGeometry* pRectangleGeometry = nullptr;
	hr = pD2DFactory_->CreateRectangleGeometry(
		&rect,
		&pRectangleGeometry
		);

	// Initialize a matrix to translate the origin of the strikethrough
	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
		);

	ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
	if(SUCCEEDED(hr))
	{
		hr = pD2DFactory_->CreateTransformedGeometry(
			pRectangleGeometry,
			&matrix,
			&pTransformedGeometry
			);
	}

	ID2D1SolidColorBrush* pBrush = nullptr;

	// If there is a drawing effect create a color brush using it, otherwise create a black brush.
	if(clientDrawingEffect != nullptr)
	{
		// Go from IUnknown to TextSelection.
		TextSelection *textSelection;

		hr = clientDrawingEffect->QueryInterface(__uuidof(TextSelection), reinterpret_cast<void**>(&textSelection));
		if(SUCCEEDED(hr)){
			// Get the color from the TextSelection object.
			D2D1_COLOR_F color;

			textSelection->GetTextColor(&color);

			// Create the brush using the color pecified by our TextSelection object.
			if(SUCCEEDED(hr))
			{
				hr = pRT_->CreateSolidColorBrush(
					color,
					&pBrush);
			}

			SafeRelease(&textSelection);
		} else{
			hr = clientDrawingEffect->QueryInterface(IID_PPV_ARGS(&pBrush));
		}
	} else
	{
		pBrush = reinterpret_cast<ID2D1SolidColorBrush*>(clientDrawingContext);
		pBrush->AddRef();
		/*// Create a black brush.
		if (SUCCEEDED(hr))
		{
		hr = pRT_->CreateSolidColorBrush(
		D2D1::ColorF(
		D2D1::ColorF::Black
		),
		&pBrush);
		}*/
	}

	// Draw the outline of the rectangle
	/*pRT_->DrawGeometry(
	pTransformedGeometry,
	pBrush
	);*/

	// Fill in the rectangle
	pRT_->FillGeometry(
		pTransformedGeometry,
		pBrush
		);

	SafeRelease(&pRectangleGeometry);
	SafeRelease(&pTransformedGeometry);
	SafeRelease(&pBrush);

	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::DrawInlineObject                           *
*                                                                 *
*  This function is not implemented for the purposes of this      *
*  sample.                                                        *
*                                                                 *
******************************************************************/

STDMETHODIMP CustomTextRenderer::DrawInlineObject(
	__maybenull void* clientDrawingContext,
	FLOAT originX,
	FLOAT originY,
	IDWriteInlineObject* inlineObject,
	BOOL isSideways,
	BOOL isRightToLeft,
	IUnknown* clientDrawingEffect
	)
{
	// Not implemented
	return E_NOTIMPL;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::AddRef                                     *
*                                                                 *
*  Increments the ref count                                       *
*                                                                 *
******************************************************************/

STDMETHODIMP_(unsigned long) CustomTextRenderer::AddRef()
{
	return InterlockedIncrement(&cRefCount_);
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::Release                                    *
*                                                                 *
*  Decrements the ref count and deletes the instance if the ref   *
*  count becomes 0                                                *
*                                                                 *
******************************************************************/

STDMETHODIMP_(unsigned long) CustomTextRenderer::Release()
{
	unsigned long newCount = InterlockedDecrement(&cRefCount_);

	if(newCount == 0)
	{
		delete this;
		return 0;
	}

	return newCount;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::IsPixelSnappingDisabled                    *
*                                                                 *
*  Determines whether pixel snapping is disabled. The recommended *
*  default is FALSE, unless doing animation that requires         *
*  subpixel vertical placement.                                   *
*                                                                 *
******************************************************************/

STDMETHODIMP CustomTextRenderer::IsPixelSnappingDisabled(
	__maybenull void* clientDrawingContext,
	__out BOOL* isDisabled
	)
{
	*isDisabled = FALSE;
	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::GetCurrentTransform                        *
*                                                                 *
*  Returns the current transform applied to the render target..   *
*                                                                 *
******************************************************************/

STDMETHODIMP CustomTextRenderer::GetCurrentTransform(
	__maybenull void* clientDrawingContext,
	__out DWRITE_MATRIX* transform
	)
{
	//forward the render target's transform
	pRT_->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::GetPixelsPerDip                            *
*                                                                 *
*  This returns the number of pixels per DIP.                     *
*                                                                 *
******************************************************************/

STDMETHODIMP CustomTextRenderer::GetPixelsPerDip(
	__maybenull void* clientDrawingContext,
	__out FLOAT* pixelsPerDip
	)
{
	float x, yUnused;

	pRT_->GetDpi(&x, &yUnused);
	*pixelsPerDip = x / 96;

	return S_OK;
}

/******************************************************************
*                                                                 *
*  CustomTextRenderer::QueryInterface                             *
*                                                                 *
*  Query interface implementation                                 *
*                                                                 *
******************************************************************/

STDMETHODIMP CustomTextRenderer::QueryInterface(
	IID const& riid,
	void** ppvObject
	)
{
	if(__uuidof(IDWriteTextRenderer) == riid)
	{
		*ppvObject = this;
	} else if(__uuidof(IDWritePixelSnapping) == riid)
	{
		*ppvObject = this;
	} else if(__uuidof(IUnknown) == riid)
	{
		*ppvObject = this;
	} else
	{
		*ppvObject = nullptr;
		return E_FAIL;
	}

	AddRef();

	return S_OK;
}

} } }
