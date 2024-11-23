#Проект DS18B20- Реализация измерения температуры с помощью датчика Dallas DS18B20

Версия 1-Wire брутфорс, просто дергаем ногой в цикле
Используется библиотека ds18b20.h

### 1) подключим printf
 В main.c вверху объявляем макрос : #define PUTCHAR_PROTOTYPE int __io_putchar(int ch);

Где нибудь внизу вставляем \code 
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
\endcode
Для отправки данных в терминал (использовал Putty) используется UART2

##2) In CubeMX активизируем UART2 
Используется UART2 bod 115200 для вывода printf  в  терминал
Connect to BluePill stm32F103C8T6 
Конвертер TTL -> USB подключется следующим образом: 
RX->A2
GND->GND
TX->A3

###3) датчик подключаем к пину B11
  /* USER CODE BEGIN 2 */
	port_init(); // инициализируем порт через который будем работать
	status = ds18b20_init(SKIP_ROM); // инициализируем датчик
	//sprintf(str1,"Init Status: %d\r\n",status);
	printf("Init Status: %d\r\n",status); // выведем статус инициализации (0 - успешно)
	//HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
  /* USER CODE END 2 */

###4) В main() объявляем 
/* USER CODE BEGIN 1 */
	uint8_t status;
		uint8_t dt[8];
		uint16_t raw_temper;
		float temper;
		char c;
  /* USER CODE END 1 */  


  