
#include "GLCommon.h"
//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include "globalThings.h"

#include "cSwing.h"
#include "cParticle.h"
#include "cWeaponPhysics.h"
#include "cParticleWorld.h"
#include "cParticlePlaneContact.h"
#include "cParticleParticleContact.h""

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>      // "string stream" just like iostream and fstream
#include <vector>       // "smart array"
#include <fstream>      // C++ file I-O library (look like any other stream)
#include <rapidjson/rapidjson/filereadstream.h>
#include <rapidjson/rapidjson/document.h>

struct sWindowDef
{
    std::string PositionOfXO[5];
    std::string WeaponSetup[11];
    std::string CameraPosition[3];
    std::string Window[2];
    float light[11];
    std::string MeshName[5];
    std::string PlacingValues[7];
};

struct sConfig
{
    sWindowDef WindowDef;
};

void DrawObject(
    cMesh* pCurrentMesh,
    glm::mat4 matModel,
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager);


void setObjects();
std::string getRandom(float low, float high);
void generateTicTacToe();
void generateWeapon();
void generateXO();
void generateO(glm::vec3 position, int i, int j);
void generateX(glm::vec3 position, int i, int j);
void generateLight(GLuint program);
void checkTicTacToe();
void checkValue(int i, int j);
bool ReadConfigFromJSON(const std::string& filePath, sConfig& configOut);
bool SetValue(const rapidjson::Value& valueIn, float& valueOut);
bool ParseValue(const rapidjson::Value& valueIn, std::string(&valueOut)[5]);
bool ParseValue(const rapidjson::Value& valueIn, std::string(&valueOut)[2]);
bool ParseValue(const rapidjson::Value& valueIn, std::string(&valueOut)[3]);
bool ParseValue(const rapidjson::Value& valueIn, std::string(&valueOut)[11]);
bool ParseValue(const rapidjson::Value& valueIn, float(&valueOut)[11]);
bool ParseValueMesh(const rapidjson::Value& valueIn, std::string(&valueOut)[5]);
bool ParseValueDetectValue(const rapidjson::Value& valueIn, std::string(&valueOut)[7]);

nPhysics::sProjectileDef FireWeapon;
cMesh* weaponMesh = new cMesh();
sConfig config;
bool won = false;
char ch;

