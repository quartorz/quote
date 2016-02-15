
#include <quote/direct2d/detail/DrawingEffect.h>

namespace quote{ namespace direct2d{ namespace detail{

/******************************************************************
*                                                                 *
*  TextSelection::TextSelection                                   *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/

TextSelection::TextSelection() :
	cRefCount_(1), color_({0.0f, 0.0f, 0.0f, 1.0f}), color_txt_({1.0f, 1.0f, 1.0f, 1.0f})
{
}

/******************************************************************
*                                                                 *
*  TextSelection::TextSelection                                   *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/

TextSelection::TextSelection(const D2D1_COLOR_F &color, const D2D1_COLOR_F &color_txt) :
    cRefCount_(1), color_(color), color_txt_(color_txt)
{
}

/******************************************************************
*                                                                 *
*  TextSelection::~TextSelection                                  *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/


TextSelection::~TextSelection()
{
}

/******************************************************************
*                                                                 *
*  TextSelection::GetColor                                        *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/

STDMETHODIMP TextSelection::GetColor(D2D1_COLOR_F *color)
{
    *color = color_;

    return S_OK;
}

/******************************************************************
*                                                                 *
*  TextSelection::SetColor                                        *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/

STDMETHODIMP TextSelection::SetColor(const D2D1_COLOR_F &color)
{
    color_ = color;

    return S_OK;
}

/******************************************************************
*                                                                 *
*  TextSelection::GetTextColor                                    *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/

STDMETHODIMP TextSelection::GetTextColor(D2D1_COLOR_F *color)
{
	*color = color_txt_;

	return S_OK;
}

/******************************************************************
*                                                                 *
*  TextSelection::SetTextColor                                    *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/

STDMETHODIMP TextSelection::SetTextColor(const D2D1_COLOR_F &color)
{
	color_txt_ = color;

	return S_OK;
}

/******************************************************************
*                                                                 *
*  TextSelection::AddRef                                          *
*                                                                 *
*  Increments the ref count                                       *
*                                                                 *
******************************************************************/

STDMETHODIMP_(unsigned long) TextSelection::AddRef()
{
    return InterlockedIncrement(&cRefCount_);
}

/******************************************************************
*                                                                 *
*  TextSelection::Release                                         *
*                                                                 *
*  Decrements the ref count and deletes the instance if the ref   *
*  count becomes 0                                                *
*                                                                 *
******************************************************************/

STDMETHODIMP_(unsigned long) TextSelection::Release()
{
    unsigned long newCount = InterlockedDecrement(&cRefCount_);
    
    if (newCount == 0)
    {
        delete this;
        return 0;
    }

    return newCount;
}

/******************************************************************
*                                                                 *
*  TextSelection::QueryInterface                                  *
*                                                                 *
*  Query interface implementation                                 *
*                                                                 *
******************************************************************/

STDMETHODIMP TextSelection::QueryInterface(
    IID const& riid,
    void** ppvObject
    )
{
    if (__uuidof(TextSelection) == riid)
    {
        *ppvObject = this;
    }
    else if (__uuidof(IUnknown) == riid)
    {
        *ppvObject = this;
    }
    else
    {
        *ppvObject = nullptr;
        return E_FAIL;
    }

    AddRef();

    return S_OK;
}

} } }
