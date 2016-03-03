/*
  CSimplePdf - class for created simple .pdf files for Borland C Builder 6.0
  https://github.com/podoroges/simplepdf

  19 Feb 16 - added maxwidth for CSimplePdf::CPage::Text
  03 Mar 16 - added CSimplePdf::CPage::ImgInline

*/

#include "csimplepdf.h"
#include <vector>

#define Byte zlib_Byte
#define MINIZ_NO_STDIO
#include "miniz.c"
#undef Byte


struct TCharMap{
  unsigned char c;
  int u;
  char * st;
};

TCharMap cp1251[] = {
{0x00,0x0000,NULL},
{0x01,0x0001,NULL},
{0x02,0x0002,NULL},
{0x03,0x0003,NULL},
{0x04,0x0004,NULL},
{0x05,0x0005,NULL},
{0x06,0x0006,NULL},
{0x07,0x0007,NULL},
{0x08,0x0008,NULL},
{0x09,0x0009,NULL},
{0x0A,0x000A,NULL},
{0x0B,0x000B,NULL},
{0x0C,0x000C,NULL},
{0x0D,0x000D,NULL},
{0x0E,0x000E,NULL},
{0x0F,0x000F,NULL},
{0x10,0x0010,NULL},
{0x11,0x0011,NULL},
{0x12,0x0012,NULL},
{0x13,0x0013,NULL},
{0x14,0x0014,NULL},
{0x15,0x0015,NULL},
{0x16,0x0016,NULL},
{0x17,0x0017,NULL},
{0x18,0x0018,NULL},
{0x19,0x0019,NULL},
{0x1A,0x001A,NULL},
{0x1B,0x001B,NULL},
{0x1C,0x001C,NULL},
{0x1D,0x001D,NULL},
{0x1E,0x001E,NULL},
{0x1F,0x001F,NULL},
{0x20,0x0020,"space\0"},
{0x21,0x0021,"exclam\0"},
{0x22,0x0022,"quotedbl\0"},
{0x23,0x0023,"numbersign\0"},
{0x24,0x0024,"dollar\0"},
{0x25,0x0025,"percent\0"},
{0x26,0x0026,"ampersand\0"},
{0x27,0x0027,"quotesingle\0"},
{0x28,0x0028,"parenleft\0"},
{0x29,0x0029,"parenright\0"},
{0x2A,0x002A,"asterisk\0"},
{0x2B,0x002B,"plus\0"},
{0x2C,0x002C,"comma\0"},
{0x2D,0x002D,"hyphen\0"},
{0x2E,0x002E,"period\0"},
{0x2F,0x002F,"slash\0"},
{0x30,0x0030,"zero\0"},
{0x31,0x0031,"one\0"},
{0x32,0x0032,"two\0"},
{0x33,0x0033,"three\0"},
{0x34,0x0034,"four\0"},
{0x35,0x0035,"five\0"},
{0x36,0x0036,"six\0"},
{0x37,0x0037,"seven\0"},
{0x38,0x0038,"eight\0"},
{0x39,0x0039,"nine\0"},
{0x3A,0x003A,"colon\0"},
{0x3B,0x003B,"semicolon\0"},
{0x3C,0x003C,"less\0"},
{0x3D,0x003D,"equal\0"},
{0x3E,0x003E,"greater\0"},
{0x3F,0x003F,"question\0"},
{0x40,0x0040,"at\0"},
{0x41,0x0041,"A\0"},
{0x42,0x0042,"B\0"},
{0x43,0x0043,"C\0"},
{0x44,0x0044,"D\0"},
{0x45,0x0045,"E\0"},
{0x46,0x0046,"F\0"},
{0x47,0x0047,"G\0"},
{0x48,0x0048,"H\0"},
{0x49,0x0049,"I\0"},
{0x4A,0x004A,"J\0"},
{0x4B,0x004B,"K\0"},
{0x4C,0x004C,"L\0"},
{0x4D,0x004D,"M\0"},
{0x4E,0x004E,"N\0"},
{0x4F,0x004F,"O\0"},
{0x50,0x0050,"P\0"},
{0x51,0x0051,"Q\0"},
{0x52,0x0052,"R\0"},
{0x53,0x0053,"S\0"},
{0x54,0x0054,"T\0"},
{0x55,0x0055,"U\0"},
{0x56,0x0056,"V\0"},
{0x57,0x0057,"W\0"},
{0x58,0x0058,"X\0"},
{0x59,0x0059,"Y\0"},
{0x5A,0x005A,"Z\0"},
{0x5B,0x005B,"bracketleft\0"},
{0x5C,0x005C,"backslash\0"},
{0x5D,0x005D,"bracketright\0"},
{0x5E,0x005E,"asciicircum\0"},
{0x5F,0x005F,"underscore\0"},
{0x60,0x0060,"grave\0"},
{0x61,0x0061,"a\0"},
{0x62,0x0062,"b\0"},
{0x63,0x0063,"c\0"},
{0x64,0x0064,"d\0"},
{0x65,0x0065,"e\0"},
{0x66,0x0066,"f\0"},
{0x67,0x0067,"g\0"},
{0x68,0x0068,"h\0"},
{0x69,0x0069,"i\0"},
{0x6A,0x006A,"j\0"},
{0x6B,0x006B,"k\0"},
{0x6C,0x006C,"l\0"},
{0x6D,0x006D,"m\0"},
{0x6E,0x006E,"n\0"},
{0x6F,0x006F,"o\0"},
{0x70,0x0070,"p\0"},
{0x71,0x0071,"q\0"},
{0x72,0x0072,"r\0"},
{0x73,0x0073,"s\0"},
{0x74,0x0074,"t\0"},
{0x75,0x0075,"u\0"},
{0x76,0x0076,"v\0"},
{0x77,0x0077,"w\0"},
{0x78,0x0078,"x\0"},
{0x79,0x0079,"y\0"},
{0x7A,0x007A,"z\0"},
{0x7B,0x007B,"braceleft\0"},
{0x7C,0x007C,"bar\0"},
{0x7D,0x007D,"braceright\0"},
{0x7E,0x007E,"asciitilde\0"},
{0x7F,0x007F,NULL},
{0x80,0x0402,"afii10051\0"},
{0x81,0x0403,"afii10052\0"},
{0x82,0x201A,"quotesinglbase\0"},
{0x83,0x0453,"afii10100\0"},
{0x84,0x201E,"quotedblbase\0"},
{0x85,0x2026,"ellipsis\0"},
{0x86,0x2020,"dagger\0"},
{0x87,0x2021,"daggerdbl\0"},
{0x88,0x20AC,"Euro\0"},
{0x89,0x2030,"perthousand\0"},
{0x8A,0x0409,"afii10058\0"},
{0x8B,0x2039,"guilsinglleft\0"},
{0x8C,0x040A,"afii10059\0"},
{0x8D,0x040C,"afii10061\0"},
{0x8E,0x040B,"afii10060\0"},
{0x8F,0x040F,"afii10145\0"},
{0x90,0x0452,"afii10099\0"},
{0x91,0x2018,"quoteleft\0"},
{0x92,0x2019,"quoteright\0"},
{0x93,0x201C,"quotedblleft\0"},
{0x94,0x201D,"quotedblright\0"},
{0x95,0x2022,"bullet\0"},
{0x96,0x2013,"endash\0"},
{0x97,0x2014,"emdash\0"},
{0x99,0x2122,"trademark\0"},
{0x9A,0x0459,"afii10106\0"},
{0x9B,0x203A,"guilsinglright\0"},
{0x9C,0x045A,"afii10107\0"},
{0x9D,0x045C,"afii10109\0"},
{0x9E,0x045B,"afii10108\0"},
{0x9F,0x045F,"afii10193\0"},
{0xA0,0x00A0,"space\0"},
{0xA1,0x040E,"afii10062\0"},
{0xA2,0x045E,"afii10110\0"},
{0xA3,0x0408,"afii10057\0"},
{0xA4,0x00A4,"currency\0"},
{0xA5,0x0490,"afii10050\0"},
{0xA6,0x00A6,"brokenbar\0"},
{0xA7,0x00A7,"section\0"},
{0xA8,0x0401,"afii10023\0"},
{0xA9,0x00A9,"copyright\0"},
{0xAA,0x0404,"afii10053\0"},
{0xAB,0x00AB,"guillemotleft\0"},
{0xAC,0x00AC,"logicalnot\0"},
{0xAD,0x00AD,"hyphen\0"},
{0xAE,0x00AE,"registered\0"},
{0xAF,0x0407,"afii10056\0"},
{0xB0,0x00B0,"degree\0"},
{0xB1,0x00B1,"plusminus\0"},
{0xB2,0x0406,"afii10055\0"},
{0xB3,0x0456,"afii10103\0"},
{0xB4,0x0491,"afii10098\0"},
{0xB5,0x00B5,"mu\0"},
{0xB6,0x00B6,"paragraph\0"},
{0xB7,0x00B7,"periodcentered\0"},
{0xB8,0x0451,"afii10071\0"},
{0xB9,0x2116,"afii61352\0"},
{0xBA,0x0454,"afii10101\0"},
{0xBB,0x00BB,"guillemotright\0"},
{0xBC,0x0458,"afii10105\0"},
{0xBD,0x0405,"afii10054\0"},
{0xBE,0x0455,"afii10102\0"},
{0xBF,0x0457,"afii10104\0"},
{0xC0,0x0410,"afii10017\0"},
{0xC1,0x0411,"afii10018\0"},
{0xC2,0x0412,"afii10019\0"},
{0xC3,0x0413,"afii10020\0"},
{0xC4,0x0414,"afii10021\0"},
{0xC5,0x0415,"afii10022\0"},
{0xC6,0x0416,"afii10024\0"},
{0xC7,0x0417,"afii10025\0"},
{0xC8,0x0418,"afii10026\0"},
{0xC9,0x0419,"afii10027\0"},
{0xCA,0x041A,"afii10028\0"},
{0xCB,0x041B,"afii10029\0"},
{0xCC,0x041C,"afii10030\0"},
{0xCD,0x041D,"afii10031\0"},
{0xCE,0x041E,"afii10032\0"},
{0xCF,0x041F,"afii10033\0"},
{0xD0,0x0420,"afii10034\0"},
{0xD1,0x0421,"afii10035\0"},
{0xD2,0x0422,"afii10036\0"},
{0xD3,0x0423,"afii10037\0"},
{0xD4,0x0424,"afii10038\0"},
{0xD5,0x0425,"afii10039\0"},
{0xD6,0x0426,"afii10040\0"},
{0xD7,0x0427,"afii10041\0"},
{0xD8,0x0428,"afii10042\0"},
{0xD9,0x0429,"afii10043\0"},
{0xDA,0x042A,"afii10044\0"},
{0xDB,0x042B,"afii10045\0"},
{0xDC,0x042C,"afii10046\0"},
{0xDD,0x042D,"afii10047\0"},
{0xDE,0x042E,"afii10048\0"},
{0xDF,0x042F,"afii10049\0"},
{0xE0,0x0430,"afii10065\0"},
{0xE1,0x0431,"afii10066\0"},
{0xE2,0x0432,"afii10067\0"},
{0xE3,0x0433,"afii10068\0"},
{0xE4,0x0434,"afii10069\0"},
{0xE5,0x0435,"afii10070\0"},
{0xE6,0x0436,"afii10072\0"},
{0xE7,0x0437,"afii10073\0"},
{0xE8,0x0438,"afii10074\0"},
{0xE9,0x0439,"afii10075\0"},
{0xEA,0x043A,"afii10076\0"},
{0xEB,0x043B,"afii10077\0"},
{0xEC,0x043C,"afii10078\0"},
{0xED,0x043D,"afii10079\0"},
{0xEE,0x043E,"afii10080\0"},
{0xEF,0x043F,"afii10081\0"},
{0xF0,0x0440,"afii10082\0"},
{0xF1,0x0441,"afii10083\0"},
{0xF2,0x0442,"afii10084\0"},
{0xF3,0x0443,"afii10085\0"},
{0xF4,0x0444,"afii10086\0"},
{0xF5,0x0445,"afii10087\0"},
{0xF6,0x0446,"afii10088\0"},
{0xF7,0x0447,"afii10089\0"},
{0xF8,0x0448,"afii10090\0"},
{0xF9,0x0449,"afii10091\0"},
{0xFA,0x044A,"afii10092\0"},
{0xFB,0x044B,"afii10093\0"},
{0xFC,0x044C,"afii10094\0"},
{0xFD,0x044D,"afii10095\0"},
{0xFE,0x044E,"afii10096\0"},
{0xFF,0x044F,"afii10097\0"}};