int main(void)
{
    ReadConfigFromJSON("config.json", config);
    Player1 = true;
    GLFWwindow* pWindow;
    GLuint program = 0;     // 0 means "no shader program"
    GLint mvp_location = -1;        // Because glGetAttribLocation() returns -1 on error

    int firstToPlay = 1;
    glfwSetErrorCallback(GLFW_error_callback);

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    pWindow = glfwCreateWindow(std::stoi(config.WindowDef.Window[0]), std::stoi(config.WindowDef.Window[1]), "OpenGL is great!", NULL, NULL);

    if (!pWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(pWindow, GLFW_key_callback);
    glfwSetCursorEnterCallback(pWindow, GLFW_cursor_enter_callback);
    glfwSetCursorPosCallback(pWindow, GLFW_cursor_position_callback);
    glfwSetScrollCallback(pWindow, GLFW_scroll_callback);
    glfwSetMouseButtonCallback(pWindow, GLFW_mouse_button_callback);
    glfwSetWindowSizeCallback(pWindow, GLFW_window_size_callback);

    glfwMakeContextCurrent(pWindow);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    GLint max_uniform_location = 0;
    GLint* p_max_uniform_location = NULL;
    p_max_uniform_location = &max_uniform_location;
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

    ::g_StartUp(pWindow);
    ::g_pFlyCamera->setEye(glm::vec3(std::stof(config.WindowDef.CameraPosition[0]), std::stof(config.WindowDef.CameraPosition[1]), std::stof(config.WindowDef.CameraPosition[2])));

    FireWeapon.Acceleration = glm::vec3(0.0f, std::stof(config.WindowDef.WeaponSetup[0]), 0.0f);
    FireWeapon.EnergyAcceleration = std::stof(config.WindowDef.WeaponSetup[1]);
    FireWeapon.Speed = std::stof(config.WindowDef.WeaponSetup[2]);
    FireWeapon.Damping = std::stof(config.WindowDef.WeaponSetup[3]);
    FireWeapon.Mass = std::stof(config.WindowDef.WeaponSetup[4]);
    FireWeapon.Radius = std::stof(config.WindowDef.WeaponSetup[5]);
    FireWeapon.GroundHeight = std::stof(config.WindowDef.WeaponSetup[6]);
    FireWeapon.Position = glm::vec3(std::stof(config.WindowDef.CameraPosition[0]), std::stof(config.WindowDef.CameraPosition[1]), std::stof(config.WindowDef.CameraPosition[2]));
    FireWeapon.Direction = glm::vec4(std::stof(config.WindowDef.WeaponSetup[7]), std::stof(config.WindowDef.WeaponSetup[8]), std::stof(config.WindowDef.WeaponSetup[9]), std::stof(config.WindowDef.WeaponSetup[10]));

    cShaderManager::cShader vertShader;
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";

    cShaderManager::cShader fragShader;
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        program = ::g_pShaderManager->getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
    }



    glUseProgram(program);
    mvp_location = glGetUniformLocation(program, "MVP");
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
    GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");


    generateLight(program);
    

    ::g_pVAOManager->setFilePath("assets/models/");

    std::vector<std::string> vecModelsToLoad;
    vecModelsToLoad.push_back(config.WindowDef.MeshName[0]);
    vecModelsToLoad.push_back(config.WindowDef.MeshName[1]);
    vecModelsToLoad.push_back(config.WindowDef.MeshName[2]);
    vecModelsToLoad.push_back(config.WindowDef.MeshName[3]);
    vecModelsToLoad.push_back(config.WindowDef.MeshName[4]);
    unsigned int totalVerticesLoaded = 0;
    unsigned int totalTrianglesLoaded = 0;
    for (std::vector<std::string>::iterator itModel = vecModelsToLoad.begin(); itModel != vecModelsToLoad.end(); itModel++)
    {
        sModelDrawInfo theModel;
        std::string modelName = *itModel;
        std::cout << "Loading " << modelName << "...";
        if (!::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program))
        {
            std::cout << "didn't work because: " << std::endl;
            std::cout << ::g_pVAOManager->getLastError(true) << std::endl;
        }
        else
        {
            std::cout << "OK." << std::endl;
            std::cout << "\t" << theModel.numberOfVertices << " vertices and " << theModel.numberOfTriangles << " triangles loaded." << std::endl;
            totalTrianglesLoaded += theModel.numberOfTriangles;
            totalVerticesLoaded += theModel.numberOfVertices;
        }
    }
    
    std::vector<nPhysics::cParticle*> particles;
    nPhysics::cParticleWorld* world = new nPhysics::cParticleWorld(5000);
    nPhysics::cParticlePlaneContact groundGenerator(-800.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    world->AddContactContactGenerator(&groundGenerator);

   /* nPhysics::cParticlePlaneContact groundGenerator1(-100.0, glm::vec3(0.0f, -1.0f, 0.0f));
    world->AddContactContactGenerator(&groundGenerator1);*/

    nPhysics::cParticlePlaneContact groundGenerator2(-1500.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    world->AddContactContactGenerator(&groundGenerator2);

    nPhysics::cParticlePlaneContact groundGenerator3(-2500.0, glm::vec3(-1.0f, 0.0f, 0.0f));
    world->AddContactContactGenerator(&groundGenerator3);

    nPhysics::cParticlePlaneContact groundGenerator4(-100.0, glm::vec3(0.0f, 0.0f, 1.0f));
    world->AddContactContactGenerator(&groundGenerator4);

    setObjects();

    cSwing clamp;
    clamp.AddToWorld(world);

    

    const double MAX_DELTA_TIME = 0.1;
    double previousTime = glfwGetTime();
    int valuecheckAdd = 1;
    int conditionValue = 0;

    bool whichoneToShow = true;
    int checkConditionValue = 0;
    int heightReduction = 100;
    int UFOcondition = 0;
    
    while (!glfwWindowShouldClose(pWindow))
    {
        valuecheckAdd++;
        UFOcondition++;
        float ratio;
        int width, height;
        glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
        glm::mat4 matProjection;        // used to be "p";
        glm::mat4 matView;              // used to be "v";

        double currentTime = glfwGetTime();
        double deltaTime = previousTime - currentTime;
        deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
        previousTime = currentTime;

        glfwGetFramebufferSize(pWindow, &width, &height);
        ratio = width / (float)height;

        // Turn on the depth buffer
        glEnable(GL_DEPTH);         // Turns on the depth buffer
        glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSetWindowTitle(pWindow, ::g_TitleText.c_str());

        ::g_pTheLights->CopyLightInfoToShader();

        matProjection = glm::perspective(
            ::g_pFlyCamera->FOV,
            ratio,
            ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
            ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)


        ::g_pFlyCamera->Update(deltaTime);

        glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        glm::vec3 cameraAt = ::g_pFlyCamera->getAtInWorldSpace();
        glm::vec3 cameraUp = ::g_pFlyCamera->getUpVector();


        matView = glm::mat4(1.0f);
        matView = glm::lookAt(cameraEye,   // "eye"
            cameraAt,    // "at"
            cameraUp);


        glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(matView));
        glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));


        

        for (int i = 0; i < 3;i++) {
            for (int j = 0; j < 3; j++) {
                if (g_tictactow[i][j] == "X") {
                    g_tictactow_meshX[i][j]->bIsVisible = true;
                }
                else if (g_tictactow[i][j] == "O") {
                    g_tictactow_meshO[i][j]->bIsVisible = true;
                }
                
            }
        }
        clamp.Update();
        checkTicTacToe();

        for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
        {
            cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

            matModel = glm::mat4(1.0f);

            DrawObject(pCurrentMesh,
                matModel,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                ::g_pVAOManager);

        }

        if (fireWeapon) {
            FireWeapon.Position.x = cameraEye.x;
            FireWeapon.Position.y = cameraEye.y;
            FireWeapon.Position.z = cameraEye.z + 100.0f;
            FireWeapon.Direction = -::g_pFlyCamera->getCameraDirection();
            nPhysics::cWeaponPhysics* projectile = new nPhysics::cWeaponPhysics(FireWeapon);
            weaponMesh->bIsVisible = true;
            particles.push_back(projectile);
            world->AddParticle(projectile);
            fireWeapon = false;
            insideLoop = true;
        }



        for (nPhysics::cParticle* p : particles)
        {
            weaponMesh->positionXYZ = p->GetPosition();
            if (insideLoop) {
                
                        if ((p->GetPosition().x >= std::stoi(config.WindowDef.PlacingValues[0])) 
                            && (p->GetPosition().z >= std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().y >= std::stoi(config.WindowDef.PlacingValues[1]))
                            && (p->GetPosition().x <= std::stoi(config.WindowDef.PlacingValues[2]))
                            && (p->GetPosition().y <= std::stoi(config.WindowDef.PlacingValues[3])))
                        {
                            checkValue(0, 0);
                            
                        }else if ((p->GetPosition().x >= std::stoi(config.WindowDef.PlacingValues[0]) + std::stoi(config.WindowDef.PlacingValues[5])
                            && (p->GetPosition().z >= std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().y >= std::stoi(config.WindowDef.PlacingValues[1]))
                            && (p->GetPosition().x <= std::stoi(config.WindowDef.PlacingValues[2]) + std::stoi(config.WindowDef.PlacingValues[5]))
                            && (p->GetPosition().y <= std::stoi(config.WindowDef.PlacingValues[3]))))
                        {
                            checkValue(0, 1);
                        }
                        else if ((p->GetPosition().x >= std::stoi(config.WindowDef.PlacingValues[0]) + std::stoi(config.WindowDef.PlacingValues[5]) + std::stoi(config.WindowDef.PlacingValues[5])
                            && (p->GetPosition().z >= std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().y >= std::stoi(config.WindowDef.PlacingValues[1]))
                            && (p->GetPosition().x <= std::stoi(config.WindowDef.PlacingValues[2]) + std::stoi(config.WindowDef.PlacingValues[5]) + std::stoi(config.WindowDef.PlacingValues[5]))
                            && (p->GetPosition().y <= std::stoi(config.WindowDef.PlacingValues[3]))))
                        {
                            checkValue(0, 2);
                        }
                        else if ((p->GetPosition().x >= std::stoi(config.WindowDef.PlacingValues[0]))
                            && (p->GetPosition().z >= std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().y >= std::stoi(config.WindowDef.PlacingValues[1]) + std::stoi(config.WindowDef.PlacingValues[6]))
                            && (p->GetPosition().x <= std::stoi(config.WindowDef.PlacingValues[2])
                            && (p->GetPosition().y <= std::stoi(config.WindowDef.PlacingValues[3]))))
                        {
                            checkValue(1, 0);
                        }
                        else if ((p->GetPosition().x >= std::stoi(config.WindowDef.PlacingValues[0]) + std::stoi(config.WindowDef.PlacingValues[5]))
                            && (p->GetPosition().z >= std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().y >= std::stoi(config.WindowDef.PlacingValues[1]) + std::stoi(config.WindowDef.PlacingValues[6]))
                            && (p->GetPosition().x <= std::stoi(config.WindowDef.PlacingValues[2]) + std::stoi(config.WindowDef.PlacingValues[5]))
                            && (p->GetPosition().y <= std::stoi(config.WindowDef.PlacingValues[3])))
                        {
                            checkValue(1, 1);
                        }
                        else if ((p->GetPosition().x >= std::stoi(config.WindowDef.PlacingValues[0]) + std::stoi(config.WindowDef.PlacingValues[5]) + std::stoi(config.WindowDef.PlacingValues[5]))
                            && (p->GetPosition().z >= std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().y >= std::stoi(config.WindowDef.PlacingValues[1]) + std::stoi(config.WindowDef.PlacingValues[6]))
                            && (p->GetPosition().x <= std::stoi(config.WindowDef.PlacingValues[2]) + std::stoi(config.WindowDef.PlacingValues[5]) + std::stoi(config.WindowDef.PlacingValues[5]))
                            && (p->GetPosition().y <= std::stoi(config.WindowDef.PlacingValues[3])))
                        {
                            checkValue(1, 2);
                        }
                        else if ((p->GetPosition().x >= std::stoi(config.WindowDef.PlacingValues[0]))
                            && (p->GetPosition().z >= std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().y >= std::stoi(config.WindowDef.PlacingValues[1]) + std::stoi(config.WindowDef.PlacingValues[6]) + std::stoi(config.WindowDef.PlacingValues[6]))
                            && (p->GetPosition().x <= std::stoi(config.WindowDef.PlacingValues[2]))
                            && (p->GetPosition().y <= std::stoi(config.WindowDef.PlacingValues[3])))
                        {
                            checkValue(2, 0);
                        }
                        else if ((p->GetPosition().x >= std::stoi(config.WindowDef.PlacingValues[0]) + std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().z >= std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().y >= std::stoi(config.WindowDef.PlacingValues[1]) + std::stoi(config.WindowDef.PlacingValues[6]) + std::stoi(config.WindowDef.PlacingValues[6]))
                            && (p->GetPosition().x <= std::stoi(config.WindowDef.PlacingValues[2]) + std::stoi(config.WindowDef.PlacingValues[5]))
                            && (p->GetPosition().y <= std::stoi(config.WindowDef.PlacingValues[3])))
                        {
                            checkValue(2, 1);
                        }
                        else if ((p->GetPosition().x >= std::stoi(config.WindowDef.PlacingValues[0]) + std::stoi(config.WindowDef.PlacingValues[4]) + std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().z >= std::stoi(config.WindowDef.PlacingValues[4]))
                            && (p->GetPosition().y >= std::stoi(config.WindowDef.PlacingValues[1]) + std::stoi(config.WindowDef.PlacingValues[6]) + std::stoi(config.WindowDef.PlacingValues[6]))
                            && (p->GetPosition().x <= std::stoi(config.WindowDef.PlacingValues[2]) + std::stoi(config.WindowDef.PlacingValues[5]) + std::stoi(config.WindowDef.PlacingValues[5]))
                            && (p->GetPosition().y <= std::stoi(config.WindowDef.PlacingValues[3])))
                        {
                            checkValue(2, 2);
                        }
                        
            }
            
        }

        world->TimeStep(deltaTime);


        std::cout << " postion x :" << g_tictactow_meshX[0][0]->positionXYZ.x << "\n";
        std::cout << " postion y :" << g_tictactow_meshX[0][0]->positionXYZ.y << "\n";
        std::cout << " postion z :" << g_tictactow_meshX[0][0]->positionXYZ.z << "\n";

        for (std::vector<nPhysics::cParticle*>::iterator it = particles.begin(); it != particles.end(); )
        {
            if ((*it)->GetIsAlive())
            {
                it++;
            }
            else
            {
                std::vector<nPhysics::cParticle*>::iterator removeIt = it;
                nPhysics::cParticle* deadParticle = *it;
                weaponMesh->bIsVisible = false;
                it = particles.erase(removeIt);
                world->RemoveParticle(deadParticle);
                delete deadParticle;
            }
        }

        clamp.Render();

        if (won) {
            canfireWeapon = false;
        }


        for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
        {
            ::g_vec_pMeshes.at(index)->bIsWireframe = bIsWireframe;
        }


        glfwSwapBuffers(pWindow);


        glfwPollEvents();

        handleAsyncKeyboard(pWindow, deltaTime);
        handleAsyncMouse(pWindow, deltaTime);

    }

    ::g_ShutDown(pWindow);


    glfwDestroyWindow(pWindow);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
