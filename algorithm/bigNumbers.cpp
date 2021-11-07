#include <iostream>
#include <cstring>
#include <sstream>
using namespace std;

//比较两个倒序数组的大小，其中[0]要求储存数字大小
//a>b 1;a<b -1;a=b 0
int compare(int a[], int b[]) {
    /*
    for (int i = 0; i < a[0]+1; i++) cout << a[i];
    cout << endl;
    for (int i = 0; i < b[0]+1; i++) cout << b[i];
    cout << endl;
    */
    if (a[0] > b[0]) return 1;
    if (a[0] < b[0]) return -1;
    for (int i = a[0]; i > 0; i--) {//因为数组已经倒序，从低位到高位，所以倒着逐个比较即可
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}

void moveNum(int a[], int b[], int n) {
    //倒序数组a向高位移动n-1位,结果存放在b[]中
    //如a=15,n=4,移动后a=15000;
    //a={5,1},a={0,0,0,5,1}
    b[0] = a[0] + n;
    for (int i = 1; i <= b[0]; i++) {
        if (i <= n) b[i] = 0;
        if (i > n) b[i] = a[i - n];
    }
}

//将两个倒序数组相减，要求[0]储存数字位数
//结果保存在a数组中
void minus_arr(int a[], int b[]) {
    int flag = compare(a, b);
    if (flag == 0) a[0] = 0;
    if (flag == 1) {
        for (int i = 1; i <= a[0]; i++) {
            if (a[i] < b[i]) {
                a[i] += 10;
                a[i + 1]--;
            }
            if (i > b[0]) continue;//因为两个数组其实大小是不同的，所以需要加这个，防止b数组越界
            a[i] -= b[i];
        }
        //修正位数
        while (a[a[0]] == 0 && a[0] > 0)a[0]--;
    }
}


//高精度算法
string plus_(string a, string b) {
    //re=a+b
    //1.先把两个string串倒序转为int数组
    //  这里用指针动态创建数组（但不知道这样会不会效率低，之后测试一下）
    //  顺便还可以看到*是和变量名结合的
    //  提前规定一个mlen，为a和b中最大的长度+2，多加1是因为数组下标从1开始，又加1是因为可能还有个进位
    //  所有数组都用mlen，可以保证数组不越界，并且没有占到的地方为0
    int mlen = a.size() > b.size() ? a.size() + 2 : b.size() + 2;
    int* na = new int[mlen], * nb = new int[mlen];
    memset(na, 0, sizeof(int) * mlen);//易错点！！！这里如果写sizeof(*na)只会初始化第一个元素
    memset(nb, 0, sizeof(int) * mlen);
    for (int i = 0; i < a.size(); i++) na[a.size() - i] = a[i] - '0';
    for (int i = 0; i < b.size(); i++) nb[b.size() - i] = b[i] - '0';
    //2.然后开个数组用于存放结果，再循环相加
    //  lenc记录最后nc的位数
    int* nc = new int[mlen], lenc = 0;
    memset(nc, 0, sizeof(int)*mlen);
    while (lenc < a.size() || lenc < b.size()) {
        nc[lenc + 1] += na[lenc + 1] + nb[lenc + 1];//易错点！！！这里要+=
        nc[lenc + 2] = nc[lenc + 1] / 10;//进位
        nc[lenc + 1] %= 10;
        lenc++;
    }
    //3.找对位置，舍掉最开头的0
    //  定义nc的时候用的int[mlen]，所以这里-1
    lenc = mlen-1;
    while (nc[lenc] == 0 && lenc > 1) lenc--;
    //4.按照找对的位置，将nc倒序输出
    stringstream re;
    for (int i = 0; i < lenc; i++) re << nc[lenc - i];
    return re.str();
}

//高精度减法
string minus_(string a, string b) {
    //cout << "a:" << a << ";b:" << b << endl;
    //re=a-b;
    //0.这里可以考虑先导0的情况，可以将先导0去掉
    stringstream sa,sb;
    for (int i = 0, flag = 0; i < a.size(); i++) {
        if (a[i] == '0' && flag == 0) continue;
        if (a[i] != '0' && flag == 0) flag = 1;
        sa << a[i];
        //cout << "touch"<<a[i] << endl;
    }
    //cout<<(a = sa.str())<<endl;
    for (int i = 0, flag = 0; i < b.size(); i++) {
        if (b[i] == '0' && flag == 0) continue;
        if (b[i] != '0' && flag == 0) flag = 1;
        sb << b[i];
    }
    b = sb.str();

    //1.确保a为大数，b为小数。如果不是的话就调整一下
    if (a.size() < b.size() || (a.size() == b.size() && a < b)) {
        string t;
        t = a;
        a = b;
        b = t;
        cout << "-";//表示这是个负数
    }
    //cout << "a:" << a << ";b:" << b << endl;
    //2.同上，先把字符串转int并倒序
    //  这里的mlen不用+2，因为没有进位
    int mlen = a.size()+1;
    int* na = new int[mlen], * nb = new int[mlen];
    memset(na, 0, sizeof(int) * mlen);
    memset(nb, 0, sizeof(int) * mlen);
    for (int i = 0; i < a.size(); i++) na[a.size() - i] = a[i] - '0';
    for (int i = 0; i < b.size(); i++) nb[b.size() - i] = b[i] - '0';

    //3.开数组存放结果并相减
    int* nc = new int[mlen],lenc=1;
    memset(nc, 0, sizeof(int) * mlen);
    while (lenc <= mlen-1) {//a串是最大的
        if (na[lenc] < nb[lenc]) {//借位
            na[lenc] += 10;
            na[lenc + 1]--;
        }
        nc[lenc] = na[lenc] - nb[lenc];
        lenc++;
    }

    //4.去掉多余的0，找到正确位置
    stringstream ss;
    lenc = mlen - 1;
    while (nc[lenc] == 0 && lenc > 1) lenc--;
    for (int i = 0; i < lenc; i++) ss << nc[lenc - i];
    return ss.str();
}

//高精度乘法
string multiply_(string a,string b){
    //1.老规矩，string转int数组+倒叙
    //  乘法是循环遍历两个数组，所以不需要mlen
    int* na = new int[a.size() + 1], * nb = new int[b.size() + 1];
    memset(na, 0, sizeof(int) * (a.size() + 1));
    memset(nb, 0, sizeof(int) * (b.size() + 1));
    for (int i = 0; i < a.size(); i++) na[a.size() - i] = a[i] - '0';
    for (int i = 0; i < b.size(); i++) nb[b.size() - i] = b[i] - '0';
    
    //2.开数组存放结果，并开两个循环进行计算
    //  乘法的结果的位数小于等于两数位数之和
    //  设父循环当前次数为i，子循环当前次数为j，则当前结果存放在结果数组的[i+j-1]位置
    //  每次父循环都要有专门的变量x存放进位，当子循环结束后，设子循环一共进行了q次，则x放在结果数组的[i+q]中
    int* nc = new int[a.size() + b.size() + 1];
    memset(nc, 0, sizeof(int) * (b.size()+a.size() + 1));
    for (int i = 1; i <= a.size(); i++) {
        int x = 0;
        for (int j = 0; j <= b.size(); j++) {
            nc[i + j - 1] += na[i] * nb[j] + x;
            x = nc[i + j - 1] / 10;
            nc[i + j - 1] %= 10;
        }
        nc[i + b.size()] = x;
    }
    
    //3.去除多余的0，找到正确位置
    int lenc = a.size() + b.size() ;
    while (nc[lenc] == 0 && lenc > 1)lenc--;

    //4.再输出
    stringstream ss;
    for (int i = 0; i < lenc; i++)ss << nc[lenc - i];
    return ss.str();
}

//高精度除法
string* devide_(string a, string b) {
    //re[0]=a/b;
    //re[1]=a%b;
    //0.过滤前导的0
    stringstream sa, sb;
    for (int i = 0, flag = 0; i < a.size(); i++) {
        if (a[i] == '0' && flag == 0) continue;
        if (a[i] != '0' && flag == 0) flag = 1;
        sa << a[i];
        //cout << "touch"<<a[i] << endl;
    }
    //cout<<(a = sa.str())<<endl;
    for (int i = 0, flag = 0; i < b.size(); i++) {
        if (b[i] == '0' && flag == 0) continue;
        if (b[i] != '0' && flag == 0) flag = 1;
        sb << b[i];
    }
    b = sb.str();
    
    //1.转倒序数组
    int* na = new int[a.size() + 1],*nb=new int[b.size()+1];
    memset(na, 0, sizeof(int) * (a.size() + 1));
    memset(nb, 0, sizeof(int) * (b.size() + 1));
    na[0] = a.size();
    nb[0] = b.size();
    for (int i = 0; i < a.size(); i++) na[a.size() - i] = a[i] - '0';
    for (int i = 0; i < b.size(); i++) nb[b.size() - i] = b[i] - '0';

    //2.开结果数组并计算
    //  这里加个特判，如果第一个数大于第二个数,如果两数相等
    //  结果数组大小取第一个除数
    int c = compare(na, nb);
    if (c == -1) {
        string* re = new string[2];
        re[0] = "0";
        re[1] = a;
        return re;
    }
    if(c==0){
        string* re = new string[2];
        re[1] = "0";
        re[0] = "1";
        return re;
    }
    //a/b，所得到的结果的位数小于等于a的位数减去b的位数
    int* nc = new int[na[0] - nb[0] +2];
    memset(nc, 0, sizeof(int) * (na[0] - nb[0] + 2));
    //位移次数等于结果位数的最大值
    nc[0] = na[0] - nb[0] + 1;
    for (int i = nc[0]; i > 0; i--) {//i>0!!!
        //temp存放除数经位移后的倒序数组
        int* temp = new int[nb[0]+i];
        moveNum(nb, temp, i - 1);//位移位数是i-1
        while (compare(na, temp) >= 0) {
            nc[i]++;
            minus_arr(na, temp);
        }
        delete[] temp;
    }

    //3.输出结果，这时候na为余数，nc为结果。先矫正nc位数
    while (nc[nc[0]] == 0 && nc[0] > 0) nc[0]--;
    string* re = new string[2];
    stringstream ss1,ss2;
    for (int i = 0; i < nc[0]; i++) ss1 << nc[nc[0] - i];
    re[0] = ss1.str();
    if (na[0] == 0) {
        re[1] = "0";
        return re;
    }
    for (int i = 0; i < na[0]; i++) ss2 << na[na[0] - i];
    re[1] = ss2.str();
    return re;
}



int main()
{
    string a, b;
    cin >> a >> b;
    cout << plus_(a, b)<<endl;
    cout << minus_(a, b)<<endl;
    cout << multiply_(a, b)<<endl;
    string* re = devide_(a, b);
    cout << re[0]<<endl<<re[1];
}