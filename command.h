#ifndef COMMAND_H
#define COMMAND_H
//---------------------------------------------------------------------------------------------------
struct sCommandComOne
{
	const char* name;
	void (*fun)(int8_t,char* (*)(int8_t) );
} ;
//---------------------------------------------------------------------------------------------------
struct sCommandCom
{
	const struct sCommandComOne *com;
	const uint8_t size;
};
//---------------------------------------------------------------------------------------------------
struct sCommandKeyOne
{
	const uint8_t key ;
	const uint8_t flag;
	void (*fun)(void);
};
//---------------------------------------------------------------------------------------------------
struct sCommandKey
{
	const struct sCommandKeyOne *key;
	const uint8_t size;
};
//---------------------------------------------------------------------------------------------------
void Test(int8_t x,char * (*arg)(int8_t));
void Echo(int8_t x,char * (*arg)(int8_t));
//---------------------------------------------------------------------------------------------------
const struct sCommandCom CommandComTest;
//const struct sCommandKey CommandKeyTest;
//---------------------------------------------------------------------------------------------------
uint8_t CommandComOnePROGMEM(const struct sCommandComOne *fun,uint8_t size,
							char *buff,int8_t len);
//---------------------------------------------------------------------------------------------------
uint8_t CommandComPROGMEM(	const struct sCommandCom *fun,
							char *buff,int8_t len);
//---------------------------------------------------------------------------------------------------
uint8_t CommandKeyOnePROGMEM(	const struct sCommandKeyOne *key,uint8_t size,
							uint8_t *keys,int8_t len);
//---------------------------------------------------------------------------------------------------
uint8_t CommandKeyPROGMEM(		const struct sCommandKey *key,
							uint8_t *keys,int8_t len);
//---------------------------------------------------------------------------------------------------
#endif
