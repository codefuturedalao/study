## C++笔记

### 1、写日志

不定参数：

​	`int func(int x, int y, ...)`

​	`va_list arg`指针指向y后的不定参数：`va_start(arg, y)`

​	获取不定参数的值：`int nParamNum = va_arg(arg, int)`，指定自动后移一位	

​	带不定参数的宏：`#define MY_LOG_PRINTF(fmt, ...) WriteLog(fmt, ##__VA_ARGS__)`

```C++
//获取系统时间
{
    time_t tNow = 0;	//unsigned long. 1900-1-1:00:00:00到现在的秒钟
    struct tm stTime;

    time(&tNow);
    localtime_s(&stTime, &tNow);
    //printf("%d-%02d-%02d %02d:%02d:%02d\n", 1900 + stTime.tm_year, 1 + stTime.tm_mon, 
    //		stTime.tm_mday, stTime.tm_hour, stTime.tm_min, stTime.tm_sec);
    strftime(szBuff, LOG_MAX_SIZE - 1, "%Y-%m-%d %X ", &stTime);
    printf("%s\n", szBuff);
}		
```

```C++
// 错误处理
const int LOG_MAX_SIZE = 512;
int g_nLogLevel = 0x4;

#define MY_LOG_PRINTF(level, fmt, ...) \
do {\
	if (level & g_nLogLevel) WriteLog(fmt, ##__VA_ARGS__);\
} while (FALSE)

//错误级别
typedef enum MY_ERR_LEVEL
{
	ERR_NONE = 0,
	ERR_WARNING = 0x1,
	ERR_MINOR = 0x1 << 1,
	ERR_MAJOR = 0x1 << 2,
	ERR_MAX = 0x1 << 3
};
```

### 2、QQ消息轰炸机

```C++
UpdateData(true);//用于将屏幕上控件中的数据交换到变量中。

UpdateData(false);//用于将数据在屏幕中对应控件中显示出来。

wchar_t* pContent = m_msg.AllocSysString();	//CString转wchar_t*
```

#### MFC全局热键

```C++
//MFC 全局热键
OnInitialDlg(){
    RegisterHotKey(m_hWnd,1001,MOD_CONTROL|MOD_SHIFT,'y');
    RegisterHotKey(m_hWnd,1002,MOD_CONTROL|MOD_SHIFT,'Y');
}

//添加OnDestroy()函数
OnDestroy(){
    CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	UnregisterHotKey(m_hWnd, 1001);
	UnregisterHotKey(m_hWnd, 1002);
}

//添加OnHotKey()函数
void CXxxx::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	if (nHotKeyId == 1001 || nHotKeyId == 1002) {
		//CWnd::SetForegroundWindow();//使得被激活窗口出现在前景 
		
		MessageBox(L"Keydown");
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
```

```C++
//wchar_t
wchar_t szName[128] = { 0 };
swprintf_s(szName, L"%s%d", "crystal", 123);
wcslen(szName);	//长度
wcscmp(szName, L"Crystal123");	//比较字符串

```

### 3、RAR文件隐藏

#### 3.1 RAR文件特征：

+ <font face="宋体" size="4">以0x52 0x61 0x72 0x21开头</font>
+ <font face="宋体" size="4">以0x00 0x40 0x07 0x00结尾</font>
+ <font face="宋体" size="4">rar文件解析时，直接找开头位置和结束位置解析</font>

#### 3.2 jpg文件特征

 + <font face="宋体" size="4">以0xFF 0xD8开头</font>
 + <font face="宋体" size="4">以0xFF 0xD9结尾</font>

### 4、C++多线程

#### 4.1 基础

```C++
void Func_1(string& msg){
    cout << msg << endl;
    msg = "modified";
}

int main(){
    std::string msg = "original";
	std::thread t1(Func_1, std::ref(msg));		//注意，ref(),传地址
    //或者
    //std::thread t1(Func_1, std::move(msg));

	return 0;
}

//std::thread t2 = t1;   错误，必须使用move();
std::thread t2 = move(t1);	//移动后t1为空，不能join()

cout << hardware_concurrency() << endl;	//查看CPU数目
```

