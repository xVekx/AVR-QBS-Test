#ifndef DEFINE_H
#define DEFINE_H
//---------------------------------------------------------------------------------------------------
//#define __AVR_ATmega2560__
#define __AVR_ATmega328P__ 1

//#define __AVR_ATmega644PA__ 1

#define ENABLE_HELP			0
//---------------------------------------------------------------------------------------------------
#define BIT(B)					(1u<<B)// преобр. номера бита в позицию в байте
#define TBIT0(ADDRESS,B)		(!(ADDRESS & (BIT(B))))// проверка бита на ноль
#define TBIT1(ADDRESS,B)		(ADDRESS & (BIT(B)))// проверка бита на единицу
#define SBIT(ADDRESS,B)			(ADDRESS |= BIT(B))// установка бита
#define CBIT(ADDRESS,B)			(ADDRESS &=~BIT(B))// сброс бита
#define SMASK(ADDRESS,BYTE)		(ADDRESS |= BYTE)// установка битов по байтовой маске
#define CMASK(ADDRESS,BYTE)		(ADDRESS &= ~(BYTE))// очистка битов по байтовой маске
//---------------------------------------------------------------------------------------------------
#define ARRAY_SIZE(x)			(sizeof(x) / sizeof((x)[0]))
#define NOP() do { __asm__ __volatile__ ("nop"); } while (0)

#define PRINT_FUN_NAME()	({				\
								/*printf("%s\n",__func__);*/		\
							})

#define GetStruct_P(t,d,s)	(memcpy_P((void*)d,(PGM_VOID_P)s,sizeof(struct t)))
//---------------------------------------------------------------------------------------------------
extern const char PRINT_NULL[];
extern const char PRINT_NOT_COMMAND[];
extern const char PRINT_ARGS_INVALID[];
//---------------------------------------------------------------------------------------------------
#if defined (__AVR_ATmega16__)
	#define F_CPU			32768		//частота микропроцессора
#elif defined (__AVR_ATmega2560__)
	#define F_CPU			16000000		//частота микропроцессора
#elif defined (__AVR_ATmega328P__)
	#define F_CPU			16000000		//частота микропроцессора
#elif defined (__AVR_ATmega644P__)
	#define F_CPU			16000000		//частота микропроцессора
#elif defined (__AVR_ATmega644PA__)
	#define F_CPU			16000000		//частота микропроцессора
#endif
//---------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//---------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <util/crc16.h>
#include <util/atomic.h>

#include "stduart.h"
#include "command.h"
//---------------------------------------------------------------------------------------------------
void PrintHexString(char *data,int len);
void PrintHexStr(char *data);
void PrintBitStr(uint8_t data);
void PrintTextFLASH(const char* text);
void PrintTextTableFLASH(const char* const* table,uint8_t len);
void TestArrayShift(void);
void ArrayShiftRight(char *buff,uint16_t sb,char s,uint16_t ss);
void ArrayShiftLeft(char *buff,uint16_t sb,char s,uint16_t ss);
void UInt16ToTextEdit(char *buff,uint16_t n,uint8_t size,int8_t point);
uint16_t TextToUInt16Edit(char *buff,uint8_t size,int8_t point);
char NumericSymbolUp(char data);
char NumericSymbolDown(char data);
uint16_t FlashCheckCRC16(void);
//---------------------------------------------------------------------------------------------------
#endif