void checkValue(int i, int j ) {

    if ((g_tictactow[i][j] != "")) {
        return;
    }

    if (Player1) {
        g_tictactow_meshX[i][j]->bIsVisible = true;
        Player1 = false;
        Player2 = true;
        insideLoop = false;
        g_tictactow[i][j] = "X";
    }
    else if (Player2) {
        g_tictactow_meshO[i][j]->bIsVisible = true;
        Player2 = false;
        Player1 = true;
        insideLoop = false;
        g_tictactow[i][j] = "O";
    }
}


void setObjects() {
    generateWeapon();
    generateXO();
    generateTicTacToe();
    
}

void generateWeapon() {
    cMesh* meshPointer3 = new cMesh();
    meshPointer3->scaleXYZ = glm::vec3(10.0f);
    meshPointer3->meshName = "sphere.ply";
    meshPointer3->positionXYZ = glm::vec3(-1990.0f, -1300.0f, 0.0f);
    meshPointer3->orientationXYZ.y = 0;
    meshPointer3->bIsWireframe = bIsWireframe;
    meshPointer3->bIsVisible = false;
    ::g_vec_pMeshes.push_back(meshPointer3);
    weaponMesh = meshPointer3;
}

void generateTicTacToe() {

    int heightDiff = 0;
    int locationChange = -1990.0f;
    cMesh* meshPointer = new cMesh();
    meshPointer->scaleXYZ = glm::vec3(10.0f);
    meshPointer->meshName = "tictactoe.ply";
    meshPointer->positionXYZ = glm::vec3(locationChange, -700.0f, 0.0f);
    meshPointer->orientationXYZ.y = 0;
    meshPointer->bDontLight = true;
    meshPointer->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);;
    meshPointer->bUseWholeObjectDiffuseColour = true;

    meshPointer->bIsWireframe = bIsWireframe;
    ::g_vec_pMeshes.push_back(meshPointer);



    for (int i = 0; i < 3; i++) {
        cMesh* meshPointer1 = new cMesh();
        meshPointer1->scaleXYZ = glm::vec3(25.0f, 25.0f, 2.0f);
        meshPointer1->meshName = "plane.ply";
        meshPointer1->positionXYZ = glm::vec3(locationChange, -700.0f + heightDiff, 100.0f);
        meshPointer1->orientationXYZ.y = 0;
        meshPointer1->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);;
        meshPointer1->bUseWholeObjectDiffuseColour = true;
        meshPointer1->bIsWireframe = bIsWireframe;
        meshPointer1->bIsVisible = false;
        ::g_vec_pMeshes.push_back(meshPointer1);


        cMesh* meshPointer2 = new cMesh();
        meshPointer2->scaleXYZ = glm::vec3(25.0f, 25.0f, 2.0f);
        meshPointer2->meshName = "plane.ply";
        meshPointer2->positionXYZ = glm::vec3(-1690.0f, -700.0f + heightDiff, 100.0f);
        meshPointer2->orientationXYZ.y = 0;
        meshPointer2->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);;
        meshPointer2->bUseWholeObjectDiffuseColour = true;
        meshPointer2->bIsWireframe = bIsWireframe;
        meshPointer2->bIsVisible = false;
        ::g_vec_pMeshes.push_back(meshPointer2);
        ::g_vec_pEnemy.push_back(meshPointer2);

        cMesh* meshPointer3 = new cMesh();
        meshPointer3->scaleXYZ = glm::vec3(25.0f, 25.0f, 2.0f);
        meshPointer3->meshName = "plane.ply";
        meshPointer3->positionXYZ = glm::vec3(-2290.0f, -700.0f + heightDiff, 100.0f);
        meshPointer3->orientationXYZ.y = 0;
        meshPointer3->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);;
        meshPointer3->bUseWholeObjectDiffuseColour = true;
        meshPointer3->bIsWireframe = bIsWireframe;
        meshPointer3->bIsVisible = false;
        ::g_vec_pMeshes.push_back(meshPointer3);
        ::g_vec_pEnemy.push_back(meshPointer3);
        heightDiff = heightDiff + 300;
    }


}

