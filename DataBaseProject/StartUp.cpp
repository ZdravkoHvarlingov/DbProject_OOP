#include <iostream>
#include "Text.h"
#include "NotImplementedException.h"

using std::cout;
using db::Text;
using db::NotImplementedException;

int main()
{
	try
	{
		Text txt;
		DbType* typeTxt = &txt;
		typeTxt->SetStringValue("okay, this is a string cell");
		cout << typeTxt->GetValueAsString() << '\n';
		cout << txt.GetValueAsString() << '\n';
	}
	catch (const db::NotImplementedException& ex)
	{
		cout << ex.what() << '\n';
	}
	
}