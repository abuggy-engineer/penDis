class acs712
{ 
    private:
        int acs_offset = 2.5 ;
        float sensitivity = 0.185; // V/A
        int pin;

    public:

        double curr;

        void set_pin(int x);
        void readCurr();
        void printCurr();
};