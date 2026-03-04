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

    const int LOGICAL_WIDTH = 2 * COL;    // 逻辑画布列数（每个元素代表一个逻辑像素）
    const int LOGICAL_HEIGHT = 2 * ROW;   // 逻辑画布行数
    const int SCALE = 6; // 每个逻辑像素在输出中放大倍数（宽高各）
    const int WIDTH = LOGICAL_WIDTH * SCALE;    // 实际输出宽度（像素）
    const int HEIGHT = LOGICAL_HEIGHT * SCALE;  // 实际输出高度（像素）
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

    // Diagnostic: print sizing and padding info
    cout << "[BMP_DIAG] LOGICAL_WIDTH=" << LOGICAL_WIDTH << " LOGICAL_HEIGHT=" << LOGICAL_HEIGHT
        << " SCALE=" << SCALE << " WIDTH=" << WIDTH << " HEIGHT=" << HEIGHT
        << " ROWSIZE=" << ROWSIZE << " PADDINGSIZE=" << PADDINGSIZE
        << " PIXELDATASIZE=" << PIXELDATASIZE << " BFSIZE=" << BFH.BFSIZE << "\n";

    // 我们把逻辑画布中的每个单元放大 SCALE 倍写入 BMP
    int total_bytes_written = 0;
    int bad_rows = 0;
    for (int logical_i = LOGICAL_HEIGHT - 1; logical_i >= 0; logical_i--) {
        // 对当前逻辑行写 SCALE 个实际像素行（垂直放大）
        for (int sy = 0; sy < SCALE; sy++) {
            int written_this_row = 0;
            for (int logical_j = 0; logical_j < LOGICAL_WIDTH; logical_j++) {
                const string &cell0 = CANVAS[logical_i][logical_j];
                // 如果当前单元为空，尝试左右邻居填充，避免竖直白线
                string cell = cell0;
                if (cell.empty()) {
                    // 向左右各尝试最多 5 个像素找非空值
                    for (int d = 1; d <= 5 && cell.empty(); d++) {
                        int lj = logical_j - d;
                        int rj = logical_j + d;
                        if (lj >= 0) cell = CANVAS[logical_i][lj];
                        if (cell.empty() && rj < LOGICAL_WIDTH) cell = CANVAS[logical_i][rj];
                    }
                }

                unsigned int r = 255, g = 255, b = 255;
                if (!cell.empty()) {
                    int rv = 255, gv = 255, bv = 255;
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

                // 对当前逻辑像素水平写 SCALE 个像素（水平放大）
                for (int sx = 0; sx < SCALE; sx++) {
                    fputc(static_cast<unsigned char>(b), FP);
                    fputc(static_cast<unsigned char>(g), FP);
                    fputc(static_cast<unsigned char>(r), FP);
                    written_this_row += 3;
                }
            }
            // 行尾填充
            fwrite(PADDING, 1, PADDINGSIZE, FP);
            written_this_row += PADDINGSIZE;
            total_bytes_written += written_this_row;

            if (written_this_row != ROWSIZE) {
                bad_rows++;
                cerr << "[BMP_DIAG] Warning: row written bytes mismatch at logical_i=" << logical_i
                     << " sy=" << sy << " written=" << written_this_row << " expected=" << ROWSIZE << "\n";
            }
        }
    }

    if (bad_rows == 0) cout << "[BMP_DIAG] All rows written with expected ROWSIZE=" << ROWSIZE << "\n";
    else cerr << "[BMP_DIAG] Found " << bad_rows << " bad rows. total_bytes_written=" << total_bytes_written << "\n";

    fclose(FP);
}