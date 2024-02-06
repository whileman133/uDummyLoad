/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC18F24Q10
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.30 and above
        MPLAB 	          :  MPLAB X 5.40	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set LCD3_B aliases
#define LCD3_B_TRIS                 TRISAbits.TRISA0
#define LCD3_B_LAT                  LATAbits.LATA0
#define LCD3_B_PORT                 PORTAbits.RA0
#define LCD3_B_WPU                  WPUAbits.WPUA0
#define LCD3_B_OD                   ODCONAbits.ODCA0
#define LCD3_B_ANS                  ANSELAbits.ANSELA0
#define LCD3_B_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define LCD3_B_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define LCD3_B_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define LCD3_B_GetValue()           PORTAbits.RA0
#define LCD3_B_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define LCD3_B_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define LCD3_B_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define LCD3_B_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define LCD3_B_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define LCD3_B_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define LCD3_B_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define LCD3_B_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set LCD3_A aliases
#define LCD3_A_TRIS                 TRISAbits.TRISA1
#define LCD3_A_LAT                  LATAbits.LATA1
#define LCD3_A_PORT                 PORTAbits.RA1
#define LCD3_A_WPU                  WPUAbits.WPUA1
#define LCD3_A_OD                   ODCONAbits.ODCA1
#define LCD3_A_ANS                  ANSELAbits.ANSELA1
#define LCD3_A_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define LCD3_A_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define LCD3_A_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define LCD3_A_GetValue()           PORTAbits.RA1
#define LCD3_A_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define LCD3_A_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define LCD3_A_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define LCD3_A_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define LCD3_A_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define LCD3_A_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define LCD3_A_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define LCD3_A_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set LCD3_F aliases
#define LCD3_F_TRIS                 TRISAbits.TRISA2
#define LCD3_F_LAT                  LATAbits.LATA2
#define LCD3_F_PORT                 PORTAbits.RA2
#define LCD3_F_WPU                  WPUAbits.WPUA2
#define LCD3_F_OD                   ODCONAbits.ODCA2
#define LCD3_F_ANS                  ANSELAbits.ANSELA2
#define LCD3_F_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define LCD3_F_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define LCD3_F_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define LCD3_F_GetValue()           PORTAbits.RA2
#define LCD3_F_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define LCD3_F_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define LCD3_F_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define LCD3_F_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define LCD3_F_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define LCD3_F_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define LCD3_F_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define LCD3_F_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set RA3 procedures
#define RA3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define RA3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define RA3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define RA3_GetValue()              PORTAbits.RA3
#define RA3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define RA3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define RA3_SetPullup()             do { WPUAbits.WPUA3 = 1; } while(0)
#define RA3_ResetPullup()           do { WPUAbits.WPUA3 = 0; } while(0)
#define RA3_SetAnalogMode()         do { ANSELAbits.ANSELA3 = 1; } while(0)
#define RA3_SetDigitalMode()        do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set LCD3_G aliases
#define LCD3_G_TRIS                 TRISAbits.TRISA4
#define LCD3_G_LAT                  LATAbits.LATA4
#define LCD3_G_PORT                 PORTAbits.RA4
#define LCD3_G_WPU                  WPUAbits.WPUA4
#define LCD3_G_OD                   ODCONAbits.ODCA4
#define LCD3_G_ANS                  ANSELAbits.ANSELA4
#define LCD3_G_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define LCD3_G_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define LCD3_G_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define LCD3_G_GetValue()           PORTAbits.RA4
#define LCD3_G_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define LCD3_G_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define LCD3_G_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define LCD3_G_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define LCD3_G_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define LCD3_G_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define LCD3_G_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define LCD3_G_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set LCD3_C aliases
#define LCD3_C_TRIS                 TRISAbits.TRISA5
#define LCD3_C_LAT                  LATAbits.LATA5
#define LCD3_C_PORT                 PORTAbits.RA5
#define LCD3_C_WPU                  WPUAbits.WPUA5
#define LCD3_C_OD                   ODCONAbits.ODCA5
#define LCD3_C_ANS                  ANSELAbits.ANSELA5
#define LCD3_C_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define LCD3_C_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define LCD3_C_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define LCD3_C_GetValue()           PORTAbits.RA5
#define LCD3_C_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define LCD3_C_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define LCD3_C_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define LCD3_C_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define LCD3_C_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define LCD3_C_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define LCD3_C_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define LCD3_C_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set LCD3_E aliases
#define LCD3_E_TRIS                 TRISAbits.TRISA6
#define LCD3_E_LAT                  LATAbits.LATA6
#define LCD3_E_PORT                 PORTAbits.RA6
#define LCD3_E_WPU                  WPUAbits.WPUA6
#define LCD3_E_OD                   ODCONAbits.ODCA6
#define LCD3_E_ANS                  ANSELAbits.ANSELA6
#define LCD3_E_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define LCD3_E_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define LCD3_E_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define LCD3_E_GetValue()           PORTAbits.RA6
#define LCD3_E_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define LCD3_E_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define LCD3_E_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define LCD3_E_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define LCD3_E_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define LCD3_E_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define LCD3_E_SetAnalogMode()      do { ANSELAbits.ANSELA6 = 1; } while(0)
#define LCD3_E_SetDigitalMode()     do { ANSELAbits.ANSELA6 = 0; } while(0)

