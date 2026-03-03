#include "seal_main.h"

//////first 将汉字转化GBK编码
	string character_gdk(char character[]){
         string s,gbk_code;
         cin >> s;
         for(int i=0;i<s.size();i++)
      	    gbk_code+=((unsigned char)s[i]);
         return gbk_code;				
	}


//////second_core 根据GBK编码读取对应点阵
        bool load_DotMatrix(const string& filename,const string& gbk_code,char a[ROW][COL+1])//args=文件名（存放点阵的文件filename=font） 注意\0 
    {
    	////打开文件
    	FILE* fp=fopen(filename,"r");
    	if(fp){
    		printf("正在初步加载字体...");//file_OPEN_成功 
		}
		else{
			printf("正在初步加载字体，字体加载失败");
			return Flase;//file_OPEN_失败 
		}
		////处理文件内容，找到对应gbk_code的行，并导入数组，转化成16进制
		char line[COL+1];
		while(fgets(line,size(line),fp))
		{
    		if(strncmp(line, "CurCode: ", 9) == 0)//注意空格 //找到有效行 
    		{    
    			//用循环导出gbk_file 
 				char gbk_file[5]   		
    	     	for(int i=0;i<4;i++)
    		  	{
    				gbk_file[i]=line[9+i];					
			 	}
    			//用gbk_file来比较判断//并导入数组 
    			if(strcmp(gbk_file, bk_code) == 0)
				{
				    // 找到了
					printf("成功加载字体,正在导入")
					//导入数组 
					 fgets(line, sizeof(line), fp);  // 跳过第一行
				 	for(int i=0;i<ROW;i++)
		    		 {
					 	fgets(line, sizeof(line), fp); 
					 	a[i][COL+1]=line[COL+1]		    		 	
				     }
		     	}	
	     	}
		 	//运用外部函数，将'X''_'，转化为16进制，为导入bmp做准备
		 	char_rgb(a,a,ROW,COL);
		 	
	 	    }
		 	////关闭文件！！安全 
		 	if(fclose(FILE* ) )	     	
		} 





///third将'X''_'，转化为16进制，为导入bmp做准备
char_rgb(char a[][COL+1],sting box[][COl+1],int row,int column){//n实际有效处理长度 
	for(int i=row-1;i>=0;i--)//从下往上读 
	 {
	     for(int j=0;j<column;j++)                     
	 	{
			if(i==0&&j==column-1)
			{
				if(a[i][j]=='X') 
				box[i][j]="00 00 FF"
				if(a[i][j]=='_') 
				box[i][j]="FF FF FF"//the_last不要空格		
			}
			else            
			{
				if(a[i][j]=='X') 
				box[i][j]="00 00 FF "
				if(a[i][j]=='_') 
				box[i][j]="FF FF FF "
	     	}
		}                
 	}	
}

 
////forth将四个数组拼接成一个大数组，按照指定布局
void concat4(string a[][COL+1], string b[][COL+1], string c[][COL+1], string d[][], string back[][126]){
    // 每个小数组是 54 行 × 63 列
    const int ROWS = 54;
    const int COLS = 63;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // 左上：c 放在 back 的左上角
            back[i][j] = c[i][j];
            // 右上：a 放在 back 的右上角
            back[i][COLS + j] = a[i][j];
            // 左下：d 放在 back 的左下角
            back[ROWS + i][j] = d[i][j];
            // 右下：b 放在 back 的右下角
            back[ROWS + i][COLS + j] = b[i][j];
        }
    }
}