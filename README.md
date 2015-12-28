Noduino SDK
===========================================
[![Build Status](https://travis-ci.org/icamgo/noduino-sdk.svg)](https://travis-ci.org/icamgo/noduino-sdk) 


This is a Professional SDK developed by MaiKe Labs for ESP8266 WiFi SoC

This project brings support for ESP8266 chip to the professional embeded environment. It lets you write sketches using C language quickly, and run them directly on ESP8266, no external microcontroller required.

It has a lot of cool examples and reference sketches. You can make a smart IoT devices quickly based on these sketches.

It's based on Espressif SDK 1.4.0


### Quick Start

- Noduino Falcon board / NodeMCU v1.0 board
- Git

```bash
# clone the whole sdk
$ git clone --recursive git://github.com/icamgo/noduino-sdk.git noduino-sdk

# generate the toolchain (you need Python 2.7)
$ cd noduino-sdk/toolchain
$ ./gen.py

# try the first example
$ cd ../example/noduino/blink
$ make

# upload the example to dev board through serial
$ make flash
```

You can try other cool examples in example directory

### Reference

* [Noduino](http://noduino.org)
* [ESP8266 WIKI](http://wiki.jackslab.org/ESP8266)
