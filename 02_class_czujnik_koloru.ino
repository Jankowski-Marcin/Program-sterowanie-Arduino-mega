#define MAX_COLORS_CNT 5

class Color_sensor{
  private:
    uint16_t last_read;
    uint16_t pin;
    uint16_t color_max[MAX_COLORS_CNT];
    uint16_t color_min[MAX_COLORS_CNT];
    public:
    uint8_t color_cnt;
  public:
    Color_sensor(uint16_t analog_pin){
      pin=analog_pin;
      pinMode(pin,INPUT);
    }
    void add_color(uint16_t new_min, uint16_t new_max){
      color_max[color_cnt]=new_max;
      color_min[color_cnt]=new_min;
      ++color_cnt;
    }
    int8_t get_color(void){
      last_read=analogRead(pin);
      for(uint8_t i=0;i<color_cnt;++i){
        if(last_read<color_max[i] && last_read>color_min[i]){
          return i;
        }
      }
      return -1;
    }
};
