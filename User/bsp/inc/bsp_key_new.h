/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_key.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_NEW_H
#define __BSP_KEY_NEW_H

/*
	�����˲�ʱ��50ms, ��λ10ms��
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */
#define KEY_DOUBLE_PRESS_TIME 30  //˫������¼�500ms


/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;

/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_K1 = 0,
	KID_K2,
	KID_K3,
	KID_JOY_U,
	KID_JOY_D,
	KID_JOY_L,
	KID_JOY_R,
	KID_JOY_OK
}KEY_ID_E;

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
//	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
//	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
	uint8_t  double_press_flag;
	uint32_t double_press_time;
	
}KEY_T;



/* ����Ӧ�ó���Ĺ��������������� */
#define KEY_DOWN_K1		KEY_1_DOWN
#define KEY_UP_K1		KEY_1_UP
#define KEY_LONG_K1		KEY_1_LONG

#define KEY_DOWN_K2		KEY_2_DOWN
#define KEY_UP_K2		KEY_2_UP
#define KEY_LONG_K2		KEY_2_LONG

#define KEY_DOWN_K3		KEY_3_DOWN
#define KEY_UP_K3		KEY_3_UP
#define KEY_LONG_K3		KEY_3_LONG

#define JOY_DOWN_U		KEY_4_DOWN		/* �� */
#define JOY_UP_U		KEY_4_UP
#define JOY_LONG_U		KEY_4_LONG

#define JOY_DOWN_D		KEY_5_DOWN		/* �� */
#define JOY_UP_D		KEY_5_UP
#define JOY_LONG_D		KEY_5_LONG

#define JOY_DOWN_L		KEY_6_DOWN		/* �� */
#define JOY_UP_L		KEY_6_UP
#define JOY_LONG_L		KEY_6_LONG

#define JOY_DOWN_R		KEY_7_DOWN		/* �� */
#define JOY_UP_R		KEY_7_UP
#define JOY_LONG_R		KEY_7_LONG

#define JOY_DOWN_OK		KEY_8_DOWN		/* ok */
#define JOY_UP_OK		KEY_8_UP
#define JOY_LONG_OK		KEY_8_LONG

#define SYS_DOWN_K1K2	KEY_9_DOWN		/* K1 K2 ��ϼ� */
#define SYS_UP_K1K2	    KEY_9_UP
#define SYS_LONG_K1K2	KEY_9_LONG

//#define SYS_DOWN_K2K3	KEY_10_DOWN		/* K2 K3 ��ϼ� */
//#define SYS_UP_K2K3  	KEY_10_UP
//#define SYS_LONG_K2K3	KEY_10_LONG


/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_1_SHORT_PRESS,				/* 1������ */
	KEY_1_LONG_PRESS,				/* 1������ */
	KEY_1_DOUBLE_PRESS,				/* 1������ */
	KEY_1_LONG_PRESS_UP,            /*��������*/

	KEY_2_SHORT_PRESS,				/* 2������ */
	KEY_2_LONG_PRESS,				/* 2������ */
	KEY_2_DOUBLE_PRESS,				/* 2������ */
	KEY_2_LONG_PRESS_UP,            /* 2��������*/
	
	KEY_3_SHORT_PRESS,				/* 3������ */
	KEY_3_LONG_PRESS,				/* 3������ */
	KEY_3_DOUBLE_PRESS,				/* 3������ */
	KEY_3_LONG_PRESS_UP,            /* 3��������*/
	
	KEY_4_SHORT_PRESS,				/* 4������ */
	KEY_4_LONG_PRESS,				/* 4������ */
	KEY_4_DOUBLE_PRESS,				/* 4������ */
	KEY_4_LONG_PRESS_UP,            /* 4��������*/

	KEY_5_SHORT_PRESS,				/* 1������ */
	KEY_5_LONG_PRESS,				/* 1������ */
	KEY_5_DOUBLE_PRESS,				/* 1������ */
	KEY_5_LONG_PRESS_UP,            /*��������*/
	
	KEY_6_SHORT_PRESS,				/* 6������ */
	KEY_6_LONG_PRESS,				/* 6������ */
	KEY_6_DOUBLE_PRESS,				/* 6������ */
	KEY_6_LONG_PRESS_UP,            /* 6��������*/

	KEY_7_SHORT_PRESS,				/* 7������ */
	KEY_7_LONG_PRESS,				/* 7������ */
	KEY_7_DOUBLE_PRESS,				/* 7������ */
	KEY_7_LONG_PRESS_UP,            /* 7��������*/

	KEY_8_SHORT_PRESS,				/* 8������ */
	KEY_8_LONG_PRESS,				/* 8������ */
	KEY_8_DOUBLE_PRESS,				/* 8������ */
	KEY_8_LONG_PRESS_UP,            /* 8��������*/
	/* ��ϼ� */
	KEY_9_SHORT_PRESS,				/* 9������ */
	KEY_9_LONG_PRESS,				/* 9������ */
	KEY_9_DOUBLE_PRESS,				/* 9������ */
	KEY_9_LONG_PRESS_UP,            /* 9��������*/
//	KEY_10_DOWN,			/* 10������ */
//	KEY_10_UP,				/* 10������ */
//	KEY_10_LONG,			/* 10������ */
}KEY_ENUM;


void KEY_Init(void);
uint8_t KEY_Get_State(KEY_ID_E key_id);
void KEY_Clear_FIFO(void);
void KEY_Scan_10MS(void);
uint8_t KEY_Read_FIFO(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/