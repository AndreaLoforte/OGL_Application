#pragma once
#include<map>
#include<string>
using namespace std;

namespace uiNS {
	class ButtonInterface;

	struct ButtonMap
	{
		static std::map<std::string, uiNS::ButtonInterface*> buttonMap;

		ButtonMap();

		static uiNS::ButtonInterface* getButtonByID(const string& bID);

		static const string
			DELETEBUTTON,
			CREATEBUTTON,
			ESCAPEBUTTON,
			CONTROLMODEBUTTON,
			EDITOBJECTMODEBUTTON,
			EDITGAMEMODEBUTTON,
			BACKBUTTON,
			STARTINGBUTTON,
			SAVEBUTTON,
			QUITANDSAVE,
			QUITNOSAVE;
	};

	struct NonButtonMap
	{
		static const string
			NEXTOBJECT,
			PREVOBJECT,
			SELECTOBJECT,
			ADJUSTSIZE,
			ADJUSTCOLOR,
			OBJECTSWITCH,
			OBJECTPOSITION;
	};


}