const int cp1251_len = sizeof(cp1251) / sizeof(cp1251[0]);

TCharMap cp1252[] ={
{0x00,0x0000,NULL},
{0x01,0x0001,NULL},
{0x02,0x0002,NULL},
{0x03,0x0003,NULL},
{0x04,0x0004,NULL},
{0x05,0x0005,NULL},
{0x06,0x0006,NULL},
{0x07,0x0007,NULL},
{0x08,0x0008,NULL},
{0x09,0x0009,NULL},
{0x0A,0x000A,NULL},
{0x0B,0x000B,NULL},
{0x0C,0x000C,NULL},
{0x0D,0x000D,NULL},
{0x0E,0x000E,NULL},
{0x0F,0x000F,NULL},
{0x10,0x0010,NULL},
{0x11,0x0011,NULL},
{0x12,0x0012,NULL},
{0x13,0x0013,NULL},
{0x14,0x0014,NULL},
{0x15,0x0015,NULL},
{0x16,0x0016,NULL},
{0x17,0x0017,NULL},
{0x18,0x0018,NULL},
{0x19,0x0019,NULL},
{0x1A,0x001A,NULL},
{0x1B,0x001B,NULL},
{0x1C,0x001C,NULL},
{0x1D,0x001D,NULL},
{0x1E,0x001E,NULL},
{0x1F,0x001F,NULL},
{0x20,0x0020,"space\0"},
{0x21,0x0021,"exclam\0"},
{0x22,0x0022,"quotedbl\0"},
{0x23,0x0023,"numbersign\0"},
{0x24,0x0024,"dollar\0"},
{0x25,0x0025,"percent\0"},
{0x26,0x0026,"ampersand\0"},
{0x27,0x0027,"quotesingle\0"},
{0x28,0x0028,"parenleft\0"},
{0x29,0x0029,"parenright\0"},
{0x2A,0x002A,"asterisk\0"},
{0x2B,0x002B,"plus\0"},
{0x2C,0x002C,"comma\0"},
{0x2D,0x002D,"hyphen\0"},
{0x2E,0x002E,"period\0"},
{0x2F,0x002F,"slash\0"},
{0x30,0x0030,"zero\0"},
{0x31,0x0031,"one\0"},
{0x32,0x0032,"two\0"},
{0x33,0x0033,"three\0"},
{0x34,0x0034,"four\0"},
{0x35,0x0035,"five\0"},
{0x36,0x0036,"six\0"},
{0x37,0x0037,"seven\0"},
{0x38,0x0038,"eight\0"},
{0x39,0x0039,"nine\0"},
{0x3A,0x003A,"colon\0"},
{0x3B,0x003B,"semicolon\0"},
{0x3C,0x003C,"less\0"},
{0x3D,0x003D,"equal\0"},
{0x3E,0x003E,"greater\0"},
{0x3F,0x003F,"question\0"},
{0x40,0x0040,"at\0"},
{0x41,0x0041,"A\0"},
{0x42,0x0042,"B\0"},
{0x43,0x0043,"C\0"},
{0x44,0x0044,"D\0"},
{0x45,0x0045,"E\0"},
{0x46,0x0046,"F\0"},
{0x47,0x0047,"G\0"},
{0x48,0x0048,"H\0"},
{0x49,0x0049,"I\0"},
{0x4A,0x004A,"J\0"},
{0x4B,0x004B,"K\0"},
{0x4C,0x004C,"L\0"},
{0x4D,0x004D,"M\0"},
{0x4E,0x004E,"N\0"},
{0x4F,0x004F,"O\0"},
{0x50,0x0050,"P\0"},
{0x51,0x0051,"Q\0"},
{0x52,0x0052,"R\0"},
{0x53,0x0053,"S\0"},
{0x54,0x0054,"T\0"},
{0x55,0x0055,"U\0"},
{0x56,0x0056,"V\0"},
{0x57,0x0057,"W\0"},
{0x58,0x0058,"X\0"},
{0x59,0x0059,"Y\0"},
{0x5A,0x005A,"Z\0"},
{0x5B,0x005B,"bracketleft\0"},
{0x5C,0x005C,"backslash\0"},
{0x5D,0x005D,"bracketright\0"},
{0x5E,0x005E,"asciicircum\0"},
{0x5F,0x005F,"underscore\0"},
{0x60,0x0060,"grave\0"},
{0x61,0x0061,"a\0"},
{0x62,0x0062,"b\0"},
{0x63,0x0063,"c\0"},
{0x64,0x0064,"d\0"},
{0x65,0x0065,"e\0"},
{0x66,0x0066,"f\0"},
{0x67,0x0067,"g\0"},
{0x68,0x0068,"h\0"},
{0x69,0x0069,"i\0"},
{0x6A,0x006A,"j\0"},
{0x6B,0x006B,"k\0"},
{0x6C,0x006C,"l\0"},
{0x6D,0x006D,"m\0"},
{0x6E,0x006E,"n\0"},
{0x6F,0x006F,"o\0"},
{0x70,0x0070,"p\0"},
{0x71,0x0071,"q\0"},
{0x72,0x0072,"r\0"},
{0x73,0x0073,"s\0"},
{0x74,0x0074,"t\0"},
{0x75,0x0075,"u\0"},
{0x76,0x0076,"v\0"},
{0x77,0x0077,"w\0"},
{0x78,0x0078,"x\0"},
{0x79,0x0079,"y\0"},
{0x7A,0x007A,"z\0"},
{0x7B,0x007B,"braceleft\0"},
{0x7C,0x007C,"bar\0"},
{0x7D,0x007D,"braceright\0"},
{0x7E,0x007E,"asciitilde\0"},
{0x7F,0x007F,NULL},
{0x80,0x20AC,"Euro\0"},
{0x82,0x201A,"quotesinglbase\0"},
{0x83,0x0192,"florin\0"},
{0x84,0x201E,"quotedblbase\0"},
{0x85,0x2026,"ellipsis\0"},
{0x86,0x2020,"dagger\0"},
{0x87,0x2021,"daggerdbl\0"},
{0x88,0x02C6,"circumflex\0"},
{0x89,0x2030,"perthousand\0"},
{0x8A,0x0160,"Scaron\0"},
{0x8B,0x2039,"guilsinglleft\0"},
{0x8C,0x0152,"OE\0"},
{0x8E,0x017D,"Zcaron\0"},
{0x91,0x2018,"quoteleft\0"},
{0x92,0x2019,"quoteright\0"},
{0x93,0x201C,"quotedblleft\0"},
{0x94,0x201D,"quotedblright\0"},
{0x95,0x2022,"bullet\0"},
{0x96,0x2013,"endash\0"},
{0x97,0x2014,"emdash\0"},
{0x98,0x02DC,"tilde\0"},
{0x99,0x2122,"trademark\0"},
{0x9A,0x0161,"scaron\0"},
{0x9B,0x203A,"guilsinglright\0"},
{0x9C,0x0153,"oe\0"},
{0x9E,0x017E,"zcaron\0"},
{0x9F,0x0178,"Ydieresis\0"},
{0xA0,0x00A0,"space\0"},
{0xA1,0x00A1,"exclamdown\0"},
{0xA2,0x00A2,"cent\0"},
{0xA3,0x00A3,"sterling\0"},
{0xA4,0x00A4,"currency\0"},
{0xA5,0x00A5,"yen\0"},
{0xA6,0x00A6,"brokenbar\0"},
{0xA7,0x00A7,"section\0"},
{0xA8,0x00A8,"dieresis\0"},
{0xA9,0x00A9,"copyright\0"},
{0xAA,0x00AA,"ordfeminine\0"},
{0xAB,0x00AB,"guillemotleft\0"},
{0xAC,0x00AC,"logicalnot\0"},
{0xAD,0x00AD,"hyphen\0"},
{0xAE,0x00AE,"registered\0"},
{0xAF,0x00AF,"macron\0"},
{0xB0,0x00B0,"degree\0"},
{0xB1,0x00B1,"plusminus\0"},
{0xB2,0x00B2,"twosuperior\0"},
{0xB3,0x00B3,"threesuperior\0"},
{0xB4,0x00B4,"acute\0"},
{0xB5,0x00B5,"mu\0"},
{0xB6,0x00B6,"paragraph\0"},
{0xB7,0x00B7,"periodcentered\0"},
{0xB8,0x00B8,"cedilla\0"},
{0xB9,0x00B9,"onesuperior\0"},
{0xBA,0x00BA,"ordmasculine\0"},
{0xBB,0x00BB,"guillemotright\0"},
{0xBC,0x00BC,"onequarter\0"},
{0xBD,0x00BD,"onehalf\0"},
{0xBE,0x00BE,"threequarters\0"},
{0xBF,0x00BF,"questiondown\0"},
{0xC0,0x00C0,"Agrave\0"},
{0xC1,0x00C1,"Aacute\0"},
{0xC2,0x00C2,"Acircumflex\0"},
{0xC3,0x00C3,"Atilde\0"},
{0xC4,0x00C4,"Adieresis\0"},
{0xC5,0x00C5,"Aring\0"},
{0xC6,0x00C6,"AE\0"},
{0xC7,0x00C7,"Ccedilla\0"},
{0xC8,0x00C8,"Egrave\0"},
{0xC9,0x00C9,"Eacute\0"},
{0xCA,0x00CA,"Ecircumflex\0"},
{0xCB,0x00CB,"Edieresis\0"},
{0xCC,0x00CC,"Igrave\0"},
{0xCD,0x00CD,"Iacute\0"},
{0xCE,0x00CE,"Icircumflex\0"},
{0xCF,0x00CF,"Idieresis\0"},
{0xD0,0x00D0,"Eth\0"},
{0xD1,0x00D1,"Ntilde\0"},
{0xD2,0x00D2,"Ograve\0"},
{0xD3,0x00D3,"Oacute\0"},
{0xD4,0x00D4,"Ocircumflex\0"},
{0xD5,0x00D5,"Otilde\0"},
{0xD6,0x00D6,"Odieresis\0"},
{0xD7,0x00D7,"multiply\0"},
{0xD8,0x00D8,"Oslash\0"},
{0xD9,0x00D9,"Ugrave\0"},
{0xDA,0x00DA,"Uacute\0"},
{0xDB,0x00DB,"Ucircumflex\0"},
{0xDC,0x00DC,"Udieresis\0"},
{0xDD,0x00DD,"Yacute\0"},
{0xDE,0x00DE,"Thorn\0"},
{0xDF,0x00DF,"germandbls\0"},
{0xE0,0x00E0,"agrave\0"},
{0xE1,0x00E1,"aacute\0"},
{0xE2,0x00E2,"acircumflex\0"},
{0xE3,0x00E3,"atilde\0"},
{0xE4,0x00E4,"adieresis\0"},
{0xE5,0x00E5,"aring\0"},
{0xE6,0x00E6,"ae\0"},
{0xE7,0x00E7,"ccedilla\0"},
{0xE8,0x00E8,"egrave\0"},
{0xE9,0x00E9,"eacute\0"},
{0xEA,0x00EA,"ecircumflex\0"},
{0xEB,0x00EB,"edieresis\0"},
{0xEC,0x00EC,"igrave\0"},
{0xED,0x00ED,"iacute\0"},
{0xEE,0x00EE,"icircumflex\0"},
{0xEF,0x00EF,"idieresis\0"},
{0xF0,0x00F0,"eth\0"},
{0xF1,0x00F1,"ntilde\0"},
{0xF2,0x00F2,"ograve\0"},
{0xF3,0x00F3,"oacute\0"},
{0xF4,0x00F4,"ocircumflex\0"},
{0xF5,0x00F5,"otilde\0"},
{0xF6,0x00F6,"odieresis\0"},
{0xF7,0x00F7,"divide\0"},
{0xF8,0x00F8,"oslash\0"},
{0xF9,0x00F9,"ugrave\0"},
{0xFA,0x00FA,"uacute\0"},
{0xFB,0x00FB,"ucircumflex\0"},
{0xFC,0x00FC,"udieresis\0"},
{0xFD,0x00FD,"yacute\0"},
{0xFE,0x00FE,"thorn\0"},
{0xFF,0x00FF,"ydieresis\0"}};