void generateLight(GLuint program) {
    ::g_pTheLights->theLights[0].position = glm::vec4(-3500.0f, -1280.0f, -600, 1.0f);
    ::g_pTheLights->theLights[0].diffuse = glm::vec4(4.0f, 4.0f, 4.0f, 1.0f);
    ::g_pTheLights->theLights[0].param1.x = 0.0f;
    ::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    ::g_pTheLights->theLights[0].param1.y = 90.0f;
    ::g_pTheLights->theLights[0].param1.z = 10.0f;
    ::g_pTheLights->theLights[0].atten.y = 0.0000001f;
    ::g_pTheLights->theLights[0].atten.z = 0.0000001f;
    ::g_pTheLights->TurnOnLight(0);
    ::g_pTheLights->SetUpUniformLocations(program);
}

void generateX(glm::vec3 position, int i, int j) {
    cMesh* meshPointer1 = new cMesh();
    meshPointer1->scaleXYZ = glm::vec3(3.0f, 3.0f, 2.0f);
    meshPointer1->meshName = "ticX.ply";
    meshPointer1->positionXYZ = position;
    meshPointer1->orientationXYZ.y = 0;
    meshPointer1->bDontLight = true;
    meshPointer1->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);;
    meshPointer1->bUseWholeObjectDiffuseColour = true;
    meshPointer1->bIsWireframe = bIsWireframe;
    meshPointer1->bIsVisible = false;
    
    ::g_vec_pMeshes.push_back(meshPointer1);
    ::g_tictactow_meshX[i][j] = meshPointer1;
}

