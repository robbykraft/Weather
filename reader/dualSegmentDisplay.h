#define MULTIPLEX_DELAY 10 // ms oscillation between digit displays
#define MINUS 999

#define NUM_PINS 8  // positive LED pins only
// PINS
#define GROUND1 10
#define GROUND2 11
#define TOP 3
#define BOTTOM 8
#define LEFT_BOTTOM 6
#define LEFT_TOP 5
#define RIGHT_BOTTOM 9
#define RIGHT_TOP 2
#define MIDDLE_LEFT 4
#define MIDDLE_RIGHT 7

class DualSegmentDisplay{
	public:

	int digit1;
	int digit2;

	void setNumber(int number){
		if(number < 100 && number > -10){
      if(number < 0){
        digit1 = MINUS;
        digit2 = abs(number) % 10;
      }
      else{
        digit1 = number / 10.0;
				digit2 = number % 10;
				if(digit1 == 0)
					digit1 = -1;
			}
		}
		else{
			digit1 = digit2 = -1;
		}
	}
	
	void setup(){
		pinMode(GROUND1, OUTPUT);
		pinMode(GROUND2, OUTPUT);    
		for(int i = 2; i < 2 + NUM_PINS; i++){
			pinMode(i, OUTPUT);
		}
	}
	
	void loop(){
		setDigit(digit1);
		digitalWrite(10, HIGH);
		delay(MULTIPLEX_DELAY);
		digitalWrite(10, LOW);
		setDigit(-1);

		setDigit(digit2);
		digitalWrite(11, HIGH);
		delay(MULTIPLEX_DELAY);
		digitalWrite(11, LOW);
		setDigit(-1);
	}

	private:

