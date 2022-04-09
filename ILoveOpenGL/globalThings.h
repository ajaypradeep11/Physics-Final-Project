#ifndef _globalThings_HG_
#define _globalThings_HG_

#include "GLCommon.h"

#include "cFlyCamera.h"
#include "cVAOManager.h"
#include "cShaderManager.h"
#include "cLightManager.h"
#include "cGameManager.h"
#include "TextureMapping/cBasicTextureManager.h"
#include "cMesh.h"
#include <vector>
// 


// These are all things that are "project global", 
//	so need to be visible to many files and parts

// Creates global things, etc. (to control the order)
bool g_StartUp(GLFWwindow* pWindow);

// Deletes global things, etc. 
bool g_ShutDown(GLFWwindow* pWindow);


// ************************************************
//     ___                         
//    / __|__ _ _ __  ___ _ _ __ _ 
//   | (__/ _` | '  \/ -_) '_/ _` |
//    \___\__,_|_|_|_\___|_| \__,_|
//                                 
extern cFlyCamera* g_pFlyCamera; // = NULL;

// This the keyboard based eye location
// Like if you AREN'T using the fly camera
//extern glm::vec3 g_cameraEye;	//	= glm::vec3(0.0f, 0.0f, +4.0f);
//extern glm::vec3 g_cameraAt;	//	= glm::vec3(0.0f, 0.0f, 0.0f);
// ************************************************

extern char Mesh_charArray[65][50];
           
extern cVAOManager* g_pVAOManager;
extern cShaderManager* g_pShaderManager;

extern cLightManager* g_pTheLights;
// This got moved into the light manager class
//extern cLightHelper* g_pTheLightHelper;

extern bool bIsWireframe;

extern bool fireWeapon;


extern cMesh* g_pDebugSphere;	// = NULL;
extern bool g_bShowDebugShere;	// = true;


extern unsigned int g_selectedObject;	// = 0;
extern unsigned int g_selectedLight;	// = 0;

extern cBasicTextureManager* g_pTextureManager;
// This will be printed in the title bar
extern std::string g_TitleText;	// = "";

extern std::vector<glm::vec3> leftWall;
extern std::vector<glm::vec3> rightWall;
extern std::vector<glm::vec3> backWall;
extern std::vector<glm::vec3> frontWall;
extern std::string g_tictactow[3][3];
extern cMesh* g_tictactow_meshX[3][3];
extern cMesh* g_tictactow_meshO[3][3];
extern bool insideLoop;
extern bool Player1;
extern bool Player2;
extern bool swingBool;
// List of objects to draw
extern std::vector< cMesh* > g_vec_pMeshes;
extern bool canfireWeapon;
extern std::vector< cMesh* > g_vec_pEnemy;
extern std::vector< cMesh* > g_vec_pEnemyTimeSkip;
extern cMesh* g_Missile;
extern cMesh* g_UFO;
extern std::vector<iGameObject*> g_vec_pEnemysShip;
extern cGameManager* g_GameManager;



// Note: This returns a COPY of the object as a stack variable (not a pointer)
bool g_FindObjectByUniqueID( unsigned int uniqueID_toFind, cMesh &theObjectIFound );
// This returns a pointer to the object
bool g_FindObjectByUniqueID( unsigned int uniqueID_toFind, cMesh* &theObjectIFound );

// If not found, return NULL (0)
cMesh* g_FindObjectByUniqueID(unsigned int uniqueID_toFind);
cMesh* g_findObjectByFriendlyName(std::string NametoFind);



// ************************************************
//    _  __         _                      _                 _ _  __ _            
//   | |/ /___ _  _| |__  ___  __ _ _ _ __| |  _ __  ___  __| (_)/ _(_)___ _ _ ___
//   | ' </ -_) || | '_ \/ _ \/ _` | '_/ _` | | '  \/ _ \/ _` | |  _| / -_) '_(_-<
//   |_|\_\___|\_, |_.__/\___/\__,_|_| \__,_| |_|_|_\___/\__,_|_|_| |_\___|_| /__/
//             |__/                                                               
//
// Note: This is here, rather than in a separate header file since
//  it's pretty "global" and we don't have to include the GFLW stuff again.
//
class cGFLWKeyboardModifiers
{
public:
    static bool isLeftShiftKeyDown(GLFWwindow* pWindow);
    static bool isRightShiftKeyDown(GLFWwindow* pWindow);
    static bool isShiftKeyDown(GLFWwindow* pWindow);        // Either down

    static bool isLeftControlKeyDown(GLFWwindow* pWindow);
    static bool isRightControlKeyDown(GLFWwindow* pWindow);
    static bool isControlKeyDown(GLFWwindow* pWindow);      // Either down

    static bool isLeftAltKeyDown(GLFWwindow* pWindow);
    static bool isRightAltKeyDown(GLFWwindow* pWindow);
    static bool isAltKeyDown(GLFWwindow* pWindow);          // Either down

    // Returns true if this pattern matches
    // example: (true, false, false) returns true if ONLY shift is down
    static bool isModifierDown(GLFWwindow* pWindow, bool bShift, bool bControl, bool bAlt);

    static bool areAllModsUp(GLFWwindow* pWindow);
};
// ************************************************

void handleAsyncKeyboard(GLFWwindow* pWindow, double deltaTime);
void handleAsyncMouse(GLFWwindow* pWindow, double deltaTime);


// ************************************************
//     ___ _    _____      __          _ _ _             _       
//    / __| |  | __\ \    / /  __ __ _| | | |__  __ _ __| |__ ___
//   | (_ | |__| _| \ \/\/ /  / _/ _` | | | '_ \/ _` / _| / /(_-<
//    \___|____|_|   \_/\_/   \__\__,_|_|_|_.__/\__,_\__|_\_\/__/
//                                                               
// In the documentation, these are static, but in C++ we don't need to do this. 
void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFW_error_callback(int error, const char* description);
void GLFW_cursor_enter_callback(GLFWwindow* window, int entered);
void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void GLFW_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void GLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void GLFW_window_size_callback(GLFWwindow* window, int width, int height);


// ************************************************


// This silly define is so that you 100% know this is NOT some pre-defined define. 
// This enables the use of the Windows context menu.
// See the cWinContextMenu class and the Win32 API code for details.
// (Obviously, if you aren't running this on Windows, you don't want this, right?)
#define YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

    // Turns off the: warning C4005: 'APIENTRY': macro redefinition
#pragma warning( disable: 4005)

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#endif  // YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG

// Note that this is declared no matter what:
void ShowWindowsContextMenu(GLFWwindow* window, int button, int action, int mods);


// SUPER quick-n-dirty template example
template <class T>
T addTwoNumbers(T a, T b)
{
    return a + b;
}

// Here's a more useful templated one, based on this one:
//https://stackoverflow.com/questions/686353/random-float-number-generation
template <class T>
T gGetRandBetween(T LO, T HI)
{
    float fLO = static_cast<float>(LO);
    float fHI = static_cast<float>(HI);
    float r3 = fLO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (fHI - fLO)));
    return r3;
}


#endif // _globalThings_HG_