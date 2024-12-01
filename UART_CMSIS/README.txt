//**********		Project UART_CMSIS		**************//
// Using stm32F103C8T6

The project demonstrates echo using UART.  
The implementation of the logic is embedded in the main loop.
This approach is not optimal, and is highly discouraged for non-educational devices.
A USB на TTL UART converter is used to connect to the COM port.
//*****************   CONNECTION *******************/////
	stm32F103C8T6			USB на TTL UART CH340
	RX->A10						TX
	TX->A9						RX
	GND							GND
	
//*********************************************************
Конфигурирование портов ввода вывода и UART не используется библиотека HAL, 
и происходит через прямое обращение к регистрам.