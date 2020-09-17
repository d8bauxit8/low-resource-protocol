# Low Resource Protocol
This protocol is mainly created for the 8 and 16 bits MCU (for example PIC12/16/18 series) 
in which there is (E)USART module with which the device can connect to another via RS-485 standard.
(Usually you will need a peripheral IC for communication, for example MAX485.) 
With this protocol you are able to transmit and receive data between embedded devices safe and sound.

Primarily I recommend this protocol to communicate between the smart home devices 
(for instance between smart switch and smart brightness controller) 
because in my view these communication solutions which available on the market are not too safe.

I'm working on the test cases :pushpin:
Be patient :wink:

## Overview
* [About the protocol](#about-the-protocol)
    - [Headers](#headers)
    - [Data](#data)
* [About the implementation](#about-the-implementation)
    - [Application layer](#application-layer)
    - [Validation layer](#validation-layer)
    - [Link layer](#link-layer)
    - [Line code layer](#line-code-layer)
* [Receive module](#receive-module)
    - [What do you need for the receive module?](#what-do-you-need-for-the-receive-module)
* [Transmit module](#transmit-module)
    - [What do you need for the transmit module?](#what-do-you-need-for-the-transmit-module)
* [Collision detection module](#collision-detection-module)
* [Calculations](#calculations)
    - [How can I check if the collision detection works right?](#how-can-i-check-if-the-collision-detection-works-right)
* [Demo](#demo)
* [Version](#version)
    
## About the protocol
When I designed the protocol, I tried to keep in mind that it should be easy and safe to use. 
Thus, the base of protocol is the frames. 
One frame contains every information which the transmission needs 
so that the data flow reaches the right device. 
These frames are similar to IP frames 
however I had to keep it in mind these devices have few resources, 
so I had to collect that relevant transmission information 
which is absolutely needed for the right communication.
As a result each frame consists of 2 main parts which are the headers, and the data parts.<br>
It looks like: `[ HEADERS ] [ DATA ]`

As these MCUs usually use (E)USART module at the transmission 
which can only send 8 bits in one stroke, the protocol only supports this. 
Thus, if you have an extra 9th bit in your devices' register, use the parity bit checking.
If there is no hardware support in your MCU, there is a parity bit checker in this library 
which is available in the [`parity_bit.c`](src/parity_bit.c) file.

### Headers
The headers also consist of 2 parts.
Tthe first includes the target device ID (in 5 bits) and some control bits (in 3 bits). 
The second also contains a device ID (in 5 bits), which is the sender ID, and 
besides that it includes the length of data (in 3 bits) 
which the device will receive or transmit during the communication.
Both device IDs can only be between 1 and 30, 
so the maximum number of devices which you can connect to one bus are 30.
(Actually the RS-458 standard contains that the maximum number of devices can be 32 in one segment.) 
The `0b00000` ID is reserved for the dynamic address allocation and 
the `0b11111` ID is also reserved for the broadcast address. 
The length of data can only be between 0 and 7. I think this is plenty enough for these devices. 

### Data
You can transmit your information through it between the devices. 

## About the implementation
The protocol provides for your devices a receiving and a transmitting module. 
The data reading needs the first, and the data sending needs the second.
Both modules have 4 layers which are the next ones:
* [Application layer](#application-layer)
* [Validation layer](#validation-layer)
* [Link layer](#link-layer)
* [Line code layer](#line-code-layer)

Besides that it's good if you know that the buffer uses FIFO method during the transmitting and receiving.

### Application layer
You have to use this layer to process the received or transmitted data.

### Validation layer
This layer is responsible for, reading the frame parameters from the buffer 
or sending the frame parameters to the buffer.

### Link layer
The task of this layer is to collect the right bytes into a buffer 
which the validation layer or the line code layer will use. 

### Line code layer
This layer is responsible for the frame encoding and decoding. 
Before the transmitter MCU sends a part of the frame, 
this layer encodes it to 4B5B coding and collects it into 8 bits group. 
At the receiver side when the encoded byte arrived, 
the layer collects it to 10 bits group to decode it to the right 8 bits.

## Receive module
This module provides for you the receiving function with 
which you are able to read data from the RS-485 bus 
which another device has sent to yours during the LRP protocol.
 
### What do you need for the receive module?
You have to create the source device ID, the session provider and the receive frame buffer. 
Then you have to initialize the session provider with the `LRP_SessionProvider_init` function.
```c
// It will be the device ID during the receiving.
// Thus your device will just get those frames at which the target device ID equals with this.  
const unsigned char const sourceDeviceId = 0b00000001;
_LRPReceiveSessionProvider sessionProvider;
_LRPFrame frameBuffer[3];

LRP_SessionProvider_init(&sessionProvider, &sourceDeviceId, frameBuffer, 3);
```
For the receive interrupt, you will need a `_LRPLineCode4B5B` of type variable. 
```c
// Parameters: .index: 0, .buffer[0]: 0, .buffer[1]: 0
_LRPLineCode4B5B lineCode4B5B;
unsigned char buffer[2] = {0, 0};
lineCode4B5B.buffer[0] = &buffer[0];
lineCode4B5B.buffer[1] = &buffer[1];
```
You need an interrupt handler in which you have to call the line code layer handler.
You have to call this handler if the hardware buffer filled, 
so physical communication of one byte ended. 
```c
void receiveInterrupt(void){
    // It is the given register from which you have to read the received data
    const unsigned char const data = RCREG; 
    LRP_ReceiveLineCodeLayer_handler(&sessionProvider, &lineCode4B5B, &data);
}
```
Besides that you will need a timer interrupt, in which you can process the decoded data. 
In this you have to call the validation layer, and the application layer. 
For the application layer, you need a receiver frame controller list 
which contains the controllers. These controllers process the received data, 
of which you have to define these logics. 
Each controller is a simple function and its type is `_LRPReceiveFrameController`. 
If you check this type you can see that it has a return value and its type is `unsigned char`. 
This return value helps you so that the right controller process the given message 
which is sent to him. In one word if your controller returned 1 (or higher which is truthy value), 
the message has been processed. Thus, other controllers won't get this message to process. 
Otherwise, if this returned value is 0, the given controller does not stop the processing flow, 
so the message can reach the right controller. 
Also, you can use this if you would like to process the given message with more controller too. 
However, you have to pay attention to the controller list array 
because the application layer calls these in order.

For example:<br>
You have two controllers in the controller array. 
Both controllers wait for the A message (frame) to which the first controller's returned value is 1, 
and the second is 0. You can see that if the layer gets an A message, 
the first controller will return 1 value, so the second will not be able to process the message. 
I recommend you to sort these controllers 
which process more messages not just one to the beginning of array.
```c
unsigned char oneOfReceiveFrameControllers(_FrameData *const frameData) {
    // Define
}

unsigned char anotherReceiveFrameControllers(_FrameData *const frameData) {
    // Define
}

_LRPReceiveFrameController controllers[] = { oneOfReceiveFrameControllers , anotherReceiveFrameControllers };
const unsigned char const receiveFrameControllerListLength = 2;

void timerInterrupt(void){
    LRP_ReceiveValidatorLayer_handler(&sessionProvider);
    LRP_ReceiveApplicationLayer_controller(&sessionProvider, controllers, receiveFrameControllerListLength);
}
```
I suggest that the timer cycle will be less than one frame's transmitting time 
between two furthest devices, because if the frame buffer is overloaded, 
the receiver module throws away the received frames until there is no free spot in the buffer.

For this, you can find the right calculation in the [Calculations](#calculations) point. 

If you did everything well, your receiver module will work.

## Transmit module
This module provides you the transmitting function 
with which you are able to send data from the RS-485 bus 
which another device will read from yours during the LRP protocol.

### What do you need for the transmit module?
In progress ... :pushpin:

## Collision detection module
In progress ... :pushpin:

## Calculations
In the calculation example, I will use those values and physical items which I recommend for the appropriate working.
First of all, lets see the physical cable type with which I will calculate.
I chose the CAT 5 about which I know the propagation delay which is `4.8–5.3 ns/m` (nanosecond/meter)

Next I should know the cable's maximum length. 
As I mentioned above with this protocol I have to use the RS-485 standard 
in which define the maximum distance between two devices. It is `1200 m` (meter).

The last information is the MCU's baud rate.
For this I chose at `9600 bit/s` (bit/second) speed as most MCU know it.

Now, I already have all the information for the calculations.

### How can I check if the collision detection works right?
About the detection I know that the length of bits to be transferred, 
the network's physical length and the signal propagation rate must be in appropriate relation for it to work it. 

Therefore, the first step is that, I have to know how long it takes for a signal to reach the end of wire.
Actually this equals the propagation delay.

Then I have to multiply this with the wire's length to get that time 
during which the signal has to reach the end of the line.
```
propagation delay (Pd) * wire length (Wl) = time of propagation delay (Tpd)
5.3 ns/m * 1200 m = 6360 ns
```
Okay I already know about the wire permeability, 
but I don't know anything about how long it takes one byte transmitting.

Besides that, I can not forget 
that in the MCUs I can only read bytes from the receiver register instead of bits.
The cost of transmitting or receiving of one byte consist of 11 bits. These are the following:<br>
```
1 start bit | 8 data bits | 1 parity bit | 1 stop bit
bits (b) = 11
```
The baud rate is `9600 bit/sec` from which I can calculate how long it takes one bit transmitting.
```
time (t) / baud rate = time of bit transmitting (Tbt)
1.000.000 us / 9600 bit/sec = 104.1666667 us
```
For this reason, the formula which I can calculate the transmitting time of the given bits is the following:
```
Tpd + number of bits * Tbt = time of transmitting (Tt)
```
Before I can calculate the frame transmitting time, I would have to know how many maximum number of bytes the 4B5B encoded headers and data bytes can take up.<br> 
```
(9 bytes * 8 bits / 4 bits group) * 5 bits group = 90 bits = 11.25 bytes ~ 12 bytes
```
From this, I can see that, one frame can contain up to 14 bytes, which are the following:<br>
```
1 start byte | 12 4B5B encoded bytes | 1 stop byte
bytes of frame (Bf) = 14 
```
Thus 1 frame transmitting time's maximum is the following with the example values:
```
Tpd + Bf * b * Tbt = 6.36 us + 14 * 11 * 104.1666667 us = 16048.02667 us ~ 16.1 ms
```
Besides that here is the formula:
```
Summary formula:
Br = baud rate (bit/second)
Pd = propagation delay (nanosecond / meter)
Wl = wire length (meter)
Be = number of bytes which have to encode

(Pd * Wl / 1000) + (2 + (Be * 8 / 4) * 5) * 11 * (1000000 / Br) 
```

## Demo
The demo software will be run on the PIC 18F45K22 microcontroller.
In progress ... :pushpin:

## Version
##### 0.5.0