#include "ds18b20.h"


//--------------------------------------------------

__STATIC_INLINE void DelayMicro(__IO uint32_t micros) {
micros *= (SystemCoreClock / 1000000) / 9;
/* Wait till done */
while (micros--) ;
}
//--------------------------------------------------

void port_init(void) {
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);
  GPIOB->CRH |= GPIO_CRH_MODE11;
  GPIOB->CRH |= GPIO_CRH_CNF11_0;
  GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
}
//--------------------------------------------------

uint8_t ds18b20_Reset(void) {
  uint16_t status;

  GPIOB->ODR &= ~GPIO_ODR_ODR11;
  DelayMicro(1000);

  GPIOB->ODR |= GPIO_ODR_ODR11;
  DelayMicro(120);

  status = GPIOB->IDR & GPIO_IDR_IDR11;
  DelayMicro(1000);

  return (status ? 1 : 0);
}
//----------------------------------------------------------

uint8_t ds18b20_ReadBit(void) {
  uint8_t bit = 0;

  GPIOB->ODR &= ~GPIO_ODR_ODR11;
  DelayMicro(2);

  GPIOB->ODR |= GPIO_ODR_ODR11;
  DelayMicro(13);

  bit = (GPIOB->IDR & GPIO_IDR_IDR11 ? 1 : 0);
  DelayMicro(45);

  return bit;
}
//-----------------------------------------------

uint8_t ds18b20_ReadByte(void) {
  uint8_t data = 0;

  for (uint8_t i = 0; i <= 7; i++)
  data += ds18b20_ReadBit() << i;

  return data;
}
//-----------------------------------------------

void ds18b20_WriteBit(uint8_t bit) {
  GPIOB->ODR &= ~GPIO_ODR_ODR11;
  DelayMicro(bit ? 3 : 65);
  GPIOB->ODR |= GPIO_ODR_ODR11;
  DelayMicro(bit ? 65 : 3);
}
//-----------------------------------------------

void ds18b20_WriteByte(uint8_t dt) {
  for (uint8_t i = 0; i < 8; i++)
  {
    ds18b20_WriteBit(dt >> i & 1);
    //Delay Protection
    DelayMicro(5);
  }
}
//-----------------------------------------------

uint8_t ds18b20_init(uint8_t mode) {

	if(ds18b20_Reset()) return 1;
	if(mode==SKIP_ROM) {
		//SKIP ROM
		ds18b20_WriteByte(0xCC);
		//WRITE SCRATCHPAD
		ds18b20_WriteByte(0x4E);
		//TH REGISTER 100 ��������
		ds18b20_WriteByte(0x64);
		//TL REGISTER - 30 ��������
		ds18b20_WriteByte(0x9E);
		//Resolution 12 bit
		ds18b20_WriteByte(RESOLUTION_12BIT);
	}
  return 0;
}
//----------------------------------------------------------

void ds18b20_MeasureTemperCmd(uint8_t mode, uint8_t DevNum) {
  ds18b20_Reset();

  if(mode==SKIP_ROM) {
	  //SKIP ROM
	  ds18b20_WriteByte(0xCC);
  }
  	  //CONVERT T
  ds18b20_WriteByte(0x44);
}
//----------------------------------------------------------

void ds18b20_ReadStratcpad(uint8_t mode, uint8_t *Data, uint8_t DevNum) {
  uint8_t i;
  ds18b20_Reset();
  if(mode==SKIP_ROM) {
    //SKIP ROM
    ds18b20_WriteByte(0xCC);
  }

  //READ SCRATCHPAD
  ds18b20_WriteByte(0xBE);

  for(i=0;i<8;i++) Data[i] = ds18b20_ReadByte();
}
//---------------------------------------------------------

uint8_t ds18b20_GetSign(uint16_t dt) {

  if (dt&(1<<11)) return 1;
  else return 0;
}
//----------------------------------------------------------

float ds18b20_Convert(uint16_t dt) {
  float t;
  t = (float) ((dt&0x07FF)>>4);
  t += (float)(dt&0x000F) / 16.0f;
  return t;
}
//----------------------------------------------------------
