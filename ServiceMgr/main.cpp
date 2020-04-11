#include "stdafx.h"
#include "ServiceMgr.h"

int main(int argc, char* argv[])
{
	printf("%s start", __FUNCTION__);
	ServiceMgr::GetInstance()->InitService();
	ServiceMgr::GetInstance()->StartService();
    return 0;
}

