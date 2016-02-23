# Project Express
Based off the work done by Jonathan Zong: (https://github.com/jonathanzong/project-express)
Node server, socket.io/serial adapter, and arduino sketches for Project Express

`grep` for 'TODO' to find some unfinished stuff
   
## Getting Started
* Installation
  * Install `node` and `npm` (developed on `node 0.12` and `npm 2.14`)
  * Run `npm install` in the project directory
* Start the server: `node bin/www`
  * The server is now running at `localhost:3000`
  * `/` serves the dot drawing page for the projector (scripts for this page live in `public/javascripts/main.js`)
  * `/client.html` is a mouse/tablet input page that sends mouse position, etc to the server over socket.io
* Start the serial adapter: `node bin/arduino-adapter`
  * The serial port name is currently hardcode it, change it to what the Arduino is connected to
  * It will establish a socket connection to the server and a serial connection to Arduino
* Make sure the Arduino has `arduino/motor_controller/motor_controller.ino` uploaded

## Hardware Pinouts

TODO