void generateO(glm::vec3 position, int i, int j) {
    cMesh* meshPointer1 = new cMesh();
    meshPointer1->scaleXYZ = glm::vec3(3.0f, 3.0f, 2.0f);
    meshPointer1->meshName = "ticO.ply";
    meshPointer1->positionXYZ = position;
    meshPointer1->orientationXYZ.y = 0;
    meshPointer1->bDontLight = true;
    meshPointer1->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);;
    meshPointer1->bUseWholeObjectDiffuseColour = true;
    meshPointer1->bIsVisible = false;
    ::g_vec_pMeshes.push_back(meshPointer1);
    ::g_tictactow_meshO[i][j] = meshPointer1;
}


void generateXO() {

    int heightDiff = std::stof(config.WindowDef.PositionOfXO[3]);
    int heightWidth = std::stof(config.WindowDef.PositionOfXO[4]);

    int j = 0;
    for (int i = 0; i < 3; i++) {
        generateX(glm::vec3(std::stof(config.WindowDef.PositionOfXO[0]), std::stof(config.WindowDef.PositionOfXO[1]) + heightDiff, std::stof(config.WindowDef.PositionOfXO[2])), i, j);
        generateO(glm::vec3(std::stof(config.WindowDef.PositionOfXO[0]), std::stof(config.WindowDef.PositionOfXO[1]) + heightDiff, std::stof(config.WindowDef.PositionOfXO[2])), i, j);

        j++;
        generateX(glm::vec3(std::stof(config.WindowDef.PositionOfXO[0]) - (heightWidth), std::stof(config.WindowDef.PositionOfXO[1]) + heightDiff, std::stof(config.WindowDef.PositionOfXO[2])), i, j);
        generateO(glm::vec3(std::stof(config.WindowDef.PositionOfXO[0]) - (heightWidth), std::stof(config.WindowDef.PositionOfXO[1]) + heightDiff, std::stof(config.WindowDef.PositionOfXO[2])), i, j);
        
        j++;
        generateX(glm::vec3(std::stof(config.WindowDef.PositionOfXO[0]) - (heightWidth * 2), std::stof(config.WindowDef.PositionOfXO[1]) + heightDiff, std::stof(config.WindowDef.PositionOfXO[2])), i, j);
        generateO(glm::vec3(std::stof(config.WindowDef.PositionOfXO[0]) - (heightWidth * 2), std::stof(config.WindowDef.PositionOfXO[1]) + heightDiff, std::stof(config.WindowDef.PositionOfXO[2])), i, j);
        
        heightDiff = heightDiff - 320;
        j = 0;
    }
}