#### 4.2 数据竞争与锁

```C++

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>

using namespace std;

//类
class lockFie
{
public:
	lockFie() {
		//of.open("Z:/test.txt");
	}
	void sharePrint(string id, int value) {
		call_once(m_flag, [&]() {of.open("Z:/test.txt"); });

		std::lock(m_mutex, m_mutex2);
		lock_guard<mutex> locker(m_mutex, adopt_lock);
		lock_guard<mutex> locker2(m_mutex2, adopt_lock);
		cout << "from" << id << ": " << value << endl;
	}

	void sharePrint2(string id, int value) {
		call_once(m_flag, [&]() {of.open("Z:/test.txt"); });
		std::lock(m_mutex, m_mutex2);
		lock_guard<mutex> locker(m_mutex, adopt_lock);
		lock_guard<mutex> locker2(m_mutex2, adopt_lock);
		cout << "from" << id << ": " << value << endl;
	}

	void sharePrint3(string id, int value) {
		call_once(m_flag, [&]() {of.open("Z:/test.txt"); });
		unique_lock<mutex> locker(m_mutex, defer_lock);

		locker.lock();
		cout << "from" << id << ": " < , value << endl;
		locker.unlock();

		unique_lock<mutex> locker2 = std::move(locker);
	}

	ofstream& GetStream() {
		return of;
	}

	void process(void func(ofstream&)) {
		func(of);
	}
protected:
private:
	mutex m_mutex;
	mutex m_mutex2;
	mutex m_mutex_open;
	once_flag m_flag;
	ofstream of;
};


void Func(lockFie& lof)
{
	for (int i = 0; i < 100; i++){
		lof.sharePrint("t1", i);
	}
}


int main()
{
	lockFie lof;
	thread t1(Func, std::ref(lof));

	string msg = "I love crystal";
	for (int i = 0; i < 100; i++){
		lof.sharePrint2("main", i);
	}
	t1.join();

	system("pause");
	return 0;
}
```

#### 4.3 条件变量

 可以避免重复低效的循环

> wait 和 notify (notify_one/notify_all) 配对使用。 
>
> “Notify one： Unblocks one of the threads currently waiting for this condition.If no threads are waiting, the function does nothing.”
>
> “But certain implementations may produce spurious wake-up calls without any of these functions being called. Therefore, users of this function shall ensure their condition for resumption is met.”
>
> notify_one是通知一个线程获取锁，notify_all是通知所有相关的线程去竞争锁。
> notify_one不能保证获得锁的线程真正需要锁，并且因此可能产生死锁。
>
> notify_one()和notify_all()都是Object对象用于通知处在等待该对象的线程的方法。
>
> 两者的最大区别在于：
>
> notify_all使所有原来在该对象上等待被notify的线程统统退出wait的状态，变成等待该对象上的锁，一旦该对象被解锁，他们就会去竞争。
>
> notify_one则文明得多他只是选择一个wait状态线程进行通知，并使它获得该对象上的锁，但不惊动其他同样在等待被该对象notify_one的线程们，当第一个线程运行完毕以后释放对象上的锁此时如果该对象没有再次使用notify_one语句，则即便该对象已经空闲，其他wait状态等待的线程由于没有得到该对象的通知，继续处在wait状态（直到这个对象发出一个notify_one或notify_all，它们等待的是被notify或notify_all，而不是锁）。

```C++
mutex mu;
deque<int> q;
condition_variable cond;		//条件变量

void Func_1()
{
	int count = 10;
	while (count > 0) {
		unique_lock<mutex> locker(mu);
		q.push_front(count);
		locker.unlock();
		cond.notify_one();		//激活锁
		std::this_thread::sleep_for(chrono::seconds(1));
		count--;
	}
}

void Func_2()
{
	int data = 0;
	while (data != 1) {
		unique_lock<mutex> locker(mu);
		cond.wait(locker, []() {return !q.empty();});
		data = q.back();
		q.pop_back();
		locker.unlock();
		cout << "t2 got a value from t1: " << data << endl;
	}
}

```