	void setDigit(int number){
		switch(number){
			case 0:
				digitalWrite(TOP, HIGH);
				digitalWrite(BOTTOM, HIGH);
				digitalWrite(LEFT_BOTTOM, HIGH);
				digitalWrite(LEFT_TOP, HIGH);
				digitalWrite(RIGHT_BOTTOM, HIGH);
				digitalWrite(RIGHT_TOP, HIGH);
				digitalWrite(MIDDLE_RIGHT, LOW);
				digitalWrite(MIDDLE_LEFT, LOW);
			break;
			
			case 1:
				digitalWrite(TOP, LOW);
				digitalWrite(BOTTOM, LOW);
				digitalWrite(LEFT_BOTTOM, LOW);
				digitalWrite(LEFT_TOP, LOW);
				digitalWrite(RIGHT_BOTTOM, HIGH);
				digitalWrite(RIGHT_TOP, HIGH);
				digitalWrite(MIDDLE_RIGHT, LOW);
				digitalWrite(MIDDLE_LEFT, LOW);
			break;

			case 2:
				digitalWrite(TOP, HIGH);
				digitalWrite(BOTTOM, HIGH);
				digitalWrite(LEFT_BOTTOM, HIGH);
				digitalWrite(LEFT_TOP, LOW);
				digitalWrite(RIGHT_BOTTOM, LOW);
				digitalWrite(RIGHT_TOP, HIGH);
				digitalWrite(MIDDLE_RIGHT, HIGH);
				digitalWrite(MIDDLE_LEFT, HIGH);
			break;

			case 3:
				digitalWrite(TOP, HIGH);
				digitalWrite(BOTTOM, HIGH);
				digitalWrite(LEFT_BOTTOM, LOW);
				digitalWrite(LEFT_TOP, LOW);
				digitalWrite(RIGHT_BOTTOM, HIGH);
				digitalWrite(RIGHT_TOP, HIGH);
				digitalWrite(MIDDLE_RIGHT, HIGH);
				digitalWrite(MIDDLE_LEFT, HIGH);
			break;

			case 4:
				digitalWrite(TOP, LOW);
				digitalWrite(BOTTOM, LOW);
				digitalWrite(LEFT_BOTTOM, LOW);
				digitalWrite(LEFT_TOP, HIGH);
				digitalWrite(RIGHT_BOTTOM, HIGH);
				digitalWrite(RIGHT_TOP, HIGH);
				digitalWrite(MIDDLE_RIGHT, HIGH);
				digitalWrite(MIDDLE_LEFT, HIGH);
			break;

			case 5:
				digitalWrite(TOP, HIGH);
				digitalWrite(BOTTOM, HIGH);
				digitalWrite(LEFT_BOTTOM, LOW);
				digitalWrite(LEFT_TOP, HIGH);
				digitalWrite(RIGHT_BOTTOM, HIGH);
				digitalWrite(RIGHT_TOP, LOW);
				digitalWrite(MIDDLE_RIGHT, HIGH);
				digitalWrite(MIDDLE_LEFT, HIGH);    
			break;

			case 6:
				digitalWrite(TOP, HIGH);
				digitalWrite(BOTTOM, HIGH);
				digitalWrite(LEFT_BOTTOM, HIGH);
				digitalWrite(LEFT_TOP, HIGH);
				digitalWrite(RIGHT_BOTTOM, HIGH);
				digitalWrite(RIGHT_TOP, LOW);
				digitalWrite(MIDDLE_RIGHT, HIGH);
				digitalWrite(MIDDLE_LEFT, HIGH);   
			break;

			case 7:
				digitalWrite(TOP, HIGH);
				digitalWrite(BOTTOM, LOW);
				digitalWrite(LEFT_BOTTOM, LOW);
				digitalWrite(LEFT_TOP, LOW);
				digitalWrite(RIGHT_BOTTOM, HIGH);
				digitalWrite(RIGHT_TOP, HIGH);
				digitalWrite(MIDDLE_RIGHT, LOW);
				digitalWrite(MIDDLE_LEFT, LOW);
			break;
			
			case 8:
				digitalWrite(TOP, HIGH);
				digitalWrite(BOTTOM, HIGH);
				digitalWrite(LEFT_BOTTOM, HIGH);
				digitalWrite(LEFT_TOP, HIGH);
				digitalWrite(RIGHT_BOTTOM, HIGH);
				digitalWrite(RIGHT_TOP, HIGH);
				digitalWrite(MIDDLE_RIGHT, HIGH);
				digitalWrite(MIDDLE_LEFT, HIGH);    
			break;

			case 9:
				digitalWrite(TOP, HIGH);
				digitalWrite(BOTTOM, HIGH);
				digitalWrite(LEFT_BOTTOM, LOW);
				digitalWrite(LEFT_TOP, HIGH);
				digitalWrite(RIGHT_BOTTOM, HIGH);
				digitalWrite(RIGHT_TOP, HIGH);
				digitalWrite(MIDDLE_RIGHT, HIGH);
				digitalWrite(MIDDLE_LEFT, HIGH);    
			break;

      case MINUS:
        digitalWrite(TOP, LOW);
        digitalWrite(BOTTOM, LOW);
        digitalWrite(LEFT_BOTTOM, LOW);
        digitalWrite(LEFT_TOP, LOW);
        digitalWrite(RIGHT_BOTTOM, LOW);
        digitalWrite(RIGHT_TOP, LOW);
        digitalWrite(MIDDLE_RIGHT, HIGH);
        digitalWrite(MIDDLE_LEFT, LOW);    
      break;

			default:
				digitalWrite(TOP, LOW);
				digitalWrite(BOTTOM, LOW);
				digitalWrite(LEFT_BOTTOM, LOW);
				digitalWrite(LEFT_TOP, LOW);
				digitalWrite(RIGHT_BOTTOM, LOW);
				digitalWrite(RIGHT_TOP, LOW);
				digitalWrite(MIDDLE_RIGHT, LOW);
				digitalWrite(MIDDLE_LEFT, LOW);
			break;
		}
	}
};
