#include<buttons.h>
#include<applicationObjectManager.h>
#include<activeCharacterManager.h>
#include<inputs.h>
#include<app.h>
#include<buttonEditObject.h>
#include<userInterface.h>
#include<ground.h>

namespace uiNS {

	const string CLUSTERNAME = "ACTIVECHARACTERCLUSTER";

	void buttonCallback_activeCharacter();
	void buttonCallback_editActiveCharacterCluster(GLFWwindow* w, int button, int action, int mods);


	void buttonCallback_setClusterGround(GLFWwindow* w, int button, int action, int mods)
	{
		if (UserInterface::clicked(ButtonMap::BACKBUTTON) || UserInterface::clicked(NonButtonMap::NOBUTTON))
		{
			buttonCallback_editActiveCharacterCluster(Application::window, 0, 0, 0);
			
			return;
		}

		string groundID = UserInterface::cursorVStext();
		myobjectNS::SurfaceBoundaries* gs = myobjectNS::Ground::getGround(groundID);

		clusterNS::Cluster c = clusterNS::ClusterManager::clusterMap.at(CLUSTERNAME);

		for (int i = 0; i < c.group.size(); i++)
		{
			static_cast<collectorNS::ActiveObject*>(c.group[i])->setActivityGround(gs);
			UserInterface::phc.showButton(ButtonMap::EDITOBJECTMODEBUTTON, "ground setted for "+c.group[i]->collectorID);
		}
			
		
		clusterNS::ClusterManager::clusterMap.at(CLUSTERNAME).group.clear();



	}

	void buttonCallback_editActiveCharacterCluster(GLFWwindow* w, int button, int action, int mods)
	{
		if (UserInterface::clicked(ButtonMap::BACKBUTTON) || UserInterface::clicked(NonButtonMap::NOBUTTON))
		{
			buttonCallback_activeCharacter();
		}

		if(UserInterface::clicked("SET LOCATION"))
		{
			UserInterface::phc.showButton(ButtonMap::EDITOBJECTMODEBUTTON, "Choose one of the following grounds for this cluster");
			UserInterface::phc.showDropDownMenu(ButtonMap::EDITOBJECTMODEBUTTON, myobjectNS::Ground::getGroundList(),false);
			UserInterface::bfl.setMouseButtonCallback(buttonCallback_setClusterGround);
		}

	}


	void StartButton::buttonCallback_selectActiveCharacter(GLFWwindow* w, int button, int action, int mods)
	{
		static clusterNS::Cluster activeCharCL;


		string acid = UserInterface::cursorVStext();

		collectorNS::ApplicationObjectCollector* coll = myobjectNS::ApplicationObjectManager::getObjectByCollectorID(acid);
		if (coll)
		{
			activeCharCL.group.push_back(coll);
			UserInterface::phc.showButton(ButtonMap::EDITOBJECTMODEBUTTON,
				coll->collectorID, coll->collectorID + " was added to the cluster");
		}

		if (UserInterface::clicked(NonButtonMap::EDITCLUSTER))
		{
			clusterNS::ClusterManager::it = clusterNS::ClusterManager::clusterMap.find(CLUSTERNAME);
			if (clusterNS::ClusterManager::it != clusterNS::ClusterManager::clusterMap.end())
				clusterNS::ClusterManager::it->second = activeCharCL;
			else
				clusterNS::ClusterManager::clusterMap.emplace(CLUSTERNAME, activeCharCL);
			activeCharCL.group.clear();
			UserInterface::phc.showDropDownMenu(ButtonMap::EDITOBJECTMODEBUTTON, { "SET LOCATION","SET STRENGHT","SET AMMOS" });
			UserInterface::phc.showButton(ButtonMap::EDITOBJECTMODEBUTTON, ButtonMap::BACKBUTTON);
			UserInterface::bfl.setMouseButtonCallback(buttonCallback_editActiveCharacterCluster);

		}

		if (UserInterface::clicked(ButtonMap::BACKBUTTON) || UserInterface::clicked(NonButtonMap::NOBUTTON))
		{
			clusterNS::ClusterManager::clusterMap.at(CLUSTERNAME).group.clear();
			activeCharCL.group.clear();
			setControls();
			mainMenu(Application::window, 0, 0, 0);
			return;
		}

	}


	

}