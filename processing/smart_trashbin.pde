import processing.serial.*;


int displayDuration = 200;
int timer = 0;
String val = null;
Serial myPort;
PImage bg;

void setup()
{
  size(400,266);
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
  printArray(Serial.list());
  bg = loadImage("./face.png");
}

void draw()
{
  background(bg);
  if (myPort.available() > 0)
  {
    val = myPort.readStringUntil('\n');
    timer = displayDuration;
  }
  if (val != null && timer > 0)
  {
    fill(0);
    textAlign(CENTER, CENTER);
    textSize(32);
    text(val, width / 2, height / 2 + 50);
    timer--;
  }
}
