#include "Main.h"
#include "App_Layer.h"

#define LINE_BASE_SPEED (MOTOR_MAX_SPEED * 66U / 100U)
#define LINE_REACQUIRE_TURN (MOTOR_MAX_SPEED / 3U)

/*
 * Giam toc do khi error lon (cua gap).
 * LINE_SLOW_SPEED:  toc do toi thieu khi error = max (|error| = 2)
 * He so giam: moi don vi |error| giam (BASE - SLOW) / 2
 */
#define LINE_SLOW_SPEED (MOTOR_MAX_SPEED * 22U / 100U)

#define LINE_PID_KP 450.0f
#define LINE_PID_KI 3.0f
#define LINE_PID_KD 30.0f
#define LINE_PID_OUT_LIMIT 450.0f
#define LINE_PID_I_LIMIT 20.0f

static Line_PID_t line_pid;

/*
 * Tinh toc do thich ung: error cang lon -> toc do cang giam
 * |error| = 0 -> LINE_BASE_SPEED
 * |error| = 2 -> LINE_SLOW_SPEED
 */
static uint16_t Adaptive_Speed(int8_t position) {
  uint16_t speed;
  int16_t abs_err = (position >= 0) ? position : -position;

  if (abs_err >= 2) {
    speed = LINE_SLOW_SPEED;
  } else {
    /* Noi suy tuyen tinh: speed = BASE - abs_err * (BASE - SLOW) / 2 */
    speed =
        LINE_BASE_SPEED - (uint16_t)((uint32_t)abs_err *
                                     (LINE_BASE_SPEED - LINE_SLOW_SPEED) / 2U);
  }

  return speed;
}

static void Line_Follow_Step(void) {
  Line_Sensor_Data_t sensor_data;
  int16_t turn_rate;
  uint16_t left_speed;
  uint16_t right_speed;
  uint16_t forward_speed;

  App_Line_Sensor_Read(&sensor_data);

  if (sensor_data.line_detected == false) {
    turn_rate = (sensor_data.position >= 0) ? (int16_t)LINE_REACQUIRE_TURN
                                            : -(int16_t)LINE_REACQUIRE_TURN;
    forward_speed = LINE_SLOW_SPEED; /* Giam toc khi mat line */
  } else {
    turn_rate = App_Line_PID_Update(&line_pid, sensor_data.position);
    forward_speed = Adaptive_Speed(sensor_data.position);
  }

  App_Line_PID_Compute_Wheels(forward_speed, turn_rate, &left_speed,
                              &right_speed);
  App_Motor_Set_Speed(left_speed, right_speed);
  App_Motor_Move_Forward();
}

int main() {
  delay.init();
  uart1.init(115200);

  App_Motor_Init();
  App_Line_Sensor_Init();
  App_Line_PID_Init(&line_pid, LINE_PID_KP, LINE_PID_KI, LINE_PID_KD,
                    LINE_PID_OUT_LIMIT, LINE_PID_I_LIMIT);

  // khoi dong
  Ip_Motor_Set_Speed(MOTOR_MAX_SPEED, MOTOR_MAX_SPEED);
  Ip_Motor_Move_Forward();
  Ip_Delay_Ms(50);
  Ip_Motor_Stop();

  uint32_t last_pid = 0;
  while (1) {

    if (Ip_Get_Tick() - last_pid > 5u) {
      Line_Follow_Step();
      last_pid = Ip_Get_Tick();
    }
  }
}
