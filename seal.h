#
#
#include <iostream>
#include <string>

using namespace std;


#define ROW 54
#define COL 63  

struct seal_information
{
    char character[5];
    int font_choice;
    int color_choice;
    string gbk_code;
};

//////background introduction
void INTRODUCE();  

//////args selection 
void USE_INTERRACTION();

//////first 将汉字转化GBK编码
string character_gdk(char character[]);

//////second_core 根据GBK编码读取对应点阵
bool load_DotMatrix(const string& filename,const string& gbk_code,char a[ROW][COL+1]);

//////将'X''_'，转化为16进制，为导入bmp做准备
void char_rgb(char a[][COL+1],string box[][COL+1],int row,int column);

//////将四个数组拼接成一个大数组        
void concat4(string a[ROW][COL+1], string b[ROW][COL+1], string c[ROW][COL+1], string d[ROW][COL+1], string back[2*ROW][2*COL+1]);

//////根据二维数组，写入bmp文件中 
void BMP_GENERATOR(string FILENAME,string CANVAS[2*ROW][2*COL+1]);  

#