#include "../nc_cmd_pch.hxx"
#include "nc_cmd_buf.hxx"
#if (defined NC_API)
/* getters */
/* setters */
/* predicates */
/* commands */
v1bit_t nc_cmd_buf_init(nc_cmd_buf_t* ref) {
    NC_CHECK(ref->handle == NC_NULL, "init error!", return NC_FALSE);
    NC_CHECK(ref->pxl_data == NC_NULL, "init error!", return NC_FALSE);
    //
    //
    return NC_TRUTH;
}
v1bit_t nc_cmd_buf_quit(nc_cmd_buf_t* ref) {
    NC_CHECK(ref->handle != NC_NULL, "quit error!", return NC_FALSE);
    NC_CHECK(ref->pxl_data != NC_NULL, "quit error!", return NC_FALSE);
    //
    CloseHandle(ref->handle);
    ref->pxl_data;
    //
    ref->handle = NC_NULL;
    ref->pxl_data = NC_NULL;
    return NC_TRUTH;
}
v1bit_t nc_cmd_buf_work(nc_cmd_buf_t* ref) {
    nc_cmd_v2u_t coord = { 0, 0 };
    WriteConsoleOutput(ref->handle, ref->pxl_data, ref->sizes, coord, &ref->viewp);
    return NC_TRUTH;
}
#endif  /* NC_API */
/* end_of_file */