void checkTicTacToe() {
    std::string row = "";
    std::string column = "";
    for (int i = 0; i < 3;i++) {
        for (int j = 0; j < 3;j++) {
            row += g_tictactow[i][j];
            column += g_tictactow[j][i];
        }
        if ((row == "XXX") || (column == "XXX")) {
            std::cout << "X is the winner\n";
            won = true;
            break;
        }
        else if ((column == "OOO") || (row == "OOO")) {
            std::cout << "O is the winner\n";
            won = true;
            break;
        }
        row = "";
    }
    std::string diag1 = g_tictactow[0][0] + g_tictactow[1][1] + g_tictactow[2][2];
    std::string diag2 = g_tictactow[0][2] + g_tictactow[1][1] + g_tictactow[2][0];
    if ((diag1 == "XXX") || (diag2 == "XXX")) {
        std::cout << "X is the winner\n";
        won = true;
        return;
    }
    else if ((diag1 == "OOO") || (diag2 == "OOO")) {
        std::cout << "O is the winner\n";
        won = true;
        return;
    }
    
}

std::string getRandom(float low, float high)
{
    int value = low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
    return std::to_string(value);

}


bool ReadConfigFromJSON(const std::string& filePath, sConfig& configOut)
{
    using namespace rapidjson;

    FILE* fp = 0;
    fopen_s(&fp, filePath.c_str(), "rb"); // non-Windows use "r"

    char* readBuffer = new char[80000];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document d;
    d.ParseStream(is);
    ParseValue(d["resolution"], configOut.WindowDef.Window);
    ParseValue(d["XOPosition"], configOut.WindowDef.PositionOfXO);
    ParseValue(d["CameraLocation"], configOut.WindowDef.CameraPosition);
    ParseValue(d["WeaponSetup"], configOut.WindowDef.WeaponSetup);
    ParseValue(d["Light"], configOut.WindowDef.light);
    ParseValueMesh(d["Meshname"], configOut.WindowDef.MeshName);
    ParseValueDetectValue(d["detectValues"], configOut.WindowDef.PlacingValues);

    delete[] readBuffer;
    return true;
}


