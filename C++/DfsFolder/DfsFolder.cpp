//https://blog.csdn.net/zwc2xm/article/details/39004083
//string 的详细用法：https://blog.csdn.net/tengfei461807914/article/details/52203202
//删除文件：https://www.cnblogs.com/xiaofeiIDO/p/8056079.html
//修改文件名：https://blog.csdn.net/u012750702/article/details/53326521
//文件复制：https://1350579085.iteye.com/blog/1993507
//正则： 原文：https://blog.csdn.net/cupidove/article/details/45871055
//char string 之间的转换：https://blog.csdn.net/yzhang6_10/article/details/51164300
/**
 * 将bear的文件开头改成hexo的要求，包含title、tags、categorys
 *
 *
 * 注：修改一下文件件格式
 */
#include <iostream>
#include <fstream>
#include <sys/dir.h>
#include <sys/stat.h>
#include <cstring>
#include <regex>
using namespace std;

bool isDir(const char* path);
void myFind(const string& filename, const string& tofind);
void findInDir(const char *path, const string& tofind);
void findInDir(const char *path, const string& tofind, int recursive);

int main(){
    findInDir("/Users/czh/BearTemp/知识", "c");
    return 0;
}

/**复制文件
filename :要复制的文件名
newfile :要复制到的文件名
*/
int cpy(char*filename,char*newfile){
    ifstream in ;
    ofstream out ;
    /**
    open函数的原型是
    open(const char*filename,ios_base::openmode mode=ios_base::in) ;
    in_stream.open("file.txt");这样写参数实际上是指向这个常量字符串的指针
    */
    //打开文件
    in.open(filename);
    //打开文件失败
    if(in.fail()){
        cout<<"打开文件失败"<<endl ;
        in.close();
        out.close();
        return 0 ;
    }
    out.open(newfile);
    if(out.fail()){
        cout<<"创建文件失败"<<endl ;
        in.close();
        out.close();
        return 0 ;
    }else{//复制文件
        out<<in.rdbuf();
        out.close();
        in.close();
        return 1 ;
    }
}

int CountLines(char *filename){
    ifstream ReadFile;
    int n=0;
    string tmp;
    ReadFile.open(filename,ios::in);//ios::in 表示以只读的方式读取文件
    if(ReadFile.fail())//文件打开失败:返回0
    {
        return 0;
    }
    else//文件存在
    {
        while(getline(ReadFile,tmp,'\n'))
        {
            n++;
        }
        ReadFile.close();
        return n;
    }
}

string ReadLine(char *filename,int line) {
    int lines, i = 0;
    string temp;
    fstream file;
    file.open(filename, ios::in);
    lines = CountLines(filename);

    if (line <= 0) {
        return "Error 1: 行数错误，不能为0或负数。";
    }
    if (file.fail()) {
        return "Error 2: 文件不存在。";
    }
    if (line > lines) {
        return "Error 3: 行数超出文件长度。";
    }
    while (getline(file, temp) && i < line - 1) {
        i++;
    }
    file.close();
    return temp;
}

