// ****************************************************************************************************************
// NOTE: Make sure that pin 3 of the stepper driver is tied to 5V. Tying to ground will disable the stepper driver.
// ****************************************************************************************************************

#define DIR_PIN 2   // Plug into pin 2 of the stepper driver
#define STEP_PIN 3  // Plug into pin 1 of the stepper driver
#define FORWARD 1
#define BACKWARD -1
#define STEPSPERREV 200 //standard for 1.8 deg stepper motors
#define MICROSTEPSPERSTEP 10 //settable according to dipswitches on stepper driver. Decimal Stepper size increments currently set on jumpers.
//10 mm linear travel per revolution of stepper


//Set initial step size and direction
  int direction = FORWARD;
  int stepSize = STEPSPERREV * MICROSTEPSPERSTEP; //initialize stepper to turn once, so travelling 10 mm
  
void setup() 
{
  //Setup pinsM
  Serial.begin(9600);
  pinMode(DIR_PIN, OUTPUT); 
  pinMode(STEP_PIN, OUTPUT); 
}

void loop() 
{
  if (Serial.available() > 0) 
  {
    char value = Serial.read();

    if (value == 'f') // Change to forward dir
    {
      direction = FORWARD;
      Serial.println("Direction set to FORWARD");
    }
    else if (value == 'b') // Change to backward dir
    {
      Serial.println("Direction set to BACKWARD");
      direction = BACKWARD;
    }
    else if (value == 'p') // print current direction and stepsize
    {
      Serial.print("The current direction is ");
      if( direction == FORWARD )
        Serial.println("FORWARD");
      else 
        Serial.println("BACKWARD");
      Serial.print("The current step size is ");
      Serial.println(stepSize);
    } 
    else if (value == 'g') // Send movement command
    {
      Serial.println("Sending movement command");
      rotate( stepSize * direction, 1);
    }
    else if (value == '+')
    {
      stepSize *= 2;
      Serial.print("Step Size doubled. Step size now ");
      Serial.println(stepSize);
    }
    else if (value = '-')
    {
      stepSize /= 2;
      if (stepSize < 1) 
        stepSize = 1;
      Serial.print("Step size halved. Step size now ");
      Serial.println(stepSize); 
    }

  }
  delay(200);
}

void rotate(int steps, float speed){ 
  //rotate a specific number of microsteps (8 microsteps per step) - (negative for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (steps > 0)? HIGH:LOW;
  steps = abs(steps);

  digitalWrite(DIR_PIN,dir); 

  float usDelay = (1/speed) * 70;

  for(int i=0; i < steps; i++){ 
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(usDelay); 

    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(usDelay); 
  } 
} 

void rotateDeg(float deg, float speed){ 
  //rotate a specific number of degrees (negative for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (deg > 0)? HIGH:LOW;
  digitalWrite(DIR_PIN,dir); 

  int steps = abs(deg)*(1/0.225);
  float usDelay = (1/speed) * 70;

  for(int i=0; i < steps; i++){ 
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(usDelay); 

    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(usDelay); 
  } 
}
