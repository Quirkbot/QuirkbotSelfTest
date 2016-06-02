#include "Quirkbot.h"
#include <avr/wdt.h>

// Defines ---------------------------------------------------------------------
#define PULL_UP_PIN 4

// Multiplex -------------------------------------------------------------------
#define MUX_0 0,0,0 // 0 + 0 + 0 = 0
#define MUX_1 1,0,0 // 1 + 0 + 0 = 1
#define MUX_2 0,1,0 // 0 + 2 + 0 = 2
#define MUX_3 1,1,0 // 1 + 2 + 0 = 3
#define MUX_4 0,0,1 // 0 + 0 + 4 = 4
#define MUX_5 1,0,1 // 1 + 0 + 4 = 5
#define MUX_6 0,1,1 // 0 + 2 + 4 = 6
#define MUX_7 1,1,1 // 1 + 1 + 1 = 7

void _mux(int s0, int s1, int s2){
	digitalWrite(BP3, (s0 == 1) ? HIGH : LOW);
	digitalWrite(BP4, (s1 == 1) ? HIGH : LOW);
	digitalWrite(BP5, (s2 == 1) ? HIGH : LOW);
}
void mux(int pin){
	switch (pin) {
		case 0: _mux(MUX_0); break;
		case 1: _mux(MUX_1); break;
		case 2: _mux(MUX_2); break;
		case 3: _mux(MUX_3); break;
		case 4: _mux(MUX_4); break;
		case 5: _mux(MUX_5); break;
		case 6: _mux(MUX_6); break;
		case 7: _mux(MUX_7); break;
	}
}
// Entry -----------------------------------------------------------------------
bool test(){
	if(!testFrontPadsOutput())return false;
	if(!testBackPadsOutput()) return false;
	if(!testFrontPadsMakey()) return false;
	if(!testBackPadsInput()) return false;

	return true;
}
// Output test -----------------------------------------------------------------
bool testFrontPadsOutput(){
	pinMode(BP1, INPUT);
	if(!testSinglePadOutput(HF,  0, BP1)) return false;
	if(!testSinglePadOutput(LAF, 1, BP1)) return false;
	if(!testSinglePadOutput(LLF, 2, BP1)) return false;
	if(!testSinglePadOutput(RLF, 3, BP1)) return false;
	if(!testSinglePadOutput(RAF, 4, BP1)) return false;
	return true;
}
bool testBackPadsOutput(){
	pinMode(BP2, INPUT);
	if(!testSinglePadOutput(HB,  0, BP2)) return false;
	if(!testSinglePadOutput(LAB, 1, BP2)) return false;
	if(!testSinglePadOutput(LLB, 2, BP2)) return false;
	if(!testSinglePadOutput(RLB, 3, BP2)) return false;
	if(!testSinglePadOutput(RAB, 4, BP2)) return false;
	return true;
}
bool testSinglePadOutput(int pad, int route, int bp){
	mux(route);
	pinMode(pad, OUTPUT);
	digitalWrite(pad, HIGH);
	delay(5);
	if(!digitalRead(bp)) return false;


	digitalWrite(pad, LOW);
	delay(5);
	if(digitalRead(bp)) return false;

	digitalWrite(pad, HIGH);
	delay(5);
	if(!digitalRead(bp)) return false;

	digitalWrite(pad, LOW);
	delay(5);
	if(digitalRead(bp)) return false;

	return true;
}
// Makey test ------------------------------------------------------------------
bool testFrontPadsMakey(){
	pinMode(BP1, OUTPUT);
	digitalWrite(BP1, LOW);
	pinMode(PULL_UP_PIN, OUTPUT);
	digitalWrite(PULL_UP_PIN, HIGH);

	if(!testSinglePadMakey(HF,  0, BP1)) return false;
	if(!testSinglePadMakey(LAF, 1, BP1)) return false;
	if(!testSinglePadMakey(LLF, 2, BP1)) return false;
	if(!testSinglePadMakey(RLF, 3, BP1)) return false;
	if(!testSinglePadMakey(RAF, 4, BP1)) return false;

	return true;
}
bool testSinglePadMakey(int pad, int route, int bp){
	// test for low
	mux(route);

	pinMode(pad, OUTPUT);
	digitalWrite(pad, LOW);
	delay(5);
	pinMode(pad, INPUT);
	delay(5);
	if(digitalRead(pad)) return false;

	// test for high
	mux(7);

	pinMode(pad, OUTPUT);
	digitalWrite(pad, LOW);
	delay(5);
	pinMode(pad, INPUT);
	delay(20);
	if(!digitalRead(pad)) return false;


	return true;
}
// Input test ------------------------------------------------------------------
bool testBackPadsInput(){
	pinMode(BP1, OUTPUT);
	pinMode(BP2, OUTPUT);

	if(!testSinglePadInput(HB,  5, BP1)) return false;
	delay(5);
	if(!testSinglePadInput(LAB, 6, BP1)) return false;
	delay(5);
	if(!testSinglePadInput(LLB, 7, BP1)) return false;
	delay(5);
	if(!testSinglePadInput(RLB, 5, BP2)) return false;
	delay(5);
	if(!testSinglePadInput(RAB, 6, BP2)) return false;

	return true;
}
bool testSinglePadInput(int pad, int route, int source){
	mux(route);
	pinMode(pad, INPUT);
	pinMode(source, OUTPUT);
	digitalWrite(source, LOW);
	delay(5);

	if(analogRead(pad) != 0) return false;

	digitalWrite(source, HIGH);
	delay(5);

	if(analogRead(pad) > 600 || analogRead(pad) < 400) return false;

	return true;
}
// Result feedback -------------------------------------------------------------
void success(){
	while(true){
		wdt_reset();
		allLedsOn();
	}
}