const int cp1252_len = sizeof(cp1252) / sizeof(cp1252[0]);

/*
   Class CTTFParser based on ttfparser.php and makefont.php
   by Olivier PLATHEY http://www.fpdf.org

   Converted to Borland C++ Builder class
   by Podoroges http://podoroges.livejournal.com

   Created solely for cp1251 cyrillic encoding
   Please feel free to modify it for your needs
*/

class CTTFParser{
  public:
  AnsiString postScriptName;
  int Embeddable;
  int Bold;
  int typoAscender, typoDescender;
  int capHeight;
  int italicAngle;
  int underlinePosition, underlineThickness;
  int isFixedPitch;
  int unitsPerEm;
  int xMin, yMin, xMax, yMax;
  int numberOfHMetrics, numGlyphs;
  int * ttfw;
  int ttfw_cnt;
  int widths[0x100];
  int Flags;
  int MissingWidth;
  AnsiString Encoding;
  int StemV;
  double koef;
  AnsiString ErrorString;

  CTTFParser(){
    ttfw = NULL;
  }
  ~CTTFParser(){
    tables.clear();
    Map.clear();
    delete [] ttfw;
  }

  void Parse(AnsiString fname){
    fs = new TFileStream(fname,fmOpenRead|fmShareDenyNone);
    char verx[5];
    char ver1[] = "OTTO";
    char ver2[] = "\x00\x01\x00\x00";
    fs->Read(verx,4);
    verx[4]=0;
    if(strncmp(verx,ver1,4)==0)
      Error("OpenType fonts based on PostScript outlines are not supported");
    if(strncmp(verx,ver2,4)!=0)
      Error((AnsiString)"Unrecognized file format");
    int numTables = ReadUShort();
    Skip(3*2); // searchRange, entrySelector, rangeShift
    for(int i=0;i<numTables;i++){
      AnsiString tag = ReadStr(4);
      Skip(4); // checkSum
      int offset = ReadULong();
      Skip(4); // length
      tables.push_back(CTable(tag,offset));
    }
    ParseHead();
    ParseHhea();
    ParseMaxp();
    ParseHmtx();
    ParseCmap();
    ParseName();
    ParseOS2();
    ParsePost();
    // capHeight
    this->capHeight = this->capHeight*koef;
    if(!capHeight)
      capHeight = this->typoAscender;
    // Flags
    Flags = 0;
    if(isFixedPitch)
      Flags += 1<<0;
    Flags += 1<<5;
    if(italicAngle!=0)
      Flags += 1<<6;
    // StemV
    if(Bold)
      StemV = 120;
    else
      StemV = 70;
    MissingWidth = ttfw[0]*koef;
    for(int a=0;a<0x100;a++)
      widths[a] = MissingWidth;
    // Mapping width of gliph from ttf file to Width array using 1251 map
    for(int a=0;a<cp1251_len;a++)
    if(cp1251[a].st){ //!='.notdef'
      int uv = cp1251[a].u;
      int found = -1;
      for(unsigned int i=0;i<Map.size();i++)
        if(Map[i].c==uv)
          found = Map[i].gid;
      if(found<0){
        Error((AnsiString)cp1251[a].st+"["+cp1251[a].c+"] not found.\n");
      }
      else{
        if(found<ttfw_cnt)
          widths[cp1251[a].c] = ttfw[found]*koef;
        else
          Error((AnsiString)"Invalid GID. "+cp1251[a].c+"\n");
      }
    }
    // Encoding differences cp1251 and cp1252
    int last = 0;
    for(int a=0;a<0x100;a++){
      int f1 = -1;
      int f2 = -1;
      for(int i=0;i<cp1251_len;i++)
        if(cp1251[i].c==a)
          f1 = i;
      for(int i=0;i<cp1252_len;i++)
        if(cp1252[i].c==a)
          f2 = i;
      if((f1>0) && (f2>0) && cp1251[f1].st && cp1252[f2].st && (strcmp(cp1251[f1].st,cp1252[f2].st)!=0)){
        if(a!=last+1)
          Encoding = (AnsiString)Encoding+a+" ";
        last = a;
        Encoding = (AnsiString)Encoding+"/"+cp1251[f1].st+" ";
      }
    }
    Encoding = Encoding.Trim();
    delete fs;
  }

