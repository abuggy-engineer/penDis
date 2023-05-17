#include "linAct.h"

class separation 
{
    private:

        #define PPR_LIMIT 13
        #define PPL_LIMIT 14

        bool ppStatus_right = 0;
        bool ppStatus_left = 0;

    public:

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
        void test_act(linAct);
        
};