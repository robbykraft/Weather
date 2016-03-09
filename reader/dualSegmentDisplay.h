#define MULTIPLEX_DELAY 10 // ms oscillation between digit displays
#define MINUS 999

// Match Pin # on display to Arduino pin #
//       display      arduino
#define GROUND_LEFT 	10
#define GROUND_RIGHT	11
// seven-segment
#define ALPHA_PIN_12	3  // top middle
#define ALPHA_PIN_7 	8  // bottom middle
#define ALPHA_PIN_1 	6  // lower left
#define ALPHA_PIN_18	5  // upper left
#define ALPHA_PIN_9 	9  // lower right
#define ALPHA_PIN_10	2  // upper right
#define ALPHA_PIN_13	4  // middle left
#define ALPHA_PIN_6 	7  // middle right
// alpha-numeric
#define ALPHA_PIN_8 	12 // radix point (decimal point)
// #define ALPHA_PIN_14	// diagonal up right 
// #define ALPHA_PIN_17	// diagonal up left
// #define ALPHA_PIN_2 	// diagonal down right
// #define ALPHA_PIN_5 	// diagonal down left
// #define ALPHA_PIN_15	// middle up
// #define ALPHA_PIN_4 	// middle down

class DualSegmentDisplay{
	public:

	int digit1;
	int digit2;

	void setInteger(int number){
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
		pinMode(GROUND_LEFT, OUTPUT);
		pinMode(GROUND_RIGHT, OUTPUT);    
		pinMode(ALPHA_PIN_1, OUTPUT);
		pinMode(ALPHA_PIN_6, OUTPUT);
		pinMode(ALPHA_PIN_7, OUTPUT);
		pinMode(ALPHA_PIN_9, OUTPUT);
		pinMode(ALPHA_PIN_10, OUTPUT);
		pinMode(ALPHA_PIN_12, OUTPUT);
		pinMode(ALPHA_PIN_13, OUTPUT);
		pinMode(ALPHA_PIN_18, OUTPUT);
		// decimal
		pinMode(ALPHA_PIN_8, OUTPUT);
		// extra alphanumeric
		// pinMode(ALPHA_PIN_14, OUTPUT);
		// pinMode(ALPHA_PIN_17, OUTPUT);
		// pinMode(ALPHA_PIN_2, OUTPUT);
		// pinMode(ALPHA_PIN_5, OUTPUT);
		// pinMode(ALPHA_PIN_15, OUTPUT);
		// pinMode(ALPHA_PIN_4, OUTPUT);
	}
	
	void loop(){
		setDigit(digit1);
		digitalWrite(GROUND_LEFT, HIGH);
		delay(MULTIPLEX_DELAY);
		digitalWrite(GROUND_LEFT, LOW);
//		setDigit(-1);

		setDigit(digit2);
		digitalWrite(GROUND_RIGHT, HIGH);
		delay(MULTIPLEX_DELAY);
		digitalWrite(GROUND_RIGHT, LOW);
//		setDigit(-1);
	}

	private:

