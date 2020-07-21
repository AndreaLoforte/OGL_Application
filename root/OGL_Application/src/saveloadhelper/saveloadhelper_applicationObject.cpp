#include<saveloadhelper.h>
#include<log.h>

using namespace std;

namespace saveloadNS {


	const string
		CollectorSavings::COLLECTORTAG = "COLLECTOR_",
		CollectorSavings::COLLECTORENDTAG = "--------ENDCOLLECTOR--------",
		CollectorSavings::OBJECTNAMETAG = "AOobjectName",
		CollectorSavings::AOPOSITIONTAG = "AOposition",
		CollectorSavings::AOORIENTATIONTAG = "AOorientation",
		CollectorSavings::AOTRMATRIXTAG = "AOTrMatrix",
		CollectorSavings::AOINSTANCENUMBERTAG = "AOinstanceNumber",
		CollectorSavings::AOCOLORTAG = "AOcolor",
		CollectorSavings::DOSIZETAG = "DOsize",
		CollectorSavings::AOISONFLAGTAG = "AOisOnFlag",
		CollectorSavings::COLLECTORISONTAG = "OCisOnFlag";

		

	void CollectorSavings::loadCollectorMapsIntoDataStructure()
	{
		/*itero su tutti i collettori*/
		for (int i = 0; i < collectorsMap.getSize() ; i++)
		{
			CollectorDataStructure collettore;
			collettore.collectorsID = collectorsMap.getLine(i);
			collettore.pos_start = collectorsMap.getPos(i);
			collettore.pos_end = collectorsDelimitersMap.getPos(i);

			/*itero su tutti gli oggetti dei collettori*/
			for (int j = 0; j < ObjectsMap.getSize(); j++)
			{
				size_t objectPosInFile = ObjectsMap.getPos(j);
				size_t NextObjectPosInFile = ObjectsMap.getPos(j + 1);

				/*quando la posizione dell'oggetto nel file, � contenuta dentro il range di posizione iniziale-finale
				di un collettore, allora carico quell'oggetto*/
				if (objectPosInFile > collettore.pos_start && objectPosInFile < collettore.pos_end)
				{
					collettore.AOobjects.push_back(AODataStructure());
					collettore.AOobjects.back().ObjectName = ObjectsMap.getLine(j);

					//ora inserisco AOPosition : per ogni tag di AOposition devo verificare che il tag sia contenuto tra il nome dell'oggetto j e j+1
					for (int k = 0; k < AOpositionMap.getSize(); k++)
					{
						size_t aoposition_posInFile = AOpositionMap.getPos(k);

						if (aoposition_posInFile > objectPosInFile && aoposition_posInFile < NextObjectPosInFile)
						{
							string AOpositionStringFormat = AOpositionMap.getLine(k);//il tag sta in k il valore in k+1;
							
							std::array<float, 3> pos = stringToVec3(AOpositionStringFormat);

							collettore.AOobjects.back().AOposition = pos;
							
						}
					}

					// LOADING ORIENTATION
					for (int k = 0; k < AOorientationMap.getSize(); k++)
					{
						size_t aoorientatino_posInFile = AOorientationMap.getPos(k);

						if (aoorientatino_posInFile > objectPosInFile && aoorientatino_posInFile < NextObjectPosInFile)
						{
							string AOorientationStringFormat = AOorientationMap.getLine(k);//il tag sta in k il valore in k+1;

							std::vector<float> orientation;
							stringToFloatVec(AOorientationStringFormat, orientation);

							collettore.AOobjects.back().AOorientation.w = orientation[0];
							collettore.AOobjects.back().AOorientation.x = orientation[1];
							collettore.AOobjects.back().AOorientation.y = orientation[2];
							collettore.AOobjects.back().AOorientation.z = orientation[3];

						}
					}

					//LOADING COLOR OF THE OBJECT
					for (int k = 0; k < AOcolorMap.getSize(); k++)
					{
						size_t aocolor_posInFile = AOcolorMap.getPos(k);

						if (aocolor_posInFile > objectPosInFile && aocolor_posInFile < NextObjectPosInFile)
						{
							string AOcolorStringFormat = AOcolorMap.getLine(k);//il tag sta in k il valore in k+1;

							std::vector<float> color;
							stringToFloatVec(AOcolorStringFormat, color);
							collettore.AOobjects.back().AOcolor[0] = color[0];
							collettore.AOobjects.back().AOcolor[1] = color[1];
							collettore.AOobjects.back().AOcolor[2] = color[2];
							collettore.AOobjects.back().AOcolor[3] = color[3];

						}
					}


					//LOADING SIZE OF THE OBJECT
					for (int k = 0; k < DOsizeMap.getSize(); k++)
					{
						size_t dosize_posInFile = DOsizeMap.getPos(k);

						if (dosize_posInFile > objectPosInFile && dosize_posInFile < NextObjectPosInFile)
						{
							string DOsizeStringFormat = DOsizeMap.getLine(k);//il tag sta in k il valore in k+1;

							std::vector<float> size;
							stringToFloatVec(DOsizeStringFormat, size);
							for (int i = 0; i < size.size(); i++)
								collettore.AOobjects.back().AOsize.push_back(size[i]);
							
						}
					}

					//LOADING APPLICATION OBJECT IS ON FLAG
					for (int k = 0; k < AOisonFlagMap.getSize(); k++)
					{
						size_t isonflag_posInFile = AOisonFlagMap.getPos(k);

						if (isonflag_posInFile > objectPosInFile && isonflag_posInFile < NextObjectPosInFile)
						{
							string isOnStringFormat = AOisonFlagMap.getLine(k);//il tag sta in k il valore in k+1;
							if (isOnStringFormat == "1")
								collettore.AOobjects.back().isOn = true;
							else
								collettore.AOobjects.back().isOn = false;
							

						}
					}

					//LOADING OBJECT COLLECTOR IS ON FLAG
					for (int k = 0; k < OCisOnMap.getSize(); k++)
					{
						size_t isonflag_posInFile = OCisOnMap.getPos(k);

						if (isonflag_posInFile > collettore.pos_start && isonflag_posInFile < collettore.pos_end)
						{
							string isOnStringFormat = OCisOnMap.getLine(k);//il tag sta in k il valore in k+1;
							if (isOnStringFormat == "1")
								collettore.isOn = true;
							else
								collettore.isOn = false;


						}
					}



					//ora inserisco AOTrMatrix : per ogni tag di AOTrMatrix devo verificare che il tag sia contenuto tra il nome dell'oggetto j e j+1
					for (int k = 0; k < AOTrMatrixMap.getSize(); k++)
					{
						size_t aoTrMatrix_posInFile = AOTrMatrixMap.getPos(k);
						size_t aoTrMatrix_posInFile_LastRow = AOTrMatrixMap.getPos(k + 3);//5 = 1 riga tag + 4 righe numeriche
						if (aoTrMatrix_posInFile > objectPosInFile && aoTrMatrix_posInFile_LastRow < NextObjectPosInFile)
						{
							//AOTrMatrix � una matrice di 4 righe, quindi devo prendere 4 stringhe
							string row1 = AOTrMatrixMap.getLine(k);
							string row2 = AOTrMatrixMap.getLine(k+1);
							string row3 = AOTrMatrixMap.getLine(k+2);
							string row4 = AOTrMatrixMap.getLine(k + 3);

							collettore.AOobjects.back().AOTrMatrix = stringToMat4(row1 + " " + row2 + " " + row3 + " " + row4);

						}
					}

					
				}
			}

			collectors.push_back(collettore);
		}
	}





