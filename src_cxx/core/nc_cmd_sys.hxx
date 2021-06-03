#ifndef NC_CMD_SYSTEM_HXX
#   define NC_CMD_SYSTEM_HXX
#   include "../nc_cmd_core.hxx"
#   if (defined NC_API)
/* includes */
/* defines */
/* types */
/* cmd_system_type
 * description:
 * ->
*/
typedef struct nc_cmd_sys_t {
    nc_sys_t base;
    nc_cmd_buf_t* fbuf;
    struct orig_t {
        ptr_t iput;
        ptr_t oput;
    } orig;
    struct keyb_t {
        struct { flag_t flag; size_t reps; } keys [NC_KEYCODE_COUNT];
    } keyb;
    struct curs_t {
        struct {
            flag_t flag;
            struct { size_t x; size_t y; } free;
            struct { size_t x; size_t y; } held;
        } keys [NC_CURCODE_COUNT];
        struct { size_t x; size_t y; } move;
    } curs ;
} nc_cmd_sys_t;
typedef const nc_cmd_sys_t nc_cmd_sys_tc;
/** getters **/
NC_API_CALL NC_API nc_cmd_sys_t* nc_cmd_sys_get();
NC_API_CALL NC_API nc_cmd_buf_t* nc_cmd_sys_get_buf();
NC_API_CALL NC_API size_t nc_cmd_sys_get_keyb_reps(indx_t indx);
NC_API_CALL NC_API flag_t nc_cmd_sys_get_keyb_flag(indx_t indx);
NC_API_CALL NC_API v1bit_t nc_cmd_sys_get_keyb_free(indx_t indx);
NC_API_CALL NC_API v1bit_t nc_cmd_sys_get_keyb_held(indx_t indx);
NC_API_CALL NC_API flag_t nc_cmd_sys_get_curs_flag(indx_t indx);
NC_API_CALL NC_API v1bit_t nc_cmd_sys_get_curs_free(indx_t indx);
NC_API_CALL NC_API v1bit_t nc_cmd_sys_get_curs_held(indx_t indx);
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_movex();
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_movey();
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_freex(indx_t indx);
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_freey(indx_t indx);
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_heldx(indx_t indx);
NC_API_CALL NC_API size_t nc_cmd_sys_get_curs_heldy(indx_t indx);
/** setters **/
/** vetters **/
/** commands **/
NC_API_CALL NC_API v1bit_t nc_cmd_sys_init();
NC_API_CALL NC_API v1bit_t nc_cmd_sys_quit();
NC_API_CALL NC_API v1bit_t nc_cmd_sys_work();
NC_API_CALL NC_API v1bit_t nc_cmd_sys_mset(ptr_t* data, size_t isize, size_t osize);
NC_API_CALL NC_API v1bit_t nc_cmd_sys_olog();
#   endif  /* NC_API */
#endif /* NC_CMD_SYSTEM_HXX */