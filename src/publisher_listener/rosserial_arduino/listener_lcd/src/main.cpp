#define USE_USBCON
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/UInt16.h>
#include <Stepper.h>
#include "pinout.h"

// -----------------------------------------------------------------------------
ros::NodeHandle nh;

int key_signal;

int ref_alpha_1=0, ref_alpha_2=0, ref_teta=200;
int alpha_1=0, alpha_2=0, teta=0;

void alpha_1_callback(const std_msgs::UInt16& angle)
{
    ref_alpha_1 = angle.data;
}

void alpha_2_callback(const std_msgs::UInt16& angle)
{
    ref_alpha_2 = angle.data;
}

void teta_callback(const std_msgs::UInt16& angle)
{
    ref_teta = angle.data;
}

ros::Subscriber<std_msgs::UInt16> sub_alpha_1("alpha_1", alpha_1_callback);
ros::Subscriber<std_msgs::UInt16> sub_alpha_2("alpha_2", alpha_2_callback);
ros::Subscriber<std_msgs::UInt16> sub_teta("teta", teta_callback);

geometry_msgs::Twist cmd_vel_msg;
ros::Publisher pub_cmd_vel("/turtle1/cmd_vel", &cmd_vel_msg);

// -----------------------------------------------------------------------------
#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void print_key()
{
    lcd.clear();
    lcd.setCursor(0,0);
        lcd.print("k:");lcd.print(key_signal);
}

void print_angle()
{
    lcd.clear();
    lcd.setCursor(0,0);
        lcd.print("as1:");lcd.print(alpha_1);lcd.print(" ");
        lcd.print("as2:");lcd.print(alpha_2);

    lcd.setCursor(0,1);
        lcd.print("ts:");lcd.print(teta);lcd.print(" ");
}

void printSignalError(int sig, int ref)
{
    lcd.clear();
    lcd.setCursor(0,0);
        lcd.print("c:");lcd.print(sig);lcd.print(" ");
        lcd.print("r:");lcd.print(ref);

    lcd.setCursor(0,1);
        lcd.print("e:");lcd.print(ref-sig);lcd.print(" ");
}


// -----------------------------------------------------------------------------
const int stepsPerRevolution = 200;

int step_action(int sig, int ref)
{
    if (sig < ref)
        sig++;
    else if (sig > ref)
        sig--;

    return sig;
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

    nh.subscribe(sub_alpha_1);
    nh.subscribe(sub_alpha_2);
    nh.subscribe(sub_teta);

    nh.advertise(pub_cmd_vel);
}

void loop()
{
    alpha_1 = step_action(alpha_1, ref_alpha_1);
    alpha_2 = step_action(alpha_2, ref_alpha_2);
    teta = step_action(teta, ref_teta);

    nh.spinOnce();

    key_signal = analogRead(0);

    if(key_signal<128)
    {
        cmd_vel_msg.linear.x = 0.1;
        cmd_vel_msg.linear.y = 0.0;
        cmd_vel_msg.linear.z = 0.0;
        cmd_vel_msg.angular.x = 0.0;
        cmd_vel_msg.angular.y = 0.0;
        cmd_vel_msg.angular.z = -0.9;
    }
    else if(key_signal>500 && key_signal<800)
    {
        cmd_vel_msg.linear.x = 0.1;
        cmd_vel_msg.linear.y = 0.0;
        cmd_vel_msg.linear.z = 0.0;
        cmd_vel_msg.angular.x = 0.0;
        cmd_vel_msg.angular.y = 0.0;
        cmd_vel_msg.angular.z = 0.9;
    }
    else if(key_signal>1000)
    {
        cmd_vel_msg.linear.x = 0.0;
        cmd_vel_msg.linear.y = 0.0;
        cmd_vel_msg.linear.z = 0.0;
        cmd_vel_msg.angular.x = 0.0;
        cmd_vel_msg.angular.y = 0.0;
        cmd_vel_msg.angular.z = 0.0;
    }

    pub_cmd_vel.publish(&cmd_vel_msg);

    print_key();

    delay(5);
}
