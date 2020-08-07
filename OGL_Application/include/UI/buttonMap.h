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

		static uiNS::ButtonInterface* getButtonByID(string bID);

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
			NOBUTTON,
			NEXTOBJECT,
			PREVOBJECT,
			SELECTOBJECT,
			SELECTCLUSTER,
			EDITCLUSTER,
			CREATECLUSTER,
			ADJUSTSIZE,
			ADJUSTCOLOR,
			OBJECTSWITCH,
			CLUSTERSWITCH,
			CLUSTERPOSITION,
			CLUSTERCOLOR,
			OBJECTPOSITION,
			SAVECLUSTER,
			CLUSTERLIST,
			TYPEPOSITION,
			TYPEROTATION,
			CHANGEPROJECT,
			PROGRAMINFOS,
			FILE,
			LOADPROJECT,
			NEWPROJECT,
			EDITSOUNDS,
			CAMERAVIEW,
			GROUNDCAMERA,
			FLYINGCAMERA,
			PANORAMICCAMERA,
			QUITBUTTON;
	};


}