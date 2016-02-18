'use strict';

// socket io
var socket = io();

socket.on('point-client', function (data) {
  // console.log(data);
  queue.push(data);
});

// canvas script
var tailLength = 10;

var queue = [];

function setup() {
  createCanvas(window.innerWidth, window.innerHeight);
}

var pointBegin, point1, point2, pointEnd;
function draw() {
  clear();
  background('black');
  
  // delete oldest points
  while (queue.length > tailLength) queue.shift();

  // always draw most recent point
  if (queue.length > 0) {
    point1 = queue[queue.length - 1];
    noStroke();
	
	//create a white outline for the airbrush head. 
	fill(255, 255, 255, 255);
	ellipse(point1.x, point1.y, point1.dotDiameter*11/10, point1.dotDiameter*11/10);
	fill(0, 0, 0, 255);
    ellipse(point1.x, point1.y, point1.dotDiameter, point1.dotDiameter);
	
	//fill based on fill opacity and color
    fill(point1.color.r, point1.color.g, point1.color.b, point1.color.a);
    ellipse(point1.x, point1.y, point1.dotDiameter, point1.dotDiameter);

  }

  // draw point trail
  noFill();
  for (var i = 0; i < queue.length - 1; i++) {
    pointBegin = i == 0 ? queue[0] : queue[i - 1];
    point1 = queue[i];
    point2 = queue[i + 1];
    pointEnd = i == queue.length - 2 ? queue[i + 1] : queue[i + 2];
    strokeWeight(point2.dotDiameter);
    stroke(point2.color.r, point2.color.g, point2.color.b, point2.color.a*(i/queue.length));
    curve(pointBegin.x, pointBegin.y,
      point1.x, point1.y,
      point2.x, point2.y,
      pointEnd.x, pointEnd.y);
  }
}