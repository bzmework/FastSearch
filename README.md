# FastSearch
FastSearch是一个快速文件搜索系统，它的原理是通过读取NTFS文件系统中的USN日志文件，快速地读取磁盘中的所有文件，并通过文件名称快速搜索。

本程序仅实现了快速扫描和快速搜索，本机测试300万的文件扫描出来大约13秒，从300万中搜索大约0.3秒，具体视不同环境会有所出入。
在扫描文件时支持通过采用多线程和文件IO端口异步方式并行扫描，但对比发现IO端口速度略有所提升但不明显。测试结果如下：  
![image](https://github.com/bzmework/FastSearch/blob/master/test.jpg)

在国内或者国外都有优秀的文件快速搜索程序，但开源的很少，网络上的源码大多数仅停留在基本的如何操作USN读取文件的层面，
出于商业目的，对具体的文件排序、文件压缩、文件索引、UI虚拟呈现等算法没有人愿意开源分享，
目前，能找到的开源程序有：https://sourceforge.net/projects/swiftsearch/ 和 https://sourceforge.net/projects/ntfs-search/
但是，前者SwiftSearch采用异步IO扫描但源码可读性很差，而NTFS Search速度太慢，并且作者在其发布的新版中，
采用了内存映射技术，令扫描出来的文件占用内存较少，然而作者敝帚自珍，不再公布源码。

总之，要实现一个像Everything（参见www.voidtools.com ）一样强大的文件搜索系统还需要做许多工作，
例如：多线程文件排序、文件压缩、文件索引、UI虚拟呈现，等等。目前做得最好的还是Everything。

由于技术水平和时间精力有限，开发一段时间以后不再继续，现在采用Mozilla协议公布源码，希望有人能不断完善下去，让更多的程序员
掌握快速文件搜索涉及到的各种技术，希望有志于这方面的朋友继续完善这个项目。现在对FastSearch做一个说明：

1、NTFS文件系统.docx。
    集合参考各种资料以后进行的摘录，采用图表和文字的形式进行总结，有不错的参考价值。

2、Lib库。
    FastSearch中有一个C/C++库，包含了各种功能函数，能帮助你学习各种算法，具体包括：
    array，dictionary，hash，libpng，list，quicklz，regex，string，utility，win32cpu，win32file，win32filejournal，win32filesearch，
    win32gdi，win32gdip，win32image，win32mmap，win32sync，win32thread，win32timer ...。其中：

    dictionary，采用微软的.net framework中提供的优秀字典算法；
    libpng， 优秀的PNG图片库，来自：http://www.libpng.org/
    quicklz，一个精致的速度超快的压缩库，来自：http://www.quicklz.com；
    regex，GRETA正则表达式库，来自微软；
    win32cpu，一个显示CPU使用状态的小部件，有不错的呈现形态(看前面的图片)；
    win32filejournal，快速文件搜索；
    win32filesearch，传统的文件搜索；
    win32gdi/win32gdip，封装GDI/GDI+的各种常用功能;
    win32image，采用GDI+呈现各种格式图像；
    win32mmap，内存映射；
    win32thread，多线程；

    utility则包含了许多常用的功能，它参考了不同操作系统中C库的算法做出的筛选，包括：
    int32 hiword(int32 n); //高字
    int32 loword(int32 n); //低字
    int32 hiwords(int32 n); //带符号位高字
    int32 lowords(int32 n); //带符号位低字
    int32 makelong(int32 low, int32 high); //制作long型
    tbool isadigit(tchar c); //是否数字
    int16 qstrasc(const tchar* str); //返回字符串中第一个字符的ASCII码
    tbool qstrmatch(const tchar* text, const tchar* filter); //字符串匹配
    uint32 qstrlen(const tchar* str); //返回字符串的长度(字符的数量)。
    uint32 qstrlenb(const tchar* str); //返回字符串的大小(字节的数量)。
    uint32 qstrnlen(const tchar* str, uint32 n); //返回指定长度的字符串的长度（字符的数量）。
    tchar* qstrdup(const tchar* str); //返回字符串的拷贝。
    tchar* qstrndup(const tchar* str, uint32 n); //返回指定长度的字符串拷贝。
    tchar* qstrcpy(tchar* dst, const tchar* src); //拷贝源字符串到目标字符串中，无论拷贝是否成功，都返回目标字符串的地址。
    tchar* qstrncpy(tchar* dst, const tchar* src, uint32 n); //拷贝指定长度的源字符串到目标字符串中，无论拷贝是否成功，都返回目标字符串的地址。
    uint32 qstrlcpy(tchar* dst, const tchar* src, uint32 n); //拷贝指定长度的源字符串到目标字符串中，返回成功拷贝的字符数量。
    tchar* qstrcat(tchar* dst, const tchar* src); //拷贝源字符串到目标字符串的末尾(即连接两个字符串)。
    tchar* qstrncat(tchar* dst, const tchar* src, uint32 n); //拷贝指定长度的源字符串到目标字符串的末尾(即部分连接两个字符串)。
    uint32 qstrlcat(tchar* dst, const tchar* src, uint32 n); //拷贝指定长度的源字符串到目标字符串的末尾(即部分连接两个字符串)，并返回成功拷贝的字符数量。
    int32 qstrcmp(const tchar* str1, const tchar* str2); //字符串比较，区分大小写，即二进制比较。
    int32 qstrncmp(const tchar* str1, const tchar* str2, uint32 n);//比较指定长度的字符串，区分大小写，即二进制比较。
    int32 qstricmp(const tchar* str1, const tchar* str2); //字符串比较，不区分大小写，即文本比较。
    int32 qstrnicmp(const tchar* str1, const tchar* str2, uint32 n); //比较指定长度的字符串，不区分大小写，即文本比较。
    tchar* qstrchr(const tchar* str, int32 c); //查找字符串str中首次出现字符c的位置。
    tchar* qstrrchr(const tchar* str, int32 c); //查找字符串str中最后一次出现字符c的位置。
    tchar* qstrstr(const tchar* str1, const tchar* str2); //查找字符串str2是否存在于字符串str1中。
    tchar* qstrrev(tchar* str); //字符串反转。
    tchar* qstrset(tchar* str, int32 c); //将字符串str中的所有字符都设置成字符c。
    tchar* qstrnset(tchar* str, int32 c, uint32 n); //将字符串str中的n个字符设置成字符c。
    uint32 qstrxspn(const tchar* str, const tchar* map, int32 parity); //查询字符串str开头连续包含字符串accept的子字符串的字符数量。
    uint32 qstrspn(const tchar* str, const tchar* accept); //查询字符串str开头连续包含字符串accept的子字符串的字符数量。即字符串对齐连续匹配。
    uint32 qstrcspn(const tchar* str, const tchar* reject); //查询字符串str中首次出现字符串reject中的任意字符的位置。
    tchar* qstrpbrk(const tchar* str, const tchar* accept);  //查找字符串str中出现accept中任意字符时的位置。
    tchar* qstrsep(tchar** pstr, const tchar* delim); //字符串分割。
    void* qmemmove(void* dst, const void* src, tint n); //将src所指的源内存区域复制n个字节到dst所指的目标内存区域中，并返回指向dst目标内存区域的指针。
    void* qmemcpy(void* dst, const void* src, tint n); //将src所指的源内存区域复制n个字节到dst所指的目标内存区域中，并返回指向dst目标内存区域的指针。注意：拷贝大数据时性能优于系统的memcpy
    void* qmemset(void* dst, int32 c, tint n); //将dst目标内存区域用n个字节的c替换，并返回指向dst目标内存区域的指针。
    int32 qmemcmp(const void* dst, const void* src, tint n); //将src源内存区域与dst目标内存区域的n个字节进行比较
    void* qmemchr(const void* buf, int chr, tint n); //从buf内存区域中的n个字节中查找字符ch。
    int32 format(tchar* dst, const tchar* fmt, ...); //字符串格式化
    int32 c2int(const tchar* str); //转换字符串为int类型值。
    uint32 c2uint(const tchar* str); //转换字符串为unsigned int类型值。
    int64 c2int64(const tchar* str); //转换字符串为int64类型值。
    long c2lng(const tchar* str); //转换字符串为long类型值。
    long c2ulng(const tchar* str); //转换字符串为unsigned long类型值。
    uint64 c2uint64(const tchar* str); //转换字符串为uint64(unsigned long long)类型值。
    float c2flt(const tchar* str); //转换字符串为float类型值。
    double c2dbl(const tchar* str); //转换字符串为double类型值。
    tchar* cstri(int32 val, tchar* out); //转换int类型值为字符串。
    tchar* cstrui(uint32 val, tchar* out); //转换unsigned int类型值为字符串。
    tchar* cstrl(long val, tchar* out); //转换long类型值为字符串。
    tchar* cstrul(ulong val, tchar* out); //转换unsigned long类型值为字符串。
    tchar* cstri64(int64 val, tchar* out); //转换int64(long long)类型值为字符串。
    tchar* cstrui64(uint64 val, tchar* out); //转换uint64(unsigned long long)类型值为字符串。
    tchar* cstrd(double val, int32 ndigits, tchar* out); //转换double类型值为字符串。
    tchar* cstrf(float val, int32 ndigits, tchar* out); //转换float类型值为字符串。

编译环境：    
Windows 10 64bit     
Microsoft Visual Studio 2019    



