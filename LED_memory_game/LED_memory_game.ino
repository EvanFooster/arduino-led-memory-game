
#define ledPin1 2
#define ledPin2 3
#define ledPin3 4

const int MIN_VAL = 1;
const int MAX_VAL = 3;
const int MAX_SEQUENCE = 3;

int sequence[MAX_SEQUENCE];

int seqIndex = 0;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}


void loop() {
  addSeq();
  flashSeq();

  //Take user input and check if user failed
  if(!takeInput()) {
    lose();
    return;
  }
  //Check if player has reached MAX_SEQUENCE
  if(seqIndex == MAX_SEQUENCE) {
    win();
    return;
  }
  //Wait briefly to update sequence and flash again
  delay(2000);
}


//add a random number to sequence
void addSeq() {
  if (seqIndex < MAX_SEQUENCE) {
    sequence[seqIndex] = random(MIN_VAL, MAX_VAL + 1);
    seqIndex++;
  }
}

//Flash sequence to user
void flashSeq() {
  for(int i = 0; i < seqIndex; i++) {
    int led;
    if(sequence[i] == 1){
      led = ledPin1;
    } else if(sequence[i] == 2) {
      led = ledPin2;
    } else {
      led = ledPin3;
    }
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
}

//Take user input
bool takeInput() {
  for(int i = 0; i < seqIndex; i++){
    while(Serial.available() == 0){}
    int input = Serial.readStringUntil('\n').toInt();
    //Return false if user fails
    if(input != sequence[i]) {
      return false;
    }
  }
  return true;
}

//Print win message and reset if user completes length of MAX_SEQUENCE
void win() {
  Serial.println("Congratulations, you win!");
  seqIndex = 0;
  delay(2000);
}

//Print lose message and reset if user gives wrong sequence
void lose() {
  seqIndex = 0;
  Serial.println("Incorrect!");
  delay(2000);
}



