#include "seal_main.h"

//////first 将汉字转化GBK编码
#include <cstdio>
using namespace std;

string character_gbk(const char* character)
{
    unsigned char a = (unsigned char)character[0];
    unsigned char b = (unsigned char)character[1];

    char buffer[5];
    sprintf(buffer, "%02X%02X", a, b);

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

            if (strcmp(gbk_file, gbk_code.c_str()) == 0)
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
void char_rgb(char a[][COL+1], string box[][COL+1], string color_choice)
{
    for(int i = ROW - 1; i >= 0; i--)   // BMP 从下往上
    {
        for(int j = 0; j < COL; j++)
        {
            bool is_last = (i == 0 && j == COL - 1);

            if(a[i][j] == 'X')
            {
                if(is_last)
                    box[i][j] = color_choice;
                else
                    box[i][j] = color_choice + " ";
            }
            else if(a[i][j] == '_')
            {
                if(is_last)
                    box[i][j] = "FF FF FF";
                else
                    box[i][j] = "FF FF FF ";
            }
        }
    }
	printf("字体导入成功，正在制作印章")
}
 
////forth将四个数组拼接成一个大数组，按照指定布局
void concat4(
    string a[][COL],
    string b[][COL],
    string c[][COL],
    string d[][COL],
    string back[][2*COL]
){
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            // 左上
            back[i][j] = c[i][j];
            // 右上
            back[i][COL + j] = a[i][j];
            // 左下
            back[ROW + i][j] = d[i][j];
            // 右下
            back[ROW + i][COL + j] = b[i][j];
        }
    }

    printf("印章制作成功，正在导出印章...\n");
}