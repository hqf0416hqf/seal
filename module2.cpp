#include "seal_main.h"

//////first 将汉字转化GBK编码
#include <cstdio>
#include <cstring>
#include <cctype>
using namespace std;

// 名称和签名与头文件保持一致：character_gdk
string character_gdk(const char character[])
{
    unsigned char a = (unsigned char)character[0];
    unsigned char b = (unsigned char)character[1];

    char buffer[5] = {0};
    // 使用小写十六进制以匹配字库文件中的 CurCode（例如 "a1a1"）
    sprintf(buffer, "%02x%02x", a, b);

    return string(buffer);
}

//////second_core 根据GBK编码读取对应点阵
bool load_DotMatrix(const string& filename,
                    const string& gbk_code,
                    char a[ROW][COL+1])
{
    FILE* fp = fopen(filename.c_str(), "r");
    if (!fp)
    {
        cout << "字体加载失败\n";
        return false;
    }

    cout << "正在加载字体...\n";

    char line[256];

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "CurCode: ", 9) == 0)
        {
            char gbk_file[5];
            for (int i = 0; i < 4; i++)
                gbk_file[i] = line[9 + i];
            gbk_file[4] = '\0';

            // 字库文件中的 CurCode 多为小写十六进制（如 a1a1），所以把我们生成的 gbk_code 转为小写再比较
                string gbk_code_lower = gbk_code;
                for (char &ch : gbk_code_lower) ch = (char)tolower((unsigned char)ch);
                if (strcmp(gbk_file, gbk_code_lower.c_str()) == 0)
                {
                cout << "成功加载字体，正在导入...\n";

                for (int i = 0; i < ROW; i++)
                {
                    if (fgets(line, sizeof(line), fp) == nullptr)
                    {
                        fclose(fp);
                        return false;
                    }

                    line[strcspn(line, "\r\n")] = 0;
                    strncpy(a[i], line, COL);
                    a[i][COL] = '\0';
                }

                fclose(fp);
                return true;
            }
        }
    }

    fclose(fp);
    cout << "未找到对应字体\n";
    return false;
}


////third
void char_rgb(char a[][COL+1], string box[][COL+1], int row, int column)
{
    // 将点阵按自然的行序（0..ROW-1，顶到底）转成颜色字符串矩阵
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (a[i][j] == 'X')
            {
                box[i][j] = seal.color_choice; // 格式 "RR GG BB"
            }
            else
            {
                box[i][j] = string("FF FF FF");
            }
        }
    }
    printf("字体导入成功，正在制作印章\n");
}

////forth将四个数组拼接成一个大数组，按照指定布局
void concat4(string a[ROW][COL+1], string b[ROW][COL+1], string c[ROW][COL+1], string d[ROW][COL+1], string back[2*ROW][2*COL+1])
{
    // 按 A B / C D 的常见排列进行拼接：
    // A = 左上, B = 右上, C = 左下, D = 右下
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            // 左上
            back[i][j] = a[i][j];
            // 右上
            back[i][COL + j] = b[i][j];
            // 左下
            back[ROW + i][j] = c[i][j];
            // 右下
            back[ROW + i][COL + j] = d[i][j];
        }
    }

    // 添加边框：厚度 thickness（像素），使用 seal.color_choice
    const int thickness = 6; // 增加边框厚度
    int width = 2 * COL;
    int height = 2 * ROW;

    for (int t = 0; t < thickness; t++) {
        // 上边和下边
        for (int x = 0; x < width; x++) {
            // 上：只在当前像素是白色时替换，避免覆盖字形笔画
            if (back[t][x] == string("FF FF FF")) back[t][x] = seal.color_choice;
            // 下
            if (back[height - 1 - t][x] == string("FF FF FF")) back[height - 1 - t][x] = seal.color_choice;
        }
        // 左边和右边
        for (int y = 0; y < height; y++) {
            if (back[y][t] == string("FF FF FF")) back[y][t] = seal.color_choice;
            if (back[y][width - 1 - t] == string("FF FF FF")) back[y][width - 1 - t] = seal.color_choice;
        }
    }

    printf("印章制作成功，正在导出印章...\n");
}