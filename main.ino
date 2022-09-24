#define STRIP_PIN 2     // пин ленты
#define NUMLEDS 858      // кол-во светодиодов
#define NUMLEDS_PERIMETR 699     
#define SPEED_START 1
#define SPEED_ALWAYS 5
#define engle_koef 0.7


#define COLOR_DEBTH 3
#include <microLED.h>   // подключаем библу
microLED<0, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

const int GRAD_PERIOD = 175;
mData gr[GRAD_PERIOD];
mData gr_less_brightness[GRAD_PERIOD];

void init_gradient(){
  mGradient< 4 > myGrad;
  myGrad.colors[0] = mRGB(255, 255, 255);
  myGrad.colors[1] = mRGB(100, 200, 255);
  myGrad.colors[2] = mRGB(0, 0, 255);
  myGrad.colors[3] = mRGB(255, 255, 255);
  for ( int i = 0; i < GRAD_PERIOD; i++){
    mData normal_brightness = myGrad.get(i, GRAD_PERIOD);
    mData less_brightness = normal_brightness;
    less_brightness.r = less_brightness.r*engle_koef;
    less_brightness.g = less_brightness.g*engle_koef;
    less_brightness.b = less_brightness.b*engle_koef;
    gr[i] = normal_brightness;
    gr_less_brightness[i] = less_brightness;
  }
}

void start_perimetr(){
  const int start_num = 563; 
  const int offset = NUMLEDS_PERIMETR - start_num;
  const int NUMLEDS_HALFPERIMETR = 294; 
  const int fullfill_count = NUMLEDS_PERIMETR - NUMLEDS_HALFPERIMETR * 2 + 2;
  for (int j = 70; j <= NUMLEDS_HALFPERIMETR; j+=3){
    const int left_border = (j < offset)?j:j+fullfill_count;
    const int right_border = NUMLEDS_PERIMETR - j;
    const int NUMLEDS_offset = NUMLEDS + offset;
    const int NUMLEDS_PERIMETR_offset = NUMLEDS_PERIMETR + offset;
    strip.begin();
    for (int i = offset; i < NUMLEDS_offset; i++) {
      strip.send(((i%NUMLEDS_PERIMETR < left_border || i%NUMLEDS_PERIMETR > right_border) && i < NUMLEDS_PERIMETR_offset)?gr[(i-offset)%GRAD_PERIOD]:mBlack);
    }
    strip.end();  
    delay(SPEED_START);
  }  
}

void start_angle() {
    for (int j = NUMLEDS - 1; j >= NUMLEDS_PERIMETR; j--){
      strip.begin();
      for (int i = 0; i < NUMLEDS; i++) {
        int index = i%GRAD_PERIOD;
        strip.send(i < NUMLEDS_PERIMETR?gr[index]:(i >= j?gr_less_brightness[index]:mBlack));
      }
      strip.end();  
      delay(SPEED_START);
    }  
}

void setup() {
  strip.setBrightness(120);
  init_gradient();
  start_perimetr();
  start_angle();
  delay(1000);
}

int offset = 0;
void loop() {
  strip.begin();
  for (int i = 0; i < NUMLEDS; i++) {
    int index = (i + offset)%GRAD_PERIOD;
    strip.send((i < NUMLEDS_PERIMETR)?gr[index]:gr_less_brightness[index]);
  }
  strip.end();
  delay(SPEED_ALWAYS);      // 30 кадров в секунду
  offset ++;
  if( offset >= GRAD_PERIOD){
    offset = 0; 
  }
}
