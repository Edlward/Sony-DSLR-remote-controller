#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#define F_CPU 1.0E6  
#include <util/delay.h>
#define ADC_VREF 0x45  

int main(void){
	
	//　1MHzで動く想定
	
	//DDRB = 0xff; // all output
	DDRB  = 0b00000010; // led以外in
	PORTB = 0b00111101; // pull up
	
	TCCR0A = 0b00100001;
	
	// TCCR0B = 0b00001001; // 位相基準PWM start
	TCCR0B = 0b00001000; // PWM stop
	
	OCR0A = 13; // MAX。周波数決める。
	OCR0B = 6; // この値あたりでH/Lが切り替わるのでdutyアレする
	
	const unsigned char shutter[] = {1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1}; // 20 data: shutter btn
	//unsigned char two_sec[] = {1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1}; // 2 sec btn
	//unsigned char* signal;
	//signal = shutter;
	unsigned short i, num, interval;
	
	for(;;){
		
		// 2sec/shutter 切り替え機能はやめる
		/*
		if (PINB & 0x10){
			signal = shutter;
		}else{
			signal = two_sec;
		}
		*/
		
		for (num = 0; num < 5; num++){
			// start bit
			TCCR0B |= 0x01; // high
			_delay_us(2500);
			TCCR0B = 0b00001000; // low
			_delay_us(500);
			
			for (i = 0; i < 20; i++){
				
				if (shutter[i]){
					// if 1
					TCCR0B |= 0x01; // high
					_delay_us(1300);
				}else{
					// if 0
					TCCR0B |= 0x01; // high
					_delay_us(700); 
				}
				
				TCCR0B = 0b00001000;
				PORTB &= ~0x02;
				_delay_us(500);
			}
				
			// 1回の送信終了
			_delay_ms(11.25);
			
		}
		
		// インターバルを取得してそれまで待つかんじ
		interval = 0;
		
		if (!(PINB & 0x01)){
			interval += 1;
		}
		if (!(PINB & 0x04)){
			interval += 2;
		}
		if (!(PINB & 0x08)){
			interval += 4;
		}
		if (!(PINB & 0x10)){
			interval = interval << 2;
		}
		
		
		for (;interval;interval--){
			_delay_ms(1000);
		}
			
	}
	
	return 0;
}
