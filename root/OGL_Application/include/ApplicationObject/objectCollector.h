#ifndef OBJECTCOLLECTOR_H
#define OBJECTCOLLECTOR_H

#include<vector>
#include<string>
//#include<applicationObject.h>
//#include<applicationDerivedObject.h>

namespace myobjectNS {
	class ApplicationObject;
	class ObjectPlane;
	class ObjectSphere;
	class ObjectBox;

}

namespace collectorNS{

	typedef std::vector<myobjectNS::ApplicationObject*> AOcontainer;
	

	///////////////////////////////////////APPLICATIONOBJECT COLLECTOR///////////////////////////////////////

	/*uso il collettore come una interfaccia :
	concretamente gli oggetti risiederanno nel vettore
	puntato da Pcollector, che viene passato 
	dall'istanza del collettore creata.
	In questo modo ho una buona flessibilit� nel senso che
	se per esempio dato l'oggetto OCEnemy contenente l'oggetto
	Gun e altri oggetti semplici.Sia OCEnemy che Gun sono collettori, ma io voglio che
	i proiettili di gun vengano messi nello stesso contenitore di OCEnemy
	=> creo in OCEnemy il contenitore e lo passo a Gun in modo che
	venga inizializzato il puntatore del collettore, in questo modo
	Gun mette i proiettili nello stesso contenitore dove vengono messi
	gli altri oggetti di OCEnemy */
class ApplicationObjectCollector {
public:
	/*flag per sapere se stiamo usando
	il contenitore interno o esterno*/
	bool externalContainer = false;
	/*contenitore se non vogliamo
	usarne uno esterno*/
	AOcontainer ownContainer;
	/*puntatore a contenitore
	passato dall'esterno */
	AOcontainer* Pcontainer;

public:
	/*dice a quale collettore appartiene questo collettore*/
	std::string collectorOwnership{ "none" };
	static const std::string COLLECTOR_TYPE;
	static int collectorCounter;
	std::string collectorName;
	std::string collectorID;
	unsigned collectorNumber;
	bool isOn = false;
	
	/*dove risiedono gli oggetti veri e propri*/
	
	ApplicationObjectCollector(const std::string s, AOcontainer* coll);
	ApplicationObjectCollector(const std::string s);
	ApplicationObjectCollector() = delete;// {};

	~ApplicationObjectCollector();

	//////////////////////METODI
	virtual myobjectNS::ApplicationObject* getSubObject(int i);
	virtual unsigned getSize() { return Pcontainer->size(); }
	virtual void OCsave(std::ofstream& out);
	virtual void OCload(std::ifstream&in,std::size_t,std::size_t);
	virtual ApplicationObjectCollector* OCgetNewInstance();
	virtual void OCcreateObject();
	virtual void OCsetParameters();
	virtual void setParameters() {}
	virtual void OCreset();
	virtual void OCupdate(const float&);
	virtual void OCrender();
	virtual void OCinsertObject(myobjectNS::ApplicationObject*);
	virtual std::string getCollectorID() { return collectorID; }
	virtual std::string getCollectorType()const { return COLLECTOR_TYPE; }
	virtual ApplicationObjectCollector* getCopy();
	virtual std::string getCollectorName();
	virtual void setCollectorName();
	virtual void setCollectorName(std::string);
	virtual void  deleteAtPos(const int&);
	static ApplicationObjectCollector* getCollector(myobjectNS::ApplicationObject*);
	virtual void canSleep(bool);
	/*getBody returns the body (not the rigid body!) of this ApplicationObjectCollector*/
	virtual myobjectNS::ApplicationObject* getBody() { return Pcontainer->at(0); }
	void virtual AOtrX(int sign);
	void virtual AOtrY(int sign);
	void virtual AOtrZ(int sign);
	void virtual AOrotX(int sign);
	void virtual AOrotY(int sign);
	void virtual AOrotZ(int sign);
	
	virtual std::string getInfos() 
	{
		return "generic collector";
	}

	virtual void setCollectorOwnership(const std::string s)
	{
		collectorOwnership = s;
	}
	
};


class ActiveObject : public ApplicationObjectCollector {
public:
	ActiveObject(const std::string collName, AOcontainer * c) :ApplicationObjectCollector(collName,c){}
	std::string activeObjectID;
	int healt = 100;
	bool isAlive = true;
	virtual int getHealt() { return healt; }
	virtual void setHealt(int c) { healt += c; }
	const bool& damage(const int& i = 10)
	{
		
			healt -= i;
			if (healt > 0)
				isAlive = true;
			else
				isAlive = false;
			return isAlive;
		

	}

};




}




#endif 