#### 4.4 future

>std::launch::defered 指定等到 wait 或 get 被调用时才执行.
>std::launch::async 指定必须到独立的线程中执行.
>默认为: std::launch::defered | std::launch::async

```C++
#include <future>

using namespace std;

int factorial(int N)
{
	int res = 1;
	for (int i = 1; i <= N; i++){
		res *= i;
	}
	cout << "result is : " << res << endl;

	return res;
}

int main()
{
	int x = 0;
	std::future<int> fu = std::async(std::launch::async, factorial, 4);
	x = fu.get();

	system("pause");
	return 0;
}
```

#### 4.5 promise

```C++
int factorial(future<int>& f)
{
	int res = 1;

	int N = f.get();
	for (int i = 1; i <= N; i++){
		res *= i;
	}
	cout << "result is : " << res << endl;

	return res;
}

int main()
{
	int x = 0;

	std::promise<int> p;
	//std::promise<int> p2 = move(p);  //promise和future只能移动，不能copy
	future<int> f = p.get_future();

	std::future<int> fu = std::async(std::launch::async, factorial, ref(f));
	p.set_value(4);		//如果此处没有set_value, 将在Func_1中f.get()抛出异常 std::future_errc::broken_promise
	x = fu.get();
	cout << "Get from child: " << x << endl;

	system("pause");
	return 0;
}
```

#### 4.6 share_future 

可被copy

```C++
std::promise<int> p;
future<int> f = p.get_future();
std::shared_future<int> sf = f.share();

std::future<int> fu = std::async(std::launch::async, factorial, sf);
std::future<int> fu2 = std::async(std::launch::async, factorial, sf);
std::future<int> fu3 = std::async(std::launch::async, factorial, sf);
```

#### 4.7 创建线程的方式

```C++
class A
{
public:
	void func(int i, char c){ }
	int operator()(int N) { return 0; }
protected:
private:
};

void foo(int x) {}

int main()
{
	A a;
	std::thread t1(a, 6);
	std::thread t2(std::ref(a), 6);		//传递a的引用
	std::thread t3(std::move(a), 6);	//移动a到子线程中，此时a在主线程已经失效
	std::thread t4(A(), 6);			//临时创建的对象

	std::thread t5(foo, 6);
	std::thread t6([](int x) {return x * x; }, 6);
	
	std::thread t7(&A::func, a, 8, 'w');	//传递a的拷贝的成员函数
	std::thread t8(&A::func, &a, 8, 'w');	//传递a的拷贝的成员函数
	
	std::async(std::launch::async, a, 6);	//传递a的地址的成员函数

	system("pause");
	return 0;
}
```

#### 4.8 packaged_task

```C++
//阶乘函数
int factorial(int N)
{
	int res = 1;
	for (int i = N; i > 1; i--) {
		res *= i;
	}
	std::cout << "result is:" << res << std::endl;

	return res;
}

std::deque<std::packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cond;

void thread_1()
{
	std::packaged_task<int()> t;
	{
		std::unique_lock<std::mutex> locker(mu);	//使用条件变量必须用unique_lock, 不能用lock_guard
		cond.wait(locker, []{return !task_q.empty(); });	//条件变量，等待
		t = std::move(task_q.front());
	}
	t();
}

int main()
{
	std::thread t1(thread_1);
	std::packaged_task<int()> t(std::bind(factorial, 6));
	std::future<int> ret = t.get_future();
	{
		std::lock_guard<std::mutex> locker(mu);
		task_q.push_back(std::move(t));
	}
	cond.notify_one();
	int value = ret.get();

	t1.join();

	//auto t = std::bind(factorial, 6);
	//t();

	system("pause");
	return 0;
}
```

#### 4.9 时间限制

##### 4.9.1 线程

