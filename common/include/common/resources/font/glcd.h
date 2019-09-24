//****************************************
//glcdfont.h from Adafruit oled demo code
//****************************************

#ifndef GLCDFONT_H
#define GLCDFONT_H

//****************************************
// standard ascii 5x7 font
// Wrapped with #ifdef GLCDFONT so that it
// is declared once, then the rest of the time
// it is declared as extern.
// GLCDFONT needs to be declared in the 
// main file before the #include statement.
//****************************************
namespace glcd {

extern const unsigned char font[ 5 * 255 ];

}

#endif // GLCDFONT_H
