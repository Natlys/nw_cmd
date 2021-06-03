#include "../nc_cmd_pch.hxx"
#include "nc_cmd_sys.hxx"
#if (defined(NC_API))
#   include "nc_cmd_buf.hxx"
#       define NC_CMD_MODE_IPUT ( NC_ZERO \
            | ENABLE_MOUSE_INPUT          \
            | ENABLE_WINDOW_INPUT         \
            | ENABLE_EXTENDED_FLAGS       \
        )   /* NC_CMD_MODE_IPUT */
/* data */
static nc_cmd_sys_t nc_cmd_sys = {
    .base = {
        .init = nc_cfg_sys_init,
        .quit = nc_cfg_sys_quit,
        .work = nc_cfg_sys_work,
        .mset = nc_cfg_sys_mset,
    },
    .fbuf = NC_NULL,
    .orig = {
        .iput = NC_NULL,
        .oput = NC_NULL,
    },
    .keyb = {
        .keys = { NC_ZERO }
    },
    .curs = {
        .keys = { NC_ZERO },
        .move = { NC_ZERO, NC_ZERO },
    }
};
/* getters */
NC_API_CALL nc_cmd_sys_t* nc_cmd_sys_get() {
    return &nc_cmd_sys;
}
NC_API_CALL nc_cmd_buf_t* nc_cmd_sys_get_buf() {
    return nc_cmd_sys.fbuf;
}
NC_API_CALL size_t nc_cmd_sys_get_keyb_reps(indx_t indx) {
    return nc_cmd_sys.keyb.keys[indx].reps;
}
NC_API_CALL flag_t nc_cmd_sys_get_keyb_flag(indx_t indx) {
    return nc_cmd_sys.keyb.keys[indx].flag;
}
NC_API_CALL v1bit_t nc_cmd_sys_get_keyb_free(indx_t indx) {
    return nc_cmd_sys.keyb.keys[indx].flag == NC_EVTCODE_FREE;
}
NC_API_CALL v1bit_t nc_cmd_sys_get_keyb_held(indx_t indx) {
    return nc_cmd_sys.keyb.keys[indx].flag == NC_EVTCODE_HELD;
}
NC_API_CALL flag_t nc_cmd_sys_get_curs_flag(indx_t indx) {
    return nc_cmd_sys.curs.keys[indx].flag;
}
NC_API_CALL v1bit_t nc_cmd_sys_get_curs_free(indx_t indx) {
    return nc_cmd_sys.curs.keys[indx].flag == NC_EVTCODE_FREE;
}
NC_API_CALL v1bit_t nc_cmd_sys_get_curs_held(indx_t indx) {
    return nc_cmd_sys.curs.keys[indx].flag == NC_EVTCODE_HELD;
}
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_movex() {
    return nc_cmd_sys.curs.move.x;
}
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_movey() {
    return nc_cmd_sys.curs.move.y;
}
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_freex(indx_t indx) {
    return nc_cmd_sys.curs.keys[indx].free.x;
}
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_freey(indx_t indx) {
    return nc_cmd_sys.curs.keys[indx].free.y;
}
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_heldx(indx_t indx) {
    return nc_cmd_sys.curs.keys[indx].held.x;
}
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_heldy(indx_t indx) {
    return nc_cmd_sys.curs.keys[indx].held.y;
}
/* settres */
/* vetters */
/* commands */
NC_API_CALL v1bit_t nc_cmd_sys_init() {
    /* init */
    NC_CHECK(
        nc_cmd_sys.orig.oput == NC_NULL,
        "mark error!",
        return NC_FALSE
    );
    NC_CHECK(
        nc_cmd_sys.orig.iput == NC_NULL,
        "mark error!",
        return NC_FALSE
    );
    /* work */
    /** input **/
    nc_cmd_sys.orig.iput = GetStdHandle(STD_INPUT_HANDLE);
    NC_CHECK(
        nc_cmd_sys.orig.iput != INVALID_HANDLE_VALUE,
        "mark error!",
        return NC_FALSE
    );
    NC_CHECK(
        SetConsoleMode(nc_cmd_sys.orig.iput, NC_CMD_MODE_IPUT),
        "input console mode is not set!",
        return NC_FALSE
    );
    /** output **/
    nc_cmd_sys.orig.oput = GetStdHandle(STD_OUTPUT_HANDLE);
    NC_CHECK(
        nc_cmd_sys.orig.oput != INVALID_HANDLE_VALUE,
        "mark error!",
        return NC_FALSE
    );
    NC_CHECK(
        nc_cmd_buf_ctor(&nc_cmd_sys.fbuf),
        "failed cmd buffer ctor!",
        return NC_FALSE
    );
    NC_CHECK(
        nc_cmd_buf_init(nc_cmd_sys.fbuf),
        "failed cmd buffer init!",
        return NC_FALSE
    );
    /* quit */
    //NC_OLOG("cmd system init;");
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_sys_quit() {
    /* init */
    NC_CHECK(
        nc_cmd_sys.orig.oput != NC_NULL,
        "mark error!",
        return NC_FALSE
    );
    NC_CHECK(
        nc_cmd_sys.orig.iput != NC_NULL,
        "mark error!",
        return NC_FALSE
    );
    /* work */
    /** buffer **/
    NC_CHECK(
        nc_cmd_buf_quit(nc_cmd_sys.fbuf),
        "failed cmd buffer quit!",
        return NC_FALSE
    );
    NC_CHECK(
        nc_cmd_buf_dtor(&nc_cmd_sys.fbuf),
        "failed cmd buffer dtor!",
        return NC_FALSE
    );
    /** origin **/
    NC_CHECK(
        SetConsoleActiveScreenBuffer(nc_cmd_sys.orig.oput),
        "failed console buffer set!",
        return NC_FALSE
    );
    nc_cmd_sys.orig.iput = NC_NULL;
    nc_cmd_sys.orig.oput = NC_NULL;
    /* quit */
    //NC_OLOG("cmd system quit;");
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_sys_work() {
    /* init */
    /* work */
    /** events **/
    {
        DWORD event_numb = NC_ZERO;
        DWORD event_read = NC_ZERO;
        NC_CHECK(
            GetNumberOfConsoleInputEvents(nc_cmd_sys.orig.iput, &event_numb),
            "event number error!",
            { NC_ELOG("last_error: %d;", GetLastError()); return NC_FALSE; }
        );
        INPUT_RECORD event_data[32] = { NC_ZERO };
        NC_CHECK(
            ReadConsoleInput(nc_cmd_sys.orig.iput, event_data, event_numb, &event_read),
            "event record error!",
            { NC_ELOG("last_error: %d;", GetLastError()); return NC_FALSE; }
        );
        for (v1u_t ievt = NC_ZERO; ievt < event_numb; ievt += NC_UNIT) {
            switch(event_data[ievt].EventType) {
                case MOUSE_EVENT: {
                    MOUSE_EVENT_RECORD* evt = &event_data[ievt].Event.MouseEvent;
                    if (evt->dwEventFlags & MOUSE_MOVED) {
                        nc_cmd_sys.curs.move.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.move.y = evt->dwMousePosition.Y;
                    }
                    if (evt->dwEventFlags & FROM_LEFT_1ST_BUTTON_PRESSED) {
                        nc_cmd_sys.curs.keys[NC_CURCODE_0].flag = NC_EVTCODE_HELD;
                        nc_cmd_sys.curs.keys[NC_CURCODE_0].held.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_0].held.y = evt->dwMousePosition.Y;
                    } else {
                        nc_cmd_sys.curs.keys[NC_CURCODE_0].flag = NC_EVTCODE_FREE;
                        nc_cmd_sys.curs.keys[NC_CURCODE_0].free.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_0].free.y = evt->dwMousePosition.Y;
                    }
                    if (evt->dwEventFlags & RIGHTMOST_BUTTON_PRESSED) {
                        nc_cmd_sys.curs.keys[NC_CURCODE_1].flag = NC_EVTCODE_HELD;
                        nc_cmd_sys.curs.keys[NC_CURCODE_1].free.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_1].free.y = evt->dwMousePosition.Y;
                    } else {
                        nc_cmd_sys.curs.keys[NC_CURCODE_1].flag = NC_EVTCODE_FREE;
                        nc_cmd_sys.curs.keys[NC_CURCODE_1].free.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_1].free.y = evt->dwMousePosition.Y;
                    }
                    if (evt->dwEventFlags & FROM_LEFT_1ST_BUTTON_PRESSED) {
                        nc_cmd_sys.curs.keys[NC_CURCODE_2].flag = NC_EVTCODE_HELD;
                        nc_cmd_sys.curs.keys[NC_CURCODE_2].held.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_2].held.y = evt->dwMousePosition.Y;
                    } else {
                        nc_cmd_sys.curs.keys[NC_CURCODE_2].flag = NC_EVTCODE_FREE;
                        nc_cmd_sys.curs.keys[NC_CURCODE_2].free.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_2].free.y = evt->dwMousePosition.Y;
                    }
                    if (evt->dwEventFlags & FROM_LEFT_3RD_BUTTON_PRESSED) {
                        nc_cmd_sys.curs.keys[NC_CURCODE_3].flag = NC_EVTCODE_HELD;
                        nc_cmd_sys.curs.keys[NC_CURCODE_3].held.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_3].held.y = evt->dwMousePosition.Y;
                    } else {
                        nc_cmd_sys.curs.keys[NC_CURCODE_3].flag = NC_EVTCODE_FREE;
                        nc_cmd_sys.curs.keys[NC_CURCODE_3].free.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_3].free.y = evt->dwMousePosition.Y;
                    }
                    if (evt->dwEventFlags & FROM_LEFT_4TH_BUTTON_PRESSED) {
                        nc_cmd_sys.curs.keys[NC_CURCODE_4].flag = NC_EVTCODE_HELD;
                        nc_cmd_sys.curs.keys[NC_CURCODE_4].held.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_4].held.y = evt->dwMousePosition.Y;
                    } else {
                        nc_cmd_sys.curs.keys[NC_CURCODE_4].flag = NC_EVTCODE_FREE;
                        nc_cmd_sys.curs.keys[NC_CURCODE_4].free.x = evt->dwMousePosition.X;
                        nc_cmd_sys.curs.keys[NC_CURCODE_4].free.y = evt->dwMousePosition.Y;
                    }
                }
                break;
                case KEY_EVENT: {
                    KEY_EVENT_RECORD* evt = &event_data[ievt].Event.KeyEvent;
                    if (evt->bKeyDown) {
                        nc_cmd_sys.keyb.keys[evt->wVirtualKeyCode].flag = NC_EVTCODE_HELD;
                        nc_cmd_sys.keyb.keys[evt->wVirtualKeyCode].reps = evt->wRepeatCount;
                    } else {
                        nc_cmd_sys.keyb.keys[evt->wVirtualKeyCode].flag = NC_EVTCODE_FREE;
                        nc_cmd_sys.keyb.keys[evt->wVirtualKeyCode].reps = NC_ZERO;
                    }
                }
                break;
                case FOCUS_EVENT: {
                    FOCUS_EVENT_RECORD* evt = &event_data[ievt].Event.FocusEvent;
                }
                break;
                case WINDOW_BUFFER_SIZE_EVENT: {
                    WINDOW_BUFFER_SIZE_RECORD* evt = &event_data[ievt].Event.WindowBufferSizeEvent;
                }
                break;
                case MENU_EVENT: {
                    MENU_EVENT_RECORD* evt = &event_data[ievt].Event.MenuEvent;
                }
                break;
            }
        }
    }
    /** framebuffer **/
    NC_CHECK(
        nc_cmd_buf_work(nc_cmd_sys.fbuf),
        "work error!",
        return NC_FALSE
    );
    /* quit */
    if (nc_cmd_sys_get_keyb_held(NC_KEYCODE_ESCP)) { return NC_FALSE; }
    //NC_OLOG("cmd system work;");
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_sys_mset(ptr_t* data, size_t isize, size_t osize) {
    /* init */
    /* work */
    NC_CHECK(
        nc_cmd_sys.base.mset(data, isize, osize),
        "mset error",
        return NC_FALSE
    );
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_sys_olog() {
    /* init */
    /* work */
    NC_OLOG("cmd_sys:");
    NC_OPUT("{" NC_ENDL);
    NC_OPUT(NC_HTAB "orig: { iput: %p; oput: %p; };" NC_SEPL NC_ENDL, nc_cmd_sys.orig.iput, nc_cmd_sys.orig.oput);
    NC_OPUT("}" NC_SEPL NC_ENDL);
    /* quit */
    return NC_TRUTH;
}
#endif  /* NC_API */
/* end_of_file */