#include "Quirkbot.h"


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
  allLedsOn();
  delay(2000);
}
void fail(){
  int i = 0;
  while(i < 10){
    allLedsOn();
    delay(200);
    allLedsOff();
    delay(200);
    i++;
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

Wave wave1;
Wave wave2;
Led led1;
Led led2;

void start(){

    /** GENERATED UUID **/

   // Setup multiplex control pins
  pinMode(BP4, OUTPUT);
  pinMode(BP5, OUTPUT);
  pinMode(BP6, OUTPUT);

  // Test and display result
  if(!test()) fail();

  // If we got here it means the test was ok
  wave1.type = WAVE_SINE;

  wave2.type = WAVE_SINE;
  wave2.offset = 0.5;

  led1.light.connect(wave1.out);
  led1.place = LE;

  led2.light.connect(wave1.out);
  led2.place = RE;

}
