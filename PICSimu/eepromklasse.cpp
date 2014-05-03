#include "eepromklasse.h"

EEPROMKlasse::EEPROMKlasse(QObject *parent) :
    QObject(parent)
{
}

EEPROMKlasse::EEPROMKlasse(int *eecon1, int *eedata, int *eeadr)
{
    //Memory auf 0 Attribute initialisieren
}
 void EEPROMKlasse::eepromRead()
 {
     while(1)
     {
         if(*eecon1&0x0001)// RD Bit ist 1
         {
             if(*eeadr > EEPROM_SIZE)
                 *eedata=0;
             else
                 *eedata = eeprom[*eeadr];
             *eecon1 = *eecon1 & 0xfffe; //lösche RD bit
         }
     }
 }

 void EEPROMKlasse::eepromWrite()
 {
     while(1)
     {
         if(eecon2==0x0055)
         {
             while(1)
             {
                 if(eecon2==0x00aa)
                 {
                     if((*eecon1&0x0006)==0x0006) //WREN und WR bit gesetzt?
                     {
                         if(*eeadr <=EEPROM_SIZE)
                         {
                             eeprom[*eeadr]= *eedata;
                         }

                         //clear WR
                         *eecon1 &=  0xfffd;

                         //set EEIF
                         *eecon1 |= 0x0010;

                     }

                     break;
                 }
             }
         }

     }
 }
