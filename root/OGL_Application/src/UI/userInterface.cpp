#include<userInterface.h>
#include<inputs.h>

#include<applicationObjectManager.h>
#include<buttonsList.h>
#include<buttonMap.h>
using namespace InputsNS;

namespace uiNS {

	double UserInterface::cursor_x, UserInterface::cursor_y;
	vector<ButtonInterface*> UserInterface::buttonFlow;
	map<string, ButtonInterface> UserInterface::buttonsList;
	//vector<ButtonInterface*> UserInterface::parentFlow;
	StartButton* UserInterface::start;
	bool UserInterface::paused{ true };
	bool UserInterface::physicsOn{ true };
	bool UserInterface::AIon{ true };
	textRendererNS::PrintHelper UserInterface::ph{ "uiInterface",-0.9f,0.9f};
	InputsNS::Controls* UserInterface::control;
	InputsNS::Typer UserInterface::typer;
	buttonFunctiosList UserInterface::bfl;


	void UserInterface::back()
	{
		deleteAllButtons();
		vector<ButtonInterface*>::iterator it;
		size_t size = buttonFlow.size();

		
		int h = 1;
		ButtonInterface* b;
		do
		{
			it = buttonFlow.begin();
			for (int i = 0; i < size - h; i++)
				it++;
			buttonFlow.erase(it);
			b = ButtonMap::getButtonByID(buttonFlow.back()->getButtonID());
			if(b)
				b->action();
		} while (h++ < size && !b);

	}
	
	void UserInterface::ShowBackButton()
	{
		UserInterface::showButton(ButtonMap::BACKBUTTON, ButtonMap::BACKBUTTON);

	}


	bool UserInterface::enableBack(const string& buttonID)
	{
		UserInterface::ShowBackButton();
		if (buttonID == ButtonMap::BACKBUTTON)
		{
			UserInterface::back();
			return true;
		}
		return false;
			
	}
	


	void UserInterface::setFlags(const bool& pause, const bool& aion, const bool& physicson)
	{
		paused = pause;
		AIon = aion;
		physicsOn = physicson;
	}

	void UserInterface::turnOffAllButtons()
	{
		for (int i = 0; i < ph.mapIDbutton_button.buttons.size(); i++)
		{
			if (ph.mapIDbutton_button.buttons[i].isHighligted)
				ph.mapIDbutton_button.buttons[i].turnOff();
		}
	}

	void UserInterface::highlightButton(ButtonInterface* BI)
	{
		for (int i = 0; i < ph.mapIDbutton_button.buttons.size(); i++)
		{
			if (ph.mapIDbutton_button.buttons[i].isHighligted)
				ph.mapIDbutton_button.buttons[i].turnOff();
		}
		BI->Highligt();
	}

	ButtonInterface* UserInterface::getButtonFromList(const string& bid)
	{
		for (int i = 0; i < ph.mapIDbutton_button.buttons.size(); i++)
			if (ph.mapIDbutton_button.buttons[i].getButtonID() == bid)
				return &ph.mapIDbutton_button.buttons[i];
	}



	void UserInterface::update()
	{
		for (int i = 0; i < ph.mapIDbutton_button.buttons.size(); i++)
		{
			ph.mapIDbutton_button.buttons[i].update();
		}
	}

	



	void UserInterface::printAssetObjectsList()
	{

		UserInterface::deleteAllButtons();
		UserInterface::ph.resetPosition();

		std::map<std::string, int>* assetIndex = AssetNS::Assets::getAssetIndex();
		std::map<std::string, int>::iterator it = assetIndex->begin();


		for (it; it != assetIndex->end(); it++)
			UserInterface::showButton(it->first,it->first);

		showButton(ButtonMap::BACKBUTTON, ButtonMap::BACKBUTTON);

	}


	void UserInterface::printExistingObjects()
	{
		UserInterface::deleteAllButtons();
		//UserInterface::setButton(ButtonMap::BACKBUTTON);

		for (int i = 0; i < myobjectNS::ApplicationObjectManager::ApplicationCollectorList.size(); i++)
		{
			string s =
				myobjectNS::ApplicationObjectManager::
				ApplicationCollectorList[i]->getCollectorID();//+"_"+std::to_string(i);
			
			UserInterface::showButton(s, s);
		}
		UserInterface::showButton(ButtonMap::BACKBUTTON, ButtonMap::BACKBUTTON);
	}



	/*stringID is exclusive, therefore entering a string with a stringID already present in MAP will override the previous string*/
	/*this string is automatically assigned to*/
	void UserInterface::mapButtonOnParentBranch(const string& stringID, const string& stringValue, const float& scale)
	{
		//ButtonInterface* newbutton{
		//	ph.mapButtonOnBranch(
		//		buttonFlow.back()->getButtonID()/*getParentButton()->getButtonID()*/,
		//		stringID,
		//		stringValue, scale) };

		//buttonFlow.push_back(newbutton);
	}