	void setDigit(int number){
		switch(number){
			case 0:
				digitalWrite(ALPHA_PIN_12, HIGH);
				digitalWrite(ALPHA_PIN_7, HIGH);
				digitalWrite(ALPHA_PIN_1, HIGH);
				digitalWrite(ALPHA_PIN_18, HIGH);
				digitalWrite(ALPHA_PIN_9, HIGH);
				digitalWrite(ALPHA_PIN_10, HIGH);
				digitalWrite(ALPHA_PIN_6, LOW);
				digitalWrite(ALPHA_PIN_13, LOW);
			break;
			
			case 1:
				digitalWrite(ALPHA_PIN_12, LOW);
				digitalWrite(ALPHA_PIN_7, LOW);
				digitalWrite(ALPHA_PIN_1, LOW);
				digitalWrite(ALPHA_PIN_18, LOW);
				digitalWrite(ALPHA_PIN_9, HIGH);
				digitalWrite(ALPHA_PIN_10, HIGH);
				digitalWrite(ALPHA_PIN_6, LOW);
				digitalWrite(ALPHA_PIN_13, LOW);
			break;

			case 2:
				digitalWrite(ALPHA_PIN_12, HIGH);
				digitalWrite(ALPHA_PIN_7, HIGH);
				digitalWrite(ALPHA_PIN_1, HIGH);
				digitalWrite(ALPHA_PIN_18, LOW);
				digitalWrite(ALPHA_PIN_9, LOW);
				digitalWrite(ALPHA_PIN_10, HIGH);
				digitalWrite(ALPHA_PIN_6, HIGH);
				digitalWrite(ALPHA_PIN_13, HIGH);
			break;

			case 3:
				digitalWrite(ALPHA_PIN_12, HIGH);
				digitalWrite(ALPHA_PIN_7, HIGH);
				digitalWrite(ALPHA_PIN_1, LOW);
				digitalWrite(ALPHA_PIN_18, LOW);
				digitalWrite(ALPHA_PIN_9, HIGH);
				digitalWrite(ALPHA_PIN_10, HIGH);
				digitalWrite(ALPHA_PIN_6, HIGH);
				digitalWrite(ALPHA_PIN_13, HIGH);
			break;

			case 4:
				digitalWrite(ALPHA_PIN_12, LOW);
				digitalWrite(ALPHA_PIN_7, LOW);
				digitalWrite(ALPHA_PIN_1, LOW);
				digitalWrite(ALPHA_PIN_18, HIGH);
				digitalWrite(ALPHA_PIN_9, HIGH);
				digitalWrite(ALPHA_PIN_10, HIGH);
				digitalWrite(ALPHA_PIN_6, HIGH);
				digitalWrite(ALPHA_PIN_13, HIGH);
			break;

			case 5:
				digitalWrite(ALPHA_PIN_12, HIGH);
				digitalWrite(ALPHA_PIN_7, HIGH);
				digitalWrite(ALPHA_PIN_1, LOW);
				digitalWrite(ALPHA_PIN_18, HIGH);
				digitalWrite(ALPHA_PIN_9, HIGH);
				digitalWrite(ALPHA_PIN_10, LOW);
				digitalWrite(ALPHA_PIN_6, HIGH);
				digitalWrite(ALPHA_PIN_13, HIGH);    
			break;

			case 6:
				digitalWrite(ALPHA_PIN_12, HIGH);
				digitalWrite(ALPHA_PIN_7, HIGH);
				digitalWrite(ALPHA_PIN_1, HIGH);
				digitalWrite(ALPHA_PIN_18, HIGH);
				digitalWrite(ALPHA_PIN_9, HIGH);
				digitalWrite(ALPHA_PIN_10, LOW);
				digitalWrite(ALPHA_PIN_6, HIGH);
				digitalWrite(ALPHA_PIN_13, HIGH);   
			break;

			case 7:
				digitalWrite(ALPHA_PIN_12, HIGH);
				digitalWrite(ALPHA_PIN_7, LOW);
				digitalWrite(ALPHA_PIN_1, LOW);
				digitalWrite(ALPHA_PIN_18, LOW);
				digitalWrite(ALPHA_PIN_9, HIGH);
				digitalWrite(ALPHA_PIN_10, HIGH);
				digitalWrite(ALPHA_PIN_6, LOW);
				digitalWrite(ALPHA_PIN_13, LOW);
			break;
			
			case 8:
				digitalWrite(ALPHA_PIN_12, HIGH);
				digitalWrite(ALPHA_PIN_7, HIGH);
				digitalWrite(ALPHA_PIN_1, HIGH);
				digitalWrite(ALPHA_PIN_18, HIGH);
				digitalWrite(ALPHA_PIN_9, HIGH);
				digitalWrite(ALPHA_PIN_10, HIGH);
				digitalWrite(ALPHA_PIN_6, HIGH);
				digitalWrite(ALPHA_PIN_13, HIGH);    
			break;

			case 9:
				digitalWrite(ALPHA_PIN_12, HIGH);
				digitalWrite(ALPHA_PIN_7, HIGH);
				digitalWrite(ALPHA_PIN_1, LOW);
				digitalWrite(ALPHA_PIN_18, HIGH);
				digitalWrite(ALPHA_PIN_9, HIGH);
				digitalWrite(ALPHA_PIN_10, HIGH);
				digitalWrite(ALPHA_PIN_6, HIGH);
				digitalWrite(ALPHA_PIN_13, HIGH);    
			break;

			case MINUS:
				digitalWrite(ALPHA_PIN_12, LOW);
				digitalWrite(ALPHA_PIN_7, LOW);
				digitalWrite(ALPHA_PIN_1, LOW);
				digitalWrite(ALPHA_PIN_18, LOW);
				digitalWrite(ALPHA_PIN_9, LOW);
				digitalWrite(ALPHA_PIN_10, LOW);
				digitalWrite(ALPHA_PIN_6, HIGH);
				digitalWrite(ALPHA_PIN_13, LOW);    
			break;

			default:
				digitalWrite(ALPHA_PIN_12, LOW);
				digitalWrite(ALPHA_PIN_7, LOW);
				digitalWrite(ALPHA_PIN_1, LOW);
				digitalWrite(ALPHA_PIN_18, LOW);
				digitalWrite(ALPHA_PIN_9, LOW);
				digitalWrite(ALPHA_PIN_10, LOW);
				digitalWrite(ALPHA_PIN_6, LOW);
				digitalWrite(ALPHA_PIN_13, LOW);
			break;
		}
	}
};
