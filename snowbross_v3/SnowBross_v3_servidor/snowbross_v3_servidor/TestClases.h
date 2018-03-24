#ifndef _TestClases_H_
#define _TestClases_H_

#include "Cliente.h"
#include "Servidor.h"

using namespace std;

class TestClases{
	private:
		static int iniciarServidorConClases();
		static int iniciarClienteConClases();

	public:
		static void test();
};

#endif