## TimerInterrupt

### Цели проекта:
1) Реализация printf(), для отладочного вывода и контроля возвращаемых датчиками значений
2) Настройка UART. Изучение возможностей и способов настройки
3) Работа с прерываниями. Прерывание по таймеру - Зажигаем светодиод и выводит надпись LED toggle from timer3
4) Работа с прерываниями. Подключение внешнего прерывания по кнопке. Жмакаем на кнопку и светодиод попеременно включается и выключается.

### Реализация: 
1) В main.c вверху объявляем макрос:`#define PUTCHAR_PROTOTYPE int __io_putchar(int ch);`

2) Где нибудь внизу вставляем:
``` 
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
```
> Для отправки данных в терминал (использовал Putty) используется UART2

3) In `CubeMX` активизируем `UART2`   
Используется `UART2 bod 115200` для вывода `printf`  в  терминал

#### Connect to BluePill stm32F103C8T6

> Конвертер TTL -> USB подключется следующим образом: 
+ RX->A2
+ GND->GND
+ TX->A3

4) Настраиваем таймер - 
`TIMER3 - Counter setting - Prescaler(PSC16bit value) 31999`

+ `counter period - 500` -  Количество тиков после которого таймер переполняется и начинает считать заново
+ `NVIC setting - TIM3 global interrupt`  галочку ставим, активируем прерывания

В конце main.c добавим обработчик прерываний по таймеру - 
```
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3)
{	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
	  printf("LED toggle from timer3\r\n");
}
```
> Настраиваем пин которым будем моргать, тут `GPIO_PIN_10`

4)  Настраиваем внешнее прерывание по кнопке
 + В CubeMX  в разделе `GPIO` настраиваем `PIN_1`, подтягиваем его к единице/
 + Вешаем на него внешнее прерывание по  по спаду(фронту) `External interrupt mode with Falling Edge trigger detection` (прерывание по заднему фронту на `PIN_1` )
+ `NVIC setting` - разрешаем прерывание `EXTI line1 interrupt` - ставим галку  
+ В конце main.c добавим обработчик внешних прерываний
```
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_1 && isSet == 0){
		isSet = 1;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
		printf("led on\r\n");
	}
	else if (GPIO_Pin == GPIO_PIN_1 && isSet == 1){
		isSet = 0;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
		printf("led off\r\n");
	}
}
```

```
Среда разработки:
STM32CubeIDE
Version: 1.15.1
Build: 21094_20240412_1041 (UTC)
OS: Windows 10, v.10.0, x86_64 / win32
Java vendor: Eclipse Adoptium
Java runtime version: 17.0.8.1+1
Java version: 17.0.8.1

STM32CubeMX - STM32 Device Configuration Tool
Version: 6.11.1-RC2
Build: 20240411-0753 (UTC)
```
