class MemoryGame {
private:
  //Pin config
  const int led1;
  const int led2;
  const int led3;

  //Game config
  static const int MIN_VAL = 1;
  static const int MAX_VAL = 3;
  static const int MAX_SEQUENCE = 50; //Max sequence length

  //State
  int selectedSequence = 5; //Default sequence length
  bool started = false;
  int sequence[MAX_SEQUENCE];
  int seqIndex = 0;

public:
  MemoryGame(int p1, int p2, int p3) : led1(p1), led2(p2), led3(p3) {}

  void begin() {
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);

    Serial.begin(9600);
    randomSeed(analogRead(A0));
  }

  void update() {
    if(!started) {  //Prompt player for sequence length before starting a game
      start();      
      if(!started) return; //invalid input - try again next loop
    }

    addSeq(); //add to sequence and flash the entire sequence
    flashSeq();

    if(!takeInput()) {  //Take user input and check if user failed
      lose();
      return;
    }

    if(seqIndex == selectedSequence) {  //Check if player has reached their selected sequence length 
      win();
      return;
    }

    printCurrSequence();
    delay(2000);  //Wait briefly to update sequence and flash again
  }

private:
  void start()  {
    Serial.print("Enter your desired sequence length (1-");
    Serial.print(MAX_SEQUENCE);
    Serial.println("):");
    while(Serial.available() == 0) {}
    String input = Serial.readStringUntil('\n');
    input.trim();
    int length = input.toInt();
    if(length < 1 || length > MAX_SEQUENCE) {
      Serial.println("Invalid input");
      return;
    }
    selectedSequence = length;
    while(Serial.available() > 0) Serial.read();  //Prevent any newline from being read as input
    started = true;
  }

  void addSeq() {
    if (seqIndex < selectedSequence) {
      sequence[seqIndex] = random(MIN_VAL, MAX_VAL + 1);
      seqIndex++;
    }
  }

  void flashSeq() {
    for(int i = 0; i < seqIndex; i++) {
      int led;
      if(sequence[i] == 1){
        led = led1;
      } else if(sequence[i] == 2) {
        led = led2;
      } else {
        led = led3;
      }
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      delay(500);
    }
  }

  void printCurrSequence()  {
    Serial.print("Your current sequence: ");
    Serial.print(seqIndex);
    Serial.print("/");
    Serial.println(selectedSequence);
  }

  bool takeInput() {
    for(int i = 0; i < seqIndex; i++){
      while(Serial.available() == 0){}
      String input = Serial.readStringUntil('\n');
      input.trim();
      int val = input.toInt();
      
      if(val != sequence[i]) {  //Return false if user fails
        return false;
      }
    }
    return true;
  }

  void win() {
    seqIndex = 0;
    started = false;
    Serial.println("Congratulations, you win!");
    winSequence();
  }

  void lose() {
    seqIndex = 0;
    started = false;
    Serial.println("Incorrect!");
    loseSequence();
  }

  void winSequence() {
    for(int i = 0; i < 5; i++) {
      digitalWrite(led1, HIGH);
      delay(50);
      digitalWrite(led2, HIGH);
      delay(50);
      digitalWrite(led3, HIGH);
      delay(200);
      digitalWrite(led1, LOW);
      delay(50);
      digitalWrite(led2, LOW);
      delay(50);
      digitalWrite(led3, LOW);
      delay(200);
    }
  }

  void loseSequence() {
    for(int i = 0; i < 3; i++) {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      delay(200);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      delay(200);
    }
  }
};


#define ledPin1 2 //Uno GPIO pins 2, 3, 4
#define ledPin2 3
#define ledPin3 4

MemoryGame game(ledPin1, ledPin2, ledPin3); //Create a MemoryGame object

void setup() {
  game.begin();
}

void loop() {
  game.update();
}
