// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <windows.h>
#include<WinSock2.h>
#include<stdio.h>
#include<ws2tcpip.h>
#include<string>
#pragma comment(lib, "ws2_32.lib")

int main(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2)
	{
	  printf("请求版本失败"); 
	  return -1;  
	}
	
	SOCKET serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == serverSocket)
	{
		printf("套接字创建失败");
		return -1;    
	}
	
	SOCKADDR_IN serverAddr = {0}; //初始化协议地址
	serverAddr.sin_family = AF_INET;//必须和创建socket的IP协议一致
	serverAddr.sin_port = htons(8080);    //服务器端口,客户端通过这个端口连接
	 
	//ip地址是点分格式的字符串 , 用inet_addr来转换整数
	//serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", (void*)&serverAddr.sin_addr.s_addr);
	if(SOCKET_ERROR == bind(serverSocket , (SOCKADDR*)&serverAddr , sizeof(serverAddr)))
	{
	  printf("绑定失败");
	  closesocket(serverSocket);  //释放socket
	  WSACleanup();  //关闭套接字
	  return -1;  
	}
	printf("绑定成功");
	
	if(SOCKET_ERROR == listen(serverSocket, 10))	//10表示监听队列大小上限 
	{
	  printf("监听失败");
	  closesocket(serverSocket);  //释放socket
	  WSACleanup();  //关闭套接字
	  return -1;  
	}
	printf("监听成功");
	
	SOCKADDR_IN clientAddr = {0};
	/*char aaa[] = "aaaa";
	std::string str = "a";
	printf(">>%d\n", strlen(aaa));
	printf(">>%d\n", sizeof(aaa));
	printf(">>%d\n", sizeof(str));
	*/
	int len = sizeof(clientAddr);
	SOCKET clientSocket = accept(serverSocket,(SOCKADDR*)&clientAddr, &len);    //阻塞函数，若连接成功clientAddr会被客户端地址填充，返回一个clientSocket用于收发
	if(clientSocket == INVALID_SOCKET)
	{
	  printf("连接失败");
	 closesocket(serverSocket);    //释放socket
	 WSACleanup();   //关闭套接字
	  return -9;
	}
	printf("连接成功");
	printf("客户端(%s)\n");
	
	//char recvBuff[128] = {};

	while(1)
	{
		uint8_t* buffer = new uint8_t[13];
	  //memset(recvBuff, 0 , sizeof(recvBuff));
	  if(recv(clientSocket , (char *) buffer,13, 0) >0)  //阻塞函数
	  {
		  int A = 0;
		  int B = 0;
		  int* a = &A;
		  int* b = &B;
		  char str[5] = {};
		  printf(">>%d\n", sizeof(str));
		  //printf(">>%d\n", sizeof(str));
		  //printf(">>%d\n", str.size());
		  memcpy(a, buffer, 4);
		  memcpy(b, buffer + 4, 4);
		  memcpy(str, buffer + 8, sizeof(str));
	       printf(">>%d\n" , *a);
		   printf(">>%d\n", *b);
		  printf(">>%s\n", str);
	  }
	  char sendBuff[128] = {};
	  memset(sendBuff, 0 , sizeof(sendBuff));
	  printf("请输入:\n");
	  scanf_s("%s" , sendBuff, sizeof(sendBuff)-1);
	  //一次收发  
	  send(clientSocket , sendBuff, strlen(sendBuff), 0);  
	}
	
	
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
