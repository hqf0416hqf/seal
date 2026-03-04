/*********************************************************************************
 ****  FileName:   seal.c
 ****  Function:  seal.bmp图片文件的创建          
 ****  Usage:     demo  xxxx.bmp 
 ****  Author:    kali  
 ****  Date:      2026-03-3        
 *********************************************************************************/
#include "seal_main.h"
#include <vector>

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
    // 强制转为小写以匹配字库文件
    for (char &ch : seal.gbk_code) ch = (char)tolower((unsigned char)ch);
    // 调试信息：打印转换后的 GBK 十六进制编码（小写）
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

    // 诊断：检查 back 中是否存在空单元（可能导致白色竖线）
    int empty_count = 0;
    vector<pair<int,int>> empties;
    for (int r = 0; r < 2*ROW; r++) {
        for (int c = 0; c < 2*COL; c++) {
            if (back[r][c].empty()) {
                empty_count++;
                if ((int)empties.size() < 20) empties.emplace_back(r, c);
            }
        }
    }
    if (empty_count > 0) {
        cout << "[DIAG] back has " << empty_count << " empty cells. Examples:";
        for (auto &p : empties) cout << " (" << p.first << "," << p.second << ")";
        cout << "\n";
    } else {
        cout << "[DIAG] back has no empty cells.\n";
    }

    // 进一步诊断：检查是否存在整列为白色的竖线（即每行均为 "FF FF FF"）
    int white_col_count = 0;
    vector<int> white_cols;
    for (int c = 0; c < 2*COL; c++) {
        bool all_white = true;
        for (int r = 0; r < 2*ROW; r++) {
            if (back[r][c] != "FF FF FF") { all_white = false; break; }
        }
        if (all_white) {
            white_col_count++;
            if ((int)white_cols.size() < 40) white_cols.push_back(c);
        }
    }
    if (white_col_count > 0) {
        cout << "[DIAG] found " << white_col_count << " all-white columns, examples:";
        for (int idx : white_cols) cout << " " << idx;
        cout << "\n";
    } else {
        cout << "[DIAG] no all-white columns found.\n";
    }

    // NOTE: previous automatic quadrant-based filling was causing new artifacts
    // by copying strokes across character boundaries. We keep diagnostics only
    // here and do not modify `back`. To reduce seams, we should fix the source
    // (font parsing/concat/char_rgb) rather than aggressive post-copying.

    // 对合成画布做轻量的填充：只填充被单像素或窄列分隔的白色点，避免跨字块的大范围拷贝
    auto simple_close = [&](int iterations){
        for (int it = 0; it < iterations; ++it) {
            string tmp[2*ROW][2*COL+1];
            for (int r = 0; r < 2*ROW; ++r) for (int c = 0; c < 2*COL; ++c) tmp[r][c] = back[r][c];
            for (int r = 0; r < 2*ROW; ++r) {
                for (int c = 0; c < 2*COL; ++c) {
                    if (tmp[r][c] == "FF FF FF") {
                        // 检查四邻域
                        if (c-1 >= 0 && tmp[r][c-1] != "FF FF FF") back[r][c] = tmp[r][c-1];
                        else if (c+1 < 2*COL && tmp[r][c+1] != "FF FF FF") back[r][c] = tmp[r][c+1];
                        else if (r-1 >= 0 && tmp[r-1][c] != "FF FF FF") back[r][c] = tmp[r-1][c];
                        else if (r+1 < 2*ROW && tmp[r+1][c] != "FF FF FF") back[r][c] = tmp[r+1][c];
                    }
                }
            }
        }
    };

    simple_close(2); // 两次迭代，填补单列/单像素缝隙

    // 导出 BMP 文件
    BMP_GENERATOR(string("seal.bmp"), back);

    return 0;
}