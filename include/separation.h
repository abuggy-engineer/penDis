#include "linAct.h"

class separation 
{
    private:

        bool ppStatus_right = 0;
        bool ppStatus_left = 0;

    public:

        #define PPR_LIMIT 27
        #define PPL_LIMIT 13

        linAct vertWedge;
        linAct ppL;
        linAct ppR;

        void init();
        void capRemoval();
        void pinPress();
        void pinPress_limit();
        void pp_limit_right();
        void pp_limit_left();
        void print_stroke();
        void test_act(linAct l1);
        
};