```C++
std::thread t1(factorial, 6);
//让线程暂停3毫秒
//方法 一
std::this_thread::sleep_for(std::chrono::milliseconds(3));
//方法 二
std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now()
    + std::chrono::microseconds(3);
std::this_thread::sleep_until(tp);
```

##### 4.9.2 互斥锁

```C++
std::mutex mu;
std::unique_lock<std::mutex> locker(mu);
//方法 一
locker.try_lock_for(std::chrono::microseconds(3));	
//方法 二
std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now()
    + std::chrono::microseconds(3);
locker.try_lock_until(tp);	
```

##### 4.9.3 条件变量

```C++
std::condition_variable cond;
cond.wait_for(locker, std::chrono::microseconds(3));
cond.wait_until(locker, tp);
```

##### 4.9.4 future

```C++
std::promise<int> p;
std::future<int> f = p.get_future();
f.wait_for(std::chrono::microseconds(3));	//方法 一
f.wait_until(tp);	//方法 二
```

### 5、Lambda函数

#### 5.1 简介

​	即匿名函数：[](){ }

​	`auto func = [](){ cout << "hello world!" << endl; }`

​	也可以显式的指明返回类型

​	`[]()->int { return 1; }`

#### 5.2 标准形式

​	`[captures](params)-> ret {Statments;} `

#### 5.3 变量截取

​	使用外部定义的变量

```C++
#include <string>  
#include <vector>  
  
class AddressBook  
{  
    public:  
    // using a template allows us to ignore the differences between functors, function pointers   
    // and lambda  
    template<typename Func>  
    std::vector<std::string> findMatchingAddresses (Func func)  
    {   
        std::vector<std::string> results;  
        for ( auto itr = _addresses.begin(), end = _addresses.end(); itr != end; ++itr )  
        {  
            // call the function passed into findMatchingAddresses and see if it matches  
            if ( func( *itr ) )  
            {  
                results.push_back( *itr );  
            }  
        }  
        return results;  
    }  
  
    private:  
    std::vector<std::string> _addresses;  
};  
```

```C++
AddressBook global_address_book;  
  
vector<string> findAddressesFromOrgs ()  
{  
    return global_address_book.findMatchingAddresses(   
        // we're declaring a lambda here; the [] signals the start  
        [] (const string& addr) { return addr.find( ".org" ) != string::npos; }   
    );  
}  
```

​	从键盘读取一个数据

```C++
vector<string> findAddressFromOrgs() {
    // read in the name from a user, which we want to search  
	string name;  
	cin>> name;  
	return global_address_book.findMatchingAddresses(   
    	// notice that the lambda function uses the the variable 'name'  
   	 	[&] (const string& addr) { return name.find( addr ) != string::npos; }
    );
}
```

​	这个lambda函数一个最大的区别是[]中间加入了&符号。这就告诉了编译器，要进行变量截取。这样lambda函数体就可以使用外部变量。如果不加入任何符号，编译器就不会进行变量截取。

下面是各种变量截取的选项：

- [] 不截取任何变量
- [&} 截取外部作用域中所有变量，并作为引用在函数体中使用
- [=] 截取外部作用域中所有变量，并拷贝一份在函数体中使用
- [=, &foo]   截取外部作用域中所有变量，并拷贝一份在函数体中使用，但是对foo变量使用引用
- [bar]   截取bar变量并且拷贝一份在函数体重使用，同时不截取其他变量
- [this]   截取当前类中的this指针。如果已经使用了&或者=就默认添加此选项。

#### 5.4 Lambda与STL

```C++
vector<int> v;  
v.push_back( 1 );
v.push_back( 2 );
//...  
for ( auto itr = v.begin(), end = v.end(); itr != end; itr++ ) {  
    cout << *itr;  
}  
```

用for_each实现

```C++
vector<int> v;
v.push_back( 1 );
v.push_back( 2 );
//...  
for_each( v.begin(), v.end(), [](int val){cout << val;} );  
```

### 6、智能指针

#### 6.1 简介

