#ifndef __BSP_MODBUS_H
#define __BSP_MODBUS_H

#include "stm32f10x.h"


#define READ_COIL     01
#define READ_DI       02
#define READ_HLD_REG  03
#define READ_AI       04
#define SET_COIL      05
#define SET_HLD_REG   06
#define NEIZU_CAIJI   07
#define READ_FIFO     24
#define PROTOCOL_EXCEPTION 0x81
#define PROTOCOL_ERR  1
#define FRM_ERR       2

#define boardnum 11
#define HI(n) ((n)>>8)
#define LOW(n) ((n)&0xff)


void construct_rtu_frm ( u8 *dst_buf,u8 *src_buf,u8 lenth);
u16  rtu_neizu_order ( u8 board_adr,u8 *com_buf,u16 start_address,u16 lenth);
u16  rtu_read_hldreg ( u8 board_adr,u8 *com_buf,u16 start_address,u16 lenth);
u16 rtu_set_hldreg( u8 board_adr,u8 *com_buf, u16 start_address, u16 value );
int rtu_data_anlys( u16  *dest_p, u8 *source_p, u16 data_start_address, u16 fr_lenth);

void modbus_rtu_dy(void);
void modbus_rtu_nz(void);
void modbus_rtu(void);


void test_modbus_master_read(void);//≤‚ ‘
#endif









