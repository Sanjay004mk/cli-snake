#include "screen.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#endif // Windows/Linux

namespace Utils
{
    static Math::Vec2i get_terminal_size() 
    {
        Math::Vec2i size;
#if defined(_WIN32)
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        size.x = (int32_t)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
        size.y = (int32_t)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#elif defined(__linux__)
        struct winsize w;
        ioctl(fileno(stdout), TIOCGWINSZ, &w);
        size.x = (int32_t)(w.ws_col);
        size.y = (int32_t)(w.ws_row);
#endif // Windows/Linux
        return size;
    }

    float LerpN1to1(float t, float v1, float v2)
    {
        t = (t + 1.f) / 2.f;
        return v2 * t + (1.f - t) * v1;
    }
}

namespace Core
{
    Screen::Screen()
    {
        m_ScreenExtent = Utils::get_terminal_size();
    }
    
    Screen::~Screen()
    {
    }

    void Screen::OnUpdate(float ts)
    {
        m_ScreenExtent = Utils::get_terminal_size();
    }

    void Screen::Draw(const View& view)
    {
    }
    
    float Screen::GetAspectRatio() const
    {
        return (float)m_ScreenExtent.x / (float)m_ScreenExtent.y;
    }
    
    Math::Vec2i Screen::WorldSpaceToScreenSpace(const Math::Vec2f& v) const
    {
        // convert to (-1, -1) -> (1, 1) 
        auto vector = v;
        float aspectRatio = GetAspectRatio();
        if (aspectRatio > 1.f)
            vector.x /= aspectRatio;
        else
            vector.y *= aspectRatio;

        return Math::Vec2i((int32_t)Utils::LerpN1to1(vector.x, 0.f, (float)m_ScreenExtent.x), (int32_t)Utils::LerpN1to1(vector.y, 0.f, (float)m_ScreenExtent.y));
    }

    Math::Vec2f Screen::ScreenSpaceToWorldSpace(const Math::Vec2i& vector) const
    {
        float x = ((float)vector.x / (float)m_ScreenExtent.x) * 2.f - 1.f, y = ((float) vector.y / (float)m_ScreenExtent.y) * 2.f - 1.f;
        return Math::Vec2f(x, y);
    }
}