// get/set LCD3_D aliases
#define LCD3_D_TRIS                 TRISAbits.TRISA7
#define LCD3_D_LAT                  LATAbits.LATA7
#define LCD3_D_PORT                 PORTAbits.RA7
#define LCD3_D_WPU                  WPUAbits.WPUA7
#define LCD3_D_OD                   ODCONAbits.ODCA7
#define LCD3_D_ANS                  ANSELAbits.ANSELA7
#define LCD3_D_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define LCD3_D_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define LCD3_D_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define LCD3_D_GetValue()           PORTAbits.RA7
#define LCD3_D_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define LCD3_D_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define LCD3_D_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define LCD3_D_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define LCD3_D_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define LCD3_D_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define LCD3_D_SetAnalogMode()      do { ANSELAbits.ANSELA7 = 1; } while(0)
#define LCD3_D_SetDigitalMode()     do { ANSELAbits.ANSELA7 = 0; } while(0)

// get/set LCD1_C aliases
#define LCD1_C_TRIS                 TRISBbits.TRISB0
#define LCD1_C_LAT                  LATBbits.LATB0
#define LCD1_C_PORT                 PORTBbits.RB0
#define LCD1_C_WPU                  WPUBbits.WPUB0
#define LCD1_C_OD                   ODCONBbits.ODCB0
#define LCD1_C_ANS                  ANSELBbits.ANSELB0
#define LCD1_C_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define LCD1_C_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define LCD1_C_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define LCD1_C_GetValue()           PORTBbits.RB0
#define LCD1_C_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define LCD1_C_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define LCD1_C_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define LCD1_C_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define LCD1_C_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define LCD1_C_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define LCD1_C_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define LCD1_C_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set LCD1_B aliases
#define LCD1_B_TRIS                 TRISBbits.TRISB1
#define LCD1_B_LAT                  LATBbits.LATB1
#define LCD1_B_PORT                 PORTBbits.RB1
#define LCD1_B_WPU                  WPUBbits.WPUB1
#define LCD1_B_OD                   ODCONBbits.ODCB1
#define LCD1_B_ANS                  ANSELBbits.ANSELB1
#define LCD1_B_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define LCD1_B_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define LCD1_B_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define LCD1_B_GetValue()           PORTBbits.RB1
#define LCD1_B_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define LCD1_B_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define LCD1_B_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define LCD1_B_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define LCD1_B_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define LCD1_B_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define LCD1_B_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define LCD1_B_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set LCD1_ADEG aliases
#define LCD1_ADEG_TRIS                 TRISBbits.TRISB2
#define LCD1_ADEG_LAT                  LATBbits.LATB2
#define LCD1_ADEG_PORT                 PORTBbits.RB2
#define LCD1_ADEG_WPU                  WPUBbits.WPUB2
#define LCD1_ADEG_OD                   ODCONBbits.ODCB2
#define LCD1_ADEG_ANS                  ANSELBbits.ANSELB2
#define LCD1_ADEG_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define LCD1_ADEG_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define LCD1_ADEG_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define LCD1_ADEG_GetValue()           PORTBbits.RB2
#define LCD1_ADEG_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define LCD1_ADEG_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define LCD1_ADEG_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define LCD1_ADEG_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define LCD1_ADEG_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define LCD1_ADEG_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define LCD1_ADEG_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define LCD1_ADEG_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set LCD_COM aliases
#define LCD_COM_TRIS                 TRISBbits.TRISB3
#define LCD_COM_LAT                  LATBbits.LATB3
#define LCD_COM_PORT                 PORTBbits.RB3
#define LCD_COM_WPU                  WPUBbits.WPUB3
#define LCD_COM_OD                   ODCONBbits.ODCB3
#define LCD_COM_ANS                  ANSELBbits.ANSELB3
#define LCD_COM_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define LCD_COM_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define LCD_COM_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define LCD_COM_GetValue()           PORTBbits.RB3
#define LCD_COM_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define LCD_COM_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define LCD_COM_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define LCD_COM_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define LCD_COM_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define LCD_COM_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define LCD_COM_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define LCD_COM_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set VDIODE aliases
#define VDIODE_TRIS                 TRISBbits.TRISB4
#define VDIODE_LAT                  LATBbits.LATB4
#define VDIODE_PORT                 PORTBbits.RB4
#define VDIODE_WPU                  WPUBbits.WPUB4
#define VDIODE_OD                   ODCONBbits.ODCB4
#define VDIODE_ANS                  ANSELBbits.ANSELB4
#define VDIODE_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define VDIODE_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define VDIODE_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define VDIODE_GetValue()           PORTBbits.RB4
#define VDIODE_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define VDIODE_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define VDIODE_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define VDIODE_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define VDIODE_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define VDIODE_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define VDIODE_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define VDIODE_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set VDIVIDER aliases
#define VDIVIDER_TRIS                 TRISBbits.TRISB5
#define VDIVIDER_LAT                  LATBbits.LATB5
#define VDIVIDER_PORT                 PORTBbits.RB5
#define VDIVIDER_WPU                  WPUBbits.WPUB5
#define VDIVIDER_OD                   ODCONBbits.ODCB5
#define VDIVIDER_ANS                  ANSELBbits.ANSELB5
#define VDIVIDER_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define VDIVIDER_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define VDIVIDER_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define VDIVIDER_GetValue()           PORTBbits.RB5
#define VDIVIDER_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define VDIVIDER_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define VDIVIDER_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define VDIVIDER_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define VDIVIDER_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define VDIVIDER_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define VDIVIDER_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define VDIVIDER_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set CLK aliases
#define CLK_TRIS                 TRISBbits.TRISB6
#define CLK_LAT                  LATBbits.LATB6
#define CLK_PORT                 PORTBbits.RB6
#define CLK_WPU                  WPUBbits.WPUB6
#define CLK_OD                   ODCONBbits.ODCB6
#define CLK_ANS                  ANSELBbits.ANSELB6
#define CLK_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define CLK_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define CLK_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define CLK_GetValue()           PORTBbits.RB6
#define CLK_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define CLK_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define CLK_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define CLK_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define CLK_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define CLK_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define CLK_SetAnalogMode()      do { ANSELBbits.ANSELB6 = 1; } while(0)
#define CLK_SetDigitalMode()     do { ANSELBbits.ANSELB6 = 0; } while(0)

