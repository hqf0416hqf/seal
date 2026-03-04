/*********************************************************************************
 ****  FileName:   seal.c
 ****  Function:  seal.bmp图片文件的创建          
 ****  Usage:     demo  xxxx.bmp 
 ****  Author:    kali  
 ****  Date:      2026-03-3        
 *********************************************************************************/
#include "seal_main.h"

seal_information seal;

int main(int argc,char *argv[]){

    char a[ROW][COL+1];
    INTRODUCE(); 
    USE_INTERRACTION(seal.character,seal.font_choice,seal.color_choice); //
    for (int i = 0; i < 4; i++)
    {
        seal.gbk_code=character_gdk(seal.character+i*2); //将每个汉字转化成GBK编码

        bool statue =load_DotMatrix(seal.font_choice,gbk_code,a[ROW][COL+1])
        if(statue)
        printf("成功导入字体，正在处理第%d个汉字...",i+1);
        else
        {
        printf("字体导入失败，请检查输入的汉字和字体选择是否正确");
        return 1;
        }
       FONT_IMPORT()
       
    }//for遍历，四个汉字依次处理

    BMP_GENERATOR();
    concat4() ;
}