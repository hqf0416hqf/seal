#include "seal_main.h"

//////根据二维数组，写入bmp文件中 
void BMP_GENERATOR(){
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

    int ROWSIZE = (3 * COL + 3) / 4 * 4;        // 每行字节数（4字节对齐）
    int PIXELDATASIZE = ROWSIZE * ROW;

    BITMAPFILEHEADER BFH;
    BITMAPINFOHEADER BIH;

    BFH.BFTYPE = 0x4D42; // 'BM'
    BFH.BFSIZE = 14 + 40 + PIXELDATASIZE;
    BFH.BFRESERVED1 = 0;
    BFH.BFRESERVED2 = 0;
    BFH.BFOFFBITS = 14 + 40;

    BIH.BISIZE = 40;
    BIH.BIWIDTH = COL;
    BIH.BIHEIGHT = ROW;
    BIH.BIPLANES = 1;
    BIH.BIBITCOUNT = 24;
    BIH.BICOMPRESSION = 0;
    BIH.BISIZEIMAGE = PIXELDATASIZE;
    BIH.BIXPELSPERMETER = 0;
    BIH.BIYPELSPERMETER = 0;
    BIH.BICLRUSED = 0;
    BIH.BICLRIMPORTANT = 0;

    FILE *FP = fopen(FILENAME, "wb");
    if (!FP) return;

    fwrite(&BFH, sizeof(BFH), 1, FP);
    fwrite(&BIH, sizeof(BIH), 1, FP);

    unsigned char PADDING[3] = {0, 0, 0};
    int PADDINGSIZE = ROWSIZE - COL * 3;

    for (int I = ROW - 1; I >= 0; I--) // 从底行开始写
    {
        for (int J = 0; J < COL; J++)
        {
            unsigned char R, G, B;
            if (CANVAS[I][J] == 1)   // 红色印章
                R = 255, G = 0, B = 0;
            else                     // 白色背景
                R = 255, G = 255, B = 255;

            fputc(B, FP);
            fputc(G, FP);
            fputc(R, FP);
        }
        fwrite(PADDING, 1, PADDINGSIZE, FP);
    }

    fclose(FP);
}