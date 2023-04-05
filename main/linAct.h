class linAct
{
  private:

    #define ENA_PIN   6 // Speed/Power to Motor
    #define IN1_PIN   7 // Polarity of Voltage ==> Direction (Fwd)
    #define IN2_PIN   8 // Polarity of Voltage ==> Direction (Rev)
    #define POT   A0 // the Arduino pin connected to the potentiometer of the actuator

    int max_pwr;
    int max_pos, min_pos;
    float max_stroke;

    unsigned int t0, t1;
    double x0, x1;
    double positionArray[100];
    unsigned int timeArray[100];
    
    float error_prev, integral_pos, kp, ki, kd;

  public:

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
    void calcVel(); //mm and ms
    double average(double array[], int size);
    
};