void allLedsOn(){
	pinMode(LE, OUTPUT);
	pinMode(RE, OUTPUT);
	digitalWrite(LE, HIGH);
	digitalWrite(RE, HIGH);
	PORTD |= (1<<5);
	PORTB |= (1<<0);
}



CircuitTouch horn;
Converter amplify1;
Converter amplify2;
Converter converter1;
Converter converter2;
Wave wave1;
Wave wave2;
Led leftEye;
Led rightEye;
ServoMotor servo1;
ServoMotor servo2;


void setup(){
	// Setup multiplex control pins
	pinMode(BP4, OUTPUT);
	pinMode(BP5, OUTPUT);
	pinMode(BP6, OUTPUT);

	// Test and if ok, turn all LEDs on and stop in a infinite loop
	wdt_reset();
	if(test()){
		success();
	}

	/** GENERATED UUID **/
	// If we got here it means the test was ok

	// Program
	horn.place = H;
	horn.sensitivity = 0;
	horn.min = 0;
	horn.max = 1;

	amplify1.in.connect(wave1.out);
	amplify1.inMin = 0.25;
	amplify1.inMax = 0.75;
	amplify1.outMin = 0;
	amplify1.outMax = 1;

	amplify2.in.connect(wave2.out);
	amplify2.inMin = 0.25;
	amplify2.inMax = 0.75;
	amplify2.outMin = 0;
	amplify2.outMax = 1;

	converter1.in.connect(horn.out);
	converter1.inMin = 0;
	converter1.inMax = 1;
	converter1.outMin = 0.8;
	converter1.outMax = 0.4;

	converter2.in.connect(horn.out);
	converter2.inMin = 0;
	converter2.inMax = 1;
	converter2.outMin = 0.4;
	converter2.outMax = 0.8;

	wave1.length.connect(converter1.out);
	wave1.type = WAVE_SINE;
	wave1.min = 0.25;
	wave1.max = 0.75;
	wave1.offset = 0;

	wave2.length.connect(converter2.out);
	wave2.type = WAVE_SINE;
	wave2.min = 0.25;
	wave2.max = 0.75;
	wave2.offset = 0;

	leftEye.light.connect(amplify1.out);
	leftEye.place = LE;

	rightEye.light.connect(amplify2.out);
	rightEye.place = RE;

	servo1.position.connect(wave1.out);
	servo1.place = SERVO_BP1;

	servo2.position.connect(wave2.out);
	servo2.place = SERVO_BP2;

	pinMode(LAF, OUTPUT);
	pinMode(LAB, OUTPUT);
	pinMode(RAF, OUTPUT);
	pinMode(RAB, OUTPUT);
	pinMode(LLF, OUTPUT);
	pinMode(LLB, OUTPUT);
	pinMode(RLF, OUTPUT);
	pinMode(RLB, OUTPUT);
}

void loop(){
	if(horn.out.get() > 0.5){
		PORTD |= (1<<5);
		PORTB |= (1<<0);

		digitalWrite(LAF, LOW);
		digitalWrite(LAB, HIGH);

		digitalWrite(RAF, LOW);
		digitalWrite(RAB, HIGH);

		digitalWrite(LLF, LOW);
		digitalWrite(LLB, HIGH);

		digitalWrite(RLF, LOW);
		digitalWrite(RLB, HIGH);
	}
	else{
		PORTD &= ~(1<<5);
		PORTB &= ~(1<<0);

		digitalWrite(LAF, HIGH);
		digitalWrite(LAB, LOW);

		digitalWrite(RAF, HIGH);
		digitalWrite(RAB, LOW);

		digitalWrite(LLF, HIGH);
		digitalWrite(LLB, LOW);

		digitalWrite(RLF, HIGH);
		digitalWrite(RLB, LOW);
	}

}
