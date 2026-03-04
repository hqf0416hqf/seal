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

    // 分析每列白色比例，找出白色密集的列（例如 >75% 白）
    vector<pair<int,int>> heavy_white_cols; // (col, white_count)
    for (int c = 0; c < 2*COL; c++) {
        int wc = 0;
        for (int r = 0; r < 2*ROW; r++) if (back[r][c] == "FF FF FF") wc++;
        if (wc * 100 >= (2*ROW) * 75) heavy_white_cols.emplace_back(c, wc);
    }
    if (!heavy_white_cols.empty()) {
        cout << "[DIAG] columns with >75% white: count=" << heavy_white_cols.size() << " examples:";
        for (size_t i = 0; i < heavy_white_cols.size() && i < 40; ++i) cout << " (" << heavy_white_cols[i].first << "," << heavy_white_cols[i].second << ")";
        cout << "\n";
    } else {
        cout << "[DIAG] no columns with >75% white detected.\n";
    }

    // 修复策略：对高度白色的列进行邻域填充（用左边或右边非白列填充），以去除明显的竖线条纹
    int filled_cols = 0;
    for (auto &p : heavy_white_cols) {
        int c = p.first;
        // 找到最近的左侧非白列作为填充源
        int src = -1;
        for (int d = 1; d < 2*COL; d++) {
            int lj = c - d;
            int rj = c + d;
            if (lj >= 0) {
                // check if lj is not heavy white
                int wc = 0;
                for (int r = 0; r < 2*ROW; r++) if (back[r][lj] == "FF FF FF") wc++;
                if (wc * 100 < (2*ROW) * 75) { src = lj; break; }
            }
            if (rj < 2*COL) {
                int wc = 0;
                for (int r = 0; r < 2*ROW; r++) if (back[r][rj] == "FF FF FF") wc++;
                if (wc * 100 < (2*ROW) * 75) { src = rj; break; }
            }
        }
        if (src != -1) {
            for (int r = 0; r < 2*ROW; r++) back[r][c] = back[r][src];
            filled_cols++;
        }
    }
    if (filled_cols > 0) cout << "[DIAG] filled " << filled_cols << " heavy-white columns by neighbor copying.\n";

    // 导出 BMP 文件
    BMP_GENERATOR(string("seal.bmp"), back);

    return 0;
}