# Low Resource Protocol
This protocol mainly is created for the 8 and 16 bits MCU (for example PIC12/16/18 series) 
in which there is (E)USART module with which the device can connect to another via RS-485 standard.
(Usually you will need a peripheral IC to the communication, for example MAX485.) 
With this protocol you are able to transmit and receive data between embedded devices with safe and sound.

Primarily I recommend this protocol to communicate between the smart home devices 
(for instance between smart switch and smart brightness controller) 
because in my view these communication solutions which available on the market are not too safe.

## Overview
* [About the protocol](#about-the-protocol)
    - [Headers](#headers)
    - [Data](#data)
* [About the implementation](#about-the-implementation)
    - [Application layer](#application-layer)
    - [Validation layer](#validation-layer)
    - [Link layer](#link-layer)
    - [Line code layer](#line-code-layer)
* [Receive service](#receive-module)
    - [What do you need with the receive module?](#what-do-you-need-with-the-receive-module)
* [Transmit module](#transmit-module)
    - [What do you need with the transmit module?](#what-do-you-need-with-the-transmit-module)
* [Calculations](#calculations)
    - [How can I check if the collision detection work right?](#how-can-i-check-if-the-collision-detection-work-right)
    
## About the protocol
When I designed the protocol, I tried to keep it in my mind to be easy and safe using. 
Thus, the base of protocol is the frames. 
One frame contains every information on which the transmission needs 
that the data flow reach the right device. 
This frames similar to IP frames 
however I had to keep it in mind these devices have few resources, 
so I had to collect that relevant transmission information 
which is absolutely needed the right communication.
As a result each frame consist of 2 main parts which are the headers, and the data parts.<br>
It looks like: `[ HEADERS ] [ DATA ]`

As this MCUs usually use (E)USART module at the transmission 
which can only send 8 bits in one stroke, so the protocol just supports this. 
Thus, if you have an extra 9th bit in your devices' register, use the parity bit checking.
If there is no hardware support in your MCU, there is a parity bit checker in this library 
which you can available in the [`parity_bit.c`](src/parity_bit.c) file.

### Headers
The headers also consist of 2 parts.
In the first includes the target device ID (in 5 bits) and some control bits (in 3 bits). 
The second also contains a device ID (in 5 bits), which is the sender ID, and 
besides that it includes the length of data (in 3 bits) 
which the device will receive or transmit during the communication.
The both device IDs only be between 1 and 30, 
so the maximum number of devices which you can connect on one bus are 30.
(Actually the RS-458 standard contains that the maximum number of devices can be 32 in one segment.) 
The `0b00000` ID reserved for the dynamic address allocation and 
the `0b11111` ID also reserved for the broadcast address. 
The length of data only between 0 and 7. I think this is plenty enough for these devices. 

### Data
You can transmit through it your information between the devices. 

## About the implementation
The protocol provides your devices a receiving and a transmitting module. 
The data reading needs the first, and the data sending needs the second.
Both modules have 4 layer which is the next:
* [Application layer](#application-layer)
* [Validation layer](#validation-layer)
* [Link layer](#link-layer)
* [Line code layer](#line-code-layer)

Besides that it's good if you know, the buffer use FIFO method at the transmitting and receiving.

### Application layer
You have to use this layer to process the received or transmitted data.

### Validation layer
This layer responsible, (in case reading) the frame parameters read from the buffer 
or (in case sending) the frame parameters write to the buffer.

### Link layer
The task of this layer is to collect the right bytes in a buffer 
which will use the validation, or the line code layer. 

### Line code layer
This layer responsible the frame encoding and decoding. Before the transmitter MCU send a part of frame, 
this layer encodes it to 4B5B coding and collect it to 8 bits group. 
In the receive side when the encoded byte arrived, 
the layer collect it to 10 bits group to decode it to the right 8 bits. 

## Receive module
This module provides you the receiving function 
with which you are able to read data from the RS-485 bus 
which another device sent to yours during the LRP protocol.
 
### What do you need with the receive module?
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
_LRPLineCode4B5B lineCode4B5B = {0, {0, 0}};
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
For the application layer, you need a receive frame controller list which contains the controllers.
These controllers process the received data, of which you have to define these logics. 
Each controller is a simple function which type is `_LRPReceiveFrameController`. 
If you check this type you can see that it have a return value which type is `unsigned char`.
This return value help you in that the right controller process the given message which is sent to him.
In one word if your controller returned 1 (or higher which is truthy value), the message has been processed.
Thus, other controllers won't get this message to process.
Otherwise, if this returned value is 0, the given controller does not stop the processing flow,
so the message can reach the right controller.
Also, you can use this if you would like to process the given message with more controller too.
However, you have to pay attention the controller list array because the application layer calls these in order.

For example:<br>
You have two controllers in the controller array. 
Both controllers wait for the `A` message (frame) 
to which the first controller's returned value is 1, and the second is 0. 
You can see that if the layer get an `A` message, 
the first controller will return 1 value, 
so the second will not be able to process the message. 
I recommend you that these controllers 
which process more messages not just one, try to sort to the beginning of array. 
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
I recommend you that the timer cycle will be less than 
one frame's transmitting time between two furthest devices, 
because if the frame buffer is overload, 
the receive module throws the received frames until in the buffer will not be free spot.

For this, you can find the right calculation in the [Calculations](#calculations) point. 

If you did everything good, your receive module will work.

## Transmit module
This module provides you the transmitting function 
with which you are able to send data from the RS-485 bus 
which another device will read from yours during the LRP protocol.

### What do you need with the transmit module?
In progress ...

## Calculations
In the calculation example, I will use those values and physical items which I recommend the appropriate working.
First of all, lets see the physical cable type with which I will calculate.
I chose the CAT 5 from which I know the propagation delay which is `4.8–5.3 ns/m` (nanosecond/meter)

The next is that, I should know the cable's maximum length. 
As I mentioned above with this protocol I have to use the RS-485 standard 
in which define the maximum distance between two devices. It is `1200 m` (meter).

The last information is the MCU's baud rate.
For this I chose `9600 bit/s` (bit/second) speed as usually the several MCU know it.

Now, I already have all information for the calculations.

### How can I check if the collision detection work right?
It is important because of the correct working. 
With this, I have to know how long it takes a signal to reach the end of wire.
The signal propagation rate equals the propagation delay.
```
5.3 ns/m = 5.3 ns/m
```
Then I have to multiply with the wire's length to get that time 
during which the signal has to reach the end of the line.
```
5.3 ns/m * 1200 m = 6360 ns
```

