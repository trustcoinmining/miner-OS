#include <iostream>
#include <unistd.h>

int main()
{
	system("/root/runvivado/runvivado.sh");
    sleep(10);
    system("open_hw");
	return 0;
}
