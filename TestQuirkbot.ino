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
#define BP1	6	// Back Pack 1		(PD7)	ADC10	PWM (HS)	Uppmost left
#define BP2	12	// Back Pack 2		(PD6)	ADC9
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
	digitalWrite(BP4, (s0 == 1) ? HIGH : LOW);
	digitalWrite(BP5, (s1 == 1) ? HIGH : LOW);
	digitalWrite(BP6, (s2 == 1) ? HIGH : LOW);
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
void setup() {
	// Turn off mouth
	PORTD &= ~(1<<5);
	PORTB &= ~(1<<0);

	// Setup multiplex control pins
	pinMode(BP4, OUTPUT);
  	pinMode(BP5, OUTPUT);
  	pinMode(BP6, OUTPUT);

	// Test and display result
	if(test()) success();
	else fail();
}
bool test(){
	if(!testFrontPadsOutput()) return false;
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

	digitalWrite(pad, HIGH);
	delay(1);
	if(!digitalRead(bp)) return false;
	
	digitalWrite(pad, LOW);
	delay(1);
	if(digitalRead(bp)) return false;
	
	digitalWrite(pad, HIGH);
	delay(1);
	if(!digitalRead(bp)) return false;
	
	digitalWrite(pad, LOW);
	delay(1);
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
	delay(3);
	pinMode(pad, INPUT);
	delay(3);
	if(digitalRead(pad)) return false;

	// test for high
	mux(7);	

	pinMode(pad, OUTPUT);
	digitalWrite(pad, LOW);
	delay(3);
	pinMode(pad, INPUT);
	delay(3);
	if(!digitalRead(pad)) return false;

	return true;
}
// Input test ------------------------------------------------------------------
bool testBackPadsInput(){
	pinMode(BP1, OUTPUT);
	pinMode(BP2, OUTPUT);

	if(!testSinglePadInput(HB,  5, BP1)) return false;
	if(!testSinglePadInput(LAB, 6, BP1)) return false;
	if(!testSinglePadInput(LLB, 7, BP1)) return false;
	if(!testSinglePadInput(RLB, 5, BP2)) return false;
	if(!testSinglePadInput(RAB, 6, BP2)) return false;

	return true;
}
bool testSinglePadInput(int pad, int route, int source){
	mux(route);

	digitalWrite(source, LOW);
	delay(3);

	if(analogRead(pad) != 0) return false;	

	digitalWrite(source, HIGH);	
	delay(3);

	if(analogRead(pad) > 600 || analogRead(pad) < 400) return false;	
	
	return true;
}
// Result feedback -------------------------------------------------------------
void success(){
	allLedsOn();
}
void fail(){
	while(true){
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
void loop() {}