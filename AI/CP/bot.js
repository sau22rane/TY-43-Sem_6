class Bot{
  constructor(brain) {
    this.r = 32;
    this.y = height - this.r/2;
    this.x = 64;

    this.gravity = 0.9;
    this.vy = 0;
    this.vx = 0;

    this.ax = 0;
    this.ay = 0;

    this.score = 0;
    this.fitness = 0;

    if (brain){
      this.brain = brain.copy();
    } 
    else{
      this.brain = new NeuralNetwork(6, 8, 2);
    }
  }

  dispose(){
    this.brain.dispose();
  }

  show(){
    stroke(81, 219, 146);
    fill(81, 219, 146, 100);
    ellipse(this.x, this.y, this.r, this.r);
  }

  mutate() {
    this.brain.mutate(0.1);
  }

  think(bars) {
    let closestBar = null;
    let closestD = Infinity;
    for (let i = 0; i < bars.length; i++) {
      let d = bars[i].x + bars[i].w - this.x;
      if (d < closestD && d > 0) {
        closestBar = bars[i];
        closestD = d;
      }
    }

    let posBar = 0
    let heiBar = 0
    if (closestBar){
      if(closestBar.x<=width){
      posBar = closestBar.x;
      heiBar = closestBar.top;
      }
    }

    if(this.y >= height - this.r/2){
      let inputs = [];
      inputs[0] = this.vx / 10;
      inputs[1] = this.vy / 10;
      inputs[2] = this.x/width; 
      inputs[3] = this.y/height; 
      inputs[4] = posBar/ width;
      inputs[5] = heiBar/ height;

      let output = this.brain.predict(inputs);
      // if(output[0]>0.10) this.ax = 1;
      // else if(output[0]<-0.10) this.ax = -1;
      // else this.ax = 0;


      // if(output[1]>0.10) this.ay = 1;
      // else if(output[1]<-0.10) this.ay = -1;
      // else this.ay = 0;

      this.ay = output[1]*1.7;
      // console.log(this.ax+" "+this.ay);
    }
    else{
      this.ax = 0;
      this.ay = 0;
    }
  }

  offScreen(){
    if(this.y < 0) return true;
    if(this.x > width || this.x < 0) return true;
    return false;
  }

  hitGround(){
    if(this.y + this.r/2>=height) return true;
    return false;
  }

  update(){
    this.score++;

    this.vy = this.vy+this.gravity-this.ay;
    this.y+=this.vy;

    this.vx+=this.ax;
    this.x+=this.vx;
  }
}