  private:
  TFileStream * fs;
  class CTable{
    public:
    AnsiString tag;
    int offset;
    CTable(AnsiString _tag,int _offset):tag(_tag),offset(_offset){};
  };
  std::vector <CTable> tables;
  class CMap{
    public:
    int c;
    int gid;
    CMap(int _c,int _gid){
      c = _c;
      gid = _gid;
    }
  };
  std::vector <CMap> Map;
  void ParseHead(){
    Seek("head");
    Skip(3*4); // version, fontRevision, checkSumAdjustment
    int magicNumber = ReadULong();
    if(magicNumber!=0x5F0F3CF5)
      Error("Incorrect magic number");
    this->Skip(2); // flags
    unitsPerEm = ReadUShort();
    koef = 1000.0/double(unitsPerEm);
    Skip(2*8); // created, modified
    xMin = ReadShort()*koef;
    yMin = ReadShort()*koef;
    xMax = ReadShort()*koef;
    yMax = ReadShort()*koef;
  }
  void ParseHhea(){
    Seek("hhea");
    Skip(4+15*2);
    numberOfHMetrics = ReadUShort();
  }
  void ParseMaxp(){
    Seek("maxp");
    Skip(4);
    numGlyphs = ReadUShort();
  }
  void ParseHmtx(){
    Seek("hmtx");
    ttfw_cnt = MAX(numberOfHMetrics,numGlyphs);
    ttfw = new int[ttfw_cnt];
    for(int i=0;i<numberOfHMetrics;i++){
      int advanceWidth = ReadUShort();
      Skip(2); // lsb
      ttfw[i] = advanceWidth;
    }
    if(numberOfHMetrics<numGlyphs){
      int lastWidth = ttfw[numberOfHMetrics-1];
      for(int i=numberOfHMetrics;i<numGlyphs;i++)
        ttfw[i] = lastWidth;
    }
  }

