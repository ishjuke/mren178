#include <LiquidCrystal.h>

#include <DS3231.h>

#define LCD_WIDTH   16
#define LCD_HEIGHT  2
#define LCD_SIZE    (LCD_WIDTH * LCD_HEIGHT)

#define PINRS       8
#define PINEN       9
#define PIND4       4 
#define PIND5       5 
#define PIND6       6 
#define PIND7       7  
#define BUTTONPIN   A0
#define ELEVATORSIZE 10

LiquidCrystal lcd(PINRS, PINEN, PIND4, PIND5, PIND6, PIND7);

enum FSMState {IDLE, MOVING_UP, MOVING_DOWN, DOOR_OPEN, DOOR_CLOSE, EMERGENCY_STOP};
enum Direction {DOWN = 0, UP = 1};

struct Elevator {
  FSMState currentState;
  Direction currentDir;
  int currentFloor;
} elevator;

// Initialize Array based queue size for requests counters keep track of number of requests in either up or down direction
int upQueue[ELEVATORSIZE];
int upCount = 0;

int downQueue[ELEVATORSIZE];
int downCount = 0; 

void insertUpRequest(int floorNum){
  upQueue[upCount] = floorNum;

  upCount++;
}

int InsertDownRequest(int floorNum){

  downCount++;
}

int popUpRequest(){
  if (upCount == 0) return;

  upCount--;
}

int popDownRequest(){
  if (downCount == 0) return;

  downCount--;
}

void insertionSort(int arr[], size_t n, Direction d) {
  if(d == UP){
    for (size_t i = 1; i < n; i++) { //going up 
      int key = arr[i]; // The element to be inserted
      int j = i - 1; 

      // Move elements of arr[0...i-1], that are greater than key, 
      // to one position ahead of their current position
      while (j >= 0 && arr[j] > key) {
        arr[j + 1] = arr[j];
        j--;
      }
      arr[j + 1] = key; // Insert the key in its correct position
    }
  }
  if (d == DOWN){ // going down
    for (size_t i = n-1; i > 0; i--){
      int key = arr[i]; 
      int j = i + 1; 

      while(j <= n && arr[j] > key ) {
        arr[j - 1] = arr [j]; 
        j++; 

      }
      arr[j-1] = key; 
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);

  elevator.currentState = IDLE;
  elevator.currentFloor = 1;

  Serial.println("Elevator FSM initialized");
  lcd.print("Elevator Ready");

}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0){
    String inputCMD = Serial.readStringUntil('\n'); // reads the input string until \n is read
    inputCMD.trim(); // cuts the \r and \n from the string

    int spaceIndex = inputCMD.indexOf(' ');

    if (spaceIndex != -1){ // Ensures a space exist in the input
      int floorNum = inputCMD.substring(0, spaceIndex).toInt(); // reads the left characters in the string and translates them into int

      String direction = inputCMD.substring(spaceIndex + 1); // reads the right characters in the string 

      Serial.print("Word: "); Serial.println(direction);
      Serial.print("Number: "); Serial.println(floorNum); //abcd

    // reads the inputted request and determines direction desired
      if (direction.equalsIgnoreCase("Up")){
        Serial.println("Place in up queue");
      }

      else if (direction.equalsIgnoreCase("Down")){
        Serial.println("Place in down queue");
      }

      else {
        Serial.println("Invalid request");
      }
    }
    
  }

}
