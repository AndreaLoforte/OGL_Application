#include<activeobjectloader.h>
#include<log.h>

using namespace std;

namespace saveloadNS {


	const string
		ActiveObjectLoader::COLLECTORTAG = "ACTIVEOBJECT_",
		ActiveObjectLoader::COLLECTORENDTAG = "--------ACTIVEOBJECTEND--------",
		ActiveObjectLoader::OBJECTNAMETAG = "AOobjectName",
		ActiveObjectLoader::AOPOSITIONTAG = "AOposition",
		ActiveObjectLoader::AOORIENTATIONTAG = "AOorientation",
		ActiveObjectLoader::AOTRMATRIXTAG = "AOTrMatrix",
		ActiveObjectLoader::AOINSTANCENUMBERTAG = "AOinstanceNumber",
		ActiveObjectLoader::AOCOLORTAG = "AOcolor",
		ActiveObjectLoader::DOSIZETAG = "DOsize",
		ActiveObjectLoader::AOISONFLAGTAG = "AOisOnFlag",
		ActiveObjectLoader::COLLECTORISONTAG = "OCisOnFlag",
		ActiveObjectLoader::ACTIVITYGROUNDID = "ACTIVE_OBJECT_ACTIVITYGROUND_ID";



	

	void ActiveObjectLoader::loadCollectorMapsIntoDataStructure()
	{

		/*itero su tutti i collettori*/
		for (int i = 0; i < TAGLIST.at(COLLECTORTAG).getSize(); i++)
		{
			ActiveObjectDataStructure collettore;
			collettore.collectorsID =TAGLIST.at(COLLECTORTAG).getLine(i);
			collettore.pos_start = TAGLIST.at(COLLECTORTAG).getPos(i);
			collettore.pos_end = TAGLIST.at(COLLECTORENDTAG).getPos(i);



			loadAttributeString(collettore.activityGroundID, collettore.pos_start, collettore.pos_end, TAGLIST.at("ACTIVE_OBJECT_ACTIVITYGROUND_ID"));
	
	



			/*itero su tutti gli oggetti dei collettori*/
			for (int j = 0; j < TAGLIST.at("AOobjectName").getSize(); j++)
			{
				size_t objectPosInFile = TAGLIST.at("AOobjectName").getPos(j);
				size_t NextObjectPosInFile = TAGLIST.at("AOobjectName").getPos(j + 1);

				/*quando la posizione dell'oggetto nel file, � contenuta dentro il range di posizione iniziale-finale
				di un collettore, allora carico quell'oggetto*/
				

				if (objectPosInFile > collettore.pos_start && objectPosInFile < collettore.pos_end)
				{
					collettore.AOobjects.push_back(AODataStructure());
		
					loadAttributeString(collettore.AOobjects.back().ObjectName, collettore.pos_start, collettore.pos_end, TAGLIST.at("AOobjectName"));
					loadAttributeArray(collettore.AOobjects.back().AOposition, collettore.pos_start, collettore.pos_end, TAGLIST.at("AOposition"));
					loadAttributeQuaternion(collettore.AOobjects.back().AOorientation, collettore.pos_start, collettore.pos_end, TAGLIST.at("AOorientation"));
					loadAttributevmathVec4(collettore.AOobjects.back().AOcolor, collettore.pos_start, collettore.pos_end, TAGLIST.at("AOcolor"));
					loadAttributevectorFloat(collettore.AOobjects.back().AOsize, collettore.pos_start, collettore.pos_end, TAGLIST.at("DOsize"));
					loadAttributeBool(collettore.AOobjects.back().isOn, collettore.pos_start, collettore.pos_end, TAGLIST.at("AOisOnFlag"));
					loadAttributeBool(collettore.isOn, collettore.pos_start, collettore.pos_end, TAGLIST.at("OCisOnFlag"));
					loadAttributevmathMatrix4(collettore.AOobjects.back().AOTrMatrix, collettore.pos_start, collettore.pos_end, TAGLIST.at("AOTrMatrix"));
				}
			}

			collectors.push_back(collettore);
		}
	}





	ActiveObjectLoader::ActiveObjectLoader(ifstream& in)
	{

		if (in.is_open())
		{
			while (!in.eof() && (int/*casting is needed to avoid ambiguous operator !=*/)in.tellg() != -1)
			{
				getline(in, s);
				size_t cursor_pos = in.tellg();
				size_t lineLength = s.length();
				size_t line_beg = cursor_pos - lineLength - 2; //2 extra char "\n"
				fileMap.insert(s, line_beg);

			}

			totalLines = fileMap.getSize();


			if (fileMap.getCharNumber() == 0)
			{
				FileIsEmpty = true;
				return;
			}


			setTAGLIST(tagList);
			loadAllCollectorsMap(TAGLIST);
			loadCollectorMapsIntoDataStructure();


		}
	}
}

