uint8_t motor[2][3] = {{32, 33, 4},{34, 35, 6}};
uint8_t motor_speed_default = 255;
uint8_t motor_speed[1][2] = {{motor_speed_default,motor_speed_default}};

uint8_t line_sensor[] = {22,19,18,17,16,15,14};
String line_status; //,line_status_old

uint8_t speed_down_list[] = {50,25};
uint8_t speed_down[] = {speed_down_list[0],int((speed_down_list[0]+speed_down_list[1])/2),speed_down_list[1]};

void robot_motor(uint8_t digital_a,uint8_t digital_b,uint8_t digital_c,uint8_t digital_d,uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  digitalWrite(motor[0][0], digital_a);
  digitalWrite(motor[0][1], digital_b);
  digitalWrite(motor[1][0], digital_c);
  digitalWrite(motor[1][1], digital_d);
  analogWrite(motor[0][2], analog_a);
  analogWrite(motor[1][2], analog_b);
  if (int_a != 0) {
    delay(int_a);
  }
}

void robot_forward(uint8_t analog_a,uint8_t analog_b,uint16_t int_a) {
  robot_motor(1,0,1,0,analog_a,analog_b,int_a);
}

void line_check() {
  //line_status_old = line_status;
  line_status = "";
  for(uint8_t c = 0;c < 7;c++) {
    line_status += String(!digitalRead(line_sensor[c]));
  }
  Serial.println(line_status);
}

void balance() {
  if (line_status == "0100000" || line_status == "0101000" || line_status == "0110000" || line_status == "0110000") {
    motor_speed[0][0] = motor_speed[0][0] - speed_down[0];
  }else if (line_status == "0010000" || line_status == "0010000"){
    motor_speed[0][0] = motor_speed[0][0] - speed_down[1];
  }else if (line_status == "0000010" || line_status == "0001010" || line_status == "0000110" || line_status == "0001110") {
    motor_speed[0][1] = motor_speed[0][1] - speed_down[0];
  }else if (line_status == "0000100" || line_status == "0001100"){
    motor_speed[0][1] = motor_speed[0][1] - speed_down[1];
  }else if (line_status == "0001000" || line_status == "0011100") {
    motor_speed[0][0] = motor_speed_default;
    motor_speed[0][1] = motor_speed_default;
  }
  analogWrite(motor[0][2], motor_speed[0][0]);
  analogWrite(motor[1][2], motor_speed[0][1]);
}

void setup() {
  Serial.begin(9600);
  for(uint8_t a = 0;a < 2;a++) {
    for(uint8_t b = 0;b < 3;b++) {
      if(motor[a][b] != 0) {
        pinMode(motor[a][b], OUTPUT);
      }
    }
  }
  for(uint8_t c = 0;c < 7;c++) {
    pinMode(line_sensor[c], INPUT);
  }
}

void loop() {
  robot_forward(motor_speed[0][0],motor_speed[0][1],0);
  balance();
  delay(1000);
}
