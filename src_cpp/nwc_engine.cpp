#include "nwc_pch.hpp"
#include "nwc_engine.h"

#define NWC_INPUT_FLAGS ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT

namespace NWC
{
    CmdEngine::CmdEngine() :
        m_wInfo(CWindowInfo()),
        m_pxInfo(CPixelInfo()),
        m_pCout(nullptr), m_pCin(nullptr)
    {
        this->SetWndSize(50, 20);
        this->SetPxSize(8, 16);
        m_fmBuf.SetSizeWH(GetWndWidth(), GetWndHeight());
        m_pCout = GetStdHandle(STD_OUTPUT_HANDLE);
        m_pCin = GetStdHandle(STD_INPUT_HANDLE);
        m_wInfo.strTitle = "cwindow";
    }
    CmdEngine::~CmdEngine() { }

    // --setters
    void CmdEngine::SetTitle(const char* strTitle) { m_wInfo.strTitle = strTitle; SetConsoleTitleA(&strTitle[0]); }
    void CmdEngine::SetWndSize(UInt16 unWidth, UInt16 unHeight) {
        m_wInfo.xywhRect.Left = m_wInfo.xywhRect.Top = 0;
        m_wInfo.xywhRect.Right = unWidth; m_wInfo.xywhRect.Bottom = unHeight;
    }
    void CmdEngine::SetPxSize(UInt16 nWidth, UInt16 nHeight){
        m_pxInfo.dwFontSize.X = nWidth;
        m_pxInfo.dwFontSize.Y = nHeight;
        m_pxInfo.FontFamily = FF_DONTCARE;
        m_pxInfo.FontWeight = FW_NORMAL;
        m_pxInfo.nFont = 0;
        swprintf_s(m_pxInfo.FaceName, L"Consolas");
        m_pxInfo.cbSize = sizeof(m_pxInfo);
    }

    void CmdEngine::SetWndInfo(const CWindowInfo& rcwInfo) { m_wInfo = rcwInfo; SetConsoleWindowInfo(m_fmBuf.GetNative(), TRUE, &m_wInfo.xywhRect); }
    void CmdEngine::SetPxInfo(const CPixelInfo& rcpxInfo) { m_pxInfo = rcpxInfo; SetCurrentConsoleFontEx(m_fmBuf.GetNative(), TRUE, &m_pxInfo); }
    void CmdEngine::SetCrsInfo(const CCursorInfoT& rcrsInfo) { m_crsInfo = rcrsInfo; SetConsoleCursorInfo(m_fmBuf.GetNative(), &m_crsInfo); }
    
    // --<core_methods>--
    void CmdEngine::Run() {
        Init();
        if (!m_bIsRunning) { return; }
        if (m_States.empty()) { return; }

        auto fnRunLoop = [this]()->void { while (m_bIsRunning) { Update(); } Quit(); };
        m_thrRun = Thread(fnRunLoop);
    }
    bool CmdEngine::Init()
    {
        if (m_bIsRunning) { return false; }
        m_bIsRunning = true;

        SetTitle(&m_wInfo.strTitle[0]);
        if (!SetConsoleMode(m_pCin, NWC_INPUT_FLAGS)) { Quit(); return false; }

        {
            V4xywh xywhMinRect = { 0, 0, 1, 1 };
            if (!SetConsoleWindowInfo(m_fmBuf.GetNative(), TRUE, &xywhMinRect)) { Quit(); return false; }
            if (!SetCurrentConsoleFontEx(m_fmBuf.GetNative(), TRUE, &m_pxInfo)) { Quit(); return false; }

            m_wInfo.whMaxSize = GetLargestConsoleWindowSize(m_pCout);
            if (m_wInfo.GetHeight() > m_wInfo.whMaxSize.Y) { m_wInfo.xywhRect.Top = 0; m_wInfo.xywhRect.Bottom = m_wInfo.whMaxSize.Y; }
            if (m_wInfo.GetHeight() > m_wInfo.whMaxSize.Y) { m_wInfo.xywhRect.Top = 0; m_wInfo.xywhRect.Bottom = m_wInfo.whMaxSize.Y; }

            m_fmBuf.Remake();
            if (!SetConsoleWindowInfo(m_fmBuf.GetNative(), TRUE, &m_wInfo.xywhRect)) { Quit(); return false; }
            if (!SetConsoleActiveScreenBuffer(m_fmBuf.GetNative())) { Quit(); return false; }
        }

        for (auto& itState : m_States) { if (!itState->Init()) { Quit(); return false; } }

        return m_bIsRunning;
    }
    void CmdEngine::Quit()
    {
        if (m_bIsRunning) { return; }
        m_bIsRunning = false;
        
        for (auto& itState : m_States) { itState->Quit(); }

        SetConsoleActiveScreenBuffer(m_pCout);
    }

