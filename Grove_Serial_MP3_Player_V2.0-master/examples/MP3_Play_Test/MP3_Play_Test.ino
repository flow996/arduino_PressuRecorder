/*
 * MP3_Play_Test.ino
 * A quick start example for Grove-Serial MP3 Player V2.0
 * Note: The MP3 chip of Grove-Serial MP3 Player V2.0 is different from Grove-Serial MP3 Player V1.0
 * Description: This demo let you can send instruction 1-8 to control the Grove-Serial MP3 Player, via the serial port.
 *
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Wuruibin
 * Created Time: Dec 2015
 * Modified Time:
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Wire.h>
#include <SoftwareSerial.h>
#include <MP3Player_KT403A.h>

static uint8_t recv_cmd[8] = {};


// Note: You must define a SoftwareSerial class object that the name must be mp3, 
//       but you can change the pin number according to the actual situation.
SoftwareSerial mp3(2, 3);

void setup()
{
    mp3.begin(9600);
    Serial.begin(9600); 
    
    while(!Serial);

    Serial.println("Grove - Serial MP3 Demo");
    Serial.println(
        "Input command:\r\n\r\n"
        "P[ ] play music by default index\r\n"
        "Pm[ ] play music in MP3 folder by index\r\n"
        "Pf[ ][ ] play music by specify folder and index\r\n"        
        "p Pause\r\n"
        "R Resume\r\n"
        "N Next\r\n"
        "L Previous\r\n"
        "l Loop\r\n"
        "I Increase volume\r\n"
        "D Decrease volumern\r\n");
    
    delay(100);
    
    SelectPlayerDevice(0x02);       // Select SD card as the player device.
    SetVolume(0x0E);                // Set the volume, the range is 0x00 to 0x1E.
}

void loop()
{
    uint8_t len = 0;
    uint8_t i;

    if(Serial.available())
    {
        char chr = '\0';
        while(chr != '\n')  // Blockly read data from serial monitor
        {
            chr = Serial.read();
            // Serial.print(chr);
            recv_cmd[len++] = chr;        
        }
    }

    if(len > 0)
    {
        // Print reveiced data    
        // Serial.print("Received cmd: ");   
        // for(i = 0; i < len; i++) {
        //     Serial.print(recv_cmd[i]);
        //     Serial.print(" ");
        // }
        // Serial.println();
            
        switch (recv_cmd[0])
        {
            case 'P':
                if(recv_cmd[1] == 'm') 
                {
                    /** 
                      * Play music in "MP3" folder by index 
                      * example:
                      * "Pm1" -> ./MP3/0001.mp3
                    */
                    PlayMP3folder(recv_cmd[2] - '0');
                    Serial.print("Play ");
                    Serial.write(recv_cmd[2]);
                    Serial.println(".mp3 in MP3 folder");
                } 
                else if(recv_cmd[1] == 'f')
                {
                    /** 
                      * Play specify folder and music
                      * example:
                      * "Pf11" -> ./01/001***.mp3
                    */
                    SpecifyfolderPlay(recv_cmd[2] - '0',recv_cmd[3] - '0');
                    Serial.print("Play ");
                    Serial.write(recv_cmd[3]);
                    Serial.print("xxx.mp3");
                    Serial.print(" in folder ");
                    Serial.write(recv_cmd[2]);
                    Serial.println();
                    
                } 
                else
                {
                    /** 
                      * Play music by default index
                      * example:
                      * "P1" -> ./***.mp3
                    */                
                    SpecifyMusicPlay(recv_cmd[1] - '0');
                    Serial.print("Play xxx.MP3 by index ");
                    Serial.write(recv_cmd[1]);
                    Serial.println();
                }            
                // Serial.println("Specify the music index to play");
                break;
            case 'p':
                PlayPause();            
                Serial.println("Pause the MP3 player");
                break;
            case 'R':            
                PlayResume();
                Serial.println("Resume the MP3 player");
                break;
            case 'N':            
                PlayNext();
                Serial.println("Play the next song");
                break;
            case 'L':
                PlayPrevious();
                Serial.println("Play the previous song");
                break;
            case 'l':
                PlayLoop();
                Serial.println("Play loop for all the songs");
                break;
            case 'I':
                IncreaseVolume();
                Serial.println("Increase volume");
                break;
            case 'D':
                DecreaseVolume();
                Serial.println("Decrease volume");
                break;
            default:
                break;
        }

        // clean data buffer
        for(i = 0; i < sizeof(recv_cmd); i++) {
            recv_cmd[i] = '\0';
        }
    }    
    delay(100);
    
//    printReturnedData();
}
