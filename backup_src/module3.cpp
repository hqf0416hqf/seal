#include "seal_main.h"
#include <cstdio>
#include <cstring>

//////根据二维数组，写入bmp文件中 
// 修改为接受文件名和画布参数，画布元素为字符串 "RR GG BB"（十六进制或十进制均可）
void BMP_GENERATOR(string FILENAME, string CANVAS[2*ROW][2*COL+1]){
	 #pragma pack(1)
    typedef struct {
        unsigned short BFTYPE;
        unsigned int BFSIZE;
        unsigned short BFRESERVED1;
        unsigned short BFRESERVED2;
        unsigned int BFOFFBITS;
    } BITMAPFILEHEADER;

    typedef struct {
        unsigned int BISIZE;
        int BIWIDTH;
        int BIHEIGHT;
        unsigned short BIPLANES;
        unsigned short BIBITCOUNT;
        unsigned int BICOMPRESSION;
        unsigned int BISIZEIMAGE;
        int BIXPELSPERMETER;
        int BIYPELSPERMETER;
        unsigned int BICLRUSED;
        unsigned int BICLRIMPORTANT;
    } BITMAPINFOHEADER;

    const int WIDTH = 2 * COL;    // canvas 列数
    const int HEIGHT = 2 * ROW;   // canvas 行数
    int ROWSIZE = (3 * WIDTH + 3) / 4 * 4;        // 每行字节数（4字节对齐）
    int PIXELDATASIZE = ROWSIZE * HEIGHT;

    BITMAPFILEHEADER BFH;
    BITMAPINFOHEADER BIH;

    BFH.BFTYPE = 0x4D42; // 'BM'
    BFH.BFSIZE = 14 + 40 + PIXELDATASIZE;
    BFH.BFRESERVED1 = 0;
    BFH.BFRESERVED2 = 0;
    BFH.BFOFFBITS = 14 + 40;

    BIH.BISIZE = 40;
    BIH.BIWIDTH = WIDTH;
    BIH.BIHEIGHT = HEIGHT;
    BIH.BIPLANES = 1;
    BIH.BIBITCOUNT = 24;
    BIH.BICOMPRESSION = 0;
    BIH.BISIZEIMAGE = PIXELDATASIZE;
    BIH.BIXPELSPERMETER = 0;
    BIH.BIYPELSPERMETER = 0;
    BIH.BICLRUSED = 0;
    BIH.BICLRIMPORTANT = 0;

    FILE *FP = fopen(FILENAME.c_str(), "wb");
    if (!FP) return;

    fwrite(&BFH, sizeof(BFH), 1, FP);
    fwrite(&BIH, sizeof(BIH), 1, FP);

    unsigned char PADDING[3] = {0, 0, 0};
    int PADDINGSIZE = ROWSIZE - WIDTH * 3;

    for (int I = HEIGHT - 1; I >= 0; I--) // 从底行开始写
    {
        for (int J = 0; J < WIDTH; J++)
        {
            unsigned int r = 255, g = 255, b = 255;
            // 解析 CANVAS[I][J]，期望格式 "RR GG BB"（可以是十六进制）
            const string &cell = CANVAS[I][J];
            if (!cell.empty()) {
                // 尝试用 sscanf 解析十六进制或十进制
                int rv=255, gv=255, bv=255;
                if (sscanf(cell.c_str(), "%x %x %x", &rv, &gv, &bv) == 3) {
                    r = static_cast<unsigned int>(rv);
                    g = static_cast<unsigned int>(gv);
                    b = static_cast<unsigned int>(bv);
                } else if (sscanf(cell.c_str(), "%d %d %d", &rv, &gv, &bv) == 3) {
                    r = static_cast<unsigned int>(rv);
                    g = static_cast<unsigned int>(gv);
                    b = static_cast<unsigned int>(bv);
                }
            }

            // BMP 写入顺序为 B G R
            fputc(static_cast<unsigned char>(b), FP);
            fputc(static_cast<unsigned char>(g), FP);
            fputc(static_cast<unsigned char>(r), FP);
        }
        fwrite(PADDING, 1, PADDINGSIZE, FP);
    }

    fclose(FP);
}