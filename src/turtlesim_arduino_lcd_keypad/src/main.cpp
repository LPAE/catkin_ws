#define USE_USBCON
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include "turtlesim/Pose.h"
#include <std_msgs/UInt16.h>
#include <Stepper.h>

// -----------------------------------------------------------------------------
ros::NodeHandle nh;

// -----------------------------------------------------------------------------
turtlesim::Pose msg_pose;

float_t x;
float_t y;
float_t theta;
float_t linear_velocity;
float_t angular_velocity;

void msg_pose_callback(const turtlesim::Pose &msg_pose)
{
    x = msg_pose.x;
    y = msg_pose.y;
    theta = msg_pose.theta;
    linear_velocity = msg_pose.linear_velocity;
    angular_velocity = msg_pose.angular_velocity;
}
ros::Subscriber<turtlesim::Pose> sub_msg_pose("/turtle1/pose", &msg_pose_callback);

// -----------------------------------------------------------------------------
geometry_msgs::Twist cmd_vel_msg;
ros::Publisher pub_cmd_vel("/turtle1/cmd_vel", &cmd_vel_msg);

// -----------------------------------------------------------------------------
#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int key_signal;

void print_key()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("k:");lcd.print(key_signal);
}

void print_pose()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("x:");lcd.print(x);
    lcd.setCursor(0,1);
    lcd.print("y:");lcd.print(y);
}

// -----------------------------------------------------------------------------
void setup()
{
    // INIT LCD  ====================================
    lcd.begin(16, 2);

    // LCD FRAME  ___________________________________
    // --------------------------"0123456789abcdef"--
    lcd.setCursor(0,0);lcd.print("                ");
    lcd.setCursor(0,1);lcd.print("                ");
    delay(250);
    // --------------------------"0123456789abcdef"--
    lcd.setCursor(0,0);lcd.print("                ");
    lcd.setCursor(0,1);lcd.print(".               ");
    delay(250);
    // --------------------------"0123456789abcdef"--
    lcd.setCursor(0,0);lcd.print("                ");
    lcd.setCursor(0,1);lcd.print("..              ");
    delay(250);
    // --------------------------"0123456789abcdef"--
    lcd.setCursor(0,0);lcd.print("                ");
    lcd.setCursor(0,1);lcd.print("...             ");
    delay(500);

    // INIT ROS NODE  ===============================
    nh.initNode();

    nh.subscribe(sub_msg_pose);
    nh.advertise(pub_cmd_vel);
}

void loop()
{
    nh.spinOnce();

    key_signal = analogRead(0);

    if(key_signal<100)
    {
        cmd_vel_msg.linear.x = 0.3;
        cmd_vel_msg.linear.y = 0.0;
        cmd_vel_msg.linear.z = 0.0;
        cmd_vel_msg.angular.x = 0.0;
        cmd_vel_msg.angular.y = 0.0;
        cmd_vel_msg.angular.z = -0.9;
    }
    else if(key_signal>100 && key_signal<400)
    {
        cmd_vel_msg.linear.x = 0.3;
        cmd_vel_msg.linear.y = 0.0;
        cmd_vel_msg.linear.z = 0.0;
        cmd_vel_msg.angular.x = 0.0;
        cmd_vel_msg.angular.y = 0.0;
        cmd_vel_msg.angular.z = 0.0;
    }
    else if(key_signal>400 && key_signal<600)
    {
        cmd_vel_msg.linear.x = -0.3;
        cmd_vel_msg.linear.y = 0.0;
        cmd_vel_msg.linear.z = 0.0;
        cmd_vel_msg.angular.x = 0.0;
        cmd_vel_msg.angular.y = 0.0;
        cmd_vel_msg.angular.z = 0.0;
    }
    else if(key_signal>600 && key_signal<900)
    {
        cmd_vel_msg.linear.x = 0.3;
        cmd_vel_msg.linear.y = 0.0;
        cmd_vel_msg.linear.z = 0.0;
        cmd_vel_msg.angular.x = 0.0;
        cmd_vel_msg.angular.y = 0.0;
        cmd_vel_msg.angular.z = 0.9;
    }
    else if(key_signal>900)
    {
        cmd_vel_msg.linear.x = 0.0;
        cmd_vel_msg.linear.y = 0.0;
        cmd_vel_msg.linear.z = 0.0;
        cmd_vel_msg.angular.x = 0.0;
        cmd_vel_msg.angular.y = 0.0;
        cmd_vel_msg.angular.z = 0.0;
    }

    pub_cmd_vel.publish(&cmd_vel_msg);

    print_pose();

    delay(100);
}
