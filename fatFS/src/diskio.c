/*
*********************************************************************************************************
*
*	ģ������ : fatfs��sd���Ľӿ��ļ�
*	�ļ����� : diskio.c
*	��    �� : V1.1
*	˵    �� : ֧��SD��
*
*	�޸ļ�¼ :
*		�汾��   ����         ����           ˵��
*       V1.0    2014-06-19   Eric2013        �׷�
*       V1.1    2016-11-28   Eric2013      sectorǰ��Ҫ��ǿ������ת��uint64_t ����Ϊ�������β���uint64_t����   
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "diskio.h"		    /* FatFs lower layer API */
#include "bsp.h"


#define SECTOR_SIZE		512
/*-------------------------------------------------------------------------------------------*/
/* Inidialize a Drive                                                                        */
/*-------------------------------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
		case FS_SD :		/* SD�� */
			if (SD_Init() == SD_OK)
			{
				stat = RES_OK;
			}
			else
			{
				stat = STA_NODISK;
			}
			break;

		default :
			break;
	}

	return stat;
}

/*-------------------------------------------------------------------------------------------*/
/* Get Disk Status                                                                           */
/*-------------------------------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
		case FS_SD :
			stat = 0;
			break;

		default:
			stat = 0;
			break;
	}
	return stat;
}

/*-------------------------------------------------------------------------------------------*/
/* Read Sector(s)                                                                            */
/*-------------------------------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res = RES_OK;

	switch (pdrv)
	{
		case FS_SD :
		{
			SD_Error Status = SD_OK;

			if (count == 1)
			{
				Status = SD_ReadBlock(buff, (uint64_t)sector << 9 , SECTOR_SIZE);
			}
			else
			{
				Status = SD_ReadMultiBlocks(buff, (uint64_t)sector << 9 , SECTOR_SIZE, count);
			}
			
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}

		#ifdef SD_DMA_MODE
			/* SDIO������DMAģʽ����Ҫ������DMA�����Ƿ���� */
			Status = SD_WaitReadOperation();
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}

			while(SD_GetStatus() != SD_TRANSFER_OK);
		#endif

			res = RES_OK;
			break;
		}
		
		default:
			res = RES_PARERR;
			break;
	}
	
	return res;
}

/*-------------------------------------------------------------------------------------------*/
/* Write Sector(s)                                                                           */
/*-------------------------------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res = RES_OK;

	switch (pdrv)
	{
		case FS_SD :
		{
			SD_Error Status = SD_OK;

			if (count == 1)
			{
				Status = SD_WriteBlock((uint8_t *)buff, (uint64_t)sector << 9 ,SECTOR_SIZE);
			}
			else
			{
				/* �˴��������ʣ� �����������д count �����������1��block�޷�д�� */
				//Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count);
				Status = SD_WriteMultiBlocks((uint8_t *)buff, (uint64_t)sector << 9 ,SECTOR_SIZE, count + 1);
			}
			
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}

		#ifdef SD_DMA_MODE
			/* SDIO������DMAģʽ����Ҫ������DMA�����Ƿ���� */
			Status = SD_WaitReadOperation();
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}
			while(SD_GetStatus() != SD_TRANSFER_OK);
		#endif
			
			res = RES_OK;
			break;
		}

		default:
			res = RES_PARERR;
			break;
	}
	return res;
}
#endif


/*-------------------------------------------------------------------------------------------*/
/* Miscellaneous Functions                                                                   */
/*-------------------------------------------------------------------------------------------*/
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case FS_SD :
		res = RES_ERROR;
		switch (cmd)
		{
			/* SD������������ SDCardInfo.CardCapacity */
			case CTRL_SYNC :		/* Wait for end of internal write process of the drive */
				res = RES_OK;
				break;

			case GET_SECTOR_COUNT :	/* Get drive capacity in unit of sector (DWORD) */
				*(DWORD*)buff = SDCardInfo.CardCapacity / 512;
				res = RES_OK;
				break;

			case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
				*(WORD*)buff = 512;
				res = RES_OK;
				break;

			case CTRL_ERASE_SECTOR: /* Erase a block of sectors (used when _USE_ERASE == 1) */
			default:
				res = RES_PARERR;
				break;
		}
		break;

	default:
		res = RES_PARERR;
		break;
	}
	return res;
}
#endif

/*
*********************************************************************************************************
*	�� �� ��: get_fattime
*	����˵��: ���ϵͳʱ�䣬���ڸ�д�ļ��Ĵ������޸�ʱ�䡣
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* �����ȫ��ʱ�ӣ��ɰ�����ĸ�ʽ����ʱ��ת��. ���������2013-01-01 00:00:00 */

	return	  ((DWORD)(2013 - 1980) << 25)	/* Year = 2013 */
			| ((DWORD)1 << 21)				/* Month = 1 */
			| ((DWORD)1 << 16)				/* Day_m = 1*/
			| ((DWORD)0 << 11)				/* Hour = 0 */
			| ((DWORD)0 << 5)				/* Min = 0 */
			| ((DWORD)0 >> 1);				/* Sec = 0 */
}