> C++11中引入了智能指针的概念，方便管理堆内存。使用普通指针，容易造成堆内存泄露（忘记释放），二次释放，程序发生异常时内存泄露等问题等，使用智能指针能更好的管理堆内存。

1.  从较浅的层面看，智能指针是利用了一种叫做RAII（资源获取即初始化）的技术对普通的指针进行封装，这使得智能指针实质是一个对象，行为表现的却像一个指针。

2.  能指针的作用是防止忘记调用delete释放内存和程序异常的进入catch块忘记释放内存。另外指针的释放时机也是非常有考究的，多次释放同一个指针会造成程序崩溃，这些都可以通过智能指针来解决。

3.  智能指针还有一个作用是把值语义转换成引用语义。C++和Java有一处最大的区别在于语义不同，在Java里面下列代码：

      　　Animal a = new Animal();

      　　Animal b = a;

   ​     你当然知道，这里其实只生成了一个对象，a和b仅仅是把持对象的引用而已。但在C++中不是这样，

   ​     	Animal a;

      	Animal b = a;

   ​     这里却是就是生成了两个对象。

#### 6.2 实现

```C++
template<typename T>
class SmartPtr
{
public:
	//构造函数
	SmartPtr() :_ptr(new T()), _ref(new int(1)) {}
	SmartPtr(T* p) :_ptr(p), _ref(new int(1)) {}
	SmartPtr(const SmartPtr& sptr) :_ptr(sptr._ptr), _ref(sptr._ref) {
		++(*_ref);
	}
	//析构函数
	~SmartPtr() {
		if (0 == --*_ref) {
			delete _ptr;
			delete _ref;
			_ptr = nullptr;
			_ref = nullptr;
		}
	}
public:
	//重载操作符->
	T* operator ->() {
		return _ptr;
	}
	//重载操作符*
	T& operator *() {
		return *_ptr;
	}
	//重载操作符=
	SmartPtr& operator =(SmartPtr& inst) {
		++*inst._ref;	//引用计数加一
		if (0 == --*_ref) {
			delete _ptr;
			delete _ref;
		}
		_ptr = inst._ptr;
		_ref = inst._ref;

		return *this;
	}
private:
	int* _ref;	//引用计数
	T*   _ptr;	//要管理的指针对象
};
```

#### 6.3 ADO操作数据库

​	

### 7、ADO

#### 7.1 简介

>ADO (ActiveX Data Object)
>
>_ConnectionPtr //对数据库的连接
>
>_CommandPtr //执行SQL语句
>
>_RecordsetPtr //可以获取表的记录集 游标

<font face="宋体" color="red" size=5 style="font-weight:bold">连接Excel、Access2007及以后的版本需要下载ACE引擎</font>

http://www.microsoft.com/downloads/details.aspx?displaylang=zh-cn&FamilyID=7554f536-8c28-4598-9b72-ef94e038c891

`"Provider=Microsoft.ACE.OLEDB.12.0;Data source=Z:\\ICRYSTAL\\UserInfo.accdb"`

#### 7.2 实现

stdafx.h文件中添加（写在一行中，可以用\结束一行）

```C++
#import "C:\\Program Files\\Common Files\\system\\ado\\msado15.dll" no_namespace\
	rename("EOF", "adoEOF") rename("BOF", "adoBOF")\
	rename("DataTypeEnum", "adoDataTypeEnum")\
	rename("FieldAttributeEnum", "adoFieldAttributeEnum") rename("EditModeEnum", "adoEditModeEnum")\
	rename("LockTypeEnum", "adoLockTypeEnum") rename("RecordStatusEnum", "adoRecordStatusEnum")\
	rename("ParameterDirectionEnum", "adoParameterDirectionEnum")
```

