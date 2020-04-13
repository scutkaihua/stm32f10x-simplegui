#include "types.h"
#include "driver_config_types.h"



static volatile U32 hc595data=0;//���ݻ���
static volatile U32 rd = 0xFFFFFFFF;
/*===================================================
                �����ļ�
====================================================*/
extern t_gpio_map gpio_map[];
extern const unsigned char gpio_number;

extern void ld_hc595_init(void);
extern void ld_hc595_send(uint32_t data);
U8 ld_gpio_refresh(void);
/*===================================================
                ȫ�ֺ���
====================================================*/
//��ʼ�����õ�iic�ӿ�
void ld_gpio_init(void)
{
  int i=0;
	for(;i<gpio_number;i++)
	{
		 cpu_gpio_map_config(gpio_map,i);
	}
}

//���õ�ƽ
void ld_gpio_set(U32 index,U8 value)
{
	index--;
	if(index>=gpio_number)return;
    cpu_gpio_map_set(gpio_map,index,value);
	
}

/*��ȡ��ƽ
* index :io����(1-n)
* return: 1:high  0:low
*/
U8 ld_gpio_get(U32 index)
{
	index--;
	if(index>=gpio_number)return 0;
    return cpu_gpio_map_get(gpio_map,index);
}