    void CmdEngine::Update()
    {
        TimeSys::Update();
        Char strTime[16]{ 0 };
        sprintf(strTime, "ups: %3.2f", 1.0f / TimeSys::GetDeltaS());
        DrawBytesXY(GetWndWidth() - 16, 2, GetWndWidth(), 3, NWC::CCD_FG_GREEN, &strTime[0], 16);

        SwapBuffers();
        m_fmBuf.Clear();
        PollEvents();
        
        for (auto& itState : m_States) { itState->Update(); }
    }

    void CmdEngine::OnEvent(AEvent& rEvt)
    {
        if (rEvt.IsInCategory(EC_CURSOR)) {
            CursorEvent* pmEvt = static_cast<CursorEvent*>(&rEvt);
            switch (pmEvt->evType) {
            case ET_CURSOR_MOVE:
                m_crs.xMoveDelta = pmEvt->nX - m_crs.xMove;
                m_crs.yMoveDelta = pmEvt->nY - m_crs.yMove;
                m_crs.xMove = pmEvt->nX;
                m_crs.yMove = pmEvt->nY;
                break;
            case ET_CURSOR_SCROLL:
                m_crs.xScrollDelta = pmEvt->nX;
                m_crs.yScrollDelta = pmEvt->nY;
                break;
            case ET_CURSOR_PRESS:
                m_crs.Buttons[pmEvt->cButton].bState = BS_PRESSED;
                break;
            case ET_CURSOR_RELEASE:
                m_crs.Buttons[pmEvt->cButton].bState = BS_RELEASED;
                break;
            }
            if (rEvt.bIsHandled) return;
            for (auto& itState : m_States) { itState->OnEvent(*pmEvt); }
        }
        else if (rEvt.IsInCategory(EC_KEYBOARD)) {
            KeyboardEvent* pkEvt = static_cast<KeyboardEvent*>(&rEvt);
            switch (pkEvt->evType) {
            case ET_KEYBOARD_PRESS:
                m_kbd.Keys[pkEvt->keyCode].bState = BS_PRESSED;
                switch (pkEvt->keyCode) {
                case KC_ESCAPE:
                    break;
                default: break;
                }
                break;
            case ET_KEYBOARD_RELEASE:
                m_kbd.Keys[pkEvt->keyCode].bState = BS_RELEASED;
                switch (pkEvt->keyCode) {
                case KC_ESCAPE:
                    StopRunning();
                    rEvt.bIsHandled = true;
                    break;
                default: break;
                }
                break;
            case ET_KEYBOARD_CHAR:
                break;
            }
            if (rEvt.bIsHandled) { return; }
            for (auto& itState : m_States) { itState->OnEvent(*pkEvt); }
        }
        else if (rEvt.IsInCategory(EC_WINDOW)) {
            WindowEvent* pwEvt = static_cast<WindowEvent*>(&rEvt);
            switch (pwEvt->evType) {
            case ET_WINDOW_RESIZE:
                break;
            case ET_WINDOW_MOVE:
                break;
            case ET_WINDOW_FOCUS:
                break;
            case ET_WINDOW_CLOSE:
                StopRunning();
                rEvt.bIsHandled = true;
                break;
            }
            if (rEvt.bIsHandled) { return; }
            for (auto& itState : m_States) { itState->OnEvent(*pwEvt); }
        }
    }
    // --==</core_methods>==--
    
