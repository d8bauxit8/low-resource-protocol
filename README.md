# Low Resource Protocol
This protocol mainly is created for the 8 and 16 bits MCU (for example PIC12/16/18 series) 
in which there is (E)USART module with which the device can connect to another via RS485 standard.
(Usually you will need a peripheral IC to the communication, for example MAX485.) 
With this protocol you are able to transmit and receive data between embedded devices with safe and sound.

Primarily I recommend this protocol to communication between the smart home devices 
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
* [Receive service](#receive-service)
    
## About the protocol
When I designed the protocol, I tried to keep it in my mind to be easy and safe using. 
Thus the base of protocol is the frames. 
One frame contains every information on which the transmission needs 
that the data flow reach the right device. 
This frames similar to IP frames 
however I had to keep it in mind these devices have few resources, 
so I had to collect that relevant transmission information 
which is absolutely needed the right communication.
As a result each frames consist of 2 main parts which are the headers and the data parts.<br>
It looks like: `[ HEADERS ] [ DATA ]`

As this MCUs usually use (E)USART module at the transmission 
which can only send 8 bits in one stroke, so the protocol just supports this. 
Thus if you have an extra 9th bit in your devices' register, use the parity bit checking.
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
The first is needed to the data reading, and the second is needed to the data sending.
Both modules have 4 layer which is the next:
* [Application layer](#application-layer)
* [Validation layer](#validation-layer)
* [Link layer](#link-layer)
* [Line code layer](#line-code-layer)

### Application layer
You have to use this layer to process the received ot transmitted data.

### Validation layer
This layer responsible, (in case reading) the frame parameters read from the buffer 
or (in case sending) the frame parameters write to the buffer.

### Link layer
The task of this layer is to collect the right bytes in a buffer 
which will use the validation or the line code layer. 

### Line code layer
This layer responsible the frame encoding and decoding. Before the transmitter MCU send a part of frame, 
this layer encode it to 4B5B coding and collect it to 8 bits group. 
In the receiver side when the encoded byte arrived, 
the layer collect it to 10 bits group to decode it to the right 8 bits. 

## Receive module
This module provide you the receiving function 
with which you are able to read the data from the RS485 bus 
which another device sent to yours during the LRP protocol.
 
### What do you need with this?
You have to create the source device ID, the session provider and the receive frame buffer. 
Then you have to initialize the session provider.
```c
const unsigned char const sourceDeviceId = 0b00000001;
_LRPReceiveSessionProvider sessionProvider;
_LRPFrame frameBuffer[3];

LRP_SessionProvider_init(&sessionProvider, &sourceDeviceId, frameBuffer, 3);
```
For the receive interrupt, you will need a _LRPLineCode4B5B of type variable. 
```c
// Parameters: .index: 0, .buffer[0]: 0, .buffer[1]: 0
_LRPLineCode4B5B lineCode4B5B = {0, {0, 0}};
```
You need an interrupt handler in which you have to call the line code layer handler.
```c
void receiveInterrupt(void){
    // It is the given register from which you have to read the received data
    const unsigned char const data = RCREG; 
    LRP_ReceiveLineCodeLayer_handler(sessionProvider, &lineCode4B5B, &data);
}
```
Besides that you will need a timer interrupt, in which you can process the decoded data.
In this you have to call the validation layer and the application layer.
```c
void timerInterrupt(void){
    LRP_ReceiveValidatorLayer_handler(&sessionProvider);
}
```
