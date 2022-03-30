#include "wifiinfo.h"

void test(void)
{
    struct iwreq wrq;
    struct wireless_info info;
    memset((char *)&info, 0, sizeof(struct wireless_info));
    printf("%d\n", iw_get_basic_config(0, "eth0", &(info.b)));
    printf("%d\n", info.b.mode);

}