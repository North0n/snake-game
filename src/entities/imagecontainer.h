#pragma once

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

class ImageContainer
{
public:
    ~ImageContainer() { free(); }

    void free();

    bool load(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = nullptr);

    operator Gdiplus::Bitmap*() const { return m_bitmap; }

private:
    HGLOBAL m_buffer{nullptr};
    Gdiplus::Bitmap* m_bitmap{nullptr};
};
