#include <iostream>

void INTRODUCE(){
 	//////background introduction
 	//Author
     printf("  Author: kali\n");
 	//program name
     printf("  Program name:Character Seal\n");
 	//Date
     printf("  Date: 2026-03-03\n");
 	//version
 	 printf("  Version: v1.0\n");
 	//function	
 	 printf("  Function:\n");
     printf("    将四个字符数组按指定布局拼接成完整印章图案\n");
     printf("    支持红、蓝、黑三种印章颜色及字体切换\n");
     printf("    输出标准BMP格式的印章图片文件\n");
 }



  
void USE_INTERRACTION(char character[],int &font_choice,int &color_choice){//
	//////args selection 
	printf("===== 汉字印章生成器 =====\n");
	printf("=====定制独一无二的印章 =====\n");
	//Chinese character
	printf("\n请输入要生成印章的汉字:");
    char character[5];
    scanf("%s", &character);
	//font字体 
	printf("\n===== 字体选择 =====\n");
	printf("1. 宋体\n");
    printf("2. 楷书\n");
    printf("3. 隶书\n");
    printf("请选择：（输入数字）");
    int font_choice;
    scanf("%d", &font_choice);
	//color 
	printf("\n===== 颜色选择 =====\n");
	printf("1. red\n");
	printf("2. blue,\n");
	printf("3. black,\n");
	printf("4. 自定义颜色，（未开放）\n");
	printf("请选择：（输入数字）");
    int color_choice;
    scanf("%d", &color_choice);
}