void myFind(const string& filename, const string& tofind)
{
    cout<<"filename:"<<filename<<endl;
    //以.md结尾的才处理
    int size = filename.size();
    if(filename.substr(size-3) != ".md") return;

    char *path=(char*)filename.c_str();
    //读取前三行的必要信息
    string p1 = ReadLine(path,1);
    string p2 = ReadLine(path,2);
    string p3 = ReadLine(path,3);

    //修改文件的前五行
    if(p1[0] == '#' && p1[1] == ' ' && p2[0] == '#'){
        p1=p1.substr(2,p1.size()-2);
        p2=p2.substr(1,p2.size()-1);


        //把p2按'/'进行分解为m份：
        string p2_n[5]={"空"};

        int m = 0;
        std::size_t begin = 0;
        std::size_t end = 0;
        std::size_t pos[5];
        pos[0] = -1;

        //先找见/位置
        for(;m < 5;m++){
            begin = end;
            end = p2.find('/',begin+1);
            pos[m+1] = end;
            if(end == std::string::npos) break;
        }
        //再截取
        for(int x =0; x <= m; x++){
            int begin2= pos[x];
            int end2 = pos[x+1];
            p2_n[x] = p2.substr(begin2+1,end2-begin2-1);
        }

        //第0~m-1是Category，第m是tags
        string join;
        for(int j=0; j < m; j++){
            join += p2_n[j];
            if(j+1 != m){
                join += "/";
            }
        }

        //正则过滤title中的特殊字符
        char buf[200];
        const char *first = (char*)p1.c_str();
        const char *last = first + strlen(first);
        cout<<"strlen(first)="<<strlen(first)<<endl;
        std::regex rx("[:;@$%&!\\[\\]]");//中文的冒号：hexo不会报错，英文的冒号:会报错
        std::string fmt("");
        *std::regex_replace(&buf[0], first, last, rx, fmt) = '\0';
        std::cout << &buf[0] << std::endl;
        //赋值转换 char t0 string
        p1 = buf;

        string a1 = "---";
        string a2 = "title: "+ p1;
        string a3 = "tags: ";
        string a4 = "- "+ p2_n[m];
        string a5 = "categories: ";
        string a6 = "- "+join;
        string a7= "---";

        //给新文件另起一个名字：加个.
        char path2[256];
        int k = 0;
        for(; k < filename.size()-3; k++){
            if(path[k] == '\0') break;
            else{
                path2[k] = path[k];
            }
        }
        path2[k++] = '.';
        path2[k++] = '.';
        path2[k++] = 'm';
        path2[k++] = 'd';
        path2[k] = '\0';

        //新文件中写
        ofstream OutFile(path2, ios::out|ios::binary);
        OutFile << a1 <<endl;
        OutFile << a2 <<endl;
        OutFile << a3 <<endl;
        OutFile << a4 <<endl;
        OutFile << a5 <<endl;
        OutFile << a6 <<endl;
        OutFile << a7 <<endl;

        //新的写、旧的读
        int num = CountLines(path);
        for(int i = 3; i<=num; i++){
            string str = ReadLine(path,i);
            OutFile << str <<endl;
        }

        //删除旧的文件
        if(remove(path)==0){
            cout<<"删除成功！"<<endl;
        } else{
            cout<<"删除失败！"<<endl;
        }
        //修改回文件名
        if(rename(path2,path)==0){
            cout<<"修改成功！"<<endl;
        } else{
            cout<<"修改失败！"<<endl;
        }

        OutFile.close();
    }
    else if(p1[0] == '#' && p1[1] == ' ' && p2[0] != '#'){
        cout<<"请修改一下文件件格式："<<filename<<endl;

        //把p2按'/'进行分解为m份：
        string p2_n[5]={"空"};

        int m = 0;
        std::size_t begin = 0;
        std::size_t end = 0;
        std::size_t pos[5];
        pos[0] = -1;

        //先找见/位置
        for(;m < 5;m++){
            begin = end;
            end = p2.find('/',begin+1);
            pos[m+1] = end;
            if(end == std::string::npos) break;
        }
        //再截取
        for(int x =0; x <= m; x++){
            int begin2= pos[x];
            int end2 = pos[x+1];
            p2_n[x] = p2.substr(begin2+1,end2-begin2-1);
        }


        //文件拷贝到别的地方
        string name_3 = "/Users/czh/Downloads/BearNote/"+p2_n[m];
        char *path_3=(char*)name_3.c_str();
        cpy(path,path_3);

        //删除
        if(remove(path)==0){
            cout<<"删除成功！"<<endl;
        } else{
            cout<<"删除失败！"<<endl;
        }

    }

//    cout << "find matched number:" << count << endl;
}
//遍历文件夹的驱动函数
void findInDir(const char *path, const string& tofind)
{
    unsigned long len;
    char temp[256];
    //去掉末尾的'/'
    len = strlen(path);
    strcpy(temp, path);
    if(temp[len - 1] == '/') temp[len -1] = '\0';

    if(isDir(temp))
    {
        //处理目录
        int recursive = 1;
        findInDir(temp, tofind, recursive);
    }
    else   //输出文件
    {
        printf("path:%s\n", path);
        myFind(path, tofind);
    }
}


//遍历文件夹de递归函数
void findInDir(const char *path, const string& tofind, int recursive)
{
    DIR *pdir;
    struct dirent *pdirent;
    char temp[256];
    try {
        pdir = opendir(path);
    }
    catch(const char *str)
    {printf("failed open dir");}

    if(pdir)
    {
        while((pdirent = readdir(pdir)))
        {
            //跳过"."和".."
            if(strcmp(pdirent->d_name, ".") == 0
               || strcmp(pdirent->d_name, "..") == 0)
                continue;
            sprintf(temp, "%s/%s", path, pdirent->d_name);

            //当temp为目录并且recursive为1的时候递归处理子目录
            if(isDir(temp) && recursive)
            {
                findInDir(temp, tofind, recursive);
            }
            else
            {
                printf("path:%s\n", temp);
                myFind(temp, tofind);
            }
        }
    }
    else
    {
        printf("opendir error:%s\n", path);
    }
    closedir(pdir);
}

bool isDir(const char* path)
{
    struct stat st;
    lstat(path, &st);
    return 0 != S_ISDIR(st.st_mode);
}