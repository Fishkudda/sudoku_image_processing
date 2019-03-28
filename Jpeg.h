#ifndef Jpeg_h
#define Jpeg_h

//IDENT
// The IDENT + Marker marks the beginning of the markers usage
#define IDENT 255
//-----------JPEG MARKER-------------//

//--Application
#define APP0 224
#define APP1 225
#define APP2 226
#define APP3 227
#define APP4 228
#define APP5 229
#define APP6 230
#define APP7 231
#define APP8 232
#define APP9 233
#define APP10 234
#define APP11 235
#define APP12 236
#define APP13 237
#define APP14 238
#define APP15 239

//--Restart
#define RST0 208
#define RST1 209
#define RST2 210
#define RST3 211
#define RST4 212
#define RST5 213
#define RST6 214
#define RST7 215

//--Start of Frame
#define SOF0 192
#define SOF1 193
#define SOF2 194
#define SOF3 195
#define SOF4 197
#define SOF5 198
#define SOF6 199
#define SOF7 201
#define SOF8 202
#define SOF9 203
#define SOF10 205
#define SOF11 206
#define SOF12 207

#define SOI 216 // IMAGE START

#define DHT 196 //DEFINE HUFFMAN TABLES
#define DQT 219 //DEFINE QUANTIZATION TABLES
#define DRI 221 //DEFINE RESTART INTERVALL
#define SOS 218 //START OF SCAN
#define COM 254 //COMMENT
#define EOI 217 //END OF IMAGE

#ifndef BYTE
typedef unsigned char BYTE;
#endif



struct grayscale{
    BYTE g;
};

struct rgb{
    BYTE r;
    BYTE g;
    BYTE b;
};

struct YCbCr{
    signed char Y;
    signed char Cb;
    signed char Cr;
};

struct SOF{
    BYTE *length;
    BYTE *data_precision;
    BYTE *image_height;
    BYTE *image_width;
    BYTE *number_of_components;
    BYTE *components;
};

#endif /* Jpeg_h */
