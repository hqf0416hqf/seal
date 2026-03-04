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

    INTRODUCE();
    USE_INTERRACTION();

    // 四个字符对应的 RGB 矩阵
    string A[ROW][COL+1];
    string B[ROW][COL+1];
    string C[ROW][COL+1];
    string D[ROW][COL+1];

    // 临时变量
    char a[ROW][COL+1];
    string tmpBox[ROW][COL+1];

    for (int i = 0; i < 4; i++)
    {
    // 将第 i 个汉字转为 GBK 十六进制编码
    seal.gbk_code = character_gdk(seal.character + i*2);
    // 调试信息：打印转换后的 GBK 十六进制编码
    cout << "[DEBUG] 字符索引 " << i << " 的 GBK 十六进制: " << seal.gbk_code << "\n";

        bool status = load_DotMatrix(seal.font_choice, seal.gbk_code, a);
        if (!status) {
            printf("字体导入失败，请检查输入的汉字和字体选择是否正确\n");
            return 1;
        }

        // 将点阵转换为颜色字符串矩阵（写入 tmpBox）
        char_rgb(a, tmpBox, ROW, COL);

        // 复制到对应的 A/B/C/D
        for (int r = 0; r < ROW; r++) {
            for (int c = 0; c < COL; c++) {
                if (i == 0) A[r][c] = tmpBox[r][c];
                else if (i == 1) B[r][c] = tmpBox[r][c];
                else if (i == 2) C[r][c] = tmpBox[r][c];
                else if (i == 3) D[r][c] = tmpBox[r][c];
            }
        }
    }

    // 结果画布
    string back[2*ROW][2*COL+1];
    concat4(A, B, C, D, back);

    // 导出 BMP 文件
    BMP_GENERATOR(string("seal.bmp"), back);

    return 0;
}