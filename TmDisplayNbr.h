#ifndef TM_DISPLAY_NBR
#define TM_DISPLAY_NBR

#include <TM1640.h>

// !!! ARDUINO UNO --> INT = 16 BITS
#define ARDUINO_UNO

#ifdef ARDUINO_UNO
typedef unsigned long LTM_INT32;
#else
typedef unsigned int LTM_INT32;
#endif


LTM_INT32 TDC_NBR_BITMAPS[20] = // http://www.coranac.com/tonc/text/text.htm
{
    0x7E76663C, 0x003C666E, 
    0x181E1C18, 0x00181818, 
    0x3060663C, 0x007E0C18, 
    0x3860663C, 0x003C6660, 
    0x33363C38, 0x0030307F, 
    0x603E067E, 0x003C6660, 
    0x3E060C38, 0x003C6666, 
    0x3060607E, 0x00181818,
    0x3C66663C, 0x003C6666,
    0x7C66663C, 0x001C3060
};


class TmPair
{

public:
    TmPair(TM1640 left, TM1640 right, bool isRed = true) : 
        left(left), right(right), isRed(isRed){}

        void set(int displayed)
        { 
            this->displayed = displayed; 
            this->display();
        }

        int get()
        { 
            return this->displayed; 
        }

        void setColor(bool isRed)
        {
            this->isRed = isRed;
        }

        void display()
        {
            this->displayNumber(this->left, this->displayed / 10);
            this->displayNumber(this->right, this->displayed % 10);
        }

        void clear()
        {
            this->left.clearDisplay();
            this->right.clearDisplay();
        }
        
private:

    void displayNumber(TM1640 &module, int nbr)
    {
        nbr *= 2; 
        module.clearDisplay();
        this->displayPart(module, TDC_NBR_BITMAPS[nbr], true);
        this->displayPart(module, TDC_NBR_BITMAPS[nbr+1], false);  
    }

    void displayPart(TM1640 &module, LTM_INT32 bitmap, bool upperPart)
    {
        for(int i = 3; i >= 0; i--)
        {
            module.setRow(this->isRed ? 1 : 2, i + (upperPart ? 4 : 0), bitmap);
            bitmap = bitmap >> 8;
        }
    }

    TM1640 left, right;
    bool isRed;
    int displayed;
};
   
#endif