    // --==<implementation_methods>==--
    inline void CmdEngine::SwapBuffers() {
        if (!WriteConsoleOutputW(m_fmBuf.GetNative(), &m_fmBuf.GetPxData()[0],
            { m_fmBuf.GetWidth(), m_fmBuf.GetHeight() }, { 0, 0 }, &m_fmBuf.GetInfo().srWindow)) { StopRunning(); return; }
    }
    inline void CmdEngine::PollEvents() {
        GetNumberOfConsoleInputEvents(m_pCin, &m_evInfo.unEvGetCount);
        if (m_evInfo.unEvGetCount > 0) {
            ReadConsoleInputA(m_pCin, m_evInfo.irEvents, m_evInfo.unEvGetCount, &m_evInfo.unEvReadCount);
            for (UInt16 evi = 0; evi < m_evInfo.unEvGetCount; evi++) {
                UInt32 evTypeId = m_evInfo.irEvents[evi].EventType;
                if (evTypeId == MOUSE_EVENT) {
                    MOUSE_EVENT_RECORD& rEvt = m_evInfo.irEvents[evi].Event.MouseEvent;
                    if (rEvt.dwEventFlags == MOUSE_MOVED) {
                        CursorEvent crsEvt(ET_CURSOR_MOVE, rEvt.dwMousePosition.X, rEvt.dwMousePosition.Y);
                        OnEvent(crsEvt);
                    }
                    else {
                        for (UInt8 mi = 0; mi < CRS_COUNT; mi++) {
                            auto& rmb = m_crs.Buttons[mi];
                            ButtonStates bsOld = rmb.bState;
                            rmb.bState = (((1 << mi) & rEvt.dwButtonState) > 0) ? BS_PRESSED : BS_RELEASED;
                            if (rmb.bState != bsOld) {
                                CursorEvent crsEvt(rmb.bState == BS_PRESSED ? ET_CURSOR_PRESS : ET_CURSOR_RELEASE, static_cast<CursorCodes>(mi));
                                OnEvent(crsEvt);
                            }
                        }
                    }
                }
                else if (evTypeId == KEY_EVENT) {
                    KEY_EVENT_RECORD& rEvt = m_evInfo.irEvents[evi].Event.KeyEvent;
                    if (rEvt.bKeyDown) {
                        if (rEvt.wRepeatCount == 1) {
                            KeyboardEvent kbdEvt(ET_KEYBOARD_PRESS, static_cast<KeyCodes>(rEvt.wVirtualKeyCode));
                            OnEvent(kbdEvt);
                        }
                    }
                    else {
                        KeyboardEvent kbdEvt(ET_KEYBOARD_RELEASE, static_cast<KeyCodes>(rEvt.wVirtualKeyCode));
                        OnEvent(kbdEvt);
                        if (rEvt.uChar.AsciiChar >= ' ' && rEvt.uChar.AsciiChar <= 'z') {
                            KeyboardEvent kbEvt(ET_KEYBOARD_CHAR, static_cast<KeyCodes>(rEvt.uChar.AsciiChar));
                            OnEvent(kbEvt);
                        }
                        else {
                        }
                    }
                }
                else if (evTypeId == FOCUS_EVENT) {
                    WindowEvent wEvt(m_evInfo.irEvents[evi].Event.FocusEvent.bSetFocus ? ET_WINDOW_FOCUS : ET_WINDOW_DEFOCUS);
                    OnEvent(wEvt);
                }
                else if (evTypeId == WINDOW_BUFFER_SIZE_EVENT) {
                    WindowEvent wEvt(ET_WINDOW_RESIZE, m_evInfo.irEvents[evi].Event.WindowBufferSizeEvent.dwSize.X, m_evInfo.irEvents[evi].Event.WindowBufferSizeEvent.dwSize.Y);
                    OnEvent(wEvt);
                }
                else if (evTypeId == MENU_EVENT) {
                    MENU_EVENT_RECORD& rEvt = m_evInfo.irEvents[evi].Event.MenuEvent;
                }
            }
        }
    }
    // --==</implementation_methods>==--
}