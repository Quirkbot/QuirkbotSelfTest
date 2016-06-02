#include "Quirkbot.h"
#include <avr/wdt.h>

// Defines ---------------------------------------------------------------------
#define LE	8	// Left Eye		 	(PB4)	ADC11
#define RE	A5	// Right Eye		(PF0)	ADC0
#define LLF	9	// Left Leg Front	(PB5)	ADC12	PWM (16BIT)
#define RLF	11	// Right Leg Front	(PB7)			PWM (8/16BIT)
#define RAF	5	// Right Arm Front	(PC6)			PWM (HS)
#define HF	13	// Horn Front		(PC7)			PWM (10BIT)
#define LAF	10	// Left Arm Front	(PB6)	ADC13	PWM (16BIT)
#define LLB	A0	// Left Leg Back	(PF7)	ADC7
#define RLB	A4	// Right Leg Back	(PF1)	ADC1
#define RAB	A3	// Right Arm Back	(PF4)	ADC4
#define HB	A2	// Horn Back		(PF5)	ADC5
#define LAB	A1	// Left Arm Back	(PF6)	ADC6
#define BP1	A7	// Back Pack 1		(PD7)	ADC10	PWM (HS)	// or digital 6
#define BP2	A11	// Back Pack 2		(PD6)	ADC9                    // or digital 12
#define BP3	0	// Back Pack 3		(PD2)	RXD1
#define BP4	2	// Back Pack 4		(PD1)	SDA
#define BP5	3	// Back Pack 5		(PD0)	SCL
#define BP6	1 	// Back Pack 6		(PD3)	TXD1
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
	    case 0:	_mux(MUX_0); break;
	    case 1:	_mux(MUX_1); break;
	    case 2:	_mux(MUX_2); break;
	    case 3:	_mux(MUX_3); break;
	    case 4:	_mux(MUX_4); break;
	    case 5:	_mux(MUX_5); break;
	    case 6:	_mux(MUX_6); break;
	    case 7:	_mux(MUX_7); break;
	}
}
// Entry -----------------------------------------------------------------------
bool test(){
	wdt_reset();
	if(!testFrontPadsOutput())return false;
	wdt_reset();
//        Serial.println("A");
	if(!testBackPadsOutput()) return false;
	wdt_reset();
//        Serial.println("B");
	if(!testFrontPadsMakey()) return false;
	wdt_reset();
//        Serial.println("C");
	if(!testBackPadsInput()) return false;
	wdt_reset();
//        Serial.println("D");

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
void fail(){
	while(true){
		wdt_reset();
		allLedsOn();
		delay(200);
		allLedsOff();
		delay(200);
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
void allLedsOff(){
	pinMode(LE, OUTPUT);
	pinMode(RE, OUTPUT);
	digitalWrite(LE, LOW);
	digitalWrite(RE, LOW);
	PORTD &= ~(1<<5);
	PORTB &= ~(1<<0);
}

CircuitTouch horn;
Converter amplify1;
Converter amplify2;
Converter converter1;
Converter converter2;
Converter converter3;
LightSensor lightSensor1;
Wave wave1;
Wave wave2;
DualColorLed leftArm;
DualColorLed rightArm;
DualColorLed rightLeg;
Led leftEye;
Led leftMouth;
Led rightEye;
Led rightMouth;
ServoMotor servo1;
ServoMotor servo2

void setup(){
	// Setup multiplex control pins
	pinMode(BP4, OUTPUT);
	pinMode(BP5, OUTPUT);
	pinMode(BP6, OUTPUT);

	// Test and if ok, turn all LEDs on and stop in a infinite loop
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

	converter3.in.connect(lightSensor1.out);
	converter3.inMin = 0.98;
	converter3.inMax = 1;
	converter3.outMin = 0;
	converter3.outMax = 1;

	lightSensor1.place = LL;
	lightSensor1.min = 1;
	lightSensor1.max = 0;

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

	leftArm.light = 1;
	leftArm.color.connect(amplify1.out);
	leftArm.place = LA;

	rightArm.light = 1;
	rightArm.color.connect(amplify2.out);
	rightArm.place = RA;

	rightLeg.light.connect(converter3.out);
	rightLeg.color.connect(amplify2.out);
	rightLeg.place = RL;

	leftEye.light.connect(amplify1.out);
	leftEye.place = LE;

	leftMouth.light.connect(horn.out);
	leftMouth.place = LM;

	rightEye.light.connect(amplify2.out);
	rightEye.place = RE;

	rightMouth.light.connect(horn.out);
	rightMouth.place = RM;

	servo1.position.connect(wave1.out);
	servo1.place = SERVO_BP1;

	servo2.position.connect(wave2.out);
	servo2.place = SERVO_BP2;

}

void loop(){

}
