# Low Resource Protocol
This protocol is mainly created for the 8 and 16 bits MCU (for example PIC12/16/18 series) 
in which there is (E)USART module with which the device can connect to another via RS-485 standard.
(Usually you will need a Full-Duplex peripheral IC for communication, for example MAX485.) 
With this protocol you are able to transmit and receive data between embedded devices safe and sound. 

Primarily I recommend this protocol to communicate between the low resource devices. 
(for instance between switch and brightness controller)

![Low Resource Protocol CI](https://github.com/d8bauxit8/low-resource-protocol/workflows/Low%20Resource%20Protocol%20CI/badge.svg?branch=master)

## Overview
* [About the protocol](#about-the-protocol)
    - [Headers](#headers)
    - [Data](#data)
* [About the implementation](#about-the-implementation)
    - [Application layer](#application-layer)
    - [Validation layer](#validation-layer)
    - [Link layer](#link-layer)
    - [Line code layer](#line-code-layer)
* [Receiver module](#receiver-module)
    - [What do you need for the receiver module?](#what-do-you-need-for-the-receiver-module)
        + [Receiver module initialization](#receiver-module-initialization)
        + [Receiver module interrupt](#receiver-module-interrupt)
        + [Receiver module timer interrupt](#receiver-module-timer-interrupt)
* [Transmitter module](#transmitter-module)
    - [What do you need for the transmitter module?](#what-do-you-need-for-the-transmitter-module)
        + [Transmitter module initialization](#transmitter-module-initialization)
        + [Transmitter module interrupt](#transmitter-module-interrupt)
        + [Transmitter module timer interrupt](#transmitter-module-timer-interrupt)
* [Collision detection module](#collision-detection-module) `Optional`
    - [What do you need for the collision detection module?](#what-do-you-need-for-the-collision-detection-module)
        + [Collision detection module initialization](#collision-detection-module-initialization)
        + [Collision detection module interrupts](#collision-detection-module-interrupts)
        + [Integrate receiver and transmitter interrupts](#integrate-receiver-and-transmitter-interrupts)
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
which is available in the [`parity-bit.utility.c`](src/utilities/parity-bit.utility.c) file.

### Headers
The headers also consist of 2 parts.
The first includes the target device (or group) ID (in 5 bits) and some command bits (in 3 bits).
Allowed commands:
- `LRP_FRAME_NO_COMMAND` - Use this when don't need any command
- `LRP_FRAME_GROUP_ID_COMMAND` - This macro help the target device to identify the type of received ID is group ID.
 
The second also contains a device ID (in 5 bits), which is the sender ID, and 
besides that it includes the length of data (in 3 bits) 
which the device will receive or transmitter during the communication.
Both device IDs can only be between 1 and 30, 
so the maximum number of devices which you can connect to one bus are 30.
(Actually the RS-458 standard contains that the maximum number of devices can be 32 in one segment.) 
The `0b00000` ID is reserved for the dynamic address allocation and 
the `0b11111` ID is also reserved for the broadcast address. 
The length of data can only be between 0 and 7. I think this is plenty enough for these devices. 

### Data
You can transmit your information through it between the devices and device groups. 

## About the implementation
The protocol provides for your devices a receiving and a transmitting module.
Besides that it includes an optional collision detection module which you can use if you need it. 
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

## Receiver module
This module provides for you the receiving function with 
which you are able to read data from the RS-485 bus 
which another device has sent to yours during the LRP protocol.
 
### What do you need for the receiver module?
#### Receiver module initialization
You have to create the source device ID, the session provider and the receiver frame buffer. 
Then you have to initialize the session provider with the `LRP_Receiver_init` function.
```c
// It will be a device and group ID during the receiving.
// Thus, your device will just get those frames at which the target ID equals with this.
// If you don't want to add your device to any group, set this variable to LRP_FRAME_BROADCAST_ID
const unsigned char const sourceDeviceId = 0b00000001;
const unsigned char const groupId = 0b00000010;
LRPReceiverSessionProvider receiverSessionProvider{};
LRPFrame receiverFrameBuffer[3]{};

LRP_Receiver_init(&receiverSessionProvider, &sourceDeviceId, receiverFrameBuffer, 3, &groupId);
```
For the receiver interrupt, you will need a `LRPLineCode4B5B` of type variable. 
```c
LRPLineCode4B5B receiverLineCode4B5B{};
```
#### Receiver module interrupt
The PIC MCUs give us some bit from which we can find have receiving errors or not.
For these you have to create a new method in which you handle them. (It will call in the receiver interrupt.)
```c
void EUSARTErrorHandler(void) {
    if(RCSTA2bits.FERR){
        LRP_LinkLayer_setError((LRPSessionProvider *) &receiverSessionProvider, LRP_LINK_LAYER_INTERNAL_ERROR);
    }

    if(RCSTA2bits.OERR){
        LRP_LinkLayer_setError((LRPSessionProvider *) &receiverSessionProvider, LRP_LINK_LAYER_INTERNAL_ERROR);
        RCSTA2bits.CREN = 0;
        RCSTA2bits.CREN = 1;
    }
}
```
You need an interrupt handler in which you have to call the line code layer handler.
You have to call this handler if the hardware buffer filled, 
so physical communication of one byte ended. 
```c
void receiverInterrupt(void) {
    // It is the given register from which you have to read the received data
    const unsigned char rData = RCREG2;

    // Call the error handler
    EUSARTErrorHandler();
    
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &receiverLineCode4B5B, &rData);

    LRP_ReceiverLinkLayer_errorStatusHandler(&receiverSessionProvider);
}
```
#### Receiver module timer interrupt
Besides that you will need a timer interrupt, in which you can process the decoded data. 
In this you have to call the validation layer, and the application layer. 
For the application layer, you need a receiver frame controller list 
which contains the controllers. These controllers process the received data, 
of which you have to define these logics. 
Each controller is a simple function and its type is `LRPReceiverFrameController`. 
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
unsigned char oneOfReceiverFrameControllers(const FrameData *const frameData) {
    // For example
    if (*frameData->data[0] == 'L') {
        // Do something
        return 1;
    }
    return 0;
}

unsigned char anotherReceiverFrameControllers(const FrameData *const frameData) {
    // For example
    if (*frameData->data[0] == 'R' && *frameData->data[1] == 'P') {
        // Do something
        return 1;
    }
    return 0;
}

LRPReceiverFrameController controllers[] = {oneOfReceiverFrameControllers, anotherReceiverFrameControllers};
const unsigned char receiverFrameControllerListLength = 2;

void receiverTimerInterrupt(void) {
    LRP_ReceiverValidatorLayer_handler(&receiverSessionProvider);
    LRP_ReceiverApplicationLayer_handler(&receiverSessionProvider, controllers, receiverFrameControllerListLength);
}
```
I suggest that the timer cycle will be less than one frame's transmitting time 
between two furthest devices, because if the frame buffer is overloaded, 
the receiver module throws away the received frames until there is no free spot in the buffer.

For this, you can find the right calculation in the [Calculations](#calculations) point. 

If you did everything well, your receiver module will work.

## Transmitter module
This module provides you the transmitting function 
with which you are able to send data from the RS-485 bus 
which another device will read from yours during the LRP protocol.

### What do you need for the transmitter module?
#### Transmitter module initialization
You have to create the source device ID, the session provider and the transmitter frame buffer. 
Then you have to initialize the session provider with the `LRP_Transmitter_init` function.
```c
// It will be the device ID during the transmitting.  
const unsigned char const sourceDeviceId = 0b00000001;
LRPTransmitterSessionProvider transmitterSessionProvider{};
LRPFrame transmitterFrameBuffer[3]{};

LRP_Transmitter_init((LRPSessionProvider *) &transmitterSessionProvider, &sourceDeviceId, transmitterFrameBuffer, 3);
```
For the transmitter interrupt, you will need a `LRPLineCode4B5B` of type variable. 
```c
LRPLineCode4B5B transmitterLineCode4B5B{};
```
#### Transmitter module interrupt
You need an interrupt handler in which you have to call the line code layer handler.
But before you call that function, you need a type of `unsigned char` variable into which the line code layer will write the encoded data.
If its value is zero, there is no data to be transmitted otherwise there is data to be sent.  
```c
void transmitterInterrupt(void) {
    // It is the given register in which you have to write the data to be transmitted.
    unsigned char tData = 0;
    LRP_TransmitterLineCodeLayer_handler(&transmitterSessionProvider, &transmitterLineCode4B5B, &tData);
    if (!tData) {
        // Stop transmitting
        // For example, disable the transmitter interrupt
        PIE3bits.TX2IE = 0;
        return;
    }
    // It is the given register in which you have to write that data which you want to send.
    TXREG2 = tData;
}
```
#### Transmitter module timer interrupt
Besides that you will need a timer interrupt, in which you can process the data which you want to send.
In this you have to call the validation layer. Then you will need a type of `unsigned char` variable 
into which the line code layer will put the starting delimiter byte. The line code layer's function have a return value
which is 1 if there is data to be transmitted. If this is met, you can write the variable's value into transmitter register. 
```c
void transmitterTimerInterrupt(void) {
    LRP_TransmitterValidatorLayer_handler(&transmitterSessionProvider);
    unsigned char data;
    if (LRP_TransmitterLineCodeLayer_isReadyToStartTransmitting(&transmitterSessionProvider)) {
        LRP_TransmitterLineCodeLayer_startTransmitting(&transmitterSessionProvider, &transmitterLineCode4B5B, &data);
        TXREG2 = data;
        // And if your transmitter interrupt was disabled, set to enabled
        PIE3bits.TX2IE = 1;
    }
}
```
If you were done the configuration, you can start using those function with which you can send data to another device.
With this, you have 3 methods which are the following:
* `LRP_TransmitterApplicationLayer_setReadyToRedefineFrameToReserved(...)`<br>
With this method you can reserve a frame in which you can add content. The method have a return value. 
If this is 0, there is no such a frame which is unused, thus the method can not reserve.
But if the return value is 1, the method could reserve a frame in which you can work.
* `LRP_TransmitterApplicationLayer_setDataIntoReservedFrame(...)`<br>
With this method you can write data into reserved frame.
* `LRP_TransmitterApplicationLayer_transmitReservedFrame(...)`<br>
With this method you can send that frame which has been reserved.

For example:
```c
// We can say that this function is called if you touch a capacitive sensor.
void touched(void) {
    // If somebody touched the sensor, we just try to reserve a frame.
    // If it is successful
    if (LRP_TransmitterApplicationLayer_setReadyToRedefineFrameToReserved(&transmitterSessionProvider)) {
        // We can write the data into the reserved frame
        const unsigned char touched[] = "touched";
        const unsigned char lengthOfData = 7;
        LRP_TransmitterApplicationLayer_setDataIntoReservedFrame(&transmitterSessionProvider, touched, lengthOfData);
        // Then send it
        const unsigned char targetDeviceId = 0b00010;
        LRP_TransmitterApplicationLayer_transmitReservedFrame(&transmitterSessionProvider, targetDeviceId, LRP_FRAME_NO_COMMAND);
    }
}
```
I suggest that the timer cycle will be less than one frame's transmitting time 
between two furthest devices, because you can not take advantage of the full bandwidth, if your validation layer is slow. 

For this, you can find the right calculation in the [Calculations](#calculations) point. 

If you did everything well, your transmitter module will work.

## Collision detection module
This function is provide you an opportunity to detect collision during communication 
and control the given frame resending OR throw the last frame because of error(s).

### What do you need for the collision detection module?
#### Collision detection module initialization
Before anything you do, the initialized transmitter and receiver providers had to be created.
After that, you have to create a collision detection variable, and have to initialize this with the `LRP_CollisionDetection_init` function.
```c
LRPCollisionDetection collisionDetection{};

LRP_CollisionDetection_init(&collisionDetection, &transmitterSessionProvider, &receiverSessionProvider);
```
#### Collision detection module interrupts
You need a transmitter error timer, and its handler. This handler responsible for the transmitter module restarting and its status checking.
The stopped timer will only start when the receiver or transmitter interrupts are called.
I recommend you that the timer's time will be higher than on frame transmitter time.
For example: If you use 9600 bit/s baudrate, one frame's transmitter time is ~16.1 ms, thus you have to choose lower value than this, for instance 17 ms.
```c
// This method will be you error timer handler
void transmitterErrorTimer(void) {
    // If transmitter module needs restarting
    if(LRP_CollisionDetection_shouldRestartTransmitterModule(&collisionDetection)) {
        // Check transmitter status
        LRP_TransmitterLinkLayer_errorStatusHandler(&transmitterSessionProvider);
        // STOP timer
        TMR6_StopTimer();
        return;
    }
    
    TMR6_WriteTimer(0);
}
```
Besides that you need a start transmitter error timer method which check the receiver link layer status.
```c
void startTransmitterErrorTimer(void) {
    // START timer
    TMR6_WriteTimer(0);
    TMR6_StartTimer();
}
```
#### Integrate receiver and transmitter interrupts
The last steps integrate these functions to the receiver and transmitter interrupts.
```c
void receiverInterrupt(void) {
    // It is the given register from which you have to read the received data
    const unsigned char rData = RCREG2;
    
    // Call the EUSART error handler
    EUSARTErrorHandler();
    
    // Handle noise stroke if it has
    if(LRP_CollisionDetection_isNoiseStrokeError(&rData)){
        LRP_CollisionDetection_noiseStrokeErrorHandler(&collisionDetection);
        startTransmitterErrorTimer();
    }
    
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &receiverLineCode4B5B, &rData);

    // If the transmitter interrupt is disabled, handle decode error, otherwise the transmitter interrupt will be handled.
    if (LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) &receiverSessionProvider)) {
        // If it has decoding error, handle this and send a noise stroke
        LRP_CollisionDetection_decodeErrorHandler(&collisionDetection);
        if(!PIE3bits.TX2IE) {
            LRP_TransmitterLinkLayer_errorStatusHandler(&transmitterSessionProvider);
            TXREG2 = LRP_COLLISION_DETECTION_NOISE_STROKE;
            PIE3bits.TX2IE = 1;
        }
    }
    
    // Handle the receiver error status
    LRP_ReceiverLinkLayer_errorStatusHandler(&receiverSessionProvider);
}

void transmitterInterrupt(void) {
    // It is the given register in which you have to write the data to be transmitted.
    unsigned char tData = 0;

    // If it has receiver error handle it
    if (LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) &transmitterSessionProvider)) {
        // If it has decoding error, handle this and send a noise stroke
        LRP_TransmitterLinkLayer_errorStatusHandler(&transmitterSessionProvider);
        TXREG2 = LRP_COLLISION_DETECTION_NOISE_STROKE;
        return;
    }
    
    LRP_TransmitterLineCodeLayer_handler(&transmitterSessionProvider, &transmitterLineCode4B5B, &tData);
    
    if (!tData) {
        // Stop transmitting
        // For example, disable the transmitter interrupt
        PIE3bits.TX2IE = 0;
        return;
    }
    // It is the given register in which you have to write that data which you want to send.
    TXREG2 = tData;
}
```
If you did everything well, your collision detection module will work.

## Calculations
In the calculation example, I will use those values and physical items which I recommend for the appropriate working.
First of all, let's see the physical cable type with which I will calculate.
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
Tpd / 1000 + number of bits * Tbt = time of transmitting (Tt)
6360 / 1000 + 11 * 104.1666667 = 1152.193334 us
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
Thus, 1 frame transmitting time's maximum is the following with the example values:
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
##### 0.9.0