  void ParseCmap(){
    Seek("cmap");
    Skip(2); // version
    int numTables = ReadUShort();
    int offset31 = 0;
    for(int i=0;i<numTables;i++){
      int platformID = ReadUShort();
      int encodingID = ReadUShort();
      int offset = ReadULong();
      if(platformID==3 && encodingID==1)
	offset31 = offset;
    }
    if(offset31==0)
      Error("No Unicode encoding found");
    Seek("cmap",offset31);
    int format = ReadUShort();
    if(format!=4)
      Error((AnsiString)"Unexpected subtable format: "+format);
    Skip(2*2); // length, language
    int segCount = ReadUShort()/2;
    int * startCount = new int[segCount];
    int * endCount = new int[segCount];
    int * idDelta = new int[segCount];
    int * idRangeOffset = new int[segCount];
    Skip(3*2); // searchRange, entrySelector, rangeShift
    for(int i=0;i<segCount;i++)
      endCount[i] = ReadUShort();
    Skip(2); // reservedPad
    for(int i=0;i<segCount;i++)
      startCount[i] = ReadUShort();
    for(int i=0;i<segCount;i++)
      idDelta[i] = ReadUShort();
    int offset = fs->Position;//ftell(f);
    for(int i=0;i<segCount;i++)
      idRangeOffset[i] = ReadUShort();
    for(int i=0;i<segCount;i++){
      int c1 = startCount[i];
      int c2 = endCount[i];
      int d = idDelta[i];
      int ro = idRangeOffset[i];
      if(ro>0)
	fs->Position = offset+2*i+ro;
      for(int c=c1;c<=c2;c++){
        int gid;
	if(c==0xFFFF)
	  break;
	if(ro>0){
	  gid = ReadUShort();
	  if(gid>0)
	    gid += d;
	}
	else
	  gid = c+d;
	if(gid>=65536)
	  gid -= 65536;
	if(gid>0)
          Map.push_back(CMap(c,gid));
      }
    }
    delete [] startCount;
    delete [] endCount;
    delete [] idDelta;
    delete [] idRangeOffset;
  }

