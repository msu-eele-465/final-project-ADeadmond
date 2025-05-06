# Final project proposal

- [*] I have reviewed the project guidelines.
- [*] I will be working alone on this project.
- [*] No significant portion of this project will be (or has been) used in other course work.

## Embedded System Description

This system will make use of a proximity sensor to set an alarm depending on if the object is too close or too far. It will have a few different operating modes that the user will be able to select depending on their desired alerting system.

One of the inputs is the proximity sensor. It is the HCSR04 echo sensor, which is an ultrasonic sensor that makes use of echos and the time it takes for sound to travel to estimate an objects distance.

The other input is the keypad that was used throughout the semester. With it users will be able to select the mode they want using the letter keys. A indiates off, B indicates too close, C indicates too far, D indicates proximity sensing.

The first output is the buzzer. It is a simple buzzer with built in oscillation that make some noise when given a DC input. The buzzer will make noise when an object gets too close, too far, or in one of the modes toggle on and off faster or slower depending on proximity.

The second output is the indicator LEDs. I have three LEDs that are colored red blue and green. Each one separately indicates which mode of operation the system is currently in.

The third output makes use of an LED bar. It will fill up as the object gets closer to the sensor and deplete as the object gets further away.

## Hardware Setup

What hardware will you require? Provide a conceptual circuit diagram and/or block diagram to help the reviewers understand your proposal. Be sure to introduce and discuss your figures in the text.

I require the HCSR04 sensor, an MSP430FR2355, an MSP430FR2310, an LED bar, a DC tone buzzer, colored LEDs, various resistors, and wires.

## Software overview

Discuss, at a high level, a concept of how your code will work. Include a *high-level* flowchart. This is a high-level concept that should concisely communicate the project's concept.
First the code will pull for the keypad input. Next the MSP will trigger the echo sensor. The sensor will record how long it takes for the echo to make it back to the sensor. From there it will calculate the distance the object is away from the sensor. It will use the keypad input and the calculated distance to turn on/off the buzzer, indicator LEDs, and LED bar.

## Testing Procedure

Briefly describe how you will test and verify that your project is a success. Think about how you will *demo* the final project. If you need extra equipment for the demo, be sure that you can either bring the equipment or convincingly simulate it. For example, if you want to build a system that uses CAN bus to interface with your car, you won't be able to bring your car into Cobleigh for the demo...

While I can't bring in my car, I can easily breadbord this circuit. I will go through each mode of operation and demonstrate how it works. I will verify that the buzzer only turns on when it is appropriate. I will make sure the correct indicator LEDs turn on based on the keypad input. I will verify that the LED bar fills and drains dependig on how close the object is to the sensor.

## Prescaler

Desired Prescaler level: 

- [ ] 100%
- [ ] 95% 
- [*] 90% 
- [ ] 85% 
- [ ] 80% 
- [ ] 75% 

### Prescalar requirements 

**Outline how you meet the requirements for your desired prescalar level**

**The inputs to the system will be:**
1.  Echo Sensor detects an object and how far away it is
2.  Keypad selects the desired mode of operation

**The outputs of the system will be:**
1.   Buzzer alarms depending on operating mode and object proximity
2.   Indicator LEDs show what the current mode of operation is
3.   LED bar fills and drains as object gets closer further

**The project objective is**

To create a system that will assist in backing something up when visibility is low. It will alarm the user when they are getting too close or too far away.

**The new hardware or software modules are:**
1. The HCSR04 sensor is an item that hasn't been used before in class
2. The tone buzzer is also an item that hasn't been used before in class


The Master will be responsible for:

It will be the one to drive the sensor, making the calculation for the distance away. It will also be responsible for reading the input of the keypad.

The Slave(s) will be responsible for:

The single slave will drive the outputs of the system. It will drive sound the buzzer, turn on the LEDs and fill/drain the LED bar when appropriate.



### Argument for Desired Prescaler

Consider the guidelines presented in the lecture notes and convince the reviewers that this proposal meets the minimum requirements for your desired prescale level.

I would argue it meets the 90% prescaler. It has three different outputs (LED bar, buzzer, and indicator LEDs) and two different inputs (keypad and echo sensor). It has the real objective of making a backup sensor that will assist in backing an object into a tight space when visibility is low. The HCSR04 sensor is a piece of hardware that has not been used before in EELE465 nor EELE371. The way that this hardware functions is not similar to previously used hardware as it requires its own specific timings, triggers, and calculations to make use of it. It is designed to make use of master slave topology to accomplish its objectives. Therefore I believe that it meets all the requirements for the 90% prescalar.
