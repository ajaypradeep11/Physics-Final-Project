#include "globalThings.h"

#include <sstream>

/*static*/ void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
    {
        if ((g_tictactow[2][0] == "") || (g_tictactow[2][0] == "")) {
            if (Player1) {
                g_tictactow[2][0] = 'X';
                Player1 = false;
                Player2 = true;
            }
            else if (Player2) {
                g_tictactow[2][0] = 'O';
                Player2 = false;
                Player1 = true;
            }
        }
    }
    if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
    {
        if ((g_tictactow[2][1] == "") || (g_tictactow[2][1] == "")) {
            if (Player1) {
                g_tictactow[2][1] = 'X';
                Player1 = false;
                Player2 = true;
            }
            else if (Player2) {
                g_tictactow[2][1] = 'O';
                Player2 = false;
                Player1 = true;
            }
        }
    }
    if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
    {
        if ((g_tictactow[2][2] == "") || (g_tictactow[2][2] == "")) {
            if (Player1) {
                g_tictactow[2][2] = 'X';
                Player1 = false;
                Player2 = true;
            }
            else if (Player2) {
                g_tictactow[2][2] = 'O';
                Player2 = false;
                Player1 = true;
            }
        }
    }
    if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS)
    {
        if ((g_tictactow[1][0] == "") || (g_tictactow[1][0] == "")) {
            if (Player1) {
                g_tictactow[1][0] = 'X';
                Player1 = false;
                Player2 = true;
            }
            else if (Player2) {
                g_tictactow[1][0] = 'O';
                Player2 = false;
                Player1 = true;
            }
        }
    }
    if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS)
    {
        if ((g_tictactow[1][1] == "") || (g_tictactow[1][1] == "")) {
            if (Player1) {
                g_tictactow[1][1] = 'X';
                Player1 = false;
                Player2 = true;
            }
            else if (Player2) {
                g_tictactow[1][1] = 'O';
                Player2 = false;
                Player1 = true;
            }
        }
    }
    if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS)
    {
        if ((g_tictactow[1][2] == "") || (g_tictactow[1][2] == "")) {
            if (Player1) {
                g_tictactow[1][2] = 'X';
                Player1 = false;
                Player2 = true;
            }
            else if (Player2) {
                g_tictactow[1][2] = 'O';
                Player2 = false;
                Player1 = true;
            }
        }
    }
    if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS)
    {
        if ((g_tictactow[0][0] == "") || (g_tictactow[0][0] == "")) {
            if (Player1) {
                g_tictactow[0][0] = 'X';
                Player1 = false;
                Player2 = true;
            }
            else if (Player2) {
                g_tictactow[0][0] = 'O';
                Player2 = false;
                Player1 = true;
            }
        }
    }
    if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS)
    {
        if ((g_tictactow[0][1] == "") || (g_tictactow[0][1] == "")) {
            if (Player1) {
                g_tictactow[0][1] = 'X';
                Player1 = false;
                Player2 = true;
            }
            else if (Player2) {
                g_tictactow[0][1] = 'O';
                Player2 = false;
                Player1 = true;
            }
        }
        

    }
    if (key == GLFW_KEY_KP_9 && action == GLFW_PRESS)
    {
        if ((g_tictactow[0][2] == "") || (g_tictactow[0][2] == "")) {
            if (Player1) {
                g_tictactow[0][2] = 'X';
                Player1 = false;
                Player2 = true;
            }
            else if (Player2) {
                g_tictactow[0][2] = 'O';
                Player2 = false;
                Player1 = true;
            }
        }

    }
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        if (bIsWireframe) bIsWireframe = false;
        else bIsWireframe = true;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (canfireWeapon) {
            if (fireWeapon) {
                fireWeapon = false;
            }
            else {
                fireWeapon = true;
            }
        }
             
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        if (swingBool) {
            swingBool = false;
        }
        else {
            swingBool = true;
        }   
    }

    float cameraSpeed = 1.0f;
    float objectMovementSpeed = 1.0f;
    float lightMovementSpeed = 1.0f;

    bool bShiftDown = false;
    bool bControlDown = false;
    bool bAltDown = false;

    //    // Shift down?
    //    if ( mods == GLFW_MOD_SHIFT )       // 0x0001   0000 0001
    //    {
    //        // ONLY shift is down
    //    }
    //    // Control down?
    //    if ( mods == GLFW_MOD_CONTROL  )    // 0x0002   0000 0010
    //    // Alt down?
    //    if ( mods == GLFW_MOD_ALT   )       // 0x0004   0000 0100

        //   0000 0111 
        // & 0000 0001
        // -----------
        //   0000 0001 --> Same as the shift mask

        // Use bitwise mask to filter out just the shift
    if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT )
    {
        // Shift is down and maybe other things, too
        bShiftDown = true;
    }
    if ( (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL )
    {
        // Shift is down and maybe other things, too
        bControlDown = true;
    }
    if ( (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT )
    {
        // Shift is down and maybe other things, too
        bAltDown = true;
    }



//   // If JUST the shift is down, move the "selected" object
//   if ( bShiftDown && (!bControlDown) && (!bAltDown) )
//   {
//       //if (key == GLFW_KEY_A)  {   g_vecMeshes[::g_selectedObject].positionXYZ.x -= objectMovementSpeed;     } // Go left
//       if ( key == GLFW_KEY_A ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.x -= objectMovementSpeed; } // Go left
//       if ( key == GLFW_KEY_D ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.x += objectMovementSpeed; } // Go right
//       if ( key == GLFW_KEY_W ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.z += objectMovementSpeed; }// Go forward 
//       if ( key == GLFW_KEY_S ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.z -= objectMovementSpeed; }// Go backwards
//       if ( key == GLFW_KEY_Q ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.y -= objectMovementSpeed; }// Go "Down"
//       if ( key == GLFW_KEY_E ) { ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.y += objectMovementSpeed; }// Go "Up"
//
//       std::stringstream strTitle;
//       // std::cout << 
//       strTitle << "::g_vec_pMeshes[" << ::g_selectedObject << "].positionXYZ : "
//           << ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.x << ", "
//           << ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.y << ", "
//           << ::g_vec_pMeshes[::g_selectedObject]->positionXYZ.z;// << std::endl;
//
//       ::g_TitleText = strTitle.str();
//
//       // TODO: Add some controls to change the "selcted object"
//       // i.e. change the ::g_selectedObject value
//
//
//   }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

   // If JUST the ALT is down, move the "selected" light
   if ( (!bShiftDown) && (!bControlDown) && bAltDown )
   {
//       if ( key == GLFW_KEY_A ) { ::g_pTheLights->theLights[::g_selectedLight].position.x -= lightMovementSpeed; } // Go left
//       if ( key == GLFW_KEY_D ) { ::g_pTheLights->theLights[::g_selectedLight].position.x += lightMovementSpeed; } // Go right
//       if ( key == GLFW_KEY_W ) { ::g_pTheLights->theLights[::g_selectedLight].position.z += lightMovementSpeed; }// Go forward 
//       if ( key == GLFW_KEY_S ) { ::g_pTheLights->theLights[::g_selectedLight].position.z -= lightMovementSpeed; }// Go backwards
//       if ( key == GLFW_KEY_Q ) { ::g_pTheLights->theLights[::g_selectedLight].position.y -= lightMovementSpeed; }// Go "Down"
//       if ( key == GLFW_KEY_E ) { ::g_pTheLights->theLights[::g_selectedLight].position.y += lightMovementSpeed; }// Go "Up"
//
//       // constant attenuation
//       if ( key == GLFW_KEY_1 )
//       {
//           ::g_pTheLights->theLights[::g_selectedLight].atten.x *= 0.99f; // -1% less
//       }
//       if ( key == GLFW_KEY_2 )
//       {
//           // Is it at (or below) zero?
//           if ( ::g_pTheLights->theLights[::g_selectedLight].atten.x <= 0.0f )
//           {
//               // Set it to some really small initial attenuation
//               ::g_pTheLights->theLights[::g_selectedLight].atten.x = 0.001f;
//           }
//           ::g_pTheLights->theLights[::g_selectedLight].atten.x *= 1.01f; // +1% more
//       }
//       // linear attenuation
//       if ( key == GLFW_KEY_3 )
//       {
//           ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 0.99f; // -1% less
//       }
//       if ( key == GLFW_KEY_4 )
//       {
//           // Is it at (or below) zero?
//           if ( ::g_pTheLights->theLights[::g_selectedLight].atten.y <= 0.0f )
//           {
//               // Set it to some really small initial attenuation
//               ::g_pTheLights->theLights[::g_selectedLight].atten.y = 0.001f;
//           }
//           ::g_pTheLights->theLights[::g_selectedLight].atten.y *= 1.01f; // +1% more
//       }
//       // quardatic attenuation
//       if ( key == GLFW_KEY_5 )
//       {
//           ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 0.99f; // -1% less
//       }
//       if ( key == GLFW_KEY_6 )
//       {
//           // Is it at (or below) zero?
//           if ( ::g_pTheLights->theLights[::g_selectedLight].atten.z <= 0.0f )
//           {
//               // Set it to some really small initial attenuation
//               ::g_pTheLights->theLights[::g_selectedLight].atten.z = 0.001f;
//           }
//           ::g_pTheLights->theLights[::g_selectedLight].atten.z *= 1.01f; // +1% more
//       }

        if ( key == GLFW_KEY_PAGE_UP )
        {
            ::g_bShowDebugShere = true;
        }
        if ( key == GLFW_KEY_PAGE_DOWN )
        {
            ::g_bShowDebugShere = false;
        }


        if ( key == GLFW_KEY_KP_ADD && action == GLFW_PRESS )
        {
            ::g_selectedLight++;
            if ( ::g_selectedLight >= cLightManager::NUMBER_OF_LIGHTS )
            {
                ::g_selectedLight = cLightManager::NUMBER_OF_LIGHTS - 1;
            }
        }
        if ( key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS )
        {
            ::g_selectedLight--;
            if ( ::g_selectedLight >= cLightManager::NUMBER_OF_LIGHTS )
            {
                ::g_selectedLight = 0;
            }
        }


        if ( key == GLFW_KEY_KP_MULTIPLY )
        {
            // Turn light on
            ::g_pTheLights->theLights[::g_selectedLight].param2.x = 1.0f;
        }
        if ( key == GLFW_KEY_KP_DIVIDE )
        {
            // Turn light off
            ::g_pTheLights->theLights[::g_selectedLight].param2.x = 0.0f;
        }



        std::stringstream strTitle;
        // std::cout << 
        strTitle << "Light # " << ::g_selectedLight << " positionXYZ : "
            << ::g_pTheLights->theLights[::g_selectedLight].position.x << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.y << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.z << "  "
            << "attenuation (C, L, Q): "
            << ::g_pTheLights->theLights[::g_selectedLight].atten.x << ", "        // Const
            << ::g_pTheLights->theLights[::g_selectedLight].atten.y << ", "        // Linear
            << ::g_pTheLights->theLights[::g_selectedLight].atten.z << "  "        // Quadratic
            << (::g_pTheLights->theLights[::g_selectedLight].param2.x > 0.0f ? " is on" : " is off");
        //<< std::endl;
        ::g_TitleText = strTitle.str();

 

    }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

    return;
}
