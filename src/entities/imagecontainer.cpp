#include "imagecontainer.h"

void ImageContainer::free()
{
    delete m_bitmap;
    m_bitmap = nullptr;

    if (m_buffer) {
        ::GlobalUnlock(m_buffer);
        ::GlobalFree(m_buffer);
        m_buffer = nullptr;
    }
}

bool ImageContainer::load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
    free();

    HRSRC hResource = ::FindResource(hInst, pName, pType);
    if (!hResource)
        return false;

    DWORD imageSize = ::SizeofResource(hInst, hResource);
    if (!imageSize)
        return false;

    const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
    if (!pResourceData)
        return false;

    m_buffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (m_buffer) {
        void* pBuffer = ::GlobalLock(m_buffer);
        if (pBuffer) {
            CopyMemory(pBuffer, pResourceData, imageSize);

            IStream* pStream = nullptr;
            if (::CreateStreamOnHGlobal(m_buffer, FALSE, &pStream) == S_OK) {
                m_bitmap = Gdiplus::Bitmap::FromStream(pStream);
                pStream->Release();
                if (m_bitmap) {
                    if (m_bitmap->GetLastStatus() == Gdiplus::Ok)
                        return true;

                    delete m_bitmap;
                    m_bitmap = nullptr;
                }
            }
            ::GlobalUnlock(m_buffer);
        }
        ::GlobalFree(m_buffer);
        m_buffer = nullptr;
    }
    return false;
}
