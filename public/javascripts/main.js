'use strict';

// socket io
var socket = io();


const TL_X = 290;
const TL_Y = 37;
const TR_X = 824;
const TR_Y = 126;
const BL_X = 321;
const BL_Y = 738;
const BR_X = 836;
const BR_Y = 704;
const BOX_SIZE = 20;


socket.on('point-client', function (data) {
  // convert coordinates from steps to pixels
  data.x = Math.floor(data.x/5.5);
  data.y = Math.floor(data.y/5.5);
  queue.push(data);
});

socket.on('config-client', function (data) {
  console.log(data);
});

// canvas script
var tailLength = 10;

var queue = [];

function setup() {
  createCanvas(window.innerWidth, window.innerHeight);

  createDiv('').id("topleft").class("draggable");
  createDiv('').id("topright").class("draggable");
  createDiv('').id("bottomleft").class("draggable");
  createDiv('').id("bottomright").class("draggable");


  $( ".draggable" ).draggable();
  $( "#topleft" ).position({
    my:"bottom right",
    at:"left+"+TL_X+" top+"+TL_Y,
    of:"body"
  });
  $( "#topright" ).position({
    my:"bottom left",
    at:"left+"+TR_X+" top+"+TR_Y,
    of:"body"
  });
  $( "#bottomleft" ).position({
    my:"top right",
    at:"left+"+BL_X+" top+"+BL_Y,
    of:"body"
  });
  $( "#bottomright" ).position({
    my:"top left",
    at:"left+"+BR_X+" top+"+BR_Y,
    of:"body"
  });

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

  //create a black outline for the airbrush head.
  fill(0, 0, 0, 255);
  ellipse(point1.x, point1.y, point1.dotDiameter*11/10, point1.dotDiameter*11/10);
  fill(255, 255, 255, 255);

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