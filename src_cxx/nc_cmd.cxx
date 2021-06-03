#include "nc_cmd_pch.hxx"
#include "nc_cmd.hxx"
#if (defined NC_BUILD)
#   if (NC_BUILD & NC_BUILD_EXE)
int main(int arg_num, const char* arg_str[])
{
    NC_OLOG(NC_GREETING);
    NC_CHECK(nc_cmd_sys_init(), "init error!", return NC_FALSE);
    nc_cmd_buf_t* buf = nc_cmd_sys_get_buf();
    nc_cmd_buf_olog(buf);
#   if (NC_FALSE)
    for (v1u_t itr = NC_ZERO; itr < 50; itr++) {
        nc_cmd_buf_set_pixel_sign(buf, rand() % 128);
        nc_cmd_buf_set_pixel_tint(buf, rand() % NC_CMD_COL_BG_16);
        nc_cmd_buf_draw_pixel(buf, rand() % buf->sizes.x, rand() % buf->sizes.y);
    }
    nc_cmd_sys_work();
    NC_BREAK();
#   endif
#   if (NC_TRUTH)
    while (nc_cmd_sys_work()) {
        if (nc_cmd_sys_get_curs_held(NC_CURCODE_0)) {
            nc_cmd_buf_draw_pixel(buf, nc_cmd_sys_get_curs_movex(), nc_cmd_sys_get_curs_movey());
        }
    }
    NC_BREAK();
#   endif
    NC_CHECK(nc_cmd_sys_quit(), "quit error!", return NC_FALSE);
    NC_BREAK();
    return 0;
}
#   endif   /* NC_BUILD_EXE */
#endif  /* NC_BUILD */
/* end_of_file */