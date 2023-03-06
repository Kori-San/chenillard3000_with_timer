#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>

//Global Var
unsigned int led = 1;
unsigned int OldState_Button = 0;
unsigned int Sequence = 1 ;

//Function Prototype
void Init_Chenillard (void); //Function that init the LEDs
void Init_BP(void);
void Lights_Off (void); //Make all LEDs goes ooff
void init_TimerA0_Mode_Up_CCR0_Interrupt(void);
unsigned int GetState_Button(void);
unsigned int Push_BP(void);

//Main Loop
int main(void)
{
	WDTCTL = WDTPW +WDTHOLD; // Watchdog Timer Stop
	Init_BP();//Push Button init
	Init_Chenillard(); //Chenillard init
	init_TimerA0_Mode_Up_CCR0_Interrupt(); //Init TIMER Interruption
	_BIS_SR(GIE); //Allow Interrputions
	OldState_Button  = GetState_Button();
    while(1)
    {
     //Peux rester vide
    }
}

//Fonctions
#pragma vector=TIMER0_A0_VECTOR //Interrupt with TA0CCR0
__interrupt void Timer_A (void)
{
  Lights_Off ();
  Push_BP();
  if ( Sequence == 1 )
  {
   switch(led) //LED var increment with each interrupt
   {
	case 1 : P3OUT |= (1<<7) ; led = led + 1; break;
	case 2 : P4OUT |= (1<<0) ; led = led + 1; break;
	case 3 : P4OUT |= (1<<3) ; led = led + 1; break;
	case 4 : P1OUT |= (1<<2) ; led = led + 1; break;
	case 5 : P3OUT |= (1<<0) ; led = led + 1; break;
	case 6 : P3OUT |= (1<<1) ; led = led + 1; break;
	case 7 : P2OUT |= (1<<6) ; led = led + 1; break;
	case 8 : P2OUT |= (1<<3) ; led = 1; break;
   }
  }
  else if ( Sequence == 2 )
  {
   switch(led)
   {
	case 1 : P3OUT |= (1<<7) ; P2OUT |= (1<<3) ; led = led + 1; break;
	case 2 : P4OUT |= (1<<0) ; P2OUT |= (1<<6) ; led = led + 1; break;
	case 3 : P4OUT |= (1<<3) ; P3OUT |= (1<<1) ; led = led + 1; break;
	case 4 : P1OUT |= (1<<2) ; P3OUT |= (1<<0) ; led = 1; break;
   }
  }
  Push_BP();
}

 void init_TimerA0_Mode_Up_CCR0_Interrupt (void) //Init TIMERA0 interupt
 {
	TA0CTL |=(1<<9);
	TA0CTL =TA0CTL&(~(1<<8));
	TA0CTL =TA0CTL&(~(1<<7));
	TA0CTL =TA0CTL&(~(1<<6));
	TA0CTL = TA0CTL &(~(1<<5));
	TA0CTL = TA0CTL|(1<<4);
	TA0CCR0 = 65535; //150ms
	TA0CCTL0 = TA0CCTL0|(1<<4);
 }

 void Init_BP(void)
 {
	P2DIR &= ~ (1<<5);
	P2REN |= (1<<5);
	P2OUT |= (1<<5); // P2.5 Out
	P2DIR |= (1<<0); // P2.0 Out
 }

 void Init_Chenillard (void)
 {
  //PINS P1.2,P4.3,P4.0,P3.7 and P3.0,P3.1,P2.6,P2.3
  P1DIR |= (1<<2); // P1.2 Out LED:1
  P3DIR |= (1<<0); // P3.0 Out LED:2
  P4DIR |= (1<<3); // P4.3 Out LED:3
  P3DIR |= (1<<1); // P3.1 Out LED:4
  P4DIR |= (1<<0); // P4.0 Out LED:5
  P2DIR |= (1<<6); // P2.6 Out LED:6
  P3DIR |= (1<<7); // P3.7 Out LED:7
  P2DIR |= (1<<3); // P3.3 Out LED:8
 }

 unsigned int GetState_Button(void)
 {
  unsigned int state;
  if (!((P2IN & (1<<5)) == (1<<5)) ) {state=0;}
  else {state=1;}
  return state;
 }

 void Lights_Off (void)
 {
  P3OUT &=  ~(1<<7);
  P2OUT &=  ~(1<<3);
  P4OUT &=  ~(1<<0);
  P2OUT &=  ~(1<<6);
  P4OUT &=  ~(1<<3);
  P3OUT &=  ~(1<<1);
  P1OUT &=  ~(1<<2);
  P3OUT &=  ~(1<<0);
 }

 unsigned int Push_BP(void)
 {
  if ((OldState_Button==1)&&(GetState_Button()==0))
  {
   if (Sequence == 1) { Sequence = 2; }
   else if (Sequence == 2) { Sequence = 1; }
   led = 1 ;
   Lights_Off();
   OldState_Button  = GetState_Button();
   return EXIT_SUCCESS;
  }
  OldState_Button  = GetState_Button();
  return EXIT_FAILURE;
 }
