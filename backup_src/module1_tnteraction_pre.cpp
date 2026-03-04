#include "seal_main.h"
#include <cstring>
#include <iconv.h>
#include <vector>
#include <cerrno>

// 将 UTF-8 字符串转换为 GBK（二进制）字符串，返回是否成功
static bool utf8_to_gbk(const std::string &in, std::string &out)
{
    iconv_t cd = iconv_open("GBK", "UTF-8");
    if (cd == (iconv_t)-1) {
        return false;
    }

    size_t inbytesleft = in.size();
    // 输出缓冲至少是输入的两倍（每个 UTF-8 汉字在 GBK 中为 2 字节）
    size_t outbufsize = inbytesleft * 2 + 16;
    std::vector<char> outbuf(outbufsize);

    char *inptr = const_cast<char*>(in.data());
    char *outptr = outbuf.data();
    size_t outbytesleft = outbufsize;

    // iconv 的参数是 char**
    size_t res = iconv(cd, &inptr, &inbytesleft, &outptr, &outbytesleft);
    if (res == (size_t)-1) {
        iconv_close(cd);
        return false;
    }

    out.assign(outbuf.data(), outptr - outbuf.data());
    iconv_close(cd);
    return true;
}

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

void USE_INTERRACTION()
{
    cout << "===== 汉字印章生成器 =====\n";
    cout << "===== 定制独一无二的印章 =====\n";

    cout << "\n请输入要生成印章的四个汉字: ";
    string input_chars;
    cin >> input_chars;
    // 尝试把 UTF-8 输入转换为 GBK（大多终端为 UTF-8）
    std::string gbk_bytes;
    if (utf8_to_gbk(input_chars, gbk_bytes)) {
        // 复制 GBK 原始字节到 seal.character（最多保留 8 字节，留 1 字节做终止符）
        size_t copy_len = std::min<size_t>(gbk_bytes.size(), sizeof(seal.character) - 1);
        memcpy(seal.character, gbk_bytes.data(), copy_len);
        seal.character[copy_len] = '\0';
    } else {
        // 转换失败时回退为直接按字节复制（可能已经是 GBK）
        strncpy(seal.character, input_chars.c_str(), sizeof(seal.character) - 1);
        seal.character[sizeof(seal.character) - 1] = '\0';
    }

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
