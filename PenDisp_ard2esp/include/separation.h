#include "linAct.h"

class separation 
{
    public:

        linAct vertWedge;
        linAct ppL;
        linAct ppR;

        void init();
        void capRemoval();
        void pinPress();
        void print_stroke();
        void test_act(linAct);
};