	void  CollectorSavings::setAllCollectorsMap()
	{
		for (int i = 0; i + 1 < totalLines; i++)
		{
				if (fileMap.getLine(i).find(COLLECTORTAG) != string::npos)
				{
					collectorsMap.insert(fileMap.getLine(i+1), fileMap.getPos(i+1));//su questa mappa metto i tag dell'inizio dei collettori
				}
				if (fileMap.getLine(i).find(COLLECTORENDTAG) != string::npos)
				{
					collectorsDelimitersMap.insert(fileMap.getLine(i), fileMap.getPos(i));//su questa mappa metto i delimitatori dei collettori
				}

				if (fileMap.getLine(i).find(OBJECTNAMETAG) != string::npos)
				{
					//inserisco in mappa la linea successiva al tag
					ObjectsMap.insert(fileMap.getLine(i + 1), fileMap.getPos(i + 1));
				}
				if (fileMap.getLine(i).find(AOCOLORTAG) != string::npos)
				{
					//inserisco in mappa la linea successiva al tag
					AOcolorMap.insert(fileMap.getLine(i + 1), fileMap.getPos(i + 1));
				}

				string line = fileMap.getLine(i);
				if (line.find(AOTRMATRIXTAG) != string::npos)
				{
					//la matrice � distribuita su 4 righe
					AOTrMatrixMap.insert(fileMap.getLine(i + 1), fileMap.getPos(i + 1));
					AOTrMatrixMap.insert(fileMap.getLine(i + 2), fileMap.getPos(i + 2));
					AOTrMatrixMap.insert(fileMap.getLine(i + 3), fileMap.getPos(i + 3));
					AOTrMatrixMap.insert(fileMap.getLine(i + 4), fileMap.getPos(i + 4));
				}
				if (fileMap.getLine(i).find(DOSIZETAG) != string::npos)
				{
					//inserisco in mappa la linea successiva al tag
					DOsizeMap.insert(fileMap.getLine(i + 1), fileMap.getPos(i + 1));
				}
				if (fileMap.getLine(i).find(AOPOSITIONTAG) != string::npos)
				{
					//inserisco in mappa la linea successiva al tag
					AOpositionMap.insert(fileMap.getLine(i + 1), fileMap.getPos(i + 1));
				}
				if (fileMap.getLine(i).find(AOORIENTATIONTAG) != string::npos)
				{
					//inserisco in mappa la linea successiva al tag
					AOorientationMap.insert(fileMap.getLine(i + 1), fileMap.getPos(i + 1));
				}
				if (fileMap.getLine(i).find(AOISONFLAGTAG) != string::npos)
				{
					//inserisco in mappa la linea successiva al tag
					AOisonFlagMap.insert(fileMap.getLine(i + 1), fileMap.getPos(i + 1));
				}
				if (fileMap.getLine(i).find(COLLECTORISONTAG) != string::npos)
				{
					//inserisco in mappa la linea successiva al tag
					OCisOnMap.insert(fileMap.getLine(i + 1), fileMap.getPos(i + 1));
				}

		}
	}



	CollectorSavings::CollectorSavings(ifstream& in)
	{

		if (in.is_open())
		{
			while (!in.eof() && in.tellg() != -1)
			{
				getline(in, s);
				size_t cursor_pos = in.tellg();
				size_t lineLength = s.length();
				size_t line_beg = cursor_pos - lineLength - 2; //2 extra char "\n"
				fileMap.insert(s, line_beg);

			}

			totalLines = fileMap.getSize();

			countChar();
			if (totalChars == 0)
			{
				FileIsEmpty = true;
				return;
			}


			countChar();
			setAllCollectorsMap();
			loadCollectorMapsIntoDataStructure();


		}


	}









}