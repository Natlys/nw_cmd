#include "../nc_cmd_pch.hxx"
#include "nc_cmd_eng.hxx"
#if (defined NC_API)
/// cmd_engine_type
/// description:
/// ->
/// interaction:
/// ->
typedef struct nc_cmd_eng_t {
    v1bit_t work_flag : 1;
    nc_cmd_buf_t buffer;
    ptr_t orig_iput;
    ptr_t orig_oput;
} nc_cmd_eng_t;
typedef const nc_cmd_eng_t nc_cmd_eng_tc;
static nc_cmd_eng_t nc_cmd_eng;
/* getters */
// settres //
/* predicates */
v1bit_t nc_cmd_eng_has_work() {
    return nc_cmd_eng.work_flag;
}
/* commands */
v1bit_t nc_cmd_eng_init() {
    NC_CHECK(nc_cmd_eng.work_flag == NC_FALSE, "init error!", return NC_FALSE);
    NC_CHECK(nc_cmd_eng.orig_oput == NC_NULL, "init error!", return NC_FALSE);
    NC_CHECK(nc_cmd_eng.orig_iput == NC_NULL, "init error!", return NC_FALSE);
    //
    NC_CHECK((nc_cmd_eng.orig_oput = GetStdHandle(STD_OUTPUT_HANDLE)) != INVALID_HANDLE_VALUE, "init error!", return NC_FALSE);
    NC_CHECK((nc_cmd_eng.orig_iput = GetStdHandle(STD_INPUT_HANDLE)) != INVALID_HANDLE_VALUE, "init error!", return NC_FALSE);
    NC_CHECK(nc_cmd_buf_init(&nc_cmd_eng.buffer), "init error!", return NC_FALSE);
    //
    NC_OPUT("nc_iop::cmd engine init;\n");
    nc_cmd_eng.work_flag = NC_TRUTH;
    return NC_TRUTH;
}
v1bit_t nc_cmd_eng_quit() {
    NC_CHECK(nc_cmd_eng.work_flag == NC_FALSE, "quit error!", return NC_FALSE);
    NC_CHECK(nc_cmd_eng.orig_oput != NC_NULL, "quit error!", return NC_FALSE);
    NC_CHECK(nc_cmd_eng.orig_iput != NC_NULL, "quit error!", return NC_FALSE);
    //
    NC_CHECK(nc_cmd_buf_quit(&nc_cmd_eng.buffer), "quit error!", return NC_FALSE);
    SetConsoleActiveScreenBuffer(nc_cmd_eng.orig_iput);
    //
    NC_OPUT("nc_iop::cmd engine quit;\n");
    nc_cmd_eng.work_flag = NC_FALSE;
    return NC_TRUTH;
}
v1bit_t nc_cmd_eng_work() {
    //
    nc_cmd_eng.work_flag = NC_FALSE;
    //
    NC_OPUT("nc_iop::cmd engine work;\n");
    return NC_TRUTH;
}
v1bit_t nc_cmd_eng_exec() {
    NC_OPUT("nc_iop::cmd engine exec;\n");
    memset(&nc_cmd_eng, NC_ZERO, sizeof(nc_cmd_eng_t));
    NC_CHECK(nc_cmd_eng_init(), "init error!", return NC_FALSE);
    //
    while(nc_cmd_eng.work_flag) { nc_cmd_eng_work(); }
    //
    NC_CHECK(nc_cmd_eng_quit(), "quit error!", return NC_FALSE);
    return NC_TRUTH;
}
#endif  /* NC_API */
/* end_of_file */