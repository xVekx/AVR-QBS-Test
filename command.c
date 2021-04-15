#include "define.h"
//---------------------------------------------------------------------------------------------------
#define COMMAND_MAX_COUNT 8
#define COMMAND_SIZE 24
//---------------------------------------------------------------------------------------------------
static char commad_arg_list[COMMAND_MAX_COUNT][COMMAND_SIZE];
//---------------------------------------------------------------------------------------------------
void Test(int8_t x,char * (*arg)(int8_t))
{
	printf("--------------\n");
	printf("%s\n",__func__);
	printf("len:%i\n",x);
	int8_t i;
	for(i=0;i<x;i++) {
		printf("%s\n",arg(i));
	}
	printf("--------------\n");
}
//---------------------------------------------------------------------------------------------------
void Echo(int8_t x,char * (*arg)(int8_t))
{
#if !defined (EMU_DEDUG)
	char *arg1 = arg(1);
	if(strlen(arg1) == 1)
		UartSetFlagRxEcho((arg1[0] == '1') ? 1:0);
#endif
}
//---------------------------------------------------------------------------------------------------
static const char CommandFun0[] PROGMEM = "test";
static const char CommandFun1[] PROGMEM = "reset";
static const char CommandFun2[] PROGMEM = "e";
//---------------------------------------------------------------------------------------------------
static const struct  sCommandComOne CommandFun[] PROGMEM =
{
	{
		.name = CommandFun0,
		.fun = Test,
	},
	{
		.name = CommandFun1,
		.fun = 0,
	},
	{
		.name = CommandFun2,
		.fun = Echo,
	},
};
//---------------------------------------------------------------------------------------------------
/*static const struct  sCommandKeyOne CommandKey[] PROGMEM = {
	{
		.key = MKEY_CK13_ESC,
		.flag = MKEY_DOWN_UP,
#if defined (GUI_LIST_H)
		.fun = ListEsc,
#else
		#warning CommandKey ListEsc GUI_LIST_H
#endif
	},{
		.key = MKEY_CK13_F3,
		.flag = MKEY_DOWN_UP,
		.fun = LcdLadSW,
	}
};*/
//---------------------------------------------------------------------------------------------------
const struct sCommandCom CommandComTest PROGMEM =
{
	.com = CommandFun,
	.size = ARRAY_SIZE(CommandFun),
};
//---------------------------------------------------------------------------------------------------
/*const struct sCommandKey CommandKeyTest PROGMEM =
{
	.key = CommandKey,
	.size = ARRAY_SIZE(CommandKey),
};*/
//---------------------------------------------------------------------------------------------------
int8_t CommandList(char *data,int8_t len)
{
	if(strlen(data) == 0 )
		return -1;

	int8_t command_count=0;
	int8_t begin = 0;
	int8_t end = 0;
	int8_t n;

	for(n=begin;n<len;n++) {
		if(data[n] == ' ' || data[n] == '\0') {
			end = n + 1;
			if((end - begin) > COMMAND_SIZE)
				return -1;

			char *str = commad_arg_list[command_count];
			memset(str,0,COMMAND_SIZE);
			memcpy(str,&data[begin],end - begin - 1);

			if(data[n] == '\0')
				break;
			command_count++;
			n++;
			begin = n;
		}
	}
	return command_count;
}
//---------------------------------------------------------------------------------------------------
void CommandListPrint( void )
{
	printf("\n%s\n",__func__);
	int8_t s;
	for(s=0;s<COMMAND_MAX_COUNT;s++) {
		char *str = commad_arg_list[s];
		PrintHexStr(str);
	}
}
//---------------------------------------------------------------------------------------------------
char *CommandString(int8_t i)
{
	return commad_arg_list[i];
}
//---------------------------------------------------------------------------------------------------
uint8_t CommandComOnePROGMEM(const struct sCommandComOne *fun, uint8_t size,
								char *buff, int8_t len)
{
	int8_t command_count = CommandList(buff,len);
	if(command_count < 0) {
		printf("not command\n");
		return 0;
	}

	char *command_name = CommandString(0);
	int8_t command_name_len = strlen(command_name);
	int8_t i,temp_cmp;

	struct sCommandComOne tmp;
	uint8_t flag = 0;

	for(i=0;i<size;i++) {
		char command_name_fun[COMMAND_SIZE];
		memcpy_P(	(void*)&tmp,
					(PGM_VOID_P)&fun[i],
					sizeof(struct sCommandComOne));

		strcpy_P(command_name_fun,tmp.name);

		if(command_name_len == strlen(command_name_fun)) {
			temp_cmp = memcmp(command_name,
							  command_name_fun,
							  command_name_len);
			if(temp_cmp == 0) {
				flag = 1;
				break;
			}
		}
	}

	if(flag) {
		tmp.fun(command_count+1,CommandString);
		return !0;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------
uint8_t CommandComPROGMEM(const struct sCommandCom *fun, char *buff, int8_t len)
{
	if((fun == NULL) || (buff == NULL))
		return 0;

	struct sCommandCom tmp;
	memcpy_P((void*)&tmp,(PGM_VOID_P)fun,sizeof(struct sCommandCom));
	return CommandComOnePROGMEM(tmp.com,tmp.size,buff,len);
}
//---------------------------------------------------------------------------------------------------
uint8_t CommandKeyOnePROGMEM(	const struct sCommandKeyOne *key,uint8_t size,
							uint8_t *keys,int8_t len)
{
	int8_t k,i;
	struct sCommandKeyOne tmp;
	for(i=0;i<size;i++) {
		memcpy_P((void*)&tmp,(PGM_VOID_P)&key[i],sizeof(struct sCommandKeyOne));
		for(k=0;k<len;k++) {
			uint8_t flag_tmp = tmp.flag;
			if((tmp.key == k) && TBIT1(keys[k],flag_tmp) && tmp.fun != NULL) {
				tmp.fun();
				return !0;
			}
		}
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------
uint8_t CommandKeyPROGMEM(const struct sCommandKey *key, uint8_t *keys, int8_t len)
{
	if((key == NULL) || (keys == NULL))
		return 0;

	struct sCommandKey tmp;
	memcpy_P((void*)&tmp,(PGM_VOID_P)key,sizeof(struct sCommandKey));
	return CommandKeyOnePROGMEM(tmp.key,tmp.size,keys,len);
}
//---------------------------------------------------------------------------------------------------