bool SetValue(const rapidjson::Value& valueIn, std::string& valueOut)
{
    if (!valueIn.IsString())
    {
        return false;
    }
    valueOut = valueIn.GetString();
    return true;
}

bool SetValue(const rapidjson::Value& valueIn, float& valueOut)
{
    if (!valueIn.IsFloat())
    {
        return false;
    }
    valueOut = valueIn.GetFloat();
    return true;
}

bool ParseValue(const rapidjson::Value& valueIn, std::string(&valueOut)[5])
{
    if (!valueIn.IsObject())
    {
        return false;
    }
    bool result = true;
    result &= SetValue(valueIn["x"], valueOut[0]);
    result &= SetValue(valueIn["y"], valueOut[1]);
    result &= SetValue(valueIn["z"], valueOut[2]);
    result &= SetValue(valueIn["heigthDiff"], valueOut[3]);
    result &= SetValue(valueIn["heightWidth"], valueOut[4]);
    return result;
}

bool ParseValue(const rapidjson::Value& valueIn, std::string(&valueOut)[2])
{
    if (!valueIn.IsObject())
    {
        return false;
    }
    bool result = true;
    result &= SetValue(valueIn["height"], valueOut[0]);
    result &= SetValue(valueIn["width"], valueOut[1]);
    return result;
}

