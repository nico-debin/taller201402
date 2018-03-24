#include "stdafx.h"
#include "TestClases.h"

void TestClases::test(){
	TestClases::iniciarServidorConClases();
	system("pause");
}

int TestClases::iniciarServidorConClases(){
	int puerto = 8200;
	Servidor servidor(puerto);
	getchar();
	return 0;
}