const int pirPin = 2;
const int buttonPin = 3;
int led1 = 13;
int led2 = 11;
int led3 = 10;
uint8_t pirState, buttonState;
uint8_t prepir, prebutton = 0;
const uint16_t timer = 0;
const uint16_t compare = 31250;
void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(13, OUTPUT);
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  TCCR1A = 0;
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  TCNT1 = timer;
  OCR1A = compare;
  TIMSK1 = (1 << OCIE1A);
  sei();
}
void loop() {
  if (prepir != pirState) {
    digitalWrite(led1, pirState);
    Serial.println("PIR sensor detected motion");
    prepir = pirState;
  }
  if (prebutton != buttonState) {
    if (digitalRead(led2) == LOW) {
      digitalWrite(led2, HIGH);
    } else {
      digitalWrite(led2, LOW);
    }

    Serial.println("Button pressed");
    prebutton = buttonState;
  }
  PORTB ^= (1 << led3);
}
ISR(PCINT2_vect) {
  pirState = PIND & (1 << PD2);
  buttonState = PIND & (1 << PD3);
}
ISR(TIMER1_COMPA_vect) {
  TCNT1 = timer;
  digitalWrite(13, digitalRead(13) ^ 1);
}