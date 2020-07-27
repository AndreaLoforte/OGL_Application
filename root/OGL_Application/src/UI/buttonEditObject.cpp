#include<buttons.h>
#include<userInterface.h>
#include<applicationDerivedObject.h>
#include<applicationObjectManager.h>
#include<objectControlsInterface.h>
#include<inputs.h>
#include<app.h>
namespace uiNS
{



	void EditObjectModeButton::action()
	{
		UserInterface::deleteAllButtons();

		UserInterface::ph.mapButtonOnBranch(ButtonMap::EDITOBJECTMODEBUTTON, ButtonMap::BACKBUTTON, ButtonMap::BACKBUTTON);
		UserInterface::ph.mapButtonOnBranch(ButtonMap::EDITOBJECTMODEBUTTON, NonButtonMap::SELECTOBJECT, NonButtonMap::SELECTOBJECT);
		
		auto L_mouse_button_callback = [](GLFWwindow* w, int button, int action, int mods)
		{
			if (action == GLFW_RELEASE) return;
			static_cast<EditObjectModeButton*>(glfwGetWindowUserPointer(w))->cursorButtonCallBack(w, button, action, mods);
		};
		glfwSetMouseButtonCallback(Application::window, L_mouse_button_callback);

	}


	void EditObjectModeButton::cursorPositionCallBack(GLFWwindow* w, double x, double y)
	{
		
		
	}
	void EditObjectModeButton::cursorButtonCallBack(GLFWwindow* w, int button, int action, int mode)
	{
			/*3 case : 
			1) do not click anything
			2) do click a NonButton
			3) do click a Button*/
			std::string buttonID{ UserInterface::cursorVStext(UserInterface::cursor_x, UserInterface::cursor_y) };
			if (buttonID == "CLICK_NO_BUTTON") return;
			if (buttonID == NonButtonMap::SELECTOBJECT)
			{
				UserInterface::printExistingObjects();
			}
			else
			{
				ButtonInterface *b = ButtonMap::getButtonByID({ buttonID });
				/*if it is not a button means that user chose an object*/
				if (!b)
				{
					/*check if the buttonID is an objectID we can set as editable object*/
					if (myobjectNS::ApplicationObjectManager::setEditableObject(buttonID))
					{
						UserInterface::deleteAllButtons();
						UserInterface::ph.mapButtonOnBranch(ButtonMap::EDITOBJECTMODEBUTTON, buttonID, "Editing Object " + buttonID, 0.5);
						UserInterface::mapButtonOnParentBranch( NonButtonMap::ADJUSTSIZE, NonButtonMap::ADJUSTSIZE);
						UserInterface::mapButtonOnParentBranch( NonButtonMap::ADJUSTCOLOR, NonButtonMap::ADJUSTCOLOR);
						UserInterface::mapButtonOnParentBranch( NonButtonMap::OBJECTPOSITION, NonButtonMap::OBJECTPOSITION);
						UserInterface::mapButtonOnParentBranch( NonButtonMap::OBJECTSWITCH, NonButtonMap::OBJECTSWITCH);
						UserInterface::setButton(ButtonMap::BACKBUTTON);

					
						auto L_mouse_button_callback = [](GLFWwindow* w, int button, int action, int mods)
						{
							if (action == GLFW_RELEASE) return;
							static_cast<EditObjectModeButton*>(glfwGetWindowUserPointer(w))->editObject(w, button, action, mods);	
						};

						glfwSetMouseButtonCallback(Application::window, L_mouse_button_callback);
						
					}
					
					return;
				}
				/*if it is a button, action it*/	
				UserInterface::buttonFlow.push_back(b);
				b->action();
			}		
	}



