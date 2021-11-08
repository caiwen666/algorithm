#include <iostream>

using namespace std;

//选择排序
//num:数组的成员数量 array:待排序的数组，排序后从小到大
void sort_select(int num,int *array) {
	for(int i = 0; i < num-1; i++) {//最后一个不用排了
		int min;//遍历查找最小的数字的下标
		for (int j = 0; j < num - i; j++) {//子循环循环num-i次
			if (j == 0) min=i+j;//当前选项为i+j
			else if (array[i + j] < array[min]) min = i + j;
		}
		//然后交换
		swap(array[i], array[min]);
	}
}

//冒泡排序
//num:数组的成员数量 array:待排序的数组，排序后从小到大
void sort_bubble(int num,int *array) {
	for (int i = 0; i < num - 1; i++) {//父循环依然是num-1次
		//这里是把大数放到数组末尾
		//flag表示子循环循环一圈是不是没有交换数字，如果没有交换说明排序完毕，无需进行后续交换
		int flag = 0;
		for (int j = 0; j < num - i - 1;j++) {//子循环是父循环次数-当前循环第次数
			if (array[j] > array[j + 1]) {
				swap(array[j], array[j + 1]);
				flag = 1;
			}
		}
		if (flag == 0) {
			break;
		}
	}
}

//插入排序
//num:数组的成员数量 array:待排序的数组，排序后从小到大
void sort_insert(int num, int* array) {
	//插入排序从第二个数开始选择，所以i从1开始，循环次数依然是num-1
	for (int i = 1; i < num; i++) {
		//将选中的数向前比对，如果这个数比比对的数小就继续向前比,并把对比到的数扔到后面
		int j = i-1,key=array[i];//注意先把选择的数拷贝出来
		//j为要进行比对的数组成员下标，key为选择到的数字
		while (j >= 0 && key < array[j]) {
			array[j+1] = array[j];
			j--;
		}
		//最后插入，插入位置为j+1
		array[j+1] = key;
	}
}

//归并排序
//num:数组的成员数量 array:待排序的数组，排序后从小到大
//这个函数为起始函数
void sort_merge(int num,int *array) {
	//0.递归界限，当num为1时直接返回
	if(num==1) return;
	//1.根据num将array分为两部分进行递归
	int* a = new int[num / 2],lenb;
	if (num % 2 == 0) lenb = num / 2;
	else lenb = num / 2 + 1;
	int* b = new int[lenb];
	memcpy(a, array, sizeof(int) * (num / 2));
	memcpy(b, array + num / 2, sizeof(int) * (lenb));
	sort_merge(num / 2, a);
	sort_merge(lenb, b);
	//2.递归后数组a,b就排序好了，再归并排序
	for (int i = 0, pa = 0, pb = 0; i <num; i++) {
		if (pa == num / 2) {
			array[i] = b[pb++];
			continue;
		}
		if(pb==lenb) {
			array[i] = a[pa++];
			continue;
		}
		if (a[pa] < b[pb]) array[i] = a[pa++];
		else array[i] = b[pb++];
	}
	//if (num % 2 != 0) array[num - 1] = b[lenb - 1];
	/*
	cout << "task" << num << endl;
	for (int i = 0; i < num / 2; i++) cout << a[i] << " ";
	cout << endl;
	for (int i = 0; i < lenb; i++) cout << b[i] << " ";
	cout << endl << endl;
	*/
}

int sort_() {
	int n,*arr;
	cin >> n;
	arr = new int[n];
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}
	sort_merge(n, arr);
	for (int i = 0; i < n; i++) {
		cout << arr[i]<<" ";
	}
	sort_();
	return 0;
}