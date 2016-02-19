/*
  CSimplePdf - class for created simple .pdf files for Borland C Builder 6.0
  https://github.com/podoroges/simplepdf

  ///////// Usage Example: ////////////////
  CSimplePdf * pdf = new CSimplePdf();
  pdf->EmbedFont("F1","c:\\Home\\Temp\\PdfTest2\\aricyr.ttf");
  pdf->EmbedFont("F2","c:\\Home\\Temp\\PdfTest2\\calligra.ttf");
  pdf->AddPage(800,600);
  pdf->CurrentFont = "F2";
  pdf->FontSize = 25;
  pdf->Page[0]->Text(100,100,"Hello!");
  pdf->Page[0]->Rect(90,90,200,140);
  pdf->FontSize = 22;
  pdf->CurrentFont = "F1";
  pdf->Page[0]->Text(100,200,"QWERTY");
  pdf->Page[0]->Text(100,150,"ZXCVBN");
  pdf->LineWidth = 5;
  pdf->Page[0]->Line(100,300,100,500);
  pdf->SaveToFile("C:\\test1.pdf");
  delete pdf;
  ////////////////////////////

  Class CTTFParser based on ttfparser.php and makefont.php
  by Olivier PLATHEY http://www.fpdf.org

  Converted to Borland C++ Builder class
  by Podoroges http://podoroges.livejournal.com

  Created solely for cp1251 cyrillic encoding
  Please feel free to modify it for your needs
*/

#include <vcl.h>
#include <vector>

class CSimplePdf{
  private:
  AnsiString buffer;
  class CCat;
  class CPages;
  class CFontDiff;
  class CFontFile;
  class CFontDescriptor;
  class CFontTTF;
  class CObj{
    public:
    CSimplePdf * parent;
    int xref;
    int ObjectID(){
      for(unsigned int a = 0;a<parent->Objects.size();a++)
        if(parent->Objects[a]==this)
          return a+1;
      return 0;
    };
    virtual AnsiString AsString() = 0;
    virtual char Type() = 0;
  };
  friend class CObj;
  std::vector <CObj *> Objects;
  class CPageContents;
  class CPage: public CObj{
    private:
    AnsiString FontsString();
    public:
    CPageContents * Contents;
    double Width, Height;
    CPage(double W,double H,CSimplePdf * p):Width(W),Height(H){parent = p;}
    void Rect(double x1,double y1,double x2,double y2);
    void FillRect(double x1,double y1,double x2,double y2);    
    void Line(double x1,double y1,double x2,double y2);
    void Text(double x1,double y1,AnsiString st,double maxwidth = -1);
    AnsiString AsString();
    char Type(){
      return 'P';
    }
  };
  friend class CPage;
  int _GetPageCount();
  CPage * _GetPage(int index);
  void _out(AnsiString s){
    buffer = (AnsiString)buffer+s+"\n";
  }
  public:
  int Compress;
  double LineWidth;
  unsigned long PenColor;
  unsigned long FillColor;
  int FontSize;
  AnsiString CurrentFont;
  int TextWidth(AnsiString);  
  __property CPage * Page[int index] = {read=_GetPage};
  __property int PageCount = {read=_GetPageCount};
  void AddPage(double Width=612,double Height=792);
  CSimplePdf();
  ~CSimplePdf(){
    Objects.clear();
    for(unsigned int a=0;a<Objects.size();a++)
      delete Objects[a];
  }
  void EmbedFont(AnsiString name,AnsiString fname);
  void SaveToFile(AnsiString fname);
};
