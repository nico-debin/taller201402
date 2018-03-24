// #include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstring>

#include "TestJson.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void TestJson::test(){

	string line;
	string allLines;
	ifstream myfile ("ejemplo.json");

	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			allLines = allLines + line;
		}
		myfile.close();
	}
	else { 
		cout << "Unable to open file"; 
	}
	cout << "leyo el archivo ejemplo.json " << endl;
	system ("pause");

	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( allLines, root );

	if ( !parsingSuccessful )
	{
		// report to the user the failure and their locations in the document.
		std::cout  << "Failed to parse configuration\n"
				   << reader.getFormattedErrorMessages();
		return;
	}
	cout << "Parseo el archivo ejemplo.json" << endl;
	system ("pause");

	Json::Value jsonEscenario = root.get("escenario", "default content" );
	Json::Value jsonObjetosDelEscenario = jsonEscenario.get("objetos", "default content" );
	if (jsonObjetosDelEscenario.isArray()){
		//int size = jsonObjetosDelEscenario.size();
		for (int i = 0; i < jsonObjetosDelEscenario.size(); i++) {
			//Figura *figura = FabricaDeFiguras::crearFigura(jsonObjetosDelEscenario[i]);
		}
	}

	system ("pause");
}


