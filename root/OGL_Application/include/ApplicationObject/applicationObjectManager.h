#ifndef APPOBJECTMANAGER_H
#define APPOBJECTMANAGER_H

#include<gl3w.h>
#include<string.h>
//#include<baseobject.h>
#include<applicationObject.h>
#include<vector>
#include<array>
#include<log.h>
#include<assets.h>
#include<objectCollector.h>
#include<cameraManager.h>
namespace myphysicsNS {
	struct CollisionObjectContainer;
}


namespace myobjectNS{


	class ApplicationObjectManager {
		//////////ATTRIBUTI PRIVATI
		static unsigned editableObjIndex;
		AssetNS::Assets assets;
		static textRendererNS::PrintHelper ph;

	public:
		////////////////////////ATTRIBUTI PUBBLICI
		static std::vector< collectorNS::ApplicationObjectCollector*> ApplicationCollectorList;
		/*mappa : [ AOobjectName, (coll. position, subobject pos) ]*/
		static std::map< std::string, std::pair<int, int>> objectListIndexMap;
		/*mappa : [ RBobjectID, (coll. position, subobject pos) ]*/
		static std::map< std::string, std::pair<int, int>> objectListRBIDMap;
		/*mappa :[ collectorID, coll. position ]*/
		static std::map< std::string, int> collectorIDMap;


		///////////COSTRUTTORI 
		ApplicationObjectManager() {
			initObjectMaps();
			
		}
		~ApplicationObjectManager() {
			/*for (int i = 0; i < AssetCollectorList.size(); i++)
				delete AssetCollectorList[i];*/
		}

		
		
		
		///////METODI PUBBLICI
		static collectorNS::ApplicationObjectCollector * getEditableCollector();
		static std::string getEditableObjectName();	
		static void nextObject();
		static void prevObject();
		static bool setEditableObject(const string&);
		static void initObjectAssetList();
		static void createNewObject(int N);//crea l'asset corrispondente alla posizione N nella lista degli asset e lo inserisce in applicationObjectList
		static void createNewObject(const string&);
		static void deleteObject(const int );
		static void deleteObject(const string&);
		static void save(std::string);
		static void loadData(string filename);
		static void initObjectMaps();
		static std::pair<int,int> getObjectListIndexByAOobjectName(std::string s);
		static std::pair<int, int> getObjectListIndexByRBobjectID(std::string s);
		static collectorNS::ApplicationObjectCollector* getObject(std::string s);
		static collectorNS::ApplicationObjectCollector* getCollector(const std::string& s);
		static std::string printEditableObjectInfos();
		static void kill(const std::string&);
		static void kill(const collectorNS::ActiveObject*);

		
	};








	





}


#endif
