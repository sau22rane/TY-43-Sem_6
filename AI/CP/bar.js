class Bar{
  constructor(pos){
    // console.log(height);
    this.top = random(height/5,height/3);
    this.x = width + pos;
    this.w = 180 * height/4 / this.top;
    this.speed = 8;
    if(Math.random()*10 > 3){
      this.y = height-this.top;
      this.inverted = 0;
    }
    else{
      this.y = 0;
      this.inverted = 1;
    }
  }

  hits(bot){
    if (bot.y > this.y && bot.y<(this.y+this.top+bot.r/2)){
      if (bot.x > this.x && bot.x < this.x + this.w){
        return true;
      }
    }
    return false;
  }

  show(){
    stroke(240, 38, 95);
    fill(240, 38, 95);
    rectMode(CORNER);
    console.log(this.x+" "+this.y+" "+this.w+" "+this.top);
    rect(this.x, this.y, this.w, this.top);
  }

  update(){
    this.x -= this.speed;
  }

  offscreen(){
    if (this.x < -this.w) {
      return true;
    } 
    return false;
  }
}
