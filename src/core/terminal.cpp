#include "terminal.h"

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
    struct ConsoleData
    {
#if defined(_WIN32)

        HANDLE stdOutHandle = 0;

#elif defined(__linux__)

#endif // Windows/Linux

        Math::Vec2i consoleSize = {};
    };

    static ConsoleData s_Data;

    static Math::Vec2i UpdateConsoleSize()
    {
        Math::Vec2i size;
#if defined(_WIN32)

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(s_Data.stdOutHandle, &csbi);
        size.x = (int32_t)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
        size.y = (int32_t)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

#elif defined(__linux__)

        struct winsize w;
        ioctl(fileno(stdout), TIOCGWINSZ, &w);
        size.x = (int32_t)(w.ws_col);
        size.y = (int32_t)(w.ws_row);

#endif // Windows/Linux

        return s_Data.consoleSize = size;
    }

    static Math::Vec2i GetConsoleSize()
    {
        return s_Data.consoleSize;
    }

    static void SetCursorPos(const Math::Vec2i& position)
    {
#if defined(_WIN32)

        COORD pos = { (SHORT)position.x, (SHORT)position.y };
        SetConsoleCursorPosition(s_Data.stdOutHandle, pos);

#elif defined(__linux__)

#endif // Windows/Linux

    }

    static void ShowConsoleCursor(bool showFlag)
    {
#if defined(_WIN32)

        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(s_Data.stdOutHandle, &cursorInfo);
        cursorInfo.bVisible = showFlag; // set the cursor visibility
        SetConsoleCursorInfo(s_Data.stdOutHandle, &cursorInfo);

#elif defined(__linux__)

#endif // Windows/Linux

    }

    static void InitData()
    {
#if defined(_WIN32)

        s_Data.stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

#elif defined(__linux__)

        

#endif // Windows/Linux

        UpdateConsoleSize();
    }

    static void Shutdown()
    {

    }
}

namespace Core
{
    Terminal::Terminal()
    {
        Utils::InitData();
        m_ScreenExtent = Utils::GetConsoleSize();
        UpdateViewSize();
        OnResize();
    }
    
    Terminal::~Terminal()
    {
        Utils::Shutdown();
    }

    void Terminal::OnUpdate(float ts)
    {
        m_ScreenExtent = Utils::UpdateConsoleSize();
        // reset cursor visibility on resize
        if (UpdateViewSize())
            OnResize();
    }

    void Terminal::FlushView()
    {
        for (int32_t j = 0; j < m_View->GetHeight(); j++)
        {
            for (int32_t i = 0; i < m_View->GetWidth(); i++)
            {
                // update the screen only if the character is different
                if (((*m_View)[i][j]) != ((*m_OldView)[i][j]))
                {
                    Utils::SetCursorPos({ i, j });
                    std::cout << (*m_View)[i][j];
                }
            }
        }
    }
    
    float Terminal::GetAspectRatio() const
    {
        return m_View->GetAspectRatio();
    }
    
    Math::Vec2i Terminal::WorldSpaceToScreenSpace(const Math::Vec2f& v) const
    {
        return m_View->WorldSpaceToScreenSpace(v);
    }

    Math::Vec2f Terminal::ScreenSpaceToWorldSpace(const Math::Vec2i& vector) const
    {
        return m_View->ScreenSpaceToWorldSpace(vector);
    }
    
    void Terminal::OnResize()
    {
        Utils::ShowConsoleCursor(false);
    }

    bool Terminal::UpdateViewSize()
    {
        // (re)create a character buffer
        if (!m_View || m_ScreenExtent != m_View->GetExtent())
        {
            m_View = std::make_shared<View>(m_ScreenExtent);
            return true;
        }
        return false;
    }
    
    View& Terminal::PrepareView()
    {
        // store previous buffer so the we can change only the characters
        // that are different to make the experience smooth
        m_OldView = std::make_shared<View>(*m_View);
        m_View->Clear();
        return *m_View;
    }
    
    void Terminal::ClearScreen()
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