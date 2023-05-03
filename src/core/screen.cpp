#include "screen.h"

#include <iostream>

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

    void SetCursorPos(const Math::Vec2i& position)
    {
#if defined(_WIN32)
        COORD pos = { (SHORT)position.x, (SHORT)position.y };
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(output, pos);
#elif defined(__linux__)
#endif // Windows/Linux
    }
}

namespace Core
{
    Screen::Screen()
    {
        m_ScreenExtent = Utils::get_terminal_size();
        UpdateViewSize();
    }
    
    Screen::~Screen()
    {
    }

    void Screen::OnUpdate(float ts)
    {
        m_ScreenExtent = Utils::get_terminal_size();
        UpdateViewSize();
    }

    void Screen::FlushView()
    {
        for (int32_t j = 0; j < m_View->GetHeight(); j++)
        {
            for (int32_t i = 0; i < m_View->GetWidth(); i++)
            {
                if (((*m_View)[i][j]) != ((*m_OldView)[i][j]))
                {
                    Utils::SetCursorPos({ i, j });
                    std::cout << (*m_View)[i][j];
                }
            }
        }
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
    
    void Screen::UpdateViewSize()
    {
        m_View = std::make_shared<View>(m_ScreenExtent);
    }
    
    View& Screen::PrepareView()
    {
        // store previous buffer so the we can change only the characters
        // that are different to make the experience smooth
        m_OldView = std::make_shared<View>(*m_View);
        m_View->Clear();
        return *m_View;
    }
    
    void Screen::ClearScreen()
    {
        Utils::SetCursorPos({ 0, 0 });
        for (int32_t j = 0; j < m_View->GetHeight(); j++)
        {
            for (int32_t i = 0; i < m_View->GetWidth(); i++)
            {
                std::cout << ' ';
            }
        }
    }
}