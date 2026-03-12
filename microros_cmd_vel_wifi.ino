#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>

// Motor pins - ADJUST THESE TO YOUR ROBOT
#define MOTOR_LEFT_PWM 25
#define MOTOR_LEFT_DIR1 26
#define MOTOR_LEFT_DIR2 27

#define MOTOR_RIGHT_PWM 32
#define MOTOR_RIGHT_DIR1 33
#define MOTOR_RIGHT_DIR2 14

// PWM settings
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8
#define PWM_CHANNEL_LEFT 0
#define PWM_CHANNEL_RIGHT 1

rcl_subscription_t subscriber;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void error_loop(){
  while(1){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}

// Callback function for cmd_vel
void subscription_callback(const void * msgin) {
  const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;
  
  // Extract linear and angular velocities
  float linear_x = msg->linear.x;   // Forward/backward (-1 to 1)
  float angular_z = msg->angular.z; // Rotation (-1 to 1)
  
  // Calculate motor speeds
  float left_speed = linear_x - angular_z;
  float right_speed = linear_x + angular_z;
  
  // Constrain to -1.0 to 1.0
  left_speed = constrain(left_speed, -1.0, 1.0);
  right_speed = constrain(right_speed, -1.0, 1.0);
  
  // Convert to PWM (0-255)
  int left_pwm = abs(left_speed * 255);
  int right_pwm = abs(right_speed * 255);
  
  // Set motor directions and speeds
  setMotor(left_speed, left_pwm, MOTOR_LEFT_DIR1, MOTOR_LEFT_DIR2, PWM_CHANNEL_LEFT);
  setMotor(right_speed, right_pwm, MOTOR_RIGHT_DIR1, MOTOR_RIGHT_DIR2, PWM_CHANNEL_RIGHT);
}

void setMotor(float speed, int pwm_value, int dir1, int dir2, int pwm_channel) {
  if (speed > 0) {
    // Forward
    digitalWrite(dir1, HIGH);
    digitalWrite(dir2, LOW);
  } else if (speed < 0) {
    // Backward
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, HIGH);
  } else {
    // Stop
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, LOW);
  }
  
  ledcWrite(pwm_channel, pwm_value);
}

void setup() {
  // Configure motor pins
  pinMode(MOTOR_LEFT_DIR1, OUTPUT);
  pinMode(MOTOR_LEFT_DIR2, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR1, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR2, OUTPUT);
  
  // Setup PWM channels
  ledcSetup(PWM_CHANNEL_LEFT, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_RIGHT, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_LEFT_PWM, PWM_CHANNEL_LEFT);
  ledcAttachPin(MOTOR_RIGHT_PWM, PWM_CHANNEL_RIGHT);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Configure serial transport
  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  
  delay(2000);
  
  allocator = rcl_get_default_allocator();
  
  // Create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  
  // Create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_esp32_node", "", &support));
  
  // Create subscriber
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel"));
  
  // Create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));
  
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  delay(10);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10)));
}
