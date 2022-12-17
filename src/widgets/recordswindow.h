#pragma once

#include "iwindow.h"

class RecordsWindow : public IWindow<RecordsWindow>
{
public:
    [[nodiscard]] PCWSTR className() const override;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};