// get/set DAT aliases
#define DAT_TRIS                 TRISBbits.TRISB7
#define DAT_LAT                  LATBbits.LATB7
#define DAT_PORT                 PORTBbits.RB7
#define DAT_WPU                  WPUBbits.WPUB7
#define DAT_OD                   ODCONBbits.ODCB7
#define DAT_ANS                  ANSELBbits.ANSELB7
#define DAT_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define DAT_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define DAT_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define DAT_GetValue()           PORTBbits.RB7
#define DAT_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define DAT_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define DAT_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define DAT_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define DAT_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define DAT_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define DAT_SetAnalogMode()      do { ANSELBbits.ANSELB7 = 1; } while(0)
#define DAT_SetDigitalMode()     do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set LCD2_B aliases
#define LCD2_B_TRIS                 TRISCbits.TRISC0
#define LCD2_B_LAT                  LATCbits.LATC0
#define LCD2_B_PORT                 PORTCbits.RC0
#define LCD2_B_WPU                  WPUCbits.WPUC0
#define LCD2_B_OD                   ODCONCbits.ODCC0
#define LCD2_B_ANS                  ANSELCbits.ANSELC0
#define LCD2_B_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define LCD2_B_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define LCD2_B_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define LCD2_B_GetValue()           PORTCbits.RC0
#define LCD2_B_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define LCD2_B_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define LCD2_B_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define LCD2_B_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define LCD2_B_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define LCD2_B_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define LCD2_B_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define LCD2_B_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set LCD2_E aliases
#define LCD2_E_TRIS                 TRISCbits.TRISC1
#define LCD2_E_LAT                  LATCbits.LATC1
#define LCD2_E_PORT                 PORTCbits.RC1
#define LCD2_E_WPU                  WPUCbits.WPUC1
#define LCD2_E_OD                   ODCONCbits.ODCC1
#define LCD2_E_ANS                  ANSELCbits.ANSELC1
#define LCD2_E_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define LCD2_E_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define LCD2_E_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define LCD2_E_GetValue()           PORTCbits.RC1
#define LCD2_E_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define LCD2_E_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define LCD2_E_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define LCD2_E_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define LCD2_E_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define LCD2_E_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define LCD2_E_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define LCD2_E_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set LCD2_D aliases
#define LCD2_D_TRIS                 TRISCbits.TRISC2
#define LCD2_D_LAT                  LATCbits.LATC2
#define LCD2_D_PORT                 PORTCbits.RC2
#define LCD2_D_WPU                  WPUCbits.WPUC2
#define LCD2_D_OD                   ODCONCbits.ODCC2
#define LCD2_D_ANS                  ANSELCbits.ANSELC2
#define LCD2_D_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define LCD2_D_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define LCD2_D_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define LCD2_D_GetValue()           PORTCbits.RC2
#define LCD2_D_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define LCD2_D_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define LCD2_D_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define LCD2_D_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define LCD2_D_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define LCD2_D_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define LCD2_D_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define LCD2_D_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set LCD2_C aliases
#define LCD2_C_TRIS                 TRISCbits.TRISC3
#define LCD2_C_LAT                  LATCbits.LATC3
#define LCD2_C_PORT                 PORTCbits.RC3
#define LCD2_C_WPU                  WPUCbits.WPUC3
#define LCD2_C_OD                   ODCONCbits.ODCC3
#define LCD2_C_ANS                  ANSELCbits.ANSELC3
#define LCD2_C_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define LCD2_C_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define LCD2_C_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define LCD2_C_GetValue()           PORTCbits.RC3
#define LCD2_C_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define LCD2_C_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define LCD2_C_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define LCD2_C_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define LCD2_C_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define LCD2_C_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define LCD2_C_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define LCD2_C_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set LCD2_A aliases
#define LCD2_A_TRIS                 TRISCbits.TRISC4
#define LCD2_A_LAT                  LATCbits.LATC4
#define LCD2_A_PORT                 PORTCbits.RC4
#define LCD2_A_WPU                  WPUCbits.WPUC4
#define LCD2_A_OD                   ODCONCbits.ODCC4
#define LCD2_A_ANS                  ANSELCbits.ANSELC4
#define LCD2_A_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define LCD2_A_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define LCD2_A_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define LCD2_A_GetValue()           PORTCbits.RC4
#define LCD2_A_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define LCD2_A_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define LCD2_A_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define LCD2_A_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define LCD2_A_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define LCD2_A_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define LCD2_A_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define LCD2_A_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set LCD2_F aliases
#define LCD2_F_TRIS                 TRISCbits.TRISC5
#define LCD2_F_LAT                  LATCbits.LATC5
#define LCD2_F_PORT                 PORTCbits.RC5
#define LCD2_F_WPU                  WPUCbits.WPUC5
#define LCD2_F_OD                   ODCONCbits.ODCC5
#define LCD2_F_ANS                  ANSELCbits.ANSELC5
#define LCD2_F_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define LCD2_F_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define LCD2_F_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define LCD2_F_GetValue()           PORTCbits.RC5
#define LCD2_F_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define LCD2_F_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define LCD2_F_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define LCD2_F_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define LCD2_F_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define LCD2_F_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define LCD2_F_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define LCD2_F_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set LCD2_G aliases
#define LCD2_G_TRIS                 TRISCbits.TRISC6
#define LCD2_G_LAT                  LATCbits.LATC6
#define LCD2_G_PORT                 PORTCbits.RC6
#define LCD2_G_WPU                  WPUCbits.WPUC6
#define LCD2_G_OD                   ODCONCbits.ODCC6
#define LCD2_G_ANS                  ANSELCbits.ANSELC6
#define LCD2_G_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define LCD2_G_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define LCD2_G_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define LCD2_G_GetValue()           PORTCbits.RC6
#define LCD2_G_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define LCD2_G_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define LCD2_G_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define LCD2_G_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define LCD2_G_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define LCD2_G_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define LCD2_G_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define LCD2_G_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set LCD_DP2 aliases
#define LCD_DP2_TRIS                 TRISCbits.TRISC7
#define LCD_DP2_LAT                  LATCbits.LATC7
#define LCD_DP2_PORT                 PORTCbits.RC7
#define LCD_DP2_WPU                  WPUCbits.WPUC7
#define LCD_DP2_OD                   ODCONCbits.ODCC7
#define LCD_DP2_ANS                  ANSELCbits.ANSELC7
#define LCD_DP2_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define LCD_DP2_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define LCD_DP2_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define LCD_DP2_GetValue()           PORTCbits.RC7
#define LCD_DP2_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define LCD_DP2_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define LCD_DP2_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define LCD_DP2_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define LCD_DP2_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define LCD_DP2_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define LCD_DP2_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define LCD_DP2_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/