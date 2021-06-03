#ifndef NC_CMD_BUFFER_HXX
#   define NC_CMD_BUFFER_HXX
#   include "../nc_cmd_core.hxx"
#       if (defined(NC_API))
#           if (NC_LANG & NC_LANG_CPP)
extern "C" {
#           endif   /* NC_LANG_CPP */
/* defines */
#       if (!(defined(NC_CMD_USE_SIZE_BUF)))
#           define NC_CMD_MIN_SIZE_BUF 10
#           define NC_CMD_MED_SIZE_BUF 50
#           define NC_CMD_MAX_SIZE_BUF 100
#           define NC_CMD_USE_SIZE_BUF NC_CMD_MED_SIZE_BUF
#       endif   /* NC_CMD_USE_SIZE_BUF */
#       if (!(defined(NC_CMD_USE_SIZE_PXL)))
#           define NC_CMD_MIN_SIZE_PXL 4
#           define NC_CMD_MED_SIZE_PXL 8
#           define NC_CMD_MAX_SIZE_PXL 16
#           define NC_CMD_USE_SIZE_PXL NC_CMD_MAX_SIZE_PXL
#       endif   /* NC_CMD_USE_SIZE_PXL */
/* includes */
/* types */
/* cmd_buffer_type
 * description:
*/
typedef struct nc_cmd_buf_t {
    ptr_t mark;
    data_t data;
    struct sizes_t { size_t x, y; } sizes;
    struct viewp_t { size_t x, y, w, h; } viewp;
    struct pixel_t { size_t size; indx_t sign; indx_t tint; } pixel;
} nc_cmd_buf_t;
typedef const nc_cmd_buf_t nc_cmd_buf_tc;
/* ctor_dtor */
NC_API_CALL NC_API v1bit_t nc_cmd_buf_ctor(nc_cmd_buf_t** ref);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_dtor(nc_cmd_buf_t** ref);
/* getters */
/* setters */
NC_API_CALL NC_API v1bit_t nc_cmd_buf_set_sizes(nc_cmd_buf_t* ref, size_t size_x, size_t size_y);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_set_viewp(nc_cmd_buf_t* ref, size_t coord_x, size_t coord_y, size_t size_x, size_t size_y);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_set_pixel_size(nc_cmd_buf_t* ref, size_t size);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_set_pixel_sign(nc_cmd_buf_t* ref, indx_t sign);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_set_pixel_tint(nc_cmd_buf_t* ref, indx_t tint);
/* vetters */
NC_API_CALL NC_API v1bit_t nc_cmd_buf_vet_sizes(nc_cmd_buf_t* ref, size_t size_x, size_t size_y);
/* commands */
NC_API_CALL NC_API v1bit_t nc_cmd_buf_init(nc_cmd_buf_t* ref);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_quit(nc_cmd_buf_t* ref);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_work(nc_cmd_buf_t* ref);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_olog(nc_cmd_buf_t* ref);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_draw_pixel(nc_cmd_buf_t* ref, size_t x0, size_t y0);
NC_API_CALL NC_API v1bit_t nc_cmd_buf_draw_sline(nc_cmd_buf_t* ref, size_t x0, size_t y0, size_t x1, size_t y1);
#           if (NC_LANG & NC_LANG_CPP)
}
#           endif   /* NC_LANG_CPP */
#       endif  /* NC_API */
#endif /* NC_CMD_BUFFER_HXX */