  void ParseName(){
    Seek("name");
    int tableOffset = fs->Position;//ftell(f);
    postScriptName = "";
    Skip(2); // format
    int count = ReadUShort();
    int stringOffset = ReadUShort();
    for(int i=0;i<count;i++){
      Skip(3*2); // platformID, encodingID, languageID
      int nameID = ReadUShort();
      int length = ReadUShort();
      int offset = ReadUShort();
      if(nameID==6){
	// PostScript name
	fs->Position = tableOffset+stringOffset+offset;
	AnsiString s = ReadStr(length);
	s = StrRep(s,"[", "");
	s = StrRep(s,"]", "");
	s = StrRep(s,"(", "");
	s = StrRep(s,")", "");
	s = StrRep(s,"{", "");
	s = StrRep(s,"}", "");
	s = StrRep(s,"<", "");
	s = StrRep(s,">", "");
	s = StrRep(s,"%", "");
	postScriptName = s;
	break;
      }
    }
    if(postScriptName=="")
      Error("PostScript name not found");
  }

  void ParseOS2(){
    Seek("OS/2");
    int version = ReadUShort();
    Skip(3*2); // xAvgCharWidth, usWeightClass, usWidthClass
    int fsType = ReadUShort();
    Embeddable = (fsType!=2) && (fsType & 0x200)==0;
    Skip(11*2+10+4*4+4);
    int fsSelection = ReadUShort();
    Bold = (fsSelection & 32)!=0;
    Skip(2*2); // usFirstCharIndex, usLastCharIndex
    typoAscender = ReadShort()*koef;
    typoDescender = ReadShort()*koef;
    if(version>=2){
      Skip(3*2+2*4+2);
      capHeight = ReadShort();
    }
    else
      capHeight = 0;
  }

  void ParsePost(){
    Seek("post");
    Skip(4); // version
    italicAngle = ReadShort();
    Skip(2); // Skip decimal part
    underlinePosition = ReadShort()*koef;
    underlineThickness = ReadShort()*koef;
    isFixedPitch = (ReadULong()!=0);
  }

  void ReadBuf(char * buf,int i){
    fs->Read(buf,i);
    buf[i] = 0;
  }

  AnsiString ReadStr(int i){
    char * buf = new char[i+1];
    fs->Read(buf,i);
    for(int a=0;a<i;a++)
      if(buf[a]==0)
        buf[a] = '%';
    buf[i] = 0;
    AnsiString st = buf;
    delete [] buf;
    return st;
  }

  unsigned short int ReadUShort(){
    unsigned short int si;
    fs->Read(&si,2);
    return (si>>8) | (si<<8);
  }
  short int ReadShort(){
    unsigned short int si;
    fs->Read(&si,2);
    //if(si>=0x8000)si -= 65536;
    if(((si>>8) | (si<<8))==-85){
      si = si;
    }
    return (si>>8) | (si<<8);
  }
  unsigned int ReadULong(){
    int num;
    fs->Read(&num,4);
    return ((num>>24)&0xff) | // move byte 3 to byte 0
                    ((num<<8)&0xff0000) | // move byte 1 to byte 2
                    ((num>>8)&0xff00) | // move byte 2 to byte 1
                    ((num<<24)&0xff000000); // byte 0 to byte 3
  }

  void Skip(int n){
    fs->Position = fs->Position+n;
  }

  void Seek(AnsiString tag,int offset = 0){
    for(unsigned int i=0;i<tables.size();i++)
      if(tables[i].tag==tag){
        fs->Position = tables[i].offset+offset;
        return;
      }
    Error((AnsiString)"Table not found: "+tag);
  }

  AnsiString StrRep(AnsiString st,AnsiString from,AnsiString to){
    while(st.Pos(from)){
      int a = st.Pos(from);
      st.Delete(a,from.Length());
      st.Insert(to,a);
    }
    return st;
  }

  int MAX(int a, int b){
    return (a>b?a:b);
  }

