//ccs6 written by David Whitfield
//The program decodes an IR signal with the TSOP32438

#include "msp430.h"

#define LED0 BIT3 //red LED
#define LEDGND BIT4 //green LED
#define monkey BIT5 //relay1 for light 1
#define IRrec BIT6 //connected to IR receiver for interrupt on rising edge of pulse
#define IRrec1 BIT7 //connected to IR receiver for interrupt on falling edge of pulse

int bit =0; //single bit to transmit
int bitt =0; //single bit used to switch bits to descending order
int count =0; //timer used to see delay between rising edge and falling edge of pulse
int resetcount =0; //timer to reset the loopcount, modify this to change how fast bits can be read
int loopcount=0; //the bit being decoded clocked by IR receiver edges
int loopcountMAX=0; //max loop count, should be 66 or increase reset count timing
int one =0; //check to see first decode value (used for debug)
int two=0; //check to see second decode value (used for debug)
int three =0; //check to see third decode value (used for debug)
int four=0; //check to see fourth decode value (used for debug)
int five=0; //check to see fifth decode value (used for debug)
int six=0; //check to see sixth decode value (used for debug)
int seven=0; //check to see seventh decode value (used for debug)
int eight =0; //check to see eigth decode value (used for debug)
unsigned long ledcount =0; //led off timer
int odd=0; //used to read IR data
unsigned long realdata =0; //complete data
unsigned long rxdata =0; //uncomplete data



void main(void){

  WDTCTL = WDTPW + WDTHOLD;                            // Stop watchdog timer

  P1DIR |= (monkey + LEDGND + LED0);     //output direction                       // Set all pins but RXD to output
  P1OUT =0;
  P1IE |= IRrec; // P1.3 interrupt enabled
  P1IES &= ~IRrec; // P1.3 interrupt enabled
  P1IFG &= ~IRrec; // P1.3 IFG cleared
  P1IE |= IRrec1; // P1.3 interrupt enabled
  P1IES |= IRrec1; // P1.3 interrupt enabled
  P1IFG &= ~IRrec1; // P1.3 IFG cleared

  TA0CCTL0 |= CCIE;                             // CCR0 interrupt enabled
  TA0CTL = TASSEL_2 + MC_1 + ID_2;           // SMCLK/8, upmode
  TA0CCR0 =  20;							//count up to 20



    __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts



  while(1)
  {
    _delay_cycles(100);

    if(resetcount == 200)
    {
      loopcountMAX=loopcount;
      loopcount =0;
      realdata = rxdata >> 1;

    }

    if(realdata == 0xFF17943 || realdata == 0xFF46731) //remote 1 code
    {
      P1OUT |=monkey;
      realdata =0;
      P1OUT |= LED0;
      ledcount=0;
    }

    if(ledcount ==40000)
    {
      P1OUT &=~LED0;
      P1OUT &=~monkey;
    }
    ledcount++;
    if(ledcount >500000)
    {
      ledcount =500000;
    }
    resetcount++;
    if(resetcount >1000)
    {
      resetcount =999;
    }
    if(count >1000)
    {
      count =999;
    }

  }
}




#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  TA0CTL |= TACLR;

  if(loopcount == 0)
  {
    rxdata =0;
  }
  if (loopcount == 50)
  {
    one = count;
  }
  else if (loopcount ==51)
  {
    two = count;
  }
  else if(loopcount ==52)
  {
    three = count;
  }
  else if(loopcount ==53)
  {
    four = count;
  }
  else if(loopcount == 54)
  {
    five = count;

  }
  else if(loopcount == 55)
  {
    six = count;

  }
  else if(loopcount == 56)
  {
    seven = count;

  }
  else if(loopcount == 57)
  {
    eight = count;

  }

  odd = (loopcount & 0x1);
  if(odd == 0)
  {
    if(count >13 && count < 24)
    {
      bit = 1;
    }
    else
    {
      bit = 0;
    }

    rxdata += bit;
    rxdata<<=1;
  }

  loopcount++;
  count=0;
  resetcount=0;
  P1IFG &= ~IRrec; // P1.3 IFG cleared
  P1IFG &= ~IRrec1; // P1.3 IFG cleared
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  count++;
}
