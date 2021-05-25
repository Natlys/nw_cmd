#ifndef NC_CMD_ENGINE_H
#   define NC_CMD_ENGINE_H
#   include "../nc_cmd_core.hxx"
#   if (defined NC_API)
/* includes */
#       include "nc_cmd_buf.hxx"
/* getters */
/* setters */
// preidcates //
NC_API v1bit_t nc_cmd_eng_has_work();
/* commands */
NC_API v1bit_t nc_cmd_eng_init();
NC_API v1bit_t nc_cmd_eng_quit();
NC_API v1bit_t nc_cmd_eng_work();
NC_API v1bit_t nc_cmd_eng_exec();
#   endif  /* NC_API */
#endif // NC_CMD_ENGINE_H //