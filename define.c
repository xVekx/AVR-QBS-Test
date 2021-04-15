#include "define.h"
//---------------------------------------------------------------------------------------------------
void PrintHexString(char *data, int len)
{
	int n;
	for(n=0;n<len;n++)
		printf("%.2x|",(uint8_t)data[n]);
	printf("\n");
}
//---------------------------------------------------------------------------------------------------
void PrintHexStr(char *data)
{
	uint8_t n;
	for(n=0;n<strlen(data);n++)
		printf("%.2x|",(uint8_t)data[n]);
	printf("\n");
}
//---------------------------------------------------------------------------------------------------
void PrintBitStr(uint8_t data)
{
	uint8_t n;
	for(n=0;n<8;n++)
		TBIT1(data,n) ? printf("1") : printf("0");
	printf("\n");
}
//---------------------------------------------------------------------------------------------------
void PrintTextFLASH(const char* text)
{
	uint16_t len = strlen_P(text);
	do {
		printf("%c",pgm_read_byte(text++));
		len--;
	} while(len);
}
//---------------------------------------------------------------------------------------------------
void PrintTextTableFLASH(const char* const* table,uint8_t len)
{
	for(uint8_t i=0;i<len;i++) {
		PGM_P tmp = (PGM_P)pgm_read_word(&(table[i]));
		PrintTextFLASH(tmp);
		printf("\n");
	}
}
//---------------------------------------------------------------------------------------------------
const char PRINT_NULL[]			PROGMEM = "NULL";
const char PRINT_NOT_COMMAND[]	PROGMEM = "not command";
const char PRINT_ARGS_INVALID[] PROGMEM = "arg's invalid";
//---------------------------------------------------------------------------------------------------
void ArrayShiftRight(char *buff,uint16_t sb,char s,uint16_t ss)
{
	uint16_t i;
	for(i=sb;(i+1) > 0;i--){
		buff[i+ss] = buff[i];
	}
	for(i=0;i<ss;i++){
		buff[i] = s;
	}
}
//---------------------------------------------------------------------------------------------------
void ArrayShiftLeft(char *buff,uint16_t sb,char s,uint16_t ss)
{
	uint16_t i;
	for(i=0;i<sb;i++){
		buff[i] = buff[i+ss];
	}
	for(i=sb;i<sb+ss;i++) {
		buff[i] = s;
	}
}
//---------------------------------------------------------------------------------------------------
void UInt16ToTextEdit(char *buff,uint16_t n,uint8_t size,int8_t point)
{
	sprintf(buff,"%i",n);
	uint8_t len = strlen(buff);
	ArrayShiftRight(buff,len,'0',(size-len) < 0 ? 0 : (size-len));

	if(point >= 0) {
		ArrayShiftRight(&buff[size-point],point,'.',1);
		ArrayShiftLeft(buff,size,0,1);
	}
}
//---------------------------------------------------------------------------------------------------
uint16_t TextToUInt16Edit(char *buff,uint8_t size,int8_t point)
{
	uint16_t ret = 0;
	if(point >= 0) {
		ArrayShiftLeft(&buff[(size-point-1) < 0 ? 0 : (size-point-1)],point,0,1);
	}

	uint8_t i=0;
	for(i=0;i<size;i++) {
		if(buff[i] != '0'){
			sscanf(&buff[i],"%i",&ret);
			break;
		}
	}
	return ret;
}
//---------------------------------------------------------------------------------------------------
char NumericSymbolUp(char data)
{
	return ((0x30 <= data) && (data < 0x39)) ? ++data : data;
}
//---------------------------------------------------------------------------------------------------
char NumericSymbolDown(char data)
{
	return ((0x30 < data) && (data <= 0x39)) ? --data : data;
}
//---------------------------------------------------------------------------------------------------
uint16_t FlashCheckCRC16(void)
{
	uint16_t crc = 0, i;
	uint8_t tmp;
	for (i = 0; i < FLASHEND - 0x2000; i++)
	{
		tmp = pgm_read_byte_near(i);
		crc = _crc16_update(crc,tmp);
	}
	return crc; // must be 0
}
//---------------------------------------------------------------------------------------------------
