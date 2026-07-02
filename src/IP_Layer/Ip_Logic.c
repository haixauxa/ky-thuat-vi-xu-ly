#include "Ip_Logic.h"
#include "Ip_Line_Sensor.h"
#include "Ip_Line_PID.h"
#include "main.h"

/*****************************************************************************
* CONFIG
******************************************************************************/
#define LINE_BASE_SPEED (MOTOR_MAX_SPEED * 66U / 100U) 	// toc do co ban
#define MISS_LINE_TURN (MOTOR_MAX_SPEED / 3U) 					// toc do khi mat line
#define LINE_SLOW_SPEED (MOTOR_MAX_SPEED * 22U / 100U)	// toc do cham

// thong so PID
#define LINE_PID_KP 400.0f
#define LINE_PID_KI 2.0f
#define LINE_PID_KD 25.0f
#define LINE_PID_OUT_LIMIT 450.0f // gioi han bam xung khong qua 450
#define LINE_PID_I_LIMIT 20.0f 		// gioi han tich luy sai so la 20

#define SAFE_DISTANCE 25 // cm

static Line_PID_t line_pid;

typedef enum 
{
  ROBOT_MODE_IDLE = 0,
  ROBOT_MODE_REMOTE, 						// dieu khien tu xa
  ROBOT_MODE_AVOID,  						// xe tranh vat can
  ROBOT_MODE_LINE_FOLLOWING 		// xe do line
} Robot_Mode_t;

static Robot_Mode_t  current_mode = ROBOT_MODE_IDLE;

typedef enum
{
	AVOID_STATE_FORWARD = 0,
	AVOID_STATE_STOP_1,
	AVOID_STATE_TURN,
	AVOID_STATE_STOP_2
} Avoid_State_t;

static Avoid_State_t avoid_state = AVOID_STATE_FORWARD;

static uint32_t avoid_timer = 0;
static uint32_t last_line_tick = 0;

static uint16_t Adaptive_Speed(int8_t position)
{
  int16_t abs_err = (position >= 0) ? position : -position;

  if (abs_err >= 2) return LINE_SLOW_SPEED;

  return LINE_BASE_SPEED - (uint16_t)((uint32_t)abs_err * (LINE_BASE_SPEED - LINE_SLOW_SPEED) / 2U);
}


// LINE FOLLOW
static void Line_Follow_Step(void)
{
   Line_Sensor_Data_t sensor;
   int16_t turn;
   uint16_t left, right;
   uint16_t forward;

   Ip_Line_Sensor_Read(&sensor);

   if (!sensor.line_detected)
   {
     turn = (sensor.position >= 0) ? (int16_t)MISS_LINE_TURN : -(int16_t)MISS_LINE_TURN;

     forward = LINE_SLOW_SPEED;
   }
   else
   {
     turn = Ip_Line_PID_Update(&line_pid, sensor.position);
     forward = Adaptive_Speed(sensor.position);
   }

   Ip_Line_PID_Drive_Compute_Speeds(forward, turn, &left, &right);

   Ip_Motor_Set_Speed(left, right);
   Ip_Motor_Move_Forward();
}

static void Avoid_Step(void)
{
  uint32_t tick = Ip_Get_Tick();

  switch (avoid_state)
  {
    case AVOID_STATE_FORWARD:
    {
      if (tick - avoid_timer > 50)
      {
        avoid_timer = tick;

        uint32_t dist = Ip_Hcsr04_Get_Distance();

        if (dist > 0 && dist < SAFE_DISTANCE)
        {
            Ip_Motor_Stop();
            avoid_state = AVOID_STATE_STOP_1;
        }
        else
        {
            Ip_Motor_Set_Speed(MOTOR_MAX_SPEED * 50 / 100, MOTOR_MAX_SPEED * 50 / 100);
            Ip_Motor_Move_Forward();
        }
      }
    } break;

    case AVOID_STATE_STOP_1:
    {
      if (tick - avoid_timer > 300)
      {
        Ip_Motor_Set_Speed(MOTOR_MAX_SPEED * 50 / 100, MOTOR_MAX_SPEED * 50 / 100);
				
        Ip_Motor_Turn_Right();

        avoid_state = AVOID_STATE_TURN;
        avoid_timer = tick;
      }
    } break;

    case AVOID_STATE_TURN:
    {
      if (tick - avoid_timer > 350)
      {
        Ip_Motor_Stop();

        avoid_state = AVOID_STATE_STOP_2;
        avoid_timer = tick;
      }
    } break;

    case AVOID_STATE_STOP_2:
    {
      if (tick - avoid_timer > 300)
      {
        avoid_state = AVOID_STATE_FORWARD;
        avoid_timer = tick;
      }
    } break;
  }
}