bool ParseValue(const rapidjson::Value& valueIn, std::string(&valueOut)[3])
{
    if (!valueIn.IsObject())
    {
        return false;
    }
    bool result = true;
    result &= SetValue(valueIn["x"], valueOut[0]);
    result &= SetValue(valueIn["y"], valueOut[1]);
    result &= SetValue(valueIn["z"], valueOut[2]);
    return result;
}

bool ParseValue(const rapidjson::Value& valueIn, std::string(&valueOut)[11])
{
    if (!valueIn.IsObject())
    {
        return false;
    }
    bool result = true;
    result &= SetValue(valueIn["Accelerationy"], valueOut[0]);
    result &= SetValue(valueIn["EnergyAcceleration"], valueOut[1]);
    result &= SetValue(valueIn["Speed"], valueOut[2]);
    result &= SetValue(valueIn["Damping"], valueOut[3]);
    result &= SetValue(valueIn["Mass"], valueOut[4]);
    result &= SetValue(valueIn["Radius"], valueOut[5]);
    result &= SetValue(valueIn["GroundHeight"], valueOut[6]);
    result &= SetValue(valueIn["Directionx"], valueOut[7]);
    result &= SetValue(valueIn["Directiony"], valueOut[8]);
    result &= SetValue(valueIn["Directionz"], valueOut[9]);
    result &= SetValue(valueIn["Directionw"], valueOut[10]);
    return result;
}

bool ParseValue(const rapidjson::Value& valueIn, float(&valueOut)[11])
{
    if (!valueIn.IsObject())
    {
        return false;
    }
    bool result = true;
    result &= SetValue(valueIn["Positionx"], valueOut[0]);
    result &= SetValue(valueIn["Positiony"], valueOut[1]);
    result &= SetValue(valueIn["Positionz"], valueOut[2]);
    result &= SetValue(valueIn["Diffusex"], valueOut[3]);
    result &= SetValue(valueIn["Diffusey"], valueOut[4]);
    result &= SetValue(valueIn["Diffusez"], valueOut[5]);
    result &= SetValue(valueIn["Paramx"], valueOut[6]);
    result &= SetValue(valueIn["Paramy"], valueOut[7]);
    result &= SetValue(valueIn["Paramz"], valueOut[8]);
    result &= SetValue(valueIn["Atteny"], valueOut[9]);
    result &= SetValue(valueIn["Attenz"], valueOut[10]);
    return result;
}

bool ParseValueMesh(const rapidjson::Value& valueIn, std::string(&valueOut)[5])
{
    if (!valueIn.IsObject())
    {
        return false;
    }
    bool result = true;
    result &= SetValue(valueIn["BasePly"], valueOut[0]);
    result &= SetValue(valueIn["PlanePly"], valueOut[1]);
    result &= SetValue(valueIn["OPly"], valueOut[2]);
    result &= SetValue(valueIn["XPly"], valueOut[3]);
    result &= SetValue(valueIn["SpherePly"], valueOut[4]);
    return result;
}

bool ParseValueDetectValue(const rapidjson::Value& valueIn, std::string(&valueOut)[7])
{
    if (!valueIn.IsObject())
    {
        return false;
    }
    bool result = true;
    result &= SetValue(valueIn["x1"], valueOut[0]);
    result &= SetValue(valueIn["y1"], valueOut[1]);
    result &= SetValue(valueIn["x2"], valueOut[2]);
    result &= SetValue(valueIn["y2"], valueOut[3]);
    result &= SetValue(valueIn["z"], valueOut[4]);
    result &= SetValue(valueIn["addonx"], valueOut[5]);
    result &= SetValue(valueIn["addony"], valueOut[6]);
    return result;
}