#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstring>

#include <json/json.h>

using namespace std;

int test_JSON(){
	printf("\n>>>>>>>>>>> %s Test: %s <<<<<<<<<<<\n\n", "Start", "JSON");
	 string json_example = "{\"array\": \
								[\"item1\", \
								\"item2\"], \
								\"not an array\": \
								\"asdf\" \
							 }";

	 // Let's parse it  
	 Json::Value root;
	 Json::Reader reader;
	 bool parsedSuccess = reader.parse(json_example, 
									   root, 
									   false);
  
	 if(!parsedSuccess)
	 {
	   // Report failures and their locations 
	   // in the document.
	   cout<<"Failed to parse JSON"<<endl 
		   <<reader.getFormatedErrorMessages()
		   <<endl;
	   return 1;
	 }
  
	 // Let's extract the array contained 
	 // in the root object
	 const Json::Value array = root["array"];
 
	 // Iterate over sequence elements and 
	 // print its values
	 for(unsigned int index=0; index<array.size(); 
		 ++index)  
	 {  
	   cout<<"Element " 
		   <<index 
		   <<" in array: "
		   <<array[index].asString()
		   <<endl;
	 }
  
	 // Lets extract the not array element 
	 // contained in the root object and 
	 // print its value
	 const Json::Value notAnArray = 
				   root["not an array"];
 
	 if(! notAnArray.isNull())
	 {
	   cout<<"Not an array: "
		   <<notAnArray.asString()
		   <<endl;
	 }
 
	 // If we want to print JSON is as easy as doing:
	 cout<<"Json Example pretty print: "
		 <<endl<<root.toStyledString()
		 <<endl;

	printf("\n>>>>>>>>>>> %s Test: %s <<<<<<<<<<<\n\n", "End", "JSON");

	 return 0;
}