#include<inputs.h>
#include<app.h>
#include<gun.h>
#include<playercharacter.h>

namespace InputsNS{

	using namespace textRendererNS;

	textRendererNS::PrintHelper Controls::ph{ "Controls" };
	bool Controls::playerIsOn = false;//settato a true quando il puntatore a gun di controls viene settato
	myobjectNS::PlayerCharacterOC* Controls::player;
	GLboolean Controls::hideCursor = true;
	std::vector<int> Controls::objNumberChoosed;
	std::map<int, int> Controls::glfw_KeyConversion
	{ {48,0},{49,1},{50,2},{51,3},{52,4},{53,5},{54,6},{55,7},{56,8},{57,9} };



	void Controls::initObjectIndex() {

		for(int l = 0; l < L ; l++)
		for (int m = 0; m < M; m++) {
			for (int n = 0; n < N; n++)
				for (int o = 0; o < O; o++)
				objectIndex[l][m][n][o] = l*1000 + m * 100 + n*10+o;
		}

		/*objectIndex = [0,1,2,3,4,5,6,7,8,9],
						[10,11,12,13,14,15,16,17,18,19],
						....
						....
		*/
	}

	void Controls::save() {}
	void Controls::load() {}


	void Controls::setAllCallbackFunction(GLFWwindow* window)
	{

		if (mode == 0) 
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			auto L_key_callbackControl = [](GLFWwindow* w, int i1, int i2, int i3, int i4)
			{
				static_cast<Controls*>(glfwGetWindowUserPointer(w))->key_callbackControl(w, i1, i2, i3, i4);
			};
			glfwSetKeyCallback(window, L_key_callbackControl);

			auto L_cursor_callback = [](GLFWwindow* w, double d1, double d2)
			{
				static_cast<Controls*>(glfwGetWindowUserPointer(w))->cursor_callback(w, d1, d2);
			};
			glfwSetCursorPosCallback(window, L_cursor_callback);

			auto L_mouse_button_callback = [](GLFWwindow* w, int i1, int i2, int i3)
			{
				static_cast<Controls*>(glfwGetWindowUserPointer(w))->mouse_button_callback(w, i1, i2, i3);
			};
			glfwSetMouseButtonCallback(window, L_mouse_button_callback);

			return;
		}

		//EDITOBJECT MODE
		if (mode == 1) 
		{
			auto L_key_callbackMove = [](GLFWwindow* w, int i1, int i2, int i3, int i4)
			{
				static_cast<Controls*>(glfwGetWindowUserPointer(w))->key_callbackMove(w, i1, i2, i3, i4);
			};
			glfwSetKeyCallback(window, L_key_callbackMove);
			return;

		}

		//EDITGAME MODE
		if (mode == 2) 
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			auto L_key_callbackEdit = [](GLFWwindow* w, int i1, int i2, int i3, int i4)
			{
				static_cast<Controls*>(glfwGetWindowUserPointer(w))->key_callbackEdit(w, i1, i2, i3, i4);
			};
			glfwSetKeyCallback(window, L_key_callbackEdit);

			auto L_cursor_callback = [](GLFWwindow* w, double d1, double d2)
			{
				static_cast<Controls*>(glfwGetWindowUserPointer(w))->cursor_callback_editMode(w, d1, d2);
			};
			glfwSetCursorPosCallback(window, L_cursor_callback);

			auto L_mouse_button_callback = [](GLFWwindow* w, int i1, int i2, int i3)
			{
				static_cast<Controls*>(glfwGetWindowUserPointer(w))->mouse_button_callback_editMode(w, i1, i2, i3);
			};
			glfwSetMouseButtonCallback(window, L_mouse_button_callback);

			return;
		}

		if (mode == 3) 
		{
			auto L_key_callbackESC = [](GLFWwindow* w, int i1, int i2, int i3, int i4)
			{
				//casto w a puntatore alla funzione key_callback di Controls
				static_cast<Controls*>(glfwGetWindowUserPointer(w))->key_callbackESC(w, i1, i2, i3, i4);
			};
			glfwSetKeyCallback(window, L_key_callbackESC);

			

			return;
		}




	}



	void Controls::initControls(GLFWwindow* window)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

		glfwSetWindowUserPointer(window, this);

		setCursorCallback(window);
		setButtonCallback(window);
		setScrollCallback(window);
		
		setAllCallbackFunction(window);

	}

	void Controls::setup(App* application) {
		/*mode is set inside in the construction of the
		button interface*/
		//mode = 0;
		initControls(Application::window);
		initObjectIndex();
		app_pt = application;

	}



int Controls::chooseObject(int key) {
	static vector<int> objNumberchoosed;
	static int iterationIndex = 0;
	int result;

	auto lambda_reset = [&]
	{
		iterationIndex = 0;
		objNumberChoosed.clear();
		ph.eraseFromMap("chooseObject_INSERTION");
		return result;
	};

	if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 0)
	{
		ph.mapNewString("chooseObject",NEWLINE + "select an object and type enter" + NEWLINE);
		return -1;//ERROR
	}

	if (key == GLFW_KEY_BACKSPACE)
	{
		ph.eraseFromMap("chooseObject_INSERTION");
		objNumberChoosed.clear();
		iterationIndex = 0;
		return -1;
	}

	
	if (key == GLFW_KEY_0 ||
		key == GLFW_KEY_1 ||
		key == GLFW_KEY_2 ||
		key == GLFW_KEY_3 ||
		key == GLFW_KEY_4 ||
		key == GLFW_KEY_5 ||
		key == GLFW_KEY_6 ||
		key == GLFW_KEY_7 ||
		key == GLFW_KEY_8 ||
		key == GLFW_KEY_9)
	{

		objNumberChoosed.push_back(glfw_KeyConversion.at(key));
		ph.concatenate("chooseObject_INSERTION",std::to_string(objNumberChoosed[iterationIndex++]));
		return -2;//WAIT FOR SECOND NUMBER
	}

	if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 1)
	{

		result = objectIndex[0][0][0][objNumberChoosed[0]];
		return lambda_reset();
	
	}
	if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 2)
	{
		result = objectIndex[0][0][objNumberChoosed[0]][objNumberChoosed[1]];
		return lambda_reset();

	}
	if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 3)
	{
		result = objectIndex[0][objNumberChoosed[0]][objNumberChoosed[1]][objNumberChoosed[2]];
		return lambda_reset();

	}
	if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 4)
	{
		result = objectIndex[objNumberChoosed[0]][objNumberChoosed[1]][objNumberChoosed[2]][objNumberChoosed[3]];
		return lambda_reset();

	}


	return -1;
}





