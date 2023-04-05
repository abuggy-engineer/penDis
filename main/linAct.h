class linAct
{
  private:

    int max_pwr;
    int max_pos, min_pos;
    float max_stroke;

  public:

    char ENA_PIN   ;//6 // Speed/Power to Motor
    char IN1_PIN   ;//7 // Polarity of Voltage ==> Direction (Fwd)
    char IN2_PIN   ;//8 // Polarity of Voltage ==> Direction (Rev)
    char POT   ;//A0 // the Arduino pin connected to the potentiometer of the actuator

    int pos;
    float stroke, vel;

    void init();
    void extend(float pwr);
    void retract(float pwr);
    void stop();
    void move(float pwr);
    unsigned int readPos();
    void calibrate();
    double pos2stroke(int position);
    int stroke2pos(double dist);
    void go2pos(float target); //input is in mm
    void set_pins(int ena, int in1, int in2, int pot);
    
};