	void EditObjectModeButton::editObject(GLFWwindow* w, int button, int action, int mods)
	{
		
		std::string buttonID{ UserInterface::cursorVStext(UserInterface::cursor_x, UserInterface::cursor_y) };

		if (buttonID == NonButtonMap::ADJUSTSIZE)
		{
			auto l_key_callbackeditsize = [](GLFWwindow* w, int key, int scancode, int action, int mods)
			{
				if (action == GLFW_RELEASE) return;
				//casto w a puntatore alla funzione key_callback di controls
				static_cast<EditObjectModeButton*>(glfwGetWindowUserPointer(w))->key_callbackEditSize(w, key, scancode, action, mods);
			};

			glfwSetKeyCallback(Application::window, l_key_callbackeditsize);
			static_cast<EditObjectModeButton*>(glfwGetWindowUserPointer(Application::window))
				->key_callbackEditSize(Application::window, 0, 0, 0, 0);
			return;

		}
			//objectControlsNS::ObjectControlsI::setKeyCallbackEditSize(w);
		if (buttonID == NonButtonMap::ADJUSTCOLOR)
		{
			auto L_key_callbackEditColor = [](GLFWwindow* w, int key, int scancode, int action, int mods)
			{
				if (action == GLFW_RELEASE) return;
				//casto w a puntatore alla funzione key_callback di Controls
				static_cast<EditObjectModeButton*>(glfwGetWindowUserPointer(w))->key_callbackEditColor(w, key, scancode, action, mods);
			};
			glfwSetKeyCallback(Application::window, L_key_callbackEditColor);
			static_cast<EditObjectModeButton*>(glfwGetWindowUserPointer(Application::window))
				->key_callbackEditColor(Application::window, 0, 0, 0, 0);
			return;
		}

		if (buttonID == NonButtonMap::OBJECTSWITCH)
		{
			ObjectSwitch(myobjectNS::ApplicationObjectManager::getEditableObject());
			return;
		}

		if (buttonID == NonButtonMap::OBJECTPOSITION)
		{

			UserInterface::mapButtonOnParentBranch(uiNS::ButtonMap::EDITOBJECTMODEBUTTON,
				"Move the "+
				myobjectNS::ApplicationObjectManager::getEditableObjectName()+
				" object using A,S,D,W,1,2 and directional Arrows");

			auto L_key_callbackMoveObject = [](GLFWwindow* w, int key, int scancode, int action, int mods)
			{
				if (action == GLFW_RELEASE) return;
				//casto w a puntatore alla funzione key_callback di Controls
				static_cast<EditObjectModeButton*>(glfwGetWindowUserPointer(w))->key_callbackMove(w, key, scancode, action, mods);
			};
			glfwSetKeyCallback(Application::window, L_key_callbackMoveObject);
			return;
		}


		if (buttonID == ButtonMap::BACKBUTTON)
			UserInterface::buttonFlow.back()->action();

	}





	void EditObjectModeButton::key_callbackMove(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_RELEASE) return;
		if (mods == GLFW_MOD_CONTROL)
		{

			if (key == GLFW_KEY_LEFT)
			{
				myobjectNS::ApplicationObjectManager::getEditableObject()->AOrotZ(-1);

				return;
			}
			if (key == GLFW_KEY_RIGHT)
			{
				myobjectNS::ApplicationObjectManager::getEditableObject()->AOrotZ(1);

				return;
			}

		}