```C++
//private:加载数据
void CAdoAccdb::LoadInfo()
{
	//初始化COM组件
	HRESULT hr = ::CoInitialize(NULL);		//每个操作数据库的线程必须初始化com组件
	if (hr) {
		MessageBox(_T("初始化com组件失败"), _T("错误"));
		return;
	}

	//创建连接
	_ConnectionPtr pConn;
	pConn.CreateInstance(__uuidof(Connection));

	//数据源
	CString strDSN = _T("Provider=Microsoft.ACE.OLEDB.12.0;Data source=Z:\\ICRYSTAL\\UserInfo.accdb");
	pConn->Open((_bstr_t)strDSN, "", "", -1);

	//数据集
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	CString strSql = _T("SELECT * FROM Info");

	//打开记录集
	pRs->Open((_bstr_t)strSql, (IDispatch*)pConn, adOpenDynamic, adLockReadOnly, adCmdText);

	_variant_t var;
	_bstr_t str;
	CString strValue;

	USES_CONVERSION;
	UINT index = 0;
	while (!pRs->GetadoEOF()) {
		var = pRs->GetCollect("UserID");
		strValue.Format(_T("%s"), A2W((_bstr_t)var.bstrVal));
		//把读出来的数据插入list控件
		m_listUsers.InsertItem(index, strValue);

		var = pRs->GetCollect("UserName");
		strValue.Format(_T("%s"), A2W((_bstr_t)var.bstrVal));
		m_listUsers.SetItemText(index, 1, strValue);

		var = pRs->GetCollect("UserTel");
		strValue.Format(_T("%s"), A2W((_bstr_t)var.bstrVal));
		m_listUsers.SetItemText(index, 2, strValue);

		var = pRs->GetCollect("UserAddr");
		strValue.Format(_T("%s"), A2W((_bstr_t)var.bstrVal));
		m_listUsers.SetItemText(index, 3, strValue);

		//移动到下一条记录
		pRs->MoveNext();
		index++;
	}

	pRs->Close();//关闭记录集
	pConn->Close();//关闭连接
	pRs.Release();
	pConn.Release();
	
	::CoUninitialize();
}

```

#### 7.3 补充与完善

+ Excel2007以后的版本(.xlsx)和Access2007以后的版本(.accdb)使用ACE引擎

  `"Provider=Microsoft.ACE.OLEDB.4.0;Data source=Z:\\ICRYSTAL\\UserInfo.accdb"`

+ 之前的版本(.xls, .mdb)使用jet引擎

  `"Provider=Microsoft.Jet.OLEDB.12.0;Data source=Z:\\ICRYSTAL\\UserInfo.accdb"`

+ 数据库较大，数据记录较多时，可创建新线程读取。

+ MFC类中线程处理函数声明为static。主线程已经自动加载com组件，其他线程<font color="red">必须</font>手动CoInitialize(NULL)和CoUninitialize()，初始化和释放com组件。

### 8、SMTP协议

#### 8.1 命令行Telnet实现

以qq邮箱为例

1. 开启smtp服务，并获取授权码

2. 进入 "control panel->Turn Windows feature on or off" 勾选Telnet Client，开启telnet。

3. 进入命令行，输入telnet，进入telnet

4. 输入 `o smtp.qq.com 25`

5. 成功连接smtp服务器后，输入 `helo qq`，服务器返回 `250 smtp.qq.com`

6. 输入 `auth login` ，服务器返回 `334 xxx`

7. 使用`base64`加密邮箱用户名，并输入，服务器返回 `334 xxx`

8. 使用`base64`加密邮箱密码，并输入，服务器返回 `235xxx`

9. 输入 `mail from:<from@qq.com>`，服务器返回 `250 OK`

10. 输入 `rcpt to:<to@qq.com>`，服务器返回 `250 OK`

11. 输入 `data`， 接下来输入邮件正文，格式如下：

    >From: from@qq.com
    >
    >To: to@qq.com
    >
    >subject: xxx
    >
    >​							//(此处敲回车，空一行)
    >
    >xxx content xxx
    >
    >​							//(此处敲回车，空一行)
    >
    >.							//(此处输入英文点号)

    服务器返回 `250 Ok: queued as`


### 9、管道通信

NULL