//funzione utile all'inserimento di un numero intero di al massimo 4 cifre
int Controls::typing(int key, int action) {
	
	
		static int iterationIndex = 0;
		static string n;

		if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 0)
		{
			//se premo invio senza aver inserito chiedo di reinserire
			ph.mapNewString("typingIdentation",  "type a number then enter");
			return -1;
		}

		if (key == GLFW_KEY_BACKSPACE)
		{
			//cancello quanto inserito
			ph.eraseFromMap("typingIdentation");
			objNumberChoosed.clear();
			iterationIndex = 0;
			return -1;
		}


		if (
			(key == GLFW_KEY_0 ||
			key == GLFW_KEY_1 ||
			key == GLFW_KEY_2 ||
			key == GLFW_KEY_3 ||
			key == GLFW_KEY_4 ||
			key == GLFW_KEY_5 ||
			key == GLFW_KEY_6 ||
			key == GLFW_KEY_7 ||
			key == GLFW_KEY_8 ||
			key == GLFW_KEY_9))
		{
			if (action != GLFW_RELEASE)
			{
				objNumberChoosed.push_back(glfw_KeyConversion[key]);
				n += std::to_string(objNumberChoosed[iterationIndex++]);
				ph.mapNewString("typing", n);
				
			}
			return -2;
		}

		if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 1)
		{

			int result = objectIndex[0][0][0][objNumberChoosed[0]];
			iterationIndex = 0;
			objNumberChoosed.clear();
			n.clear();
			ph.eraseFromMap("typing");
			return result;
		}
		if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 2)
		{
			int result = objectIndex[0][0][objNumberChoosed[0]][objNumberChoosed[1]];
			iterationIndex = 0;
			n.clear();
			objNumberChoosed.clear();
			ph.eraseFromMap("typing");
			return result;
		}
		if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 3)
		{
			int result = objectIndex[0][objNumberChoosed[0]][objNumberChoosed[1]][objNumberChoosed[2]];
			iterationIndex = 0;
			n.clear();
			objNumberChoosed.clear();
			ph.eraseFromMap("typing");
			return result;
		}
		if (key == GLFW_KEY_ENTER && objNumberChoosed.size() == 4)
		{
			int result = objectIndex[objNumberChoosed[0]][objNumberChoosed[1]][objNumberChoosed[2]][objNumberChoosed[3]];
			iterationIndex = 0;
			n.clear();
			objNumberChoosed.clear();
			ph.eraseFromMap("typing");
			return result;
		}
	

	return -100;

}



bool Controls::NInsertion(int key, int action, int numberToInsert, vector<float>& vec)
{
	static vector<bool> insertionSelector(numberToInsert, false);
	static vector<bool> totalInsertion(numberToInsert, false);
	static vector<float> insertedNumbers(numberToInsert, -1);
	static unsigned insertionIndex = 0;
	insertionSelector[insertionIndex ] = true;


	if (insertionSelector[insertionIndex] &&  insertionIndex < numberToInsert)
		{
		
			insertedNumbers[insertionIndex] = typing(key, action);
			if (insertedNumbers[insertionIndex] < 0) return false;
			else
			{
				insertionSelector[insertionIndex] = false;
				totalInsertion[insertionIndex] = true;
				if(insertionIndex < numberToInsert-1)
				insertionSelector[++insertionIndex] = true;

				
				ph.mapNewString("Ninsert", "digit new Number");
				
			}
		}

	bool insertionCompleted = true;
	for (int i = 0; i < totalInsertion.size(); i++)
		insertionCompleted &= totalInsertion[i];

	if (insertionCompleted)
	{
		vec = insertedNumbers;
		insertedNumbers = vector<float>(numberToInsert, -1);
		insertionSelector = vector<bool>(numberToInsert, false);
		totalInsertion = vector<bool>(numberToInsert, false);
		insertionIndex = 0;
		ph.eraseFromMap("Ninsert");
	}
		
	return insertionCompleted;


}






void Controls::printMode() {
	/*switch (mode) {
	case 0:
		ph.mapNewString("PRINTMODE", NEWLINE +"CONTROL MODE (H for more commands)");
		break;
	case 1:
		ph.mapNewString("PRINTMODE", NEWLINE +"EDIT OBJECT MODE (H for more commands)");
		break;
	case 2:
		ph.mapNewString("PRINTMODE", NEWLINE+"EDIT GAME MODE (H for more commands)");
		break;
	}*/
		
	
	/*
		ph.mapNewString("PRINTMODE", NEWLINE + "CONTROL MODE (H for more commands)");
	
	
		ph.mapNewString("PRINTMODE", NEWLINE + "EDIT OBJECT MODE (H for more commands)");


		ph.mapNewString("PRINTMODE", NEWLINE + "EDIT GAME MODE (H for more commands)");*/

	

}

















    
}//InputsNS