static void Process_IR(Ir_Cmd_e cmd)
{
  switch (cmd)
  {
    case IR_CMD_1:
      current_mode = ROBOT_MODE_REMOTE;
      Ip_Motor_Stop();
      break;

    case IR_CMD_2:
      current_mode = ROBOT_MODE_AVOID;
      avoid_state = AVOID_STATE_FORWARD;
      avoid_timer = Ip_Get_Tick();
      Ip_Motor_Stop();
      break;

    case IR_CMD_3:
      current_mode = ROBOT_MODE_LINE_FOLLOWING;
      Ip_Line_PID_Reset(&line_pid);
      Ip_Motor_Stop();
      break;

    case IR_CMD_OK:
      current_mode = ROBOT_MODE_IDLE;
      Ip_Motor_Stop();
      break;

    case IR_CMD_UP:
      if (current_mode == ROBOT_MODE_REMOTE)
      {
        Ip_Motor_Set_Speed(MOTOR_MAX_SPEED * 70 / 100, MOTOR_MAX_SPEED * 70 / 100);
        Ip_Motor_Move_Forward();
      }
      break;

    case IR_CMD_DOWN:
      if (current_mode == ROBOT_MODE_REMOTE)
      {
        Ip_Motor_Set_Speed(MOTOR_MAX_SPEED * 70 / 100, MOTOR_MAX_SPEED * 70 / 100);
        Ip_Motor_Move_Backward();
      }
      break;

    case IR_CMD_LEFT:
      if (current_mode == ROBOT_MODE_REMOTE)
      {
        Ip_Motor_Set_Speed(MOTOR_MAX_SPEED * 60 / 100, MOTOR_MAX_SPEED * 60 / 100);
        Ip_Motor_Turn_Left();
      }
      break;

    case IR_CMD_RIGHT:
      if (current_mode == ROBOT_MODE_REMOTE)
      {
        Ip_Motor_Set_Speed(MOTOR_MAX_SPEED * 60 / 100, MOTOR_MAX_SPEED * 60 / 100);
        Ip_Motor_Turn_Right();
      }
      break;

    default:
        break;
  }
}

void Ip_Logic_Init(void)
{
	Ip_Motor_Init();
  Ip_Line_Sensor_Init();
  Ip_Hcsr04_Init();
  Ip_Ir_Remote_Init();
	
	Ip_Line_PID_Init(&line_pid, LINE_PID_KP, LINE_PID_KI, LINE_PID_KD, LINE_PID_OUT_LIMIT, LINE_PID_I_LIMIT);
	
	Ip_Motor_Set_Speed(MOTOR_MAX_SPEED, MOTOR_MAX_SPEED);
	Ip_Delay_Ms(50);
	Ip_Motor_Stop();
	// printf("System Ready\n");
}

void Ip_Logic_Run(void)
{
	// IR
	Ir_Cmd_e cmd = Ip_Ir_Remote_Get_Cmd();
  if (cmd != IR_CMD_NONE)
  {
      Process_IR(cmd);
  }
	// MODE
	switch (current_mode)
  {
    case ROBOT_MODE_IDLE:
    case ROBOT_MODE_REMOTE:
      break;

    case ROBOT_MODE_AVOID:
      Avoid_Step();
      break;

    case ROBOT_MODE_LINE_FOLLOWING:
      if (Ip_Get_Tick() - last_line_tick > 5)
			{
        Line_Follow_Step();
        last_line_tick = Ip_Get_Tick();
      }
      break;
  }
}


