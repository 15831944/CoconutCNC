#pragma once

#include "ImGuiWidget.h"

namespace Coconut
{
    class StateWindow : public ImGuiWidget
    {
    public:
        StateWindow(AppState* proj);
        ~StateWindow() override;
        void Draw() override;
    protected:
        void DrawProgressBar();
        void DrawBufferUsageBar();
    private:
        float mProgress;
        float mBufferUsage;
    };
}
