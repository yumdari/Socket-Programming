#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define OPSZ 4
void error_handling(char *message);	// 에러 처리 함수 선언
int calculate(int opnum, int opnds[], char oprator);	// calculate 함수 선언

int main(int argc, char *argv[])	// main function
{
	int serv_sock, clnt_sock;	
	char opinfo[BUF_SIZE];		
	int result, opnd_cnt, idx;	// result : 연산 결과값
								// opnd_cnt : 피연산자 개수
								// idx : 
	int recv_cnt, recv_len;
	struct sockaddr_in serv_adr, clnt_adr;	// 서버 및 클라이언트 주소를 포함한sockaddr  구조체 생성
	socklen_t clnt_adr_sz;
	if(argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	clnt_adr_sz = sizeof(clnt_adr);
	
	for(idx = 0; idx < 5; idx++)
	{
		opnd_cnt = 0;
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		read(clnt_sock, &opnd_cnt, 1);

		recv_len = 0;
		while((opnd_cnt*OPSZ + 1) > recv_len)
		{
			recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}
		result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);	// calculate 함수 호출
		write(clnt_sock, (char*)&result, sizeof(result));
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

int calculate(int opnum, int opnds[], char op)	// calculate 함수 정의
{
	int result = opnds[0], idx;	//
	switch(op)
	{
		case '+' :
			for ( idx = 1; idx < opnum; idx ++) result +=opnds[idx];
			break;
		case '-' :
			for (idx = 1; idx < opnum; idx ++) result -= opnds[idx];
			break;
		case '*' :
			for ( idx = 1; idx < opnum ; idx ++) result *= opnds[idx];
			break;
		}
	return result;
}

void error_handling(char *message)
{
}