		switch (key) {
		case GLFW_KEY_I:
			textRendererNS::TextRenderer::printEditableObjectInfos();
			break;
		case GLFW_KEY_C:
			fpcameraNS::CameraManager::switchCamera();
			break;
		case GLFW_KEY_W:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOtrZ(-1);
			break;
		case GLFW_KEY_S:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOtrZ(1);
			break;
		case GLFW_KEY_D:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOtrX(1);
			break;
		case GLFW_KEY_A:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOtrX(-1);
			break;
		case GLFW_KEY_1:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOtrY(-1);
			break;
		case GLFW_KEY_2:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOtrY(1);
			break;

		case GLFW_KEY_UP:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOrotX(1);
			break;
		case GLFW_KEY_DOWN:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOrotX(-1);
			break;
		case GLFW_KEY_RIGHT:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOrotY(-1);
			break;
		case GLFW_KEY_LEFT:
			myobjectNS::ApplicationObjectManager::getEditableObject()->AOrotY(1);
			break;
		case GLFW_KEY_R:
			fpcameraNS::CameraManager::resetAll();
			UserInterface::control->app_pt->resetAll();
			break;
	
		}
	}






	void EditObjectModeButton::ObjectSwitch(collectorNS::ApplicationObjectCollector* obj)
	{

		if (obj->isOn)
		{
			obj->canSleep(true);
			UserInterface::ph.mapButtonOnBranch(
				uiNS::ButtonMap::EDITOBJECTMODEBUTTON,
				uiNS::ButtonMap::EDITOBJECTMODEBUTTON, "OBJECT OFF");

		}
		else
		{
			obj->canSleep(false);
			UserInterface::ph.mapButtonOnBranch(
				uiNS::ButtonMap::EDITOBJECTMODEBUTTON,
				uiNS::ButtonMap::EDITOBJECTMODEBUTTON, "OBJECT ON");
		}


	}



	void EditObjectModeButton::key_callbackEditSize(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			UserInterface::ph.eraseFromMap(uiNS::ButtonMap::EDITOBJECTMODEBUTTON);
		}

		if (myobjectNS::ApplicationObjectManager::getEditableObject()->getBody()->AOobjectClass ==
			myobjectNS::classSphere)
		{
			uiNS::UserInterface::mapButtonOnParentBranch(uiNS::ButtonMap::EDITOBJECTMODEBUTTON, "Type the new radius : ");

			
			float radius =  UserInterface::control->typing(key, action);
			if (radius <= 0) return;
			else
				static_cast<myobjectNS::ObjectSphere*>
				(myobjectNS::ApplicationObjectManager::getEditableObject()->getBody())->changeRadius(radius);
		}



		if (myobjectNS::ApplicationObjectManager::getEditableObject()->getBody()->AOobjectClass ==
			myobjectNS::classBox)
		{
			UserInterface::mapButtonOnParentBranch(uiNS::ButtonMap::EDITOBJECTMODEBUTTON,"Type 3 dimensions : " );


			static GLfloat x, y, z;
			static bool typefirst = true, typesecond = false, typethird = false;
			if (typefirst)
			{
				UserInterface::mapButtonOnParentBranch(uiNS::ButtonMap::EDITOBJECTMODEBUTTON, "enter first dimension");
				x = UserInterface::control->typing(key, action);
				if (x <= 0) return;
				else
				{
					typefirst = false;
					typesecond = true;
				}

			}
			if (typesecond)
			{
				UserInterface::mapButtonOnParentBranch(uiNS::ButtonMap::EDITOBJECTMODEBUTTON, "enter second dimension");
				y = UserInterface::control->typing(key, action);
				if (y <= 0) return;
				else
				{
					typesecond = false;
					typethird = true;
				}

			}
			if (typethird)
			{
				UserInterface::mapButtonOnParentBranch(uiNS::ButtonMap::EDITOBJECTMODEBUTTON, "enter third dimension");
				z = UserInterface::control->typing(key, action);
				if (z <= 0) return;
				else
				{
					typefirst = true;
					typesecond = false;
					typethird = false;
				}
				static_cast<myobjectNS::ObjectBox*>
					(myobjectNS::ApplicationObjectManager::getEditableObject()->getBody())->changeDimensions(x, y, z);
			}
		}



		if (myobjectNS::ApplicationObjectManager::getEditableObject()->getBody()->AOobjectClass ==
			myobjectNS::classPlane)
		{
			UserInterface::mapButtonOnParentBranch(
				uiNS::ButtonMap::EDITOBJECTMODEBUTTON,  "Type 2 dimensions w, h : ");


			static float x, y;
			static bool typefirst = true, typesecond = false;
			if (typefirst)
			{
				UserInterface::mapButtonOnParentBranch(uiNS::ButtonMap::EDITOBJECTMODEBUTTON, "enter first dimension");
				x = UserInterface::control->typing(key, action);
				if (x <= 0) return;
				else
				{
					typefirst = false;
					typesecond = true;
				}

			}
			if (typesecond)
			{
				UserInterface::mapButtonOnParentBranch(uiNS::ButtonMap::EDITOBJECTMODEBUTTON, "enter second dimension");
				y = UserInterface::control->typing(key, action);
				if (y <= 0) return;
				else
				{
					typefirst = true;
					typesecond = false;
				}


			}


			static_cast<myobjectNS::ObjectPlane*>
				(myobjectNS::ApplicationObjectManager::getEditableObject()->getBody())->changeDimensions(x, y);
		}




	}






	void EditObjectModeButton::key_callbackEditColor(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			//controls.setScrollCallback(window);
			//myobjectNS::TextRenderer::clearEditMenuString();
			UserInterface::ph.eraseFromMap(uiNS::ButtonMap::EDITOBJECTMODEBUTTON);
			//controls.setAllCallbackFunction(window);

			return;
		}

		UserInterface::mapButtonOnParentBranch
		(uiNS::ButtonMap::EDITOBJECTMODEBUTTON, "Enter the color components r,g,b,a in range [0,100]");

		static vector<float> color;

		if (UserInterface::control->NInsertion(key, action, 4, color))
		{
			mymathlibNS::stdVectorProdFloat(color, 0.01);
			static_cast<myobjectNS::ApplicationObject*>(myobjectNS::ApplicationObjectManager::getEditableObject()->getBody())->changeColor(color);
		}
	}
	




}



