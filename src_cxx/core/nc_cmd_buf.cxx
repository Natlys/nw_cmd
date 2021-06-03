#include "../nc_cmd_pch.hxx"
#include "nc_cmd_buf.hxx"
#if (defined NC_API)
#   include "nc_cmd_sys.hxx"
/* ctor_dtor */
NC_API_CALL NC_API v1bit_t nc_cmd_buf_ctor(nc_cmd_buf_t** ref) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    NC_CHECK(
        *ref == NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    /* work */
    NC_CHECK(
        nc_cmd_sys_mset(ref, NC_ZERO, sizeof(nc_cmd_buf_t)),
        "memory error!",
        return NC_FALSE
    );
    nc_cmd_buf_t* buf = *ref;
    buf->mark = NC_NULL;
    buf->data = NC_NULL;
    buf->sizes.x = NC_CMD_USE_SIZE_BUF / 1u + NC_UNIT;
    buf->sizes.y = NC_CMD_USE_SIZE_BUF / 2u + NC_UNIT;
    buf->viewp.x = NC_ZERO;
    buf->viewp.y = NC_ZERO;
    buf->viewp.w = NC_CMD_USE_SIZE_BUF / 1u;
    buf->viewp.h = NC_CMD_USE_SIZE_BUF / 2u;
    buf->pixel.size = NC_CMD_USE_SIZE_PXL;
    buf->pixel.sign = NC_CMD_PXT_MIX;
    buf->pixel.tint = NC_CMD_COL_BG_01 | NC_CMD_COL_FG_16;
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL NC_API v1bit_t nc_cmd_buf_dtor(nc_cmd_buf_t** ref) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    NC_CHECK(
        *ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    nc_cmd_buf_t* buf = *ref;
    NC_CHECK(
        buf->mark == NC_NULL,
        "mark error!",
        return NC_FALSE
    );
    NC_CHECK(
        buf->data == NC_NULL,
        "data error!",
        return NC_FALSE
    );
    NC_CHECK(
        buf->sizes.x == NC_ZERO,
        "size error!",
        return NC_FALSE
    );
    NC_CHECK(
        buf->sizes.y == NC_ZERO,
        "size error!",
        return NC_FALSE
    );
    /* work */
    NC_CHECK(
        nc_cmd_sys_mset(ref, sizeof(nc_cmd_buf_t), NC_ZERO),
        "memory error!",
        return NC_FALSE
    );
    /* quit */
    return NC_TRUTH;
}
/* getters */
/* setters */
NC_API_CALL v1bit_t nc_cmd_buf_set_sizes(nc_cmd_buf_t* ref, size_t size_x, size_t size_y) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->data == NC_NULL,
        "data error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->sizes.x == NC_ZERO,
        "size error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->sizes.y == NC_ZERO,
        "size error!",
        return NC_FALSE
    );
    /* work */
    ref->sizes.x = size_x + NC_UNIT;
    ref->sizes.y = size_y + NC_UNIT;
    nc_cmd_buf_set_viewp(ref, NC_ZERO, NC_ZERO, size_x, size_y);
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_buf_set_viewp(nc_cmd_buf_t* ref, size_t coord_x, size_t coord_y, size_t size_x, size_t size_y) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    /* work */
    ref->viewp.x = coord_x;
    ref->viewp.y = coord_y;
    ref->viewp.w = coord_x + size_x;
    ref->viewp.h = coord_y + size_y;
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_buf_set_pixel_size(nc_cmd_buf_t* ref, size_t size) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->data == NC_NULL,
        "data error!",
        return NC_FALSE
    );
    /* work */
    ref->pixel.size = size;
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_buf_set_pixel_sign(nc_cmd_buf_t* ref, indx_t sign) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    /* work */
    ref->pixel.sign = sign;
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_buf_set_pixel_tint(nc_cmd_buf_t* ref, indx_t tint) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    /* work */
    ref->pixel.tint = tint;
    /* quit */
    return NC_TRUTH;
}
/* vetters */
NC_API_CALL NC_API v1bit_t nc_cmd_buf_vet_sizes(nc_cmd_buf_t* ref, size_t size_x, size_t size_y) {
    return ref->sizes.x >= size_x && ref->sizes.y >= size_y;
}
/* commands */
NC_API_CALL v1bit_t nc_cmd_buf_init(nc_cmd_buf_t* ref) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->mark == NC_NULL,
        "mark error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->data == NC_NULL,
        "data error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->sizes.x != NC_ZERO,
        "size error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->sizes.y != NC_ZERO,
        "size error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->pixel.size != NC_ZERO,
        "size error!",
        return NC_FALSE
    );
    /* work */
    /** handle **/
    ref->mark = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,       /* console-read/write functions */
        FILE_SHARE_READ | FILE_SHARE_WRITE, /* file-read/write functions */
        NC_NULL,                            /* optional security attributes */
        CONSOLE_TEXTMODE_BUFFER,            /* the only supported type */
        NC_NULL                             /* screenbufferdata should be null */
    );
    NC_CHECK(
        ref->mark != INVALID_HANDLE_VALUE,
        "failed console buffer creation!",
        { NC_ELOG("last_error: %d", GetLastError()); return NC_FALSE; }
    );
    /** window **/
    SMALL_RECT wnd_info = { NC_ZERO, NC_ZERO, NC_UNIT, NC_UNIT };
    wnd_info.Left = NC_ZERO; wnd_info.Top = NC_ZERO; wnd_info.Right = NC_UNIT; wnd_info.Bottom = NC_UNIT;
    NC_CHECK(
        SetConsoleWindowInfo(ref->mark, NC_TRUTH, &wnd_info),
        "console window info error!",
        { NC_ELOG("last_error: %d", GetLastError()); return NC_FALSE; }
    );
    /** pixels **/
    const CONSOLE_FONT_INFOEX pxl_info = {
        .cbSize = sizeof(CONSOLE_FONT_INFOEX),
        .nFont = NC_ZERO,
        .dwFontSize = { .X = ref->pixel.size, .Y = ref->pixel.size },
        .FontFamily = FF_DONTCARE,
        .FontWeight = FW_DONTCARE,
        .FaceName = { 'C', 'o', 'n', 's', 'o', 'l', 'a', 's', '\0' },
    };
    NC_CHECK(
        SetCurrentConsoleFontEx(ref->mark, NC_TRUTH, &pxl_info),
        "pixel info is not set!",
        { NC_ELOG("last_error: %d", GetLastError()); return NC_FALSE; }
    );
    const COORD max_size = GetLargestConsoleWindowSize(ref->mark);
    NC_CHECK(
        ref->viewp.w <= max_size.X && ref->viewp.h <= max_size.Y,
        "size error!",
        return NC_FALSE
    );
    /** memory **/
    NC_CHECK(
        nc_cmd_sys_mset(&ref->data, NC_ZERO, sizeof(CHAR_INFO) * ref->sizes.x * ref->sizes.x),
        "console buffer data is not allocated!",
        return NC_FALSE
    );
    const COORD sizes = { .X = ref->sizes.x, .Y = ref->sizes.y};
    NC_CHECK(
        SetConsoleScreenBufferSize(ref->mark, sizes),
        "console buffer data is not set!",
        { NC_ELOG("last_error: %d", GetLastError()); return NC_FALSE; }
    );
    /** buffer **/
    const CONSOLE_SCREEN_BUFFER_INFOEX buf_info = {
        .cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX),
        .dwSize = { .X = ref->sizes.x, .Y = ref->sizes.y },
        .dwCursorPosition = { .X = NC_ZERO, .Y = NC_ZERO },
        .wAttributes = NC_CMD_COL_BG_04 | NC_CMD_COL_FG_16,
        .srWindow = { ref->viewp.x, ref->viewp.y, ref->viewp.w, ref->viewp.h },
        .dwMaximumWindowSize = { ref->sizes.x, ref->sizes.y },
        .wPopupAttributes = NC_CMD_COL_BG_01 | NC_CMD_COL_FG_16,
        .bFullscreenSupported = NC_FALSE,
        .ColorTable = {
            0x00'00'00, 0x11'11'11, 0x22'22'22, 0x33'33'33,
            0x44'44'44, 0x55'55'55, 0x66'66'66, 0x77'77'77,
            0x88'88'88, 0x99'99'99, 0xaa'aa'aa, 0xbb'bb'bb,
            0xcc'cc'cc, 0xdd'dd'dd, 0xee'ee'ee, 0xff'ff'ff,
        },
    };
    NC_CHECK(
        SetConsoleScreenBufferInfoEx(ref->mark, &buf_info),
        "buffer info is not set!",
        { NC_ELOG("last_error: %d", GetLastError()); return NC_FALSE; }
    );
    /** window **/
    wnd_info.Left = ref->viewp.x; wnd_info.Top = ref->viewp.y; wnd_info.Right = ref->viewp.w; wnd_info.Bottom = ref->viewp.h;
    NC_CHECK(
        SetConsoleWindowInfo(ref->mark, NC_TRUTH, &wnd_info),
        "window info error!",
        { NC_ELOG("last_error: %d", GetLastError()); return NC_FALSE; }
    );
    /** handle **/
    NC_CHECK(
        SetConsoleActiveScreenBuffer(ref->mark),
        "active screen buffer is not set!",
        { NC_ELOG("last_error: %d", GetLastError()); return NC_FALSE; }
    );
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_buf_quit(nc_cmd_buf_t* ref) {
    /* init */
    NC_CHECK(
        ref->mark != NC_NULL,
        "mark error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->data != NC_NULL,
        "data error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->sizes.x != NC_ZERO,
        "size error!",
        return NC_FALSE
    );
    NC_CHECK(
        ref->sizes.x != NC_ZERO,
        "size error!",
        return NC_FALSE
    );
    /* work */
    CloseHandle(ref->mark);
    NC_CHECK(
        nc_cmd_sys_mset(&ref->data, sizeof(CHAR_INFO) * ref->sizes.x * ref->sizes.y, NC_ZERO),
        "failed to delocate console buffer data!",
        return NC_FALSE
    );
    ref->mark = NC_NULL;
    ref->data = NC_NULL;
    ref->sizes.x = NC_ZERO;
    ref->sizes.y = NC_ZERO;
    ref->viewp.x = NC_ZERO;
    ref->viewp.y = NC_ZERO;
    ref->viewp.w = NC_ZERO;
    ref->viewp.h = NC_ZERO;
    ref->pixel.size = NC_ZERO;
    ref->pixel.sign = NC_ZERO;
    ref->pixel.tint = NC_ZERO;
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_buf_work(nc_cmd_buf_t* ref) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    /* work */
    const CHAR_INFO* pdata = NC_CAST(ref->data, CHAR_INFO*);
    const COORD sizes = { .X = ref->sizes.x, .Y = ref->sizes.y };
    const COORD coord = { .X = NC_ZERO, .Y = NC_ZERO };
    const SMALL_RECT viewp = {
        .Left = ref->viewp.x, .Top = ref->viewp.y,
        .Right = ref->viewp.x + ref->viewp.w, .Bottom = ref->viewp.y + ref->viewp.h
    };
    NC_CHECK(
        WriteConsoleOutput(
            ref->mark, /* what is the buffer */
            pdata,     /* what to draw */
            sizes,     /* how many to draw */
            coord,     /* where to start */
            &viewp     /* where to draw */
        ),
        "console write error!",
        { NC_ELOG("last_error: %d", GetLastError()); return NC_FALSE; }
    );
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_buf_olog(nc_cmd_buf_t* ref) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    /* work */
    NC_OLOG("cmd_buf:");
    NC_OPUT("{" NC_ENDL);
    NC_OPUT(NC_HTAB "mark: %p" NC_SEPL NC_ENDL, ref->mark);
    NC_OPUT(NC_HTAB "data: %p" NC_SEPL NC_ENDL, ref->data);
    NC_OPUT(NC_HTAB "sizes: { x: %d; y: %d; };" NC_SEPL NC_ENDL, ref->sizes.x, ref->sizes.y);
    NC_OPUT(NC_HTAB "viewp: { x: %d; y: %d; w: %d; h: %d; };" NC_SEPL NC_ENDL, ref->viewp.x, ref->viewp.y, ref->viewp.w, ref->viewp.h);
    NC_OPUT(NC_HTAB "pixel: { size: %i; sign: %d; tint: %d; };" NC_SEPL NC_ENDL, ref->pixel.size, ref->pixel.sign, ref->pixel.tint);
    NC_OPUT("}" NC_SEPL NC_ENDL);
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_buf_draw_pixel(nc_cmd_buf_t* ref, size_t x0, size_t y0) {
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    NC_CHECK(
        nc_cmd_buf_vet_sizes(ref, x0, y0),
        "size error!",
        return NC_FALSE
    );
    /* work */
    const size_t coord = (y0 * ref->sizes.x + x0);
    const CHAR_INFO pixel = {
        .Char = { ref->pixel.sign },
        .Attributes = ref->pixel.tint,
    };
    memset(ref->data + coord * sizeof(pixel), &pixel, sizeof(pixel) * 2u);
    /* quit */
    return NC_TRUTH;
}
NC_API_CALL v1bit_t nc_cmd_buf_draw_sline(nc_cmd_buf_t* ref, size_t x0, size_t y0, size_t x1, size_t y1){
    /* init */
    NC_CHECK(
        ref != NC_NULL,
        "ref error!",
        return NC_FALSE
    );
    NC_CHECK(
        nc_cmd_buf_vet_sizes(ref, x0, y0) && nc_cmd_buf_vet_sizes(ref, x1, y1),
        "size error!",
        return NC_FALSE
    );
    /* work */
    v1s_t xdir = NC_NUM_DIR(x0, x1), ydir = NC_NUM_DIR(y0, y1);
    v1s_t xdif = NC_NUM_DIFF(x0, x1), ydif = NC_NUM_DIFF(y0, y1);
    v1s_t err = xdif;
    while(x0 != x1 && y0 != y1) {
        x0 += xdir;
        err += xdif;
        if (err > ydif) {
            y0 += ydir;
            err -= ydif;
        }
    }
    /* quit */
    return NC_TRUTH;
}
#endif  /* NC_API */
/* end_of_file */