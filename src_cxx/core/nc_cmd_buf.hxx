#ifndef NC_CMD_BUFFER_H
#   define NC_CMD_BUFFER_H
#   include "../nc_cmd_core.hxx"
#       if (defined NC_API)
/* includes */
// types //
/// cmd_framebuffer_type
/// description:
typedef struct {
    ptr_t handle;
    nc_cmd_pxl_t* pxl_data;
    nc_cmd_v2u_t sizes;
    nc_cmd_v4u_t viewp;
    nc_cmd_pxl_info_t pxl_info;
    nc_cmd_buf_info_t buf_info;
} nc_cmd_buf_t;
typedef const nc_cmd_buf_t nc_cmd_buf_tc;
/* getters */
/* setters */
NC_API v1bit_t nc_cmd_buf_set_pixel(nc_cmd_buf_t* ref);
NC_API v1bit_t nc_cmd_buf_set_sizes(nc_cmd_buf_t* ref, v1u_t size_x, v1u_t size_y);
NC_API v1bit_t nc_cmd_buf_set_viewp(nc_cmd_buf_t* ref, v1u_t coord_x, v1u_t coord_y, v1u_t size_x, v1u_t size_y);
/* commands */
NC_API v1bit_t nc_cmd_buf_init(nc_cmd_buf_t* ref);
NC_API v1bit_t nc_cmd_buf_quit(nc_cmd_buf_t* ref);
NC_API v1bit_t nc_cmd_buf_work(nc_cmd_buf_t* ref);
#       endif  /* NC_API */
#endif // NC_CMD_BUFFER_H //