  void Error(AnsiString st){
    ErrorString = ErrorString+st;
  }

};







  class CSimplePdf::CCat:public CObj{
    public:
    CCat(CSimplePdf * p){
      parent = p;
    }
    AnsiString AsString(){
      return "<<\n/Pages 2 0 R\n/Type /Catalog\n>>";
    }
    char Type(){
      return 'A';
    }
  };
  class CSimplePdf::CPages:public CObj{
    public:
    CPages(CSimplePdf * p){
      parent = p;
    }
    AnsiString AsString(){
      AnsiString st = (AnsiString)"<<\n/Count "+parent->PageCount+"\n/Kids [\n";
      for(int a=0;a<parent->PageCount;a++)
        st = (AnsiString)st+"  "+parent->Page[a]->ObjectID()+" 0 R\n";
      st = (AnsiString)st+"]\n/Type /Pages\n>>";
      return st;
    }
    char Type(){
      return 'B';
    }
  };
  class CSimplePdf::CFontDiff: public CObj{
    private:
    public:
    AnsiString Encoding;
    CFontDiff(CSimplePdf * p,AnsiString _Encoding){
      Encoding = _Encoding;
      parent = p;
    }
    AnsiString AsString(){
      AnsiString st = (AnsiString)"<< /Type /Encoding /BaseEncoding /WinAnsiEncoding /Differences [ "+Encoding+" ] >>";
      return st;
    }
    char Type(){
      return 'F';
    }
  };



  class CSimplePdf::CFontFile: public CObj{
    private:
    AnsiString FileName;
    public:
    CFontFile(AnsiString fname,CSimplePdf * p){
      FileName = fname;
      parent = p;
    }
    AnsiString AsString(){
      TFileStream * fs = new TFileStream(FileName,fmOpenRead|fmShareDenyNone);
      int flen = fs->Size;
      char * src = new char[flen];
      fs->Read(src,flen);
      unsigned long l1 = compressBound(flen);
      char * buf = new char[l1];
      compress(buf,&l1,src,flen);
      TStringStream * s = new TStringStream("");
      s->Write(buf,l1);
      AnsiString packed = s->DataString;
      delete s;
      delete [] buf;
      delete fs;
      delete [] src;
      return (AnsiString)"<< /Length "+packed.Length()+" /Length1 "+flen+" /Filter /FlateDecode >>\nstream\n"+packed+"\nendstream";
    }
    char Type(){
      return 'F';
    }
  };
  class CSimplePdf::CFontDescriptor: public CObj{
    public:
    AnsiString FontName;
    int Flags;
    int xMin, yMin, xMax, yMax;
    int Ascent,Descent,CapHeight,ItalicAngle,MissingWidth;
    double StemV;
    CFontFile * FontFile;
    CFontDescriptor(CTTFParser * parser,CSimplePdf * p){
      parent = p;
      FontName = parser->postScriptName;
      Flags = parser->Flags;
      xMin = parser->xMin;
      yMin = parser->yMin;
      xMax = parser->xMax;
      yMax = parser->yMax;
      Ascent = parser->typoAscender;
      CapHeight = parser->capHeight;
      Descent = parser->typoDescender;
      ItalicAngle = parser->italicAngle;
      StemV = parser->StemV;
      MissingWidth = parser->MissingWidth;
    }
    AnsiString AsString(){
      return (AnsiString)"<< /Type /FontDescriptor"
        +" /FontName /"+FontName
        +" /Flags "+Flags
        +" /FontBBox [ "+xMin+" "+yMin+" "+xMax+" "+yMax+" ]"
        +" /Ascent "+Ascent
        +" /CapHeight "+CapHeight
        +" /Descent "+Descent
        +" /MissingWidth "+MissingWidth
        +" /ItalicAngle "+ItalicAngle
        +" /StemV "+AnsiString().sprintf("%.2f",StemV)
        +" /FontFile2 "+FontFile->ObjectID()+" 0 R"
        +" >>";
    }
    char Type(){
      return 'F';
    }
  };
  class CSimplePdf::CFontTTF: public CObj{
    private:

    AnsiString BaseFont;
    AnsiString WidthsStr(){
      AnsiString st;
      for(int a=32;a<0x100;a++)
        st = (AnsiString)st+Widths[a]+" ";
      return st;
    }
    public:
    int Widths[0x100];
    AnsiString Name;
    CFontDiff * Encoding;
    CFontDescriptor * Descriptor;
    CFontTTF(CTTFParser * p,CSimplePdf * _parent,AnsiString _name){
      Name = _name;
      parent = _parent;
      BaseFont = p->postScriptName;
      for(int a=0;a<0x100;a++)
        Widths[a] = p->widths[a];
    }
    ~CFontTTF(){
//      delete [] Widths;
    }
    AnsiString AsString(){
      return (AnsiString)"<< /Type /Font /Subtype /TrueType"
        +" /FirstChar 32 /LastChar 255"
        +" /BaseFont /"+BaseFont
        +" /Widths [ "+WidthsStr()+"]"
        +" /Encoding "+Encoding->ObjectID()+" 0 R"
        +" /FontDescriptor "+
        Descriptor->ObjectID()+" 0 R >>";
    }
    char Type(){
      return 'T';//ttf - used in FontsString()
    }
  };

  class CSimplePdf::CPageContents: public CObj{
    public:
    CPage * Page;
    AnsiString Contents;
    CPageContents(CSimplePdf * p){
      parent = p;
    }
    AnsiString AsString(){
      if(parent->Compress){
        unsigned long l1 = Contents.Length()+100;
        char * buf = new char[l1];
        compress(buf,&l1, Contents.c_str(),Contents.Length());
        TStringStream * s = new TStringStream("");
        s->Write(buf,l1);
        AnsiString packed = s->DataString;
        delete s;
        delete [] buf;
        return (AnsiString)"<< /Length "+packed.Length()+" /Filter /FlateDecode >>\nstream\n"+packed+"\nendstream";
      }
      return (AnsiString)"<< /Length "+Contents.Length()+" >>\nstream\n"+Contents+"\nendstream";
    }
    char Type(){
      return 'C';
    }
  };

  int CSimplePdf::_GetPageCount(){
    int count = 0;
    for(unsigned int a=0;a<Objects.size();a++)
      if(Objects[a]->Type()=='P')
        count++;
    return count;
  }
  CSimplePdf::CPage * CSimplePdf::_GetPage(int index){
    int cnt = 0;
    CPage * LastPage = NULL;
    for(unsigned int a=0;a<Objects.size();a++)
    if(Objects[a]->Type()=='P'){
      if(index==cnt)
        return (CPage *)Objects[a];
      cnt++;
      LastPage = (CPage *)Objects[a];
    };
    if(!LastPage){
      AddPage();
      return _GetPage(0);
    }
    return LastPage;
  }
  void CSimplePdf::AddPage(double Width,double Height){
    CPage * p = new CPage(Width,Height,this);
    Objects.push_back(p);
    CPageContents * c = new CPageContents(this);
    Objects.push_back(c);
    p->Contents = c;
    c->Page = p;
  }
  CSimplePdf::CSimplePdf(){
    LineWidth = 1;
    FontSize = 1;
    Compress = 1;
    CCat * c = new CSimplePdf::CCat(this);
    Objects.push_back(c);
    CPages * p = new CSimplePdf::CPages(this);
    Objects.push_back(p);
  }

  void CSimplePdf::EmbedFont(AnsiString name,AnsiString fname){
    CurrentFont = name;
    CTTFParser * parser = new CTTFParser();
    parser->Parse(fname);
    // Font Object
    CFontTTF * f = new CFontTTF(parser,this,name);
    Objects.push_back(f);
    // Font Descriptor object
    CFontDescriptor * d = new CFontDescriptor(parser,this);
    f->Descriptor = d;
    Objects.push_back(d);
    // Embedded TTF file object
    CFontFile * ff = new CFontFile(fname,this);
    d->FontFile = ff;
    Objects.push_back(ff);
    // Creating Encoding Differences object
    CFontDiff * diff = new CFontDiff(this,parser->Encoding);
    f->Encoding = diff;
    Objects.push_back(diff);
    delete parser;
  }
  void CSimplePdf::SaveToFile(AnsiString fname){
    _out("%PDF-1.3");
    _out("");
    for(unsigned int a=0;a<Objects.size();a++){
      Objects[a]->xref = buffer.Length();
      _out((AnsiString)(a+1)+" 0 obj");
      _out(Objects[a]->AsString());
      _out("endobj");
      _out("");

    }
    int startxref = buffer.Length();
    _out("xref");
    _out((AnsiString)"0 "+(Objects.size()+1));
    _out("0000000000 65535 f ");
    for(unsigned int a=0;a<Objects.size();a++){
      _out(AnsiString().sprintf("%010d 00000 n",Objects[a]->xref));
    }
    _out("trailer");
    _out("<<");
    _out((AnsiString)"/Size "+Objects.size());
    _out("/Root 1 0 R");
    _out(">>");
    _out("");
    _out("startxref");
    _out(startxref);
    _out("%%EOF");
    TFileStream * fs = new TFileStream(fname,fmCreate);
    fs->Write(buffer.c_str(),buffer.Length());
    delete fs;
  }

  AnsiString CSimplePdf::CPage::FontsString(){
      AnsiString st;
      for(unsigned int a = 0;a<parent->Objects.size();a++)
        if(parent->Objects[a]->Type()=='T')
          st = (AnsiString)st+"/"+((CFontTTF *)parent->Objects[a])->Name+" "+parent->Objects[a]->ObjectID()+" 0 R ";
      return st;
  }
    void CSimplePdf::CPage::Rect(double x1,double y1,double x2,double y2){
      Contents->Contents = (AnsiString)Contents->Contents
        +AnsiString().sprintf("q %.2f w %.2f %.2f %.2f %.2f re S Q\n",parent->LineWidth,x1,y1,x2-x1,y2-y1);
    }

    void CSimplePdf::CPage::FillRect(double x1,double y1,double x2,double y2){
      double R1 = double(GetRValue(parent->PenColor))/double(255);
      double G1 = double(GetGValue(parent->PenColor))/double(255);
      double B1 = double(GetBValue(parent->PenColor))/double(255);
      double r1 = double(GetRValue(parent->FillColor))/double(255);
      double g1 = double(GetGValue(parent->FillColor))/double(255);
      double b1 = double(GetBValue(parent->FillColor))/double(255);

      Contents->Contents = (AnsiString)Contents->Contents
        +AnsiString().sprintf("q %.2f w %.2f %.2f %.2f RG %.2f %.2f %.2f rg %.2f %.2f %.2f %.2f re B Q\n",parent->LineWidth,R1,G1,B1,r1,g1,b1,x1,y1,x2-x1,y2-y1);
    }


    void CSimplePdf::CPage::Line(double x1,double y1,double x2,double y2){
      Contents->Contents = (AnsiString)Contents->Contents
        +AnsiString().sprintf("q %.2f w %.2f %.2f m %.2f %.2f l S Q\n",parent->LineWidth,x1,y1,x2,y2);
    }


    void CSimplePdf::CPage::ImgInline(AnsiString fname,double x1,double y1){
      int W = 0;
      int H = 0;
      AnsiString data;
      if(fname.LowerCase().Pos(".bmp")){
        Graphics::TBitmap * bmp = new Graphics::TBitmap();
        bmp->LoadFromFile(fname);
        W = bmp->Width;
        H = bmp->Height;

        for(int j=0;j<H;j++)
        for(int i=0;i<W;i++)
          data = (AnsiString)data
            +AnsiString().sprintf("%02x",GetRValue(bmp->Canvas->Pixels[i][j]))
            +AnsiString().sprintf("%02x",GetGValue(bmp->Canvas->Pixels[i][j]))
            +AnsiString().sprintf("%02x",GetBValue(bmp->Canvas->Pixels[i][j]));

        delete bmp;
      }
      Contents->Contents = (AnsiString)Contents->Contents
        +AnsiString().sprintf("q %i 0 0 %i %.2f %.2f cm BI /W %i /H %i /CS /RGB /BPC 8 /F [/AHx] \nID\n %s \nEI\n Q\n",W,H,x1,y1,W,H,data.c_str());
    }

    void CSimplePdf::CPage::Text(double x1,double y1,AnsiString st,double maxwidth){
      int len = st.Length();
      if(maxwidth>0){
        len = 0;
        for(int a=1;a<=st.Length();a++)
          if(parent->TextWidth(st.SubString(1,a))<maxwidth)
            len = a;
          else
            a = st.Length();
      }
      Contents->Contents = (AnsiString)Contents->Contents
        +AnsiString().sprintf("q BT 0 0 0 rg /%s %i Tf %.2f %.2f Td ( ",parent->CurrentFont.c_str(),parent->FontSize,x1,y1)+st.SubString(1,len)+" ) Tj ET Q\n";
    }
    AnsiString CSimplePdf::CPage::AsString(){
      return (AnsiString)"<< /Type /Page /Parent 2 0 R /Resources << /Font << "+FontsString()+">> >>  /Contents "+
       Contents->ObjectID()+" 0 R >> ";
    }

    int CSimplePdf::TextWidth(AnsiString st){
      CFontTTF * f = NULL;
      for(unsigned int a = 0;a<Objects.size();a++)
        if(Objects[a]->Type()=='T')
          if(((CFontTTF *)Objects[a])->Name==CurrentFont)
            f = (CFontTTF *)Objects[a];
      if(!f)
        return 0; // Should not be
      int w = 0;
      for(int a=1;a<=st.Length();a++){
        char c = st[a];
        w = w+f->Widths[c];
      }
      return w*FontSize/1000;
    }
