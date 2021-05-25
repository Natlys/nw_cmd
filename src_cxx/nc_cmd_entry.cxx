#include "nc_cmd_pch.hxx"
#include "nc_cmd.hxx"
#if (defined NC_BUILD)
#   if (NC_BUILD & NC_BUILD_EXE)
int main(int arg_num, const char* arg_str[])
{
    NC_OLOG(NC_GREETING);
    system("pause");
    return 0;
}
#   endif   /* NC_BUILD_EXE */
#endif  /* NC_BUILD */
/* end_of_file */