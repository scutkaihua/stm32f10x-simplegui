
#include "dev.h"
extern void YLF_init(void);


int main(void)
{
	extern void DemoMainProcess(void);
	
	ld_dev_init();
	//LcdInit(); 
	YLF_init();
	(void)DemoMainProcess();	
}

