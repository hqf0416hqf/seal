#include "seal_main.h"

void INTRODUCE()
{
    cout << "=====================================\n";
    cout << "        汉字印章生成器 v1.0\n";
    cout << "=====================================\n";

    cout << "Author        : kali\n";
    cout << "Program Name  : Character Seal\n";
    cout << "Date          : 2026-03-03\n";
    cout << "Version       : v1.0\n";

    cout << "\nFunction Description:\n";
    cout << "1. 将四个字符数组按指定布局拼接成完整印章图案\n";
    cout << "2. 支持红、蓝、黑三种印章颜色及字体切换\n";
    cout << "3. 输出标准 BMP 格式的印章图片文件\n";

    cout << "=====================================\n\n";
}

void USE_INTERRACTION(Seal &seal, string &character)
{
    cout << "===== 汉字印章生成器 =====\n";
    cout << "===== 定制独一无二的印章 =====\n";

    cout << "\n请输入要生成印章的四个汉字: ";
    cin >> character;

    // 字体选择
    string font_choice;
    cout << "\n===== 字体选择 =====\n";
    cout << "1. 楷书\n";
    cout << "2. 隶书\n";
    cout << "请选择（输入数字）: ";
    cin >> font_choice;

    while(font_choice != "1" && font_choice != "2")
    {
        cout << "输入错误，请重新选择字体（输入数字）: ";
        cin >> font_choice;
    }

    if(font_choice == "1"){
        cout << "您选择了楷书字体\n";
        seal.font_choice = "XingKai56.txt";
    }
    else{
        cout << "您选择了隶书字体\n";
        seal.font_choice = "LiShu56.txt";
    }

    // 颜色选择
    string color_choice;
    cout << "\n===== 颜色选择 =====\n";
    cout << "1. red\n";
    cout << "2. blue\n";
    cout << "3. black\n";
    cout << "4. green\n";
    cout << "请选择（输入数字）: ";
    cin >> color_choice;

    while(color_choice != "1" &&
          color_choice != "2" &&
          color_choice != "3" &&
          color_choice != "4")
    {
        cout << "输入错误，请重新选择颜色（输入数字）: ";
        cin >> color_choice;
    }

    if(color_choice == "1"){
        cout << "您选择了红色\n";
        seal.color_choice = "00 00 FF";  // BGR
    }
    else if(color_choice == "2"){
        cout << "您选择了蓝色\n";
        seal.color_choice = "FF 00 00";
    }
    else if(color_choice == "3"){
        cout << "您选择了黑色\n";
        seal.color_choice = "00 00 00";
    }
    else{
        cout << "您选择了绿色\n";
        seal.color_choice = "00 FF 00";
    }
}
