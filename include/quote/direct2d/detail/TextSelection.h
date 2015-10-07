
#pragma once

namespace quote{ namespace direct2d{ namespace detail{

class DECLSPEC_UUID("BD9BBC75-5FE9-4A09-91D6-7143069944BE") TextSelection : public IUnknown
{
public:
    TextSelection();
    TextSelection(const D2D1_COLOR_F &color, const D2D1_COLOR_F &color_txt);
    ~TextSelection();

    STDMETHOD(GetColor)(D2D1_COLOR_F *color);
    STDMETHOD(SetColor)(const D2D1_COLOR_F &color);
	STDMETHOD(GetTextColor)(D2D1_COLOR_F *color);
	STDMETHOD(SetTextColor)(const D2D1_COLOR_F &color);

public:
    unsigned long STDMETHODCALLTYPE AddRef();
    unsigned long STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(
        IID const& riid,
        void** ppvObject
    );

private:
    unsigned long cRefCount_;

    // The color of the text.
    D2D1_COLOR_F color_, color_txt_;
};

} } }

#include "TextSelection.cpp"
