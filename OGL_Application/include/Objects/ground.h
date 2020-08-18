#pragma once

#include<array>
#include<vector>
#include<applicationObjectManager.h>
#include<userInterface.h>
#include<core.h>
#include<applicationDerivedObject.h>
#include<utility>
#include<objectCollector.h>

namespace aiNS {
	class myfirstIA;
}



namespace myobjectNS {

	class GroundSurfaceOC;



	class Ground {
	public:
		typedef std::map<string, GroundSurfaceOC*>::iterator GroudMapIterator;
		/*groundID , ground*/
		static std::map<string, GroundSurfaceOC*> groundMap;
		//static std::vector<GroundSurfaceOC> grounds;
		static GroudMapIterator groundMapIT;

		static void addSurface(GroundSurfaceOC* c);
		static void addSurface(myobjectNS::ApplicationObject* c);
		static vector<string> getGroundList();
		static GroundSurfaceOC* getGround(const string&);

	};





	class GroundSurface :public ObjectPlane, public BaseObject {
		string objectName = "GroundSurface";
		static unsigned instanceCounter;
		unsigned instanceNumber;

		vector<array<float, 3>> vertices;

	public:
		GroundSurface(std::string sh_prog_name = "groundSurface", GLfloat l = 100) :
			ObjectPlane(sh_prog_name, 100, 0, 100),
			BaseObject(sh_prog_name)
		{

			CollisionFinitePlane::size[0] = l;
			CollisionFinitePlane::size[1] = 0;
			CollisionFinitePlane::size[2] = l;

			instanceNumber = instanceCounter++;
			body->RBobjectName = objectName;
			
			
		}



		void clean() {}
		void render(const fpcameraNS::Transformation&) override;
		void renderSurface();
		void renderCollisionPrimitive();
		void update(const float&) override;
		void setColor(float, float, float, float);
		void create()override;
		void AOsetShaders() override { setShaders(); }
		void setRigidBodyParameters();
		void setParameters()override;
		GroundSurface* getNewInstance()override {
			GroundSurface* s(new GroundSurface("groundSurface"));
			return s;
		}
		int getCollisorID() override { return AOCollisorID; }



	};



	class GroundSurfaceOC : public collectorNS::ApplicationObjectCollector
	{
		collectorNS::AOcontainer ground;
		GroundSurface gs;
	public:
		GroundSurfaceOC();
		GroundSurfaceOC* OCgetNewInstance() override;
	};


}