	//void UserInterface::clickButton(const string& stringID)
	//{
	//	UserInterface::ph.mapIDbutton_button.parentID = stringID;
	//	/*buttonList is created when a button is shown */
	//	ButtonInterface b = buttonsList.at(stringID);
	//	mapButtonOnParentBranch(b.getButtonID(), b.button.buttonName, b.button.textScale);
	//	
	//}


	void UserInterface::clickButton(const string& stringID)
	{
		ButtonInterface* newB = new ButtonInterface(*UserInterface::getButtonFromList(stringID));
		buttonFlow.push_back(newB);

	}

	void UserInterface::showButton(const string& stringID, const string& stringValue, const float& scale)
	{
		string partentID = buttonFlow.back()->getButtonID();/* getParentButton()->getButtonID();*/
		ButtonInterface* newbutton{
		ph.mapButtonOnBranch(
			partentID,
			stringID,
			stringValue, scale) };

		buttonsList.emplace(stringID,*newbutton);
	}


	void UserInterface::setButton(const string& bID)
	{
		ButtonInterface* b = ButtonMap::getButtonByID(bID);
		if (b->getButtonID() != "")
		ph.mapButtonOnBranch(
			getParentButton()->getButtonID(),
			b->getButtonID(),
			b->getButtonID());

	}

	void UserInterface::setButton(const string& bID, const string& bName)
	{
		ButtonInterface* b = ButtonMap::getButtonByID(bID);
		if(b->getButtonID() != "")
		ph.mapButtonOnBranch(
			getParentButton()->getButtonID(),
			b->getButtonID(),
			b->getButtonID());

	}




	void UserInterface::pause()
	{

		if (paused)
		{
			paused = false;
			UserInterface::ph.eraseFromMap("PAUSED");

		}
		else
		{
			UserInterface::mapButtonOnParentBranch("PAUSED", "PAUSE");
			paused = true;
		}

	}




	ButtonInterface* UserInterface::getParentButton()
	{
		if (buttonFlow.size() >= 2)
			return buttonFlow.at(buttonFlow.size() - 2);
		else
			return buttonFlow.at(0);
	}

	UserInterface::UserInterface(InputsNS::Controls * c) 
	{
		control = c;
		control->setUserInterface(this);
		using namespace textRendererNS;
		textRendererNS::TextRenderer::printList.push_back(&ph);
		start = new StartButton();
		buttonFlow.push_back(start);
		ButtonMap();
	}

	void UserInterface::init()
	{
		//buttonFlow.push_back(start);
		//parentFlow.push_back(start);
		start->action();
	}

	void UserInterface::deleteButtonsByBranch(const string& buttonID)
	{
		buttonFlow.pop_back();
		ph.eraseByBranch(buttonID);
	}


	void UserInterface::deleteNonButtonsByBranch(const string& buttonID)
	{
		ph.eraseByBranch(buttonID);
	}

	void UserInterface::deleteAllButtons()
	{
		using namespace textRendererNS;
		for (int i = 0; i < buttonFlow.size(); i++)
			ph.eraseByBranch(buttonFlow[i]->getButtonID());
	}
	

	std::string UserInterface::cursorVStext(const double& x, const double& y)
	{
		using namespace textRendererNS;

		/*x,y go from 0 to window_width, window_height.*/
		float transformed_x = (x - Application::window_width / 2) / Application::window_width * 2;
		float transformed_y = -(y - Application::window_height / 2) / Application::window_height * 2;

		for (int i = 0; i < ph.mapIDbutton_button.buttons.size(); i++)
			if (transformed_x < ph.mapIDbutton_button.buttons[i].button.x_min ||
				transformed_x > ph.mapIDbutton_button.buttons[i].button.x_max ||
				transformed_y < ph.mapIDbutton_button.buttons[i].button.y_min_frame ||
				transformed_y > ph.mapIDbutton_button.buttons[i].button.y_max_frame)
				/*do nothing*/;
			else
				return ph.mapIDbutton_button.buttons[i].button.buttonID;

		return NonButtonMap::NOBUTTON;
	}

	std::string UserInterface::cursorVStext()
	{
		using namespace textRendererNS;

		/*x,y go from 0 to window_width, window_height.*/
		float transformed_x = (UserInterface::cursor_x - Application::window_width / 2) / Application::window_width * 2;
		float transformed_y = -(UserInterface::cursor_y - Application::window_height / 2) / Application::window_height * 2;

		for (int i = 0; i < ph.mapIDbutton_button.buttons.size(); i++)
			if (transformed_x < ph.mapIDbutton_button.buttons[i].button.x_min ||
				transformed_x > ph.mapIDbutton_button.buttons[i].button.x_max ||
				transformed_y < ph.mapIDbutton_button.buttons[i].button.y_min_frame ||
				transformed_y > ph.mapIDbutton_button.buttons[i].button.y_max_frame)
				/*do nothing*/;
			else
				return ph.mapIDbutton_button.buttons[i].button.buttonID;

		return NonButtonMap::NOBUTTON;
	}



	
}