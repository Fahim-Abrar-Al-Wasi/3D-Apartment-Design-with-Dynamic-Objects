#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "BezierCurve.h"
#include "pointLight.h"
#include "SpotLight.h"
#include "sphere.h"
#include "cube.h" //e
#include "stb_image.h" //e
#include <cmath>
#include <iostream>
#include <chrono>
#include <cstdlib> 
#include <ctime>

using namespace std;
void GenSheet_Transparent_collapsible_gate_draw(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture, float sc);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b, float shininess);
void axis(unsigned int& cubeVAO, Shader& lightingShader);
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void floor(unsigned int& cubeVAO, Shader& lightingShader);
void frontWall(unsigned int& cubeVAO, Shader& lightingShader, Cylinder& cylinder, HemiSphere& hemisphere, Cone& cone);
void rightWall(unsigned int& cubeVAO, Shader& lightingShader, CutCone& cone);
void collapsible_gate_draw(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float sc);
//void ambienton_off(Shader& lightingShader);
//void diffuse_on_off(Shader& lightingShader);
//void specular_on_off(Shader& lightingShader);
void GenHalfCylinder_Transparent(
    unsigned int VAO, Shader ourShader, float radius, float height, int segments,
    float rot_x, float rot_y, float rot_z,
    float trans_x, float trans_y, float trans_z,
    unsigned int texture);
void Glass_plant(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void GenSheet_Transparent(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture);
void Glass_window(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float freeze_angle, float rota);
void Glass_Lift(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift);
void load_texture(unsigned int& texture, string image_name, GLenum format);
unsigned int createFramebuffer(unsigned int& texture, unsigned int width, unsigned int height);
void renderMirrorSurface(unsigned int mirrorTexture, const glm::mat4& projection, Shader& mirrorShaderProgram, const glm::vec3& mirrorPosition, glm::mat4& mirroredView);
void Glass_stair(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float rot, float length, float sh);
void GenSheet_Transparent_2(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture, float sh);
void Glass_Clock_bg(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift);
void GenSheet_Transparent_for_window(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture, float freeze_angle, float rota);
void Glass_window_2(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float rot);
void Glass_varanda_door(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float rot);
void collapsible_gate_draw_2(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float sc);
void GenSheet_Transparent_collapsible_gate_draw_2(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture, float sc);

glm::mat4 myPerspective(float fov, float aspect, float near, float far) {
    glm::mat4 result(0.0f); // Initialize to a zero matrix

    float tanHalfFovy = tan(fov / 2.0f);

    result[0][0] = 1.0f / (aspect * tanHalfFovy);
    result[1][1] = 1.0f / tanHalfFovy;
    result[2][2] = -(far + near) / (far - near);
    result[2][3] = -1.0f;
    result[3][2] = -(2.0f * far * near) / (far - near);

    return result;
}

//win_1_1
float rotateAxis_Y_win_2_1 = 0.0;

// settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 900;

// Time management
double lastKeyPressTime = 0.0;
const double keyPressDelay = 0.2; // delay in seconds

//swing info
// Global or static variables to track swing state
bool isSwinging = false;            // Tracks if the swing is active
float swingVelocity = 0.0f;         // Controls the swing's velocity
float maxSwingVelocity = 0.08f;      // Maximum velocity for the swing
float damping = 0.0001f;              // Damping factor to control acceleration/deceleration
float swingAngleAmplitude = 0.0f;    // Maximum angle of the swing (dynamic based on velocity)
float maxSwingAngle = 50.0f;
float currenttime = 0.0f;                  // Time variable for the swing motion


// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

//add delay to key stroke
float keyCooldown = 0.2f;        // Cooldown time in seconds (adjust as needed)

// camera
Camera camera(glm::vec3(10.0f, 5.0f, -50.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;//pos
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;//direction
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);//up
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// General shear matrix function considering all axes
glm::mat4 createShearMatrix(float sx_y, float sx_z, float sy_x, float sy_z, float sz_x, float sz_y) {
    glm::mat4 shearMatrix = glm::mat4(1.0f); // Identity matrix

    // Apply shear factors
    shearMatrix[1][0] = sx_y; // Shear X based on Y
    shearMatrix[2][0] = sx_z; // Shear X based on Z
    shearMatrix[0][1] = sy_x; // Shear Y based on X
    shearMatrix[2][1] = sy_z; // Shear Y based on Z
    shearMatrix[0][2] = sz_x; // Shear Z based on X
    shearMatrix[1][2] = sz_y; // Shear Z based on Y

    return shearMatrix;
}

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(12.50f,  19.8f,  38.5f),
    glm::vec3(10.0f,  29.8f,  30.0f),
    glm::vec3(30.0f,  29.8f,  30.0f),
    glm::vec3(10.0f,  29.8f,  45.0f),
    glm::vec3(30.0f,  29.8f,  45.0f),// give to mirror
    glm::vec3(30.0f,  10.0f,  40.0f),
    glm::vec3(25.0f,  10.0f,  15.0f),
    glm::vec3(5.0f,  10.0f,  15.0f),
    glm::vec3(15.0f,  40.0f,  5.0f),
    glm::vec3(28.0f, 22.0f, 13.0f)
    
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);
PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);
PointLight pointlight6(

    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    6       // light number
);
PointLight pointlight7(

    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    7       // light number
);
PointLight pointlight8(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    8       // light number
);
PointLight pointlight9(

    pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    9       // light number
);
PointLight pointlight10(

    pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    10       // light number
);


//-------------------------SpotLight-----------------------------
glm::vec3 spotPositions[] = {
    glm::vec3(20.0f,15.0f,-20.0f)
};

SpotLight spotlight1(
    spotPositions[0].x, spotPositions[0].y, spotPositions[0].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1,       // light number
    glm::cos(glm::radians(35.5f)),
    glm::cos(glm::radians(40.0f)),
    0, -1, 0
);

void getTime(float& hours, float& minutes, float& seconds) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    tm local_time;
    localtime_s(&local_time, &time); // Use localtime_s instead of localtime
    hours = static_cast<float>(local_time.tm_hour);
    minutes = static_cast<float>(local_time.tm_min);
    seconds = static_cast<float>(local_time.tm_sec);
}


unsigned int texture0, glass,glass2,hanging_plant,clock_bg,fountain_tex, collapsible_gate;
glm::vec4 off_white = glm::vec4(0.9098039215686274, 0.8549019607843137, 0.8, 1.0f);

// light settings
bool pointLightOn = true;
bool directionalLightOn = true;
bool SpotLightOn = true;
bool AmbientON = true;
bool DiffusionON = true;
bool SpecularON = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool onOffToggle = true;

//variable for fan control
bool isFanON = false;
float fanAngleStep = 2.0f;
float fanAngle = 0.0f;


// Variables for car movement
float carPositionX = 20.0f;
float carPositionZ = 15.0f;
float carRotationAngle = 0.0f; // Current rotation angle of the car
bool isTranslatingX = false;    // Car translation state
bool isTranslatingZ = false;
bool isTranslatingXr = false;
bool isTranslatingZr = false;
bool isRotatingX = false;       // Car rotation state
bool isRotatingZ = false;
bool isRotatingXr = false;
float rotationTarget = 0.0f;   // Target rotation angle
float translationSpeed = 0.05f; // Speed of translation
float rotationSpeed = 0.6f;    // Speed of rotation (degrees per frame)
float carX = 5.0f;
float carZ = 5.0f;
float carXr = 5.0f;

//freeze
bool isFreezeOpen = false;
bool isOpening = true;
float freezeThreshold = 95.0f;
float freezeStep = 1.0f;
float FreezeAngle = 0.0f;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

//lift movement
bool isLiftON = false;
float liftPosition = 0.6005f;

bool isLiftDoorOpenDown = false;
float liftDoorPositionDown = 0.0f;

bool isLiftDoorOpenUp = false;
float liftDoorPositionUp = 0.0f;

bool glass_gateOpen = false;
float sc = 1.0f;

// Helper function to generate a random float in a given range
float getRandomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (max - min));
}

// Structure to hold the random positions for each leaf
struct LeafPosition {
    float x, y, z;
};

std::vector<LeafPosition> leafPositions;

void generateLeaves(int numLeaves) {
    for (int i = 0; i < numLeaves; ++i) {
        // Generate random positions for each leaf and store it
        LeafPosition position;
        position.x = getRandomFloat(-1.5f, 11.5f);
        position.y = getRandomFloat(6.0f, 15.8f);
        position.z = getRandomFloat(-40.5f, -29.5f);
        leafPositions.push_back(position);
    }
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    Shader TransparentShader("vertexShaderForBlending.vs", "fragmentShaderForBlending.fs");//for transparent object

    Shader mirrorShaderProgram("MirrorSurfaceVertexShader.vs", "MirrorSurfaceFragmentShader.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    GLfloat cp_pitcher[] = {
    -0.50, 1.95, 0.0,
    -0.30, 1.88, 0.0,
    -0.30, 1.65, 0.0,
    -0.70, 1.32, 0.0,
    -0.70, 1.045, 0.0,
    -0.70, 0.675, 0.0,
    -0.60, 0.42, 0.0,
    -0.20, 0.175, 0.0,
    -0.20, 0.06, 0.0
    };

    GLfloat cp_fountain_secondary_base[] = {
    -1.5, 1.1, 0.0,
    -1.4, 1.6, 0.0,
    -1.2, 1.6, 0.0,
    -1.0, 0.8, 0.0,
    -0.8, 0.6, 0.0,
    -0.6, 1.3, 0.0,
    -0.4, 1.1, 0.0,
    -0.3, 0.7, 0.0,
    -0.2, 0.2, 0.0,
    - 0.08, 0.2, 0.0,
    - 0.01, 0.2, 0.0
    };

    GLfloat cp_fountain_primary_base[] = {
        - 2.25, 1.6, 0.0,
        - 2.20, 1.6, 0.0,
        - 2.15, 1.6, 0.0,
        - 2.10, 1.6, 0.0,
        - 2.05, 1.9, 0.0,
        - 2.0, 3.4, 0.0,
        - 1.95, 3.6, 0.0,
        - 1.90, 3.6, 0.0,
        - 1.85, 3.5, 0.0,
        - 1.80, 3.5, 0.0,
        - 1.75, 3.3, 0.0,
        - 1.7, 1.9, 0.0,
        - 1.7, 1.6, 0.0,
        - 1.65, 1.6, 0.0
    };

    GLfloat cp_ralling[] = {
        -0.05, 1.995, 0.0,
        -4.05, 1.795, 0.0,
        1.5, 1.775, 0.0,
        1.5, 0.35, 0.0,
        -4.05, 0.15, 0.0
        - 0.0, 0.0, 0.0,
    };

    GLfloat cp_tree_base[] = {
    -0.355, 1.935, 0.0,
    -0.31, 1.555, 0.0,
    -0.415, 1.29, 0.0,
    -0.415, 1.09, 0.0,
    -0.345, 0.93, 0.0,
    -0.32, 0.72, 0.0,
    -0.4, 0.515, 0.0,
    -0.44, 0.38, 0.0,
    -0.51, 0.205, 0.0,
    -0.61, 0.075, 0.0,
    -0.75, 0.04, 0.0
    };

    GLfloat cp_tree_brantch[] = {
    -0.06, 0.165, 0.0,
    -0.07, 0.355, 0.0,
    -0.1, 0.575, 0.0,
    -0.07, 0.73, 0.0,
    -0.055, 0.95, 0.0,
    -0.055, 1.07, 0.0
    };

    //leaf
    srand(static_cast<unsigned int>(time(0)));
    generateLeaves(200);

     // Transparent VAO
    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
         0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        1.0f,  0.0f,  0.0f,  1.0f,  1.0f,

        0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  0.0f,  1.0f,  0.0f
    };


    // transparent VAO
    unsigned int transparentVAO, transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // Half-cylinder vertices
    std::vector<float> halfCylinderVertices;
    const int segments = 36; // Number of segments for the curved surface
    const float PI_CONSTANT = 3.14159265359f;

    for (int i = 0; i <= segments; ++i) {
        float angle = (PI_CONSTANT * i) / segments; // Angle from 0 to 180 degrees

        // Bottom vertex (x, y, z, u, v)
        float x = cos(angle);     // X-coordinate on the circular base
        float y = 0.0f;           // Bottom of the cylinder
        float z = sin(angle);     // Z-coordinate on the circular base
        float u = (float)i / segments; // Texture u-coordinate
        float v = 1.0f;           // Texture v-coordinate for bottom
        halfCylinderVertices.insert(halfCylinderVertices.end(), { x, y, z, u, v });

        // Top vertex (x, y, z, u, v)
        y = 1.0f;                 // Top of the cylinder
        v = 0.0f;                 // Texture v-coordinate for top
        halfCylinderVertices.insert(halfCylinderVertices.end(), { x, y, z, u, v });
    }

    // transparent VAO
    unsigned int transparentVAO2, transparentVBO2;
    glGenVertexArrays(1, &transparentVAO2);
    glGenBuffers(1, &transparentVBO2);
    glBindVertexArray(transparentVAO2);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO2);
    // Corrected call
    glBufferData(GL_ARRAY_BUFFER, halfCylinderVertices.size() * sizeof(float), halfCylinderVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    


    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Texture loading
    //Texture 0
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("transparent.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture 0" << endl;
    }
    stbi_image_free(data);

    string laughEmoPath = "emoji.png";
    unsigned int laughEmoji = loadTexture(laughEmoPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(laughEmoji, laughEmoji, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string bathroom_cabinet = "wood_texture_4.png";
    unsigned int bathroom_cabinet_texture = loadTexture(bathroom_cabinet.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bathroom_cabinet_cube = Cube(bathroom_cabinet_texture, bathroom_cabinet_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string bathroom_floor_2 = "Bathroom_floor_4.png";
    unsigned int bathroom_floor_2_texture = loadTexture(bathroom_floor_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bathroom_floor_2_cube = Cube(bathroom_floor_2_texture, bathroom_floor_2_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string bathroom_wall_2 = "Bathroom_wall_3.png";
    unsigned int bathroom_wall_2_texture = loadTexture(bathroom_wall_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bathroom_wall_2_cube = Cube(bathroom_wall_2_texture, bathroom_wall_2_texture, 32.0f, 0.0f, 0.0f, 4.0f, 4.0f);

    //---------------------Ground Floor--------------------------
    // 
    //tree
    string tree_bark = "tree_bark.png";
    unsigned int tree_bark_tex = loadTexture(tree_bark.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string leaf = "leaf.png";
    unsigned int leaf_tex = loadTexture(leaf.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string boundary_wall = "boundary_wall.png";
    unsigned int boundary_wall_tex = loadTexture(boundary_wall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube boundary_wall_cube = Cube(boundary_wall_tex, boundary_wall_tex, 32.0f, 0.0f, 0.0f, 20.0f, 2.0f);

    Cube boundary_wall_cube_2 = Cube(boundary_wall_tex, boundary_wall_tex, 32.0f, 0.0f, 0.0f, 7.0f, 2.0f);

    //outside
    string fountain_4 = "fountain_4.png";
    unsigned int fountain_4_tex = loadTexture(fountain_4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string fountain_5 = "fountain_5.png";
    unsigned int fountain_5_tex = loadTexture(fountain_5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string grass_field = "grass_field.png";
    unsigned int grass_field_tex = loadTexture(grass_field.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube grass_field_cube = Cube(grass_field_tex, grass_field_tex, 32.0f, 0.0f, 0.0f, 10.0f, 10.0f);

    string grass_field_1 = "grass_field.png";
    unsigned int grass_field_1_tex = loadTexture(grass_field_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube grass_field_1_cube = Cube(grass_field_1_tex, grass_field_1_tex, 32.0f, 0.0f, 0.0f, 2.0f, 20.0f);

    string grass_field_2 = "grass_field.png";
    unsigned int grass_field_2_tex = loadTexture(grass_field_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube grass_field_2_cube = Cube(grass_field_1_tex, grass_field_1_tex, 32.0f, 0.0f, 0.0f, 15.0f, 2.0f);

    string walkway_tile = "walkway_tile.png";
    unsigned int walkway_tile_tex = loadTexture(walkway_tile.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube walkway_tile_cube = Cube(walkway_tile_tex, walkway_tile_tex, 32.0f, 0.0f, 0.0f, 2.0f, 10.0f);

    Cube walkway_tile_cube_2 = Cube(walkway_tile_tex, walkway_tile_tex, 32.0f, 0.0f, 0.0f, 5.0f, 2.0f);

    //inside
    string toilet = "toilet.png";
    unsigned int toilet_texture = loadTexture(toilet.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube toilet_cube = Cube(toilet_texture, toilet_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string basin_point = "basin_point.png";
    unsigned int basin_point_texture = loadTexture(basin_point.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string ground_room_floor = "ground_room_floor.png";
    unsigned int ground_room_floor_texture = loadTexture(ground_room_floor.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube ground_room_floor_cube = Cube(ground_room_floor_texture, ground_room_floor_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string ground_bath_floor = "ground_bath_floor.png";
    unsigned int ground_bath_floor_texture = loadTexture(ground_bath_floor.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube ground_bath_floor_cube = Cube(ground_bath_floor_texture, ground_bath_floor_texture, 32.0f, 0.0f, 0.0f, 2.0f, 2.0f);

    string outside_tile = "outside_tile.png";
    unsigned int outside_tile_texture = loadTexture(outside_tile.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube outside_tile_cube = Cube(outside_tile_texture, outside_tile_texture, 32.0f, 0.0f, 0.0f, 4.0f, 4.0f);

    string Stair_corridor = "Stair_corridor.png";
    unsigned int Stair_corridor_texture = loadTexture(Stair_corridor.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube Stair_corridor_cube = Cube(Stair_corridor_texture, Stair_corridor_texture, 32.0f, 0.0f, 0.0f, 4.0f, 12.0f);

    string ground_bath_wall = "rsz_1ground_bath_wall2.png";
    unsigned int ground_bath_wall_texture = loadTexture(ground_bath_wall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube ground_bath_wall_cube = Cube(ground_bath_wall_texture, ground_bath_wall_texture, 32.0f, 0.0f, 0.0f, 5.0f, 5.0f);

    string bed = "bed.png";
    unsigned int bed_texture = loadTexture(bed.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bed_cube = Cube(bed_texture, bed_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string cushion = "cushion.png";
    unsigned int cushion_texture = loadTexture(cushion.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string ground_almirah = "ground_almirah.png";
    unsigned int ground_almirah_texture = loadTexture(ground_almirah.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube ground_almirah_cube = Cube(ground_almirah_texture, ground_almirah_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string wood_texture_3 = "wood_texture_3.png";
    unsigned int wood_texture_3_texture = loadTexture(wood_texture_3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wood_texture_3_cube = Cube(wood_texture_3_texture, wood_texture_3_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string Laptop_keyboard = "Laptop_keyboard.png";
    unsigned int Laptop_keyboard_texture = loadTexture(Laptop_keyboard.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube Laptop_keyboard_cube = Cube(Laptop_keyboard_texture, Laptop_keyboard_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string Laptop_display = "display.png";
    unsigned int Laptop_display_texture = loadTexture(Laptop_display.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube Laptop_display_cube = Cube(Laptop_display_texture, Laptop_display_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string Stair = "Stair.png";
    unsigned int Stair_texture = loadTexture(Stair.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube Stair_cube = Cube(Stair_texture, Stair_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string door = "door.png";
    unsigned int door_texture = loadTexture(door.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube_for_door door_cube = Cube_for_door(door_texture, door_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string rsz_1lift_door = "rsz_1lift_door.png";
    unsigned int rsz_1lift_door_texture = loadTexture(rsz_1lift_door.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube_for_door rsz_1lift_door_cube = Cube_for_door(rsz_1lift_door_texture, rsz_1lift_door_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    //--------------------Ground Floor End---------------------------
    // 
    // 
    // 
    // 
    // 
    // 
    //--------------------First Floor -------------------------------
    string fan_tex = "fan_tex.png";
    unsigned int fan_texture = loadTexture(fan_tex.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube fan_cube = Cube(fan_texture, fan_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string wall_texture_1 = "wall-texture-1.png";
    unsigned int wall_texture_1_texture = loadTexture(wall_texture_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall_texture_1_cube = Cube(wall_texture_1_texture, wall_texture_1_texture, 32.0f, 0.0f, 0.0f, 4.0f, 4.0f);

    string wall_texture_2 = "wall-texture-2.png";
    unsigned int wall_texture_2_texture = loadTexture(wall_texture_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall_texture_2_cube = Cube(wall_texture_2_texture, wall_texture_2_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string wall_texture_2_1 = "wall-texture-2.1.png";
    unsigned int wall_texture_2_1_texture = loadTexture(wall_texture_2_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall_texture_2_1_cube = Cube(wall_texture_2_1_texture, wall_texture_2_1_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string wall_texture_3 = "wall-texture-9.png";
    unsigned int wall_texture_3_texture = loadTexture(wall_texture_3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall_texture_3_cube = Cube(wall_texture_3_texture, wall_texture_3_texture, 32.0f, 0.0f, 0.0f, 4.0f, 4.0f);

    string guestroom_bed_sheet = "guestroom_bed_sheet_2.png";
    unsigned int guestroom_bed_sheet_texture = loadTexture(guestroom_bed_sheet.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube guestroom_bed_sheet_cube = Cube(guestroom_bed_sheet_texture, guestroom_bed_sheet_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string guest_room_bed = "guest_room_bed.png";
    unsigned int guest_room_bed_texture = loadTexture(guest_room_bed.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube guest_room_bed_cube = Cube(guest_room_bed_texture, guest_room_bed_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string guest_room_bed_low = "guest_room_bed_low_1.png";
    unsigned int guest_room_bed_low_texture = loadTexture(guest_room_bed_low.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube guest_room_bed_low_cube = Cube(guest_room_bed_low_texture, guest_room_bed_low_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string guest_room_floor_mat = "guest_room_floor_mat.png";
    unsigned int guest_room_floor_mat_texture = loadTexture(guest_room_floor_mat.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string guest_room_floor_tile_2 = "guest_room_floor_tile_1.png";
    unsigned int guest_room_floor_tile_2_texture = loadTexture(guest_room_floor_tile_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube guest_room_floor_tile_2_cube = Cube(guest_room_floor_tile_2_texture, guest_room_floor_tile_2_texture, 32.0f, 0.0f, 0.0f, 7.0f, 7.0f);

    string guest_room_varand_floor_tile_2 = "guest_room_floor_tile_1.png";
    unsigned int guest_room_varand_floor_tile_2_texture = loadTexture(guest_room_varand_floor_tile_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube guest_room_varand_floor_tile_2_cube = Cube(guest_room_varand_floor_tile_2_texture, guest_room_varand_floor_tile_2_texture, 32.0f, 0.0f, 0.0f, 2.0f, 4.0f);

    string guest_room_bedside = "guest_room_bedside.png";
    unsigned int guest_room_bedside_texture = loadTexture(guest_room_bedside.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube guest_room_bedside_cube = Cube(guest_room_bedside_texture, guest_room_bedside_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string kitchen_cabinet_1 = "kitchen_cabinet_1.png";
    unsigned int kitchen_cabinet_1_texture = loadTexture(kitchen_cabinet_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kitchen_cabinet_1_cube = Cube(kitchen_cabinet_1_texture, kitchen_cabinet_1_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string kitchen_cabinet_2 = "kitchen_cabinet_2.png";
    unsigned int kitchen_cabinet_2_texture = loadTexture(kitchen_cabinet_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kitchen_cabinet_2_cube = Cube(kitchen_cabinet_2_texture, kitchen_cabinet_2_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string kitchen_cabinet_3 = "kitchen_cabinet_3.png";
    unsigned int kitchen_cabinet_3_texture = loadTexture(kitchen_cabinet_3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kitchen_cabinet_3_cube = Cube(kitchen_cabinet_3_texture, kitchen_cabinet_3_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string kitchen_cabinet_4 = "kitchen_cabinet_4.png";
    unsigned int kitchen_cabinet_4_texture = loadTexture(kitchen_cabinet_4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kitchen_cabinet_4_cube = Cube(kitchen_cabinet_4_texture, kitchen_cabinet_4_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string kitchen_cabinet_tile_wall = "kitchen_cabinet_tile_wall.png";
    unsigned int kitchen_cabinet_tile_wall_texture = loadTexture(kitchen_cabinet_tile_wall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kitchen_cabinet_tile_wall_cube = Cube(kitchen_cabinet_tile_wall_texture, kitchen_cabinet_tile_wall_texture, 32.0f, 0.0f, 0.0f, 3.0f, 3.0f);

    string kitchen_cabinet_tile = "kitchen_cabinet_tile.png";
    unsigned int kitchen_cabinet_tile_texture = loadTexture(kitchen_cabinet_tile.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kitchen_cabinet_tile_cube = Cube(kitchen_cabinet_tile_texture, kitchen_cabinet_tile_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string kitchen_cabinet_tile_2 = "kitchen_cabinet_tile.png";
    unsigned int kitchen_cabinet_tile_2_texture = loadTexture(kitchen_cabinet_tile_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kitchen_cabinet_tile_2_cube = Cube(kitchen_cabinet_tile_2_texture, kitchen_cabinet_tile_2_texture, 32.0f, 0.0f, 0.0f, 1.0f, 3.0f);

    string kitchen_cabinet_tile_3 = "kitchen_cabinet_tile.png";
    unsigned int kitchen_cabinet_tile_3_texture = loadTexture(kitchen_cabinet_tile_3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kitchen_cabinet_tile_3_cube = Cube(kitchen_cabinet_tile_3_texture, kitchen_cabinet_tile_3_texture, 32.0f, 0.0f, 0.0f, 3.0f, 1.0f);

    string kitchen_cabinet_tile_4 = "kitchen_cabinet_tile_4.png";
    unsigned int kitchen_cabinet_tile_4_texture = loadTexture(kitchen_cabinet_tile_4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kitchen_cabinet_tile_4_cube = Cube(kitchen_cabinet_tile_4_texture, kitchen_cabinet_tile_4_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string frezz_1 = "frezz_1.png";
    unsigned int frezz_1_texture = loadTexture(frezz_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube frezz_1_cube = Cube(frezz_1_texture, frezz_1_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string freeze_door_1 = "freeze_door_1.png";
    unsigned int freeze_door_1_texture = loadTexture(freeze_door_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube freeze_door_1_cube = Cube(freeze_door_1_texture, freeze_door_1_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string freeze_door_2 = "freeze_door_2.png";
    unsigned int freeze_door_2_texture = loadTexture(freeze_door_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube freeze_door_2_cube = Cube(freeze_door_2_texture, freeze_door_2_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string first_floor_tile = "first_floor_tile_1.png";
    unsigned int first_floor_tile_texture = loadTexture(first_floor_tile.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube first_floor_tile_cube = Cube(first_floor_tile_texture, first_floor_tile_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string first_floor_tile_1 = "first_floor_tile.png";
    unsigned int first_floor_tile_1_texture = loadTexture(first_floor_tile_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube first_floor_tile_1_cube = Cube(first_floor_tile_1_texture, first_floor_tile_1_texture, 32.0f, 0.0f, 0.0f, 3.0f, 2.0f);

    string kitchen_basin = "kitchen_basin_1.png";
    unsigned int kitchen_basin_texture = loadTexture(kitchen_basin.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string sofa = "sofa_4.png";
    unsigned int sofa_texture = loadTexture(sofa.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube sofa_cube = Cube(sofa_texture, sofa_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string sofa_2 = "sofa_4.png";
    unsigned int sofa_2texture = loadTexture(sofa_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube sofa_cube_2 = Cube(sofa_2texture, sofa_2texture, 32.0f, 0.0f, 0.0f, 1.0f, 3.0f);

    string sofa_3 = "sofa_4.png";
    unsigned int sofa_3texture = loadTexture(sofa_3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube sofa_cube_3 = Cube(sofa_3texture, sofa_3texture, 32.0f, 0.0f, 0.0f, 3.0f, 1.0f);

    string dinning = "dinning.png";
    unsigned int dinning_texture = loadTexture(dinning.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string tea_table = "tea_table.png";
    unsigned int tea_table_texture = loadTexture(tea_table.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string chandeliers_1 = "chandeliers_5.png";
    unsigned int chandeliers_1_texture = loadTexture(chandeliers_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //--------------------First Floor End -------------------------------





    //--------------------Second Floor Start -------------------------------

    string room_door = "rsz_1room_door.png";
    unsigned int room_door_texture = loadTexture(room_door.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube_for_door room_door_cube = Cube_for_door(room_door_texture, room_door_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    string leftroom_mat = "leftroom_mat.png";
    unsigned int leftroom_mat_texture = loadTexture(leftroom_mat.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    string rsz_leftroom_bedsheet = "rsz_leftroom_bedsheet.png";
    unsigned int rsz_leftroom_bedsheet_texture = loadTexture(rsz_leftroom_bedsheet.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube rsz_leftroom_bedsheet_cube = Cube(rsz_leftroom_bedsheet_texture, rsz_leftroom_bedsheet_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string leftroom_bed_back_wall = "leftroom_bed_back_wall.png";
    unsigned int leftroom_bed_back_wall_texture = loadTexture(leftroom_bed_back_wall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube leftroom_bed_back_wall_cube = Cube(leftroom_bed_back_wall_texture, leftroom_bed_back_wall_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string leftroom_bedBack = "leftroom_bedBack.png";
    unsigned int leftroom_bedBack_texture = loadTexture(leftroom_bedBack.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube leftroom_bedBack_cube = Cube(leftroom_bedBack_texture, leftroom_bedBack_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string leftroom_bedside = "leftroom_bedside.png";
    unsigned int leftroom_bedside_texture = loadTexture(leftroom_bedside.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube leftroom_bedside_cube = Cube(leftroom_bedside_texture, leftroom_bedside_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string master_bed_room_tile = "master_bed_room_tile.png";
    unsigned int master_bed_room_tile_texture = loadTexture(master_bed_room_tile.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube master_bed_room_tile_cube = Cube(master_bed_room_tile_texture, master_bed_room_tile_texture, 32.0f, 0.0f, 0.0f, 3.0f, 3.0f);

    string master_bed_room_tile_1 = "master_bed_room_tile_2.png";
    unsigned int master_bed_room_tile_1_texture = loadTexture(master_bed_room_tile_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube master_bed_room_tile_1_cube = Cube(master_bed_room_tile_1_texture, master_bed_room_tile_1_texture, 32.0f, 0.0f, 0.0f, 3.0f, 3.0f);

    string bathroom_floor = "Bathroom_floor_2.png";
    unsigned int bathroom_floor_texture = loadTexture(bathroom_floor.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bathroom_floor_cube = Cube(bathroom_floor_texture, bathroom_floor_texture, 32.0f, 0.0f, 0.0f, 2.0f, 2.0f);

    string bathroom_floor_3 = "Bathroom_floor_5.png";
    unsigned int bathroom_floor_3_texture = loadTexture(bathroom_floor_3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bathroom_floor_3_cube = Cube(bathroom_floor_3_texture, bathroom_floor_3_texture, 32.0f, 0.0f, 0.0f, 3.0f, 2.0f);

    string corridor = "corridor.png";
    unsigned int corridor_tile_texture = loadTexture(corridor.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube corridor_cube = Cube(corridor_tile_texture, corridor_tile_texture, 32.0f, 0.0f, 0.0f, 8.0f, 1.0f);

    string bathroom_wall = "Bathroom_wall_5.png";
    unsigned int bathroom_wall_texture = loadTexture(bathroom_wall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bathroom_wall_cube = Cube(bathroom_wall_texture, bathroom_wall_texture, 32.0f, 0.0f, 0.0f, 4.0f, 4.0f);

    string Bathroom_wall_5 = "Bathroom_wall_5.png";
    unsigned int Bathroom_wall_5_texture = loadTexture(Bathroom_wall_5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube Bathroom_wall_5_cube = Cube(Bathroom_wall_5_texture, Bathroom_wall_5_texture, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    string Bathroom_wall_6 = "Bathroom_wall_7.png";
    unsigned int Bathroom_wall_6_texture = loadTexture(Bathroom_wall_6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube Bathroom_wall_6_cube = Cube(Bathroom_wall_6_texture, Bathroom_wall_6_texture, 32.0f, 0.0f, 0.0f, 4.0f, 4.0f);

    string Bathroom_wall_6_1 = "Bathroom_wall_7.png";
    unsigned int Bathroom_wall_6_1_texture = loadTexture(Bathroom_wall_6_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube Bathroom_wall_6_1_cube = Cube(Bathroom_wall_6_1_texture, Bathroom_wall_6_1_texture, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    string master_bed_room_wall_2 = "master_bed_room_wall_5.png";
    unsigned int master_bed_room_wall_2_texture = loadTexture(master_bed_room_wall_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube master_bed_room_wall_2_cube = Cube(master_bed_room_wall_2_texture, master_bed_room_wall_2_texture, 32.0f, 0.0f, 0.0f, 3.0f, 1.0f);

    string master_bed_room_wall_2_1 = "master_bed_room_wall_5_1.png";
    unsigned int master_bed_room_wall_2_1_texture = loadTexture(master_bed_room_wall_2_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube master_bed_room_wall_2_1_cube = Cube(master_bed_room_wall_2_1_texture, master_bed_room_wall_2_1_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string master_bed_room_wall_1 = "master_bed_room_wall_1.png";
    unsigned int master_bed_room_wall_1_texture = loadTexture(master_bed_room_wall_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube master_bed_room_wall_1_cube = Cube(master_bed_room_wall_1_texture, master_bed_room_wall_1_texture, 32.0f, 0.0f, 0.0f, 3.0f, 1.0f);

    string master_bed_room_wall_1_1 = "master_bed_room_wall_1_1.png";
    unsigned int master_bed_room_wall_1_1_texture = loadTexture(master_bed_room_wall_1_1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube master_bed_room_wall_1_1_cube = Cube(master_bed_room_wall_1_1_texture, master_bed_room_wall_1_1_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string door_left = "rsz_door_left.png";
    unsigned int door_left_texture = loadTexture(door_left.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube_for_door door_left_cube = Cube_for_door(door_left_texture, door_left_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string door_right = "rsz_door_right.png";
    unsigned int door_right_texture = loadTexture(door_right.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube_for_door door_right_cube = Cube_for_door(door_right_texture, door_right_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //--------------------Second Floor End -------------------------------



    //--------------------Roof Start -------------------------------
    string roof_grass = "roof_grass.png";
    unsigned int roof_grass_texture = loadTexture(roof_grass.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube roof_grass_cube = Cube(roof_grass_texture, roof_grass_texture, 32.0f, 0.0f, 0.0f, 6.0f, 6.0f);

    string roof_grass1 = "roof_grass.png";
    unsigned int roof_grass1_texture = loadTexture(roof_grass1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube roof_grass1_cube = Cube(roof_grass1_texture, roof_grass1_texture, 32.0f, 0.0f, 0.0f, 5.0f, 3.0f);

    Prism roof_wall_triangle = Prism(wall_texture_1_texture, wall_texture_1_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube roof_tile_cube = Cube(guest_room_floor_tile_2_texture, guest_room_floor_tile_2_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube roof_plant_base_cube_1 = Cube(guest_room_floor_tile_2_texture, guest_room_floor_tile_2_texture, 32.0f, 0.0f, 0.0f, 9.0f, 1.0f);

    Cube roof_plant_base_cube_2 = Cube(guest_room_floor_tile_2_texture, guest_room_floor_tile_2_texture, 32.0f, 0.0f, 0.0f, 10.0f, 1.0f);

    string roof_platform = "roof_platform.png";
    unsigned int roof_platform_texture = loadTexture(roof_platform.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube roof_platform_cube = Cube(roof_platform_texture, roof_platform_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string swing = "swing.png";
    unsigned int swing_texture = loadTexture(swing.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube swing_cube = Cube(swing_texture, swing_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string roof_plant = "roof_plant.png";
    unsigned int roof_plant_texture = loadTexture(roof_plant.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube roof_plant_cube = Cube(roof_plant_texture, roof_plant_texture, 32.0f, 0.0f, 0.0f, 18.0f, 1.0f);

    Cube roof_plant_cube_1 = Cube(roof_plant_texture, roof_plant_texture, 32.0f, 0.0f, 0.0f, 1.0f, 18.0f);

    //--------------------Roof End -------------------------------
        //car
    string car = "car.png";
    unsigned int car_texture = loadTexture(car.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Prismforcar car_prism = Prismforcar(car_texture, car_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string car_2 = "car_2.png";
    unsigned int car_2_texture = loadTexture(car_2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube car_2_cube = Cube(car_2_texture, car_2_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string car_tire = "car_tire.png";
    unsigned int car_tire_texture = loadTexture(car_tire.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //car end

    string cone_bulb = "Bulb_tex.png";
    unsigned int cone_bulb_tex = loadTexture(cone_bulb.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    string sphere_bulb = "Bulb_tex4.png";
    unsigned int sphere_bulb_tex = loadTexture(sphere_bulb.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    load_texture(glass, "glass-png-3.png", GL_RGBA);//used
    load_texture(glass2, "glass-png-2.png", GL_RGBA);//used
    load_texture(hanging_plant, "hanging_plant_1.png", GL_RGBA);//used
    load_texture(clock_bg, "clock_bg.png", GL_RGBA);//used
    load_texture(collapsible_gate, "collapsible gate.png", GL_RGBA);//used

    SphereTex spheretex = SphereTex();
    CylinderTex cylindertex = CylinderTex();
    ConeTex conetex = ConeTex();
    HemiSphereTex hemispheretex = HemiSphereTex();
    TorusTex torustex = TorusTex(1,2.0f,0.1f);
    TorusTex torustex1 = TorusTex(1, 1.5f, 0.1f);
    TorusTex torustex2 = TorusTex(1, 1.0f, 0.1f);
    TorusTex torustex3 = TorusTex(1, 0.5f, 0.1f);

    Torus torus_varanda = Torus(2, 2.5f, 0.05f,1.0f, 36, 18, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

    Sphere sphere = Sphere();
    Cylinder cylinder = Cylinder();
    Cone cone = Cone();
    CutCone cutcone = CutCone();
    Torus torus = Torus(4,2.0f,0.1f);
    Torus torus1 = Torus(1, 1.0f, 0.05f);
    HemiSphere hemisphere = HemiSphere();
    DiscTex disctex = DiscTex();
    HalfDiscTex halfdisctex = HalfDiscTex();

    BezierCurve pitcher = BezierCurve(cp_pitcher, 9 * 3, fountain_4_tex);

    BezierCurve fountain_secondary_base = BezierCurve(cp_fountain_secondary_base, 11 * 3, first_floor_tile_texture);

    BezierCurve fountain_primary_base = BezierCurve(cp_fountain_primary_base, 14 * 3, first_floor_tile_texture);

    BezierCurve ralling_bez = BezierCurve(cp_ralling, 6 * 3, fountain_5_tex);

    BezierCurve tree = BezierCurve(cp_tree_base, 11 * 3, tree_bark_tex);

    BezierCurve tree_brantch = BezierCurve(cp_tree_brantch, 6 * 3, tree_bark_tex);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Setup framebuffers
    unsigned int framebuffer, texture;
    framebuffer = createFramebuffer(texture, SCR_WIDTH, SCR_HEIGHT);

    // Projection matrix
    //glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);


    //ourShader.use();
    //lightingShader.use();
    TransparentShader.use();
    TransparentShader.setInt("texture1", 0);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        //------------------for mirror-----------------
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Fixed mirror position and normal in world space
        glm::vec3 mirrorPosition = glm::vec3(29.0f, 25.8f, 40.8f); // Example: At y=1, z=-5
        glm::vec3 mirrorNormal = glm::vec3(0.0f, 0.0f, 1.0f);    // Facing along positive z-axis

        // Calculate mirrored camera position
        glm::vec3 eye = camera.Position; // Current camera position
        glm::vec3 forward = camera.Front; // Camera forward direction


        // Compute mirrored position along the y-axis
        glm::vec3 mirroredPosition = glm::vec3(29.0f, 25.8f, 40.8f); // Flip x and z components

        // Compute mirrored forward direction along the y-axis
        glm::vec3 mirroredForward = glm::vec3(forward.x, forward.y, -forward.z); // Flip x and z components

        // Mirrored view matrix
        glm::mat4 mirroredView = glm::lookAt(mirroredPosition,mirroredPosition - mirroredForward, camera.WorldUp);


        glm::mat4 projection = myPerspective(
            glm::radians(camera.Zoom),         // fov: field of view in radians
            (float)SCR_WIDTH / (float)SCR_HEIGHT, // aspect: aspect ratio
            0.1f,                              // near: near clipping plane
            200.0f                             // far: far clipping plane
        );
        //-----------------Normal Shader--------------------------------
        lightingShader.use();
        lightingShader.setVec3("viewPos", mirroredPosition);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", mirroredView);


        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);
        pointlight7.setUpPointLight(lightingShader);
        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);
        pointlight10.setUpPointLight(lightingShader);

        //--------------------directional Light---------
        lightingShader.setVec3("directionalLight.directiaon", 0.5f, -3.0f, -3.0f);
        lightingShader.setVec3("directionalLight.ambient", .5f, .5f, .5f);
        lightingShader.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setBool("directionalLightON", directionalLightOn);

        //spot light
        spotlight1.setUpspotLight(lightingShader);

        // lighting shader -----------------------------------
        //commode left
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(38.0, 20.8, 47.5));
        model = glm::scale(model, glm::vec3(2.0, 2.0, 1.5));
        cutcone.drawCutCone(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(38.0, 22.2, 47.5));
        model = glm::scale(model, glm::vec3(2.0, 0.2, 1.5));
        cutcone.drawCutCone(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(39.0, 22.2, 46.3));
        model = glm::scale(model, glm::vec3(0.5, 2.0, 2.5));
        drawCube(cubeVAO, lightingShader, model, 0.122, 0.361, 0.357, 32.0);

        //basin
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(29.5f, 25.3f, 41.2f));
        model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5));
        hemisphere.drawHemiSphere(lightingShader, model);
        
        // lighting shader with texture----------------------------------------

        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", mirroredPosition);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", mirroredView);

        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);
        pointlight10.setUpPointLight(lightingShaderWithTexture);

        //directional light
        lightingShaderWithTexture.setVec3("directionalLight.directiaon", 0.5f, -3.0f, -3.0f);
        lightingShaderWithTexture.setVec3("directionalLight.ambient", .5f, .5f, .5f);
        lightingShaderWithTexture.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShaderWithTexture.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShaderWithTexture.setBool("directionLightOn", directionalLightOn);

        //spot light
        spotlight1.setUpspotLight(lightingShaderWithTexture);

        //object draw
        //left bathroom
        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 20.7f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.1f, 10.0f));
        bathroom_floor_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        //2
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.1f, 20.7f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(19.9f, 10.0f, -0.1f));
        bathroom_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.1f, 20.7f, 49.9f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 10.0f, -9.9f));
        bathroom_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 20.7f, 49.9f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.1f, 5.0f, -9.9f));
        bathroom_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(25.1f, 20.7f, 40.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.9f, 10.0f, -0.1f));
        bathroom_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.1f, 27.7f, 40.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 3.0f, -0.1f));
        Bathroom_wall_5_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom cabinet left
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(28.0f, 20.8f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 3.0f, 2.0f));
        bathroom_cabinet_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //basin point
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(29.5f, 23.9f, 41.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1, 1.0, 0.1));
        disctex.drawCylinder(lightingShaderWithTexture, basin_point_texture, modelMatrixForContainer);

        //bathroom floor 2
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0f, 20.8f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 0.05f, 5.0f));
        bathroom_floor_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //ceilling
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 30.5, 16.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.0f, 0.2f, 34.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //side wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 20.7f, 35.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 15.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //left room bathroom door
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(25.0f, 20.8f, 39.9f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 7.0f, 0.1f));
        room_door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[4]);
        model = glm::scale(model, glm::vec3(0.2f));
        conetex.drawCone(lightingShaderWithTexture, cone_bulb_tex, model);

        //-----------------------texture shader end---------------------------------
        //-----------------------transparent shader------------------------------------

        TransparentShader.use();
        //glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 view2 = camera.GetViewMatrix();
        TransparentShader.setMat4("projection", projection);
        TransparentShader.setMat4("view", mirroredView);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //left bathroom
        float x_shift = 34.995;
        float y_shift = 20.8;
        float z_shift = -43.0f;
        Glass_window_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 90.0f);

        x_shift = 34.995;
        y_shift = 20.8;
        z_shift = -50.0f;
        Glass_window_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 90.0f);

        x_shift = 37.0;
        y_shift = 20.8;
        z_shift = -45.005f;
        Glass_window_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 0.0f);


        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //----------------------mirror part end-----------------------




        // Step 2: Render the scene normally
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);
        pointlight7.setUpPointLight(lightingShader);
        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);
        pointlight10.setUpPointLight(lightingShader);

        lightingShader.setVec3("directionalLight.directiaon", 0.5f, -3.0f, -3.0f);
        lightingShader.setVec3("directionalLight.ambient", .5f, .5f, .5f);
        lightingShader.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);


        lightingShader.setBool("directionalLightON", directionalLightOn);

        spotlight1.setUpspotLight(lightingShader);

        // activate shader
        //lightingShader.use();

        //lightingShader.setVec3("directionalLight.direction", 0.5f, -3.0f, -3.0f);
        //if (AmbientON) {
        //    lightingShader.setVec3("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
        //    lightingShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        //}
        //if (DiffusionON) {
        //    lightingShader.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
        //    lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        //}
        //if (SpecularON) {

        //    lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        //    lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        //}

        //lightingShader.setBool("directionalLightON", directionalLightOn);
        //lightingShader.setBool("SpotLightON", SpotLightOn);

        //lightingShader.setVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
        //lightingShader.setVec3("spotLight.position", -3.0f, 4.0f, 4.0f);



        //lightingShader.setFloat("spotLight.k_c", 1.0f);
        //lightingShader.setFloat("spotLight.k_l", 0.09f);
        //lightingShader.setFloat("spotLight.k_q", 0.032f);
        //lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(35.5f)));
        //lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(40.5f)));

        // pass projection matrix to shader (note that in this case it could change every frame)
       // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);

        // Modelling Transformation
        // make sure to initialize matrix to identity matrix first

        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //bed(cubeVAO, lightingShader, model);
        //draw floor
        //floor(cubeVAO, lightingShader);
        

        //-------------------------BezierCurve---------------------------------

        glm::mat4 Piller_BordermodelFortorus = glm::mat4(1.0f);
        Piller_BordermodelFortorus = glm::translate(Piller_BordermodelFortorus, glm::vec3(4.0f, -0.8f, -2.0f));
        Piller_BordermodelFortorus = glm::scale(Piller_BordermodelFortorus, glm::vec3(0.5f, 0.5f, 0.5f));
        //Piller_Border.drawBezierCurve(lightingShader, Piller_BordermodelFortorus);
        //--------------------------------BezierCurve end-----------------------
        //axis(cubeVAO, lightingShader);
        //frontWall(cubeVAO, lightingShader, cylinder, hemisphere, cone);
        //rightWall(cubeVAO, lightingShader, cutcone);
        glm::mat4 modelForSphere = glm::mat4(1.0f);
        rotateYMatrix = glm::rotate(modelForSphere, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelForSphere = glm::translate(rotateYMatrix, glm::vec3(0.0f, 0.0f, -4.0f));
        //sphere.drawSphere(lightingShader, modelForSphere);

        glm::mat4 modelForHemiSphere = glm::mat4(1.0f);
        modelForHemiSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
        //hemisphere.drawHemiSphere(lightingShader, modelForHemiSphere);

        glm::mat4 modelForCylinder = glm::mat4(1.0f);
        modelForCylinder = glm::translate(modelForCylinder, glm::vec3(0.0f, 0.0f, 0.0f));
        modelForCylinder = glm::rotate(modelForCylinder, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelForCylinder = glm::scale(modelForCylinder, glm::vec3(0.05f, 2.0f, 2.0f));
        //cylinder.drawCylinder(lightingShader, modelForCylinder);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 10.0f, -10.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.2f, 10.0f, 0.2f));
        //drawCube(cubeVAO, lightingShader, model, 0.212, 0.067, 0.031, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 10.0f, -10.0f));
        model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.2f, 10.0f, 0.2f));
        //drawCube(cubeVAO, lightingShader, model, 0.212, 0.067, 0.031, 32.0);

        glm::mat4 modelForCone = glm::mat4(1.0f);
        modelForCone = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
        //cone.drawCone(lightingShader, modelForCone);

        glm::mat4 modelForCutCone = glm::mat4(1.0f);
        translateMatrix = glm::translate(model, glm::vec3(-5.5f, 1.2f, 0.5f));
        modelForCutCone = glm::scale(translateMatrix, glm::vec3(1.0f, 1.0f, -2.0f));
        //cutcone.drawCutCone(lightingShader, modelForCutCone);

        glm::mat4 modelFortorus = glm::mat4(1.0f);
        modelFortorus = glm::translate(model, glm::vec3(4.5f, 1.2f, 0.5f));
        //torus.drawTorus(lightingShader, modelFortorus);

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);


        // Inside the render loop:
        float currentTime_2 = glfwGetTime(); // Get the current time

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && currentTime_2 - lastKeyPressTime > keyCooldown) {
            lastKeyPressTime = currentTime_2; // Update the last key press time

            isSwinging = !isSwinging; // Toggle the swing state
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && currentTime_2 - lastKeyPressTime > keyCooldown) {
            lastKeyPressTime = currentTime_2; // Update the last key press time

            isFanON = !isFanON; // Toggle the swing state
        }

        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && currentTime_2 - lastKeyPressTime > keyCooldown) {
            lastKeyPressTime = currentTime_2; // Update the last key press time

            isLiftON = !isLiftON; // Toggle the swing state
        }

        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && currentTime_2 - lastKeyPressTime > keyCooldown) {
            lastKeyPressTime = currentTime_2; // Update the last key press time

            isLiftDoorOpenDown = !isLiftDoorOpenDown; // Toggle the swing state
        }

        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && currentTime_2 - lastKeyPressTime > keyCooldown) {
            lastKeyPressTime = currentTime_2; // Update the last key press time

            isLiftDoorOpenUp = !isLiftDoorOpenUp; // Toggle the swing state
        }

        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && currentTime_2 - lastKeyPressTime > keyCooldown) {
            lastKeyPressTime = currentTime_2; // Update the last key press time

            glass_gateOpen = !glass_gateOpen; // Toggle the swing state
        }

        // Update swing velocity
        if (isSwinging) {
            // Accelerate towards max velocity
            if (swingVelocity < maxSwingVelocity) {
                swingVelocity += damping;
            }
        }
        else {
            // Decelerate to stop
            if (swingVelocity > 0.0f) {
                swingVelocity -= damping;
            }
            else {
                swingVelocity = 0.0f; // Stop completely when velocity is zero
            }
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {
            lastKeyPressTime = currentTime_2; // Update the last key press time
            if (!isTranslatingX && !isRotatingX) { // Start translating only if idle
                isTranslatingX = true;
            }
        }


        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && currentTime_2 - lastKeyPressTime > keyCooldown) {
            lastKeyPressTime = currentTime_2; // Update the last key press time
            if (!isFreezeOpen) {
                isFreezeOpen = true;         // Activate the door
                isOpening = (FreezeAngle == 0.0f); // Determine if it should open or close
            }
            else {
                isOpening = !isOpening;      // Toggle between opening and closing
            }
        }

        

        //----------------------------Ground Floor Design for Lighting Shader--------------------

        //bath room basin
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(12.0f, 4.6f, 45.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        hemisphere.drawHemiSphere(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(12.0f, 1.6f, 45.0f));
        model = glm::rotate(model,glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.1f));
        cylinder.drawCylinder(lightingShader, model);

        //walls




        //bed
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.2f, 2.6f, 40.2f));
        model = glm::scale(model, glm::vec3(4.6f, 0.5f, 7.0f));
        drawCube(cubeVAO, lightingShader, model, 0.212, 0.067, 0.031, 32.0);

        //almirah
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(21.0f, 0.6f, 46.0f));
        model = glm::scale(model, glm::vec3(5.0f, 7.0f, 3.0f));
        drawCube(cubeVAO, lightingShader, model, 0.976, 0.91, 0.725, 32.0);

        //table and laptop
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(36.0f, 2.85f, 46.01f));
        model = glm::scale(model, glm::vec3(1.0f, 0.7f, 0.01f));
        drawCube(cubeVAO, lightingShader, model, 0.153, 0.153, 0.153, 32.0);





        //glm::mat4 shearMatrix = createShearMatrix(sx_y, sx_z, sy_x, sy_z, sz_x, sz_y);

        //----------------------------Ground Floor Design End for Lighting Shader--------------------
        
        //----------------------------1st Floor Design start for Lighting Shader--------------------
        //freeze
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f, 10.7f, 28.0f));
        model = glm::scale(model, glm::vec3(3.0f, 6.0f, 5.0f));
        drawCube(cubeVAO, lightingShader, model, 0.157, 0.145, 0.125, 32.0);

        if (isFreezeOpen) {
            if (isOpening) {
                if (FreezeAngle < freezeThreshold) {
                    FreezeAngle += freezeStep; // Open the door
                }
                else {
                    isFreezeOpen = false;      // Stop when fully opened
                }
            }
            else {
                if (FreezeAngle > 0.0f) {
                    FreezeAngle -= freezeStep; // Close the door
                }
                else {
                    isFreezeOpen = false;      // Stop when fully closed
                }
            }
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.516f, 10.7f, 28.0f));
        model = glm::rotate(model, glm::radians(FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.008f, 6.0f, 2.9f));
        drawCube(cubeVAO, lightingShader, model, 0.937, 0.937, 0.937, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.51696f, 10.7f, 30.9f));
        model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.1f));
        model = glm::rotate(model, glm::radians(-FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(-0.008f, 6.0f, 2.1f));
        drawCube(cubeVAO, lightingShader, model, 0.937, 0.937, 0.937, 32.0);


        //kitchen cabin
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 17.5f, 40.0f));
        model = glm::scale(model, glm::vec3(2.0f, 3.2f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.886, 0.886, 0.886, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 10.7f, 40.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.294, 0.29, 0.31, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 17.5f, 48.0f));
        model = glm::scale(model, glm::vec3(8.0f, 3.2f, 2.0f));
        drawCube(cubeVAO, lightingShader, model, 0.886, 0.886, 0.886, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 10.7f, 47.0f));
        model = glm::scale(model, glm::vec3(8.0f, 3.0f, 3.0f));
        drawCube(cubeVAO, lightingShader, model, 0.294, 0.29, 0.31, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(7.0f, 10.7f, 33.5f));
        model = glm::scale(model, glm::vec3(4.0f, 3.0f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.294, 0.29, 0.31, 32.0);

        //guest room bedside table
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(26.0f, 10.8f, 48.7f));
        model = glm::scale(model, glm::vec3(3.0f, 2.0f, -2.0f));
        drawCube(cubeVAO, lightingShader, model, 0.761, 0.659, 0.561, 32.0);

        //guest room varanda
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 10.72f, 43.2f));
        model = glm::scale(model, glm::vec3(0.1f, 3.0f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 10.72f, 33.0f));
        model = glm::scale(model, glm::vec3(0.1f, 3.0f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 13.72f, 32.9f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.4f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 13.22f, 33.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.2f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 12.72f, 33.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.2f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 12.22f, 33.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.2f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 11.72f, 33.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.2f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 11.22f, 33.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.2f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 13.72f, 32.9f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 13.22f, 32.9f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 12.72f, 32.9f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 12.22f, 32.9f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 11.72f, 32.9f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 11.22f, 32.9f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 13.72f, 43.3f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 13.22f, 43.3f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 12.72f, 43.3f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 12.22f, 43.3f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 11.72f, 43.3f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(44.9f, 11.22f, 43.3f));
        model = glm::scale(model, glm::vec3(-4.7f, 0.1f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        //sofa
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.0f, 10.7f, 10.0f));
        model = glm::scale(model, glm::vec3(0.7f, 3.0f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.357, 0.286, 0.259, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.7f, 10.7f, 19.3f));
        model = glm::scale(model, glm::vec3(5.0f, 3.0f, 0.7f));
        drawCube(cubeVAO, lightingShader, model, 0.357, 0.286, 0.259, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.7f, 13.7f, 10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 9.3f));
        drawCube(cubeVAO, lightingShader, model, 0.153, 0.098, 0.094, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.7f, 13.7f, 19.3f));
        model = glm::scale(model, glm::vec3(5.0f, 0.1f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.153, 0.098, 0.094, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.7f, 10.7f, 10.0f));
        model = glm::scale(model, glm::vec3(2.0f, 1.5f, 9.3f));
        drawCube(cubeVAO, lightingShader, model, 0.357, 0.286, 0.259, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(22.7f, 10.7f, 17.3f));
        model = glm::scale(model, glm::vec3(3.0f, 1.5f, 2.0f));
        drawCube(cubeVAO, lightingShader, model, 0.357, 0.286, 0.259, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(22.7f, 12.2f, 10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 7.3f));
        drawCube(cubeVAO, lightingShader, model, 0.153, 0.098, 0.094, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(22.7f, 12.2f, 17.3f));
        model = glm::scale(model, glm::vec3(3.0f, 0.1f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.153, 0.098, 0.094, 32.0);

        //dinning table
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(19.0f, 10.7f, 42.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
        torus.drawTorus(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(15.0f, 10.7f, 42.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
        torus.drawTorus(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(17.0f, 10.7f, 44.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
        torus.drawTorus(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(17.0f, 10.7f, 40.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
        torus.drawTorus(lightingShader, model);

        //Big varanda
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(15.0f, 10.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 3.0f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.0f, 10.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 3.0f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(25.0f, 10.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 3.0f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(30.0f, 10.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 3.0f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(35.0f, 10.7f, -10.0f));
        model = glm::scale(model, glm::vec3(-0.1f, 3.0f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);
        //
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(14.8f, 13.7f, -10.0f));
        model = glm::scale(model, glm::vec3(20.4f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(14.8f, 12.7f, -10.0f));
        model = glm::scale(model, glm::vec3(20.4f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(14.8f, 11.7f, -10.0f));
        model = glm::scale(model, glm::vec3(20.4f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);
        //
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(14.8f, 13.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(14.8f, 12.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(14.8f, 11.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);
        //
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(35.2f, 13.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(35.2f, 12.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(35.2f, 11.7f, -10.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 10.0f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        //tea table
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(28.0f, 10.9f, 13.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        torus1.drawTorus(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(26.0f, 10.9f, 13.0f));
        model = glm::scale(model, glm::vec3(0.05f, 1.5f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(30.0f, 10.9f, 13.0f));
        model = glm::scale(model, glm::vec3(0.05f, 1.5f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(28.0f, 10.9f, 15.0f));
        model = glm::scale(model, glm::vec3(0.05f, 1.5f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(28.0f, 10.9f, 11.0f));
        model = glm::scale(model, glm::vec3(0.05f, 1.5f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        //clock
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 14.9f, 8.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        sphere.drawSphere(lightingShader, model);

        float hours, minutes, seconds;
        getTime(hours, minutes, seconds);

        // Calculate angles based on system time
        float secondAngle = -360.0f * (seconds / 60.0f);               // Full rotation in 60 seconds
        float minuteAngle = -360.0f * ((minutes + seconds / 60.0f) / 60.0f); // Full rotation in 60 minutes
        float hourAngle = -360.0f * ((((int)hours % 12) + minutes / 60.0f) / 12.0f); // Full rotation in 12 hours

        // Second dial
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.2f, 14.4f, 7.995f));
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.005f));
        model = glm::rotate(model, glm::radians(secondAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, -0.005f));
        model = glm::scale(model, glm::vec3(0.05f, 2.2f, 0.01f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 1.0, 32.0);

        // Minute dial
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.15f, 14.9f, 7.95f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.05f));
        model = glm::rotate(model, glm::radians(minuteAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.05f));
        model = glm::scale(model, glm::vec3(0.05f, 1.6f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0, 32.0);

        // Hour dial
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 14.9f, 7.9f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.1f));
        model = glm::rotate(model, glm::radians(hourAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.1f));
        model = glm::scale(model, glm::vec3(0.05f, 1.5f, 0.2f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 1.0, 0.0, 32.0);

        //12
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 16.9f, 7.9f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 16.9f, 8.05f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 16.9f, 8.20f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 16.9f, 8.30f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //11
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 16.632f, 9.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 16.632f, 9.15f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 16.632f, 9.25f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //10
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 15.9f, 9.732f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 15.9f, 9.832f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //9
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 14.9f, 10.0f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 14.9f, 10.10f));
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 14.9f, 10.25f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //8
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.9f, 9.732f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.9f, 9.882f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.9f, 10.032f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.9f, 10.282f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.9f, 10.282f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);


        //7
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.168f, 9.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.168f, 9.15f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.168f, 9.4f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.168f, 9.4f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //6
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 12.9f, 8.1f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 12.9f, 8.1f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 12.9f, 7.85f));
        //model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //5
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.168f, 7.0f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.168f, 7.0f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //4
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.9f, 6.268f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.9f, 6.018));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 13.9f, 6.018));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //3
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 14.9f, 6.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 14.9f, 5.85f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 14.9f, 5.7f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 15.9f, 6.268f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 15.9f, 6.118f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 16.632f, 7.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);
        //----------------------------1st Floor Design End for Lighting Shader--------------------

        //----------------------------2nd Floor Design Start for Lighting Shader--------------------

        //left room varanda
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.0f, 23.7f, 30.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.1f, 2.5f, 2.1f));
        torus_varanda.drawTorus(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.0f, 22.7f, 30.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.1f, 2.25f, 2.1f));
        torus_varanda.drawTorus(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.0f, 21.7f, 30.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.1f, 2.0f, 2.1f));
        torus_varanda.drawTorus(lightingShader, model);

        //commode left
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(38.0, 20.8, 47.5));
        model = glm::scale(model, glm::vec3(2.0, 2.0, 1.5));
        cutcone.drawCutCone(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(38.0, 22.2, 47.5));
        model = glm::scale(model, glm::vec3(2.0, 0.2, 1.5));
        cutcone.drawCutCone(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(39.0, 22.2, 46.3));
        model = glm::scale(model, glm::vec3(0.5, 2.0, 2.5));
        drawCube(cubeVAO, lightingShader, model, 0.122, 0.361, 0.357, 32.0);

        //basin
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(29.5f, 25.3f, 41.2f));
        model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5));
        hemisphere.drawHemiSphere(lightingShader, model);

        //guest room bedside table
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(26.0f, 20.8f, 38.7f));
        model = glm::scale(model, glm::vec3(3.0f, 2.0f, -2.0f));
        drawCube(cubeVAO, lightingShader, model, 0.761, 0.659, 0.561, 32.0);

        //right room varanda
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 23.7f, 30.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.1f, -2.5f, 2.1f));
        torus_varanda.drawTorus(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 22.7f, 30.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.1f, -2.25f, 2.1f));
        torus_varanda.drawTorus(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 21.7f, 30.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.1f, -2.0f, 2.1f));
        torus_varanda.drawTorus(lightingShader, model);

        //commode right
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(18.0, 20.8, 47.5));
        model = glm::scale(model, glm::vec3(2.0, 2.0, 1.5));
        cutcone.drawCutCone(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(18.0, 22.2, 47.5));
        model = glm::scale(model, glm::vec3(2.0, 0.2, 1.5));
        cutcone.drawCutCone(lightingShader, model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(19.0, 22.2, 46.3));
        model = glm::scale(model, glm::vec3(0.5, 2.0, 2.5));
        drawCube(cubeVAO, lightingShader, model, 0.122, 0.361, 0.357, 32.0);

        //basin right
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(9.5f, 25.3f, 41.2f));
        model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5));
        hemisphere.drawHemiSphere(lightingShader, model);




        //----------------------------2nd Floor Design End for Lighting Shader--------------------
        // 
        // 
        // 
        // 
        // 
        //----------------------------Roof Design Start for Lighting Shader--------------------



        //relling

        for (float i = 11.0f; i < 40.0f; i += 1.0f) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(i, 30.8f, 0.0f));
            model = glm::scale(model, glm::vec3(0.1f, 2.9f, 0.1f));
            drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.2, 30.7f, 0.0f));
        model = glm::scale(model, glm::vec3(-0.1f, 3.0f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);


        for (float i = 1.0f; i < 50.0f; i += 1.0f)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(40.2, 32.2f, i));
            model = glm::scale(model, glm::vec3(-0.1f, 1.5f, 0.1f));
            drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.2, 32.2f, 50.0));
        model = glm::scale(model, glm::vec3(-0.1f, 1.5f, -0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0, 33.7f, 0.0));
        model = glm::scale(model, glm::vec3(30.2f, 0.1f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.2, 33.7f, 0.0));
        model = glm::scale(model, glm::vec3(-0.1f, 0.1f, 50.0f));
        drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

        // roof table
        for (float i = 0.0f; i < 360.0f; i += 60.0f)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(13.0, 30.8f, 23.0));
            model = glm::rotate(model, glm::radians(i), glm::vec3(0.0, 1.0f, 0.0));
            model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0, 0.0f, 1.0));
            model = glm::scale(model, glm::vec3(0.2f, 2.0f, 0.2f));
            drawCube(cubeVAO, lightingShader, model, 0.525, 0.298, 0.149, 32.0);
        }

        


        //----------------------------Roof Design End for Lighting Shader--------------------
        // 
        // 
        // 
        // 
        // 
        //-----------------------------texture shader---------------------------------

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);
        pointlight10.setUpPointLight(lightingShaderWithTexture);

        lightingShaderWithTexture.setVec3("directionalLight.directiaon", 0.5f, -3.0f, -3.0f);
        lightingShaderWithTexture.setVec3("directionalLight.ambient", .5f, .5f, .5f);
        lightingShaderWithTexture.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShaderWithTexture.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);


        lightingShaderWithTexture.setBool("directionLightOn", directionalLightOn);

        spotlight1.setUpspotLight(lightingShaderWithTexture);

        for (unsigned int i = 0; i < 5; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.4f)); 
            conetex.drawCone(lightingShaderWithTexture, cone_bulb_tex, model);
        }
        for (unsigned int i = 5; i < 9; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.4f)); 
            spheretex.drawSphere(lightingShaderWithTexture, sphere_bulb_tex, model);
        }



        //bathroom floor
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-5.0f, -1.0f, -5.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 0.2f, 10.0f));
        //bathroom_floor_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(4.9f, -0.8f, -5.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 5.0f, 10.0f));
        //bathroom_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom cabinet
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(2.9f, -0.8f, -1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 1.2f, 2.0f));
        //bathroom_cabinet_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom floor 2
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, -0.8f, 2.4f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.9f, 0.05f, 2.5f));
        //bathroom_floor_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom wall 2
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-5.0f, -0.8f, 4.9f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 2.5f, 0.1f));
        //bathroom_wall_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //spheretex.drawSphere(lightingShaderWithTexture, laughEmoji, modelMatrixForContainer);
        //cylindertex.drawCylinder(lightingShaderWithTexture, laughEmoji, modelMatrixForContainer);
        //conetex.drawCone(lightingShaderWithTexture, laughEmoji, modelMatrixForContainer);



        



        //----------------------------Ground Floor Design for Lighting with Texture Shader--------------------
        // 
        // 
        // 
        // 
        
        //boundary wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-10.2f, -0.1f, -40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.1f, 7.0f,100.0f));
        boundary_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(50.2f, -0.1f, -40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 7.0f, 100.0f));
        boundary_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-10.2f, -0.1f, 60.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(60.4f, 7.0f, 0.1f));
        boundary_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-10.3f, -0.1f, -40.1f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.3f, 7.0f, 0.1f));
        boundary_wall_cube_2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(50.3f, -0.1f, -40.1f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-20.3f, 7.0f, 0.1f));
        boundary_wall_cube_2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        //lift
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.6f, 20.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, 4.8f));
        halfdisctex.drawCylinder(lightingShaderWithTexture,wall_texture_1_texture ,modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 20.6f, 20.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, 4.8f));
        halfdisctex.drawCylinder(lightingShaderWithTexture, wall_texture_1_texture, modelMatrixForContainer);

        if (isLiftON)
        {
            if (liftPosition <= 10.6f)
            {
                liftPosition += 0.1f;
            }
        }
        else
        {
            if (liftPosition > 0.6005f + 0.01f) 
            {
                liftPosition -= 0.1f;
            }
            else
            {
                liftPosition = 0.6005f;
            }
        }
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, liftPosition, 20.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, 4.8f));
        halfdisctex.drawCylinder(lightingShaderWithTexture, Stair_corridor_texture, modelMatrixForContainer);

        if (isLiftDoorOpenDown)
        {
            if (liftDoorPositionDown <= 5.0f)
            {
                liftDoorPositionDown += 0.1f;
            }
        }
        else
        {
            if (liftDoorPositionDown > 0.01f)
            {
                liftDoorPositionDown -= 0.1f;
            }
            else
            {
                liftDoorPositionDown = 0.0f;
            }
        }

        //lift door
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.15f, 0.6f, 20.0f + liftDoorPositionDown));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 10.0f, 5.0f));
        rsz_1lift_door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.15f, 0.6f, 20.0f - liftDoorPositionDown));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 10.0f, -5.0f));
        rsz_1lift_door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        if (isLiftDoorOpenUp)
        {
            if (liftDoorPositionUp <= 5.0f)
            {
                liftDoorPositionUp += 0.1f;
            }
        }
        else
        {
            if (liftDoorPositionUp > 0.01f)
            {
                liftDoorPositionUp -= 0.1f;
            }
            else
            {
                liftDoorPositionUp = 0.0f;
            }
        }

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.15f, 10.7f, 20.0f + liftDoorPositionUp));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 10.0f, 5.0f));
        rsz_1lift_door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.15f, 10.7f, 20.0f - liftDoorPositionUp));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 10.0f, -5.0f));
        rsz_1lift_door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        //right wall of room
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.5f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 7.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.5f, 42.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 7.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.5f, 37.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.5f, 7.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 8.0f, 37.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.5f, 7.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //left wall of room
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 0.5f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 7.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 0.5f, 42.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 7.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 0.5f, 37.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.5f, 7.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 8.0f, 37.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.5f, 7.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //back wall of room
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.5f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.2f, 10.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 0.5f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(7.5f, 10.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(32.5f, 0.5f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(7.7f, 10.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(27.5f, 0.5f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 2.5f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(27.5f, 8.0f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 2.5f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //front wall of room
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 0.6f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(13.75f, 10.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(13.75f, 0.6f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 2.5f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(13.75f, 8.1f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 2.5f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(18.75f, 0.6f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.75f, 10.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(18.75f, 0.6f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.75f, 2.5f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(22.5f, 7.6f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 3.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(27.5f, 0.6f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.75f, 10.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(31.25f, 0.6f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 2.5f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(31.25f, 8.1f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 2.5f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(36.25f, 0.6f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.75f, 10.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //room door
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(22.5f, 0.6f, 30.2f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 7.0f, 0.1f));
        door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //stair base
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 5.6f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(9.8f, 0.2f, 5.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //Corridor wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.5f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 15.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.5f, 25.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 5.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 0.6f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 9.9f, 17.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 0.6f, 24.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 9.9f, 6.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 8.1f, 17.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 1.9f, 7.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //outside piller
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(18.0f, 5.3f, 2.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.3f, 1.0f, 1.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, wall_texture_1_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(28.0f, 5.3f, 2.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.3f, 1.0f, 1.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, wall_texture_1_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(39.0f, 5.3f, 10.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.3f, 1.0f, 1.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, wall_texture_1_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(39.0f, 5.3f, 20.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.3f, 1.0f, 1.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, wall_texture_1_texture, modelMatrixForContainer);

        //bathroom wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 0.6f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 9.9f, 10.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(19.8f, 0.6f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 9.9f, 10.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 0.6f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.5f, 10.0f, -0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(15.5f, 0.6f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.5f, 10.0f, -0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(12.5f, 7.6f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 3.0f, -0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom door
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(12.5f, 0.6f, 40.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 7.0f, 0.1f));
        door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom toilet
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(15.0f, 0.7f, 45.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 0.05f, 4.0f));
        toilet_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom basin point
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(12.0f, 2.7f, 45.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.05f, 0.2f));
        disctex.drawCylinder(lightingShaderWithTexture, basin_point_texture,modelMatrixForContainer);

        //outside
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, -0.1f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.4f, 0.1f, -40.0f));
        grass_field_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, -0.1f, -40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-10.0f, 0.1f, 90.0f));
        grass_field_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.2f, -0.1f, -40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 0.1f, 90.0f));
        grass_field_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-10.2f, -0.1f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(60.4f, 0.1f, 10.0f));
        grass_field_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(30.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.2f, 0.02f, -40.0f));
        walkway_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 0.0f, -40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.02f, 10.0f));
        walkway_tile_cube_2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //car
        if (isTranslatingX) {
            carPositionX += translationSpeed;

            if (carPositionX >= 25.0f + carX) {
                isTranslatingX = false;       
            }
            if (fabs(carPositionX - 23.0f) < 0.01f) {
                isRotatingX = true;           
                rotationTarget = carRotationAngle + 90.0f; 
                isTranslatingZ = true;
            }
        }
        if (isRotatingX) {
            if (carRotationAngle < rotationTarget) {
                carRotationAngle += rotationSpeed; 
                cout << "carRotationAngle" << carRotationAngle << endl;
                cout << "rotationTarget" << rotationTarget << endl;
            }
            else {
                carRotationAngle = rotationTarget; 
                isRotatingX = false;                
                isTranslatingZ = true;              
                carX += 10.0f;
            }
        }
        if (isTranslatingZ) {
            carPositionZ -= translationSpeed;

            if (carPositionZ <= -32.0f - carZ) {
                isTranslatingZ = false;       
            }
            if (fabs(fabs(carPositionZ) - 30.0f) < 0.01f) { 
                isRotatingZ = true;           
                rotationTarget = carRotationAngle + 90.0f; 
                isTranslatingXr = true;
            }
        }
        if (isRotatingZ) {
            if (carRotationAngle < rotationTarget) {
                carRotationAngle += rotationSpeed; 
                cout << "carRotationAngle" << carRotationAngle << endl;
                cout << "rotationTarget" << rotationTarget << endl;
            }
            else {
                carRotationAngle = rotationTarget; 
                isRotatingZ = false;                
                isTranslatingXr = true;              
                carZ += 10.0f;
            }
        }
        if (isTranslatingXr) {
            carPositionX -= translationSpeed;

            if (carPositionX <= 5.0f + carXr) {
                isTranslatingXr = false;       
            }
            if (fabs(carPositionX - 12.0f) < 0.01f) { 
                isRotatingXr = true;           
                rotationTarget = carRotationAngle - 90.0f; 
                isTranslatingZr = true;
            }
        }
        if (isRotatingXr) {
            if (carRotationAngle > rotationTarget) {
                carRotationAngle -= rotationSpeed; 
                cout << "carRotationAngle" << carRotationAngle << endl;
                cout << "rotationTarget" << rotationTarget << endl;
            }
            else {
                carRotationAngle = rotationTarget; 
                isRotatingZ = false;                
                isTranslatingZr = false;              
                carXr += 10.0f;
            }
        }
        if (isTranslatingZr) {
            carPositionZ -= translationSpeed;

            if (carPositionZ <= -50.0f ) {
                isTranslatingZr = false;       
            }
        }

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(carPositionX, 2.0f, carPositionZ));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(4.0f, 0.0f, 2.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(carRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-4.0f, 0.0f, -2.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(8.0f, 2.0f, 5.0f));
        car_prism.drawPrismforcar(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(carPositionX, 0.5f, carPositionZ));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(4.0f, 0.0f, 2.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(carRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-4.0f, 0.0f, -2.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(8.0f, 1.5f, 5.0f));
        car_2_cube.drawCube(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(carPositionX, 0.5f, carPositionZ));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(4.0f, 0.0f, 2.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(carRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-4.0f + 1.5f, 0.0f, -2.5f - 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.5f, 0.5f));
        cylindertex.drawCylinder(lightingShaderWithTexture, car_tire_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(carPositionX, 0.5f, carPositionZ));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(4.0f, 0.0f, 2.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(carRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-4.0f + 6.5f, 0.0f, -2.5f - 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.5f, 0.5f));
        cylindertex.drawCylinder(lightingShaderWithTexture, car_tire_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(carPositionX, 0.5f, carPositionZ));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(4.0f, 0.0f, 2.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(carRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-4.0f + 1.5f, 0.0f, -2.5f + 5.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.5f, 0.5f));
        cylindertex.drawCylinder(lightingShaderWithTexture, car_tire_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(carPositionX, 0.5f, carPositionZ));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(4.0f, 0.0f, 2.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(carRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-4.0f + 6.5f, 0.0f, -2.5f + 5.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.5f, 0.5f));
        cylindertex.drawCylinder(lightingShaderWithTexture, car_tire_texture, modelMatrixForContainer);

        //fountain
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 6.154f, -22.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.5f, 1.5f, 1.5f));
        pitcher.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 8.94f, -21.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 1.0f, 1.0f));
        pitcher.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 11.66f, -20.8f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.5f, 0.5f, 0.5f));
        pitcher.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 4.0f, -20.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 1.0f, 3.0f));
        fountain_secondary_base.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 7.0f, -20.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 1.0f, 2.0f));
        fountain_secondary_base.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 10.0f, -20.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 1.0f, 1.0f));
        fountain_secondary_base.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, -0.75f, -20.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.2f, 0.5f, 3.2f));
        fountain_primary_base.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 0.05f, -20.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.5f, 0.5f, 5.5f));
        disctex.drawCylinder(lightingShaderWithTexture,first_floor_tile_texture,modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 5.4f, -20.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer,glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.4f, 0.4f, 0.4f));
        cylindertex.drawCylinder(lightingShaderWithTexture, first_floor_tile_texture, modelMatrixForContainer);

        //tree
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 0.0f, -35.0f));
        //modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.5f, 3.0f, 1.5f));
        tree.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        //branch 1
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 4.8f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.2f, 4.0f, 4.2f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.75f, 7.75f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.7f, 3.5f, 3.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.2f, 7.75f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.7f, 3.5f, 3.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(1.1f, 9.65f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.2f, 3.0f, 3.2f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.5f, 9.8f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.7f, 2.5f, 2.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        //branch 2
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 4.8f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.2f, 4.0f, 4.2f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(6.25f, 7.75f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.7f, 3.5f, 3.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(6.8f, 7.75f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.7f, 3.5f, 3.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(8.9f, 9.65f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.2f, 3.0f, 3.2f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.5f, 9.8f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.7f, 2.5f, 2.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        //branch 3
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 4.8f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.2f, 4.0f, 4.2f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 7.75f, -33.75f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.7f, 3.5f, 3.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 7.75f, -33.2f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.7f, 3.5f, 3.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 9.65f, -31.1f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.2f, 3.0f, 3.2f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 9.8f, -30.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(120.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.7f, 2.5f, 2.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        //branch 4
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 4.8f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.2f, 4.0f, 4.2f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 7.75f, -36.25f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.7f, 3.5f, 3.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 7.75f, -36.8f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.7f, 3.5f, 3.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 9.65f, -38.9f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.2f, 3.0f, 3.2f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 9.8f, -39.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-120.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.7f, 2.5f, 2.7f));
        tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        //leaf
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 10.8f, -39.5f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.2f, 0.2f));
        //disctex.drawCylinder(lightingShaderWithTexture,leaf_tex ,modelMatrixForContainer);

        for (const auto& position : leafPositions) {
            // Set up the transformation matrix
            glm::mat4 modelMatrixForLeaf = glm::mat4(1.0f);
            modelMatrixForLeaf = glm::translate(modelMatrixForLeaf, glm::vec3(position.x, position.y, position.z));
            modelMatrixForLeaf = glm::rotate(modelMatrixForLeaf, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrixForLeaf = glm::rotate(modelMatrixForLeaf, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrixForLeaf = glm::rotate(modelMatrixForLeaf, glm::radians( position.x * position.y * position.z), glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrixForLeaf = glm::scale(modelMatrixForLeaf, glm::vec3(0.7f, 0.2f, 0.5f));

            // Draw the leaf
            disctex.drawCylinder(lightingShaderWithTexture, leaf_tex, modelMatrixForLeaf);
        }





        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 4.8f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 1.0f));
        //modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.2f, 4.0f, 4.2f));
        //tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 4.8f, -35.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        //modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.2f, 4.0f, 4.2f));
        //tree_brantch.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);

        //outside
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(30.0f, -0.1f, 30.0f));
        outside_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //stair corridor
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 0.6f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, -0.1f, 30.0f));
        Stair_corridor_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //room floor
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 0.6f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.0f, -0.1f, 20.0f));
        ground_room_floor_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bath floor
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 0.62f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, -0.1f, 10.0f));
        ground_bath_floor_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 0.6f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 9.9f, 10.0f));
        ground_bath_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(19.6f, 0.6f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 9.9f, 10.0f));
        ground_bath_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bed
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(1.0f, 0.6f, 47.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 4.0f, 0.2f));
        bed_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(1.0f, 2.6f, 40.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 0.2f, 7.0f));
        bed_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(1.0f, 0.6f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 2.5f, 0.2f));
        bed_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.5f, 3.405f, 45.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.5f, 0.3f, 0.6f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture,cushion_texture,modelMatrixForContainer);

        //almirah
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(21.0f, 0.6f, 45.95f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 7.0f, 0.05f));
        ground_almirah_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //table and laptop
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0f, 2.6f, 43.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 6.0f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0f, 0.6f, 43.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(38.8f, 0.6f, 43.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0f, 0.6f, 48.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(38.8f, 0.6f, 48.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(37.0f, 2.8f, 46.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 0.05f, 0.7f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        Laptop_keyboard_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(36.0f, 2.85f, 46.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 0.7f, 0.01f));
        Laptop_display_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //chair
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(33.0f, 1.6f, 45.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.5f, 0.2f, 1.8f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(33.0f, 3.6f, 45.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.2f, 1.4f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(33.0f, 2.6f, 45.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.2f, 1.4f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(33.0f, 0.6f, 45.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 1.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(33.0f, 1.8f, 45.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(33.0f, 0.6f, 46.6f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 1.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(33.0f, 1.8f, 46.6f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(34.3f, 0.6f, 45.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 1.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(34.3f, 0.6f, 46.6f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 1.0f, 0.2f));
        wood_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //stair 1.1
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 5.1f, 5.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 4.6f, 6.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 4.1f, 7.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 3.6f, 8.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 3.1f, 9.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 2.6f, 10.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 2.1f, 11.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 1.6f, 12.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 1.1f, 13.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.8f, 0.6f, 14.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //stair 1.2
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 5.9f, 5.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 6.4f, 6.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 6.9f, 7.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 7.4f, 8.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 7.9f, 9.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 8.4f, 10.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 8.9f, 11.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 9.4f, 12.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 9.9f, 13.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        



        //----------------------------Ground Floor Design End for Lighting with Texture Shader--------------------

        //----------------------------First Floor Design for Lighting with Texture Shader--------------------
        //stair 2.1
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(34.0f, 10.7f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 0.7f, 6.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0f, 11.4f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 0.7f, 5.0f));
        Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        for (float i = 12.6f, j = 5.0f; i < 20.1f; i += 0.7f, j += 1.0f)
        {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(36.0f, i, j));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, 1.0f));
            Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        }

        //wall in
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(24.8f, 10.7f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 2.0f));
        wall_texture_2_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(24.8f, 10.7f, 37.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 13.0f));
        wall_texture_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(25.0f, 10.7f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(15.0f, 10.0f, 0.2f));
        wall_texture_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //first floor varanda
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(15.0f, 10.5f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.1f, -10.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(15.0f, 10.6f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.1f, -10.0f));
        first_floor_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        //front wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 10.5f, -0.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 10.2f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0f, 10.5f, -0.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 10.2f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 20.5f, -0.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(30.0f, 10.2f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.0f, -0.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.2f, 30.7f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //ground
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 0.0f, -0.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.7f, 30.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.2f, 0.0f, 29.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(30.0f, 0.7f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.2f, 0.7f, 50.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.2f, 0.0f, 30.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.7f, 20.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 0.0f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.4f, 0.7f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 10.7f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 15.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 10.7f, 25.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 2.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 16.7f, 15.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 4.0f, 10.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 10.7f, 27.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 10.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 10.5f, 25.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.2f, 9.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 10.5f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.2f, 15.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 10.5f, 39.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.2f, 10.5f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 10.5f, 34.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.7f, 5.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 18.2f, 34.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.5f, 5.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //walls 2
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 10.5f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(15.2f, 10.2f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 10.5f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.2f, 10.2f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(15.0f, 10.5f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 2.7f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(15.0f, 18.2f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 2.5f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        //wall 3
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 10.5f, 43.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.2f, 6.8f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 10.5f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.2f, 33.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //guest room wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(25.0f, 10.7f, 49.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(15.0f, 10.0f, 0.1f));
        wall_texture_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //fan

        if (isFanON) {
            fanAngle += fanAngleStep;
        }

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(32.5f, 19.35f, 40.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.65f, 0.05f, 0.05f));
        cylindertex.drawCylinder(lightingShaderWithTexture, fan_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(32.5f, 18.7f, 40.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(fanAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.5f, 1.0f, 0.5f));
        disctex.drawCylinder(lightingShaderWithTexture,fan_texture ,modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(32.5f, 18.7f, 40.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.00f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(fanAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.5f, 1.0f, 0.5f));
        disctex.drawCylinder(lightingShaderWithTexture, fan_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(32.5f, 18.7f, 40.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(fanAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 0.2f, 0.2f));
        spheretex.drawSphere(lightingShaderWithTexture, fountain_4_tex, modelMatrixForContainer);

        //guestroom bed
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(29.5f, 10.7f, 49.7f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 5.0f, 0.2f));
        guest_room_bed_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(29.5f, 10.7f, 49.6f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 1.5f, -7.0f));
        guest_room_bed_low_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(29.5f, 12.2f, 49.6f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 0.5f, -7.0f));
        guestroom_bed_sheet_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(31.2f, 13.005f, 48.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.2f, 0.3f, 0.6f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, cushion_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(34.0f, 13.005f, 48.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.2f, 0.3f, 0.6f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, cushion_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(32.5f, 10.805f, 41.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, 5.0f));
        disctex.drawCylinder(lightingShaderWithTexture, guest_room_floor_mat_texture, modelMatrixForContainer);

        //guest room floor
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(25.0f, 10.7f, 49.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(15.0f, 0.1f, -19.7f));
        guest_room_floor_tile_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //guest room bedside table
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(26.0f, 10.8f, 46.7f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 2.0f, -0.1f));
        guest_room_bedside_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //guest room varanda
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 10.5f, 33.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 0.2f, 10.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 10.7f, 33.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 0.02f, 10.2f));
        guest_room_varand_floor_tile_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //kitchen cabinet
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.0f, 10.7f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.05f, 3.0f, 7.0f));
        kitchen_cabinet_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 10.7f, 39.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.05f, 10.0f, 10.2f));
        kitchen_cabinet_tile_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.05f, 10.7f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.0f, 10.0f, -0.05f));
        kitchen_cabinet_tile_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 13.7f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.1f, 0.05f, 6.9f));
        kitchen_cabinet_tile_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 13.7f, 46.9f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.1f, 0.05f, 3.1f));
        kitchen_cabinet_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.0f, 10.7f, 47.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(8.0f, 3.0f, -0.05f));
        kitchen_cabinet_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.1f, 13.7f, 46.90f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(7.9f, 0.05f, 3.1f));
        kitchen_cabinet_tile_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(2.0f, 17.5f, 48.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(8.0f, 3.2f, -0.05f));
        kitchen_cabinet_4_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(2.0f, 17.5f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.05f, 3.2f, 8.0f));
        kitchen_cabinet_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(6.8f, 13.7f, 33.3f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.4f, 0.01f, 10.4f));
        kitchen_cabinet_tile_4_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //freeze
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.5f, 10.7f, 28.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.008f, 6.0f, 5.0f));
        frezz_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.516f, 10.7f, 28.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.08f, 6.0f, 2.9f));
        freeze_door_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.516f, 10.7f, 30.9f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 0.0f, -2.1f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.08f, 6.0f, 2.1f));
        freeze_door_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //basin
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.0f, 14.01f, 37.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 0.3f, 2.0f));
        hemispheretex.drawHemiSphere(lightingShaderWithTexture, kitchen_basin_texture, modelMatrixForContainer);

        //floor
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 10.6f, 14.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.0f, 0.1f, 36.0f));
        first_floor_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 10.6f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(30.2f, 0.1f, 14.0f));
        first_floor_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 10.5f, 14.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.0f, 0.1f, 36.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 10.5f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(30.2f, 0.1f, 14.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //sofa
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.8f, 12.2f, 10.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.9f, 0.1f, 7.3f));
        sofa_cube_2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.7f, 12.2f, 17.3f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 0.1f, 1.9f));
        sofa_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 13.7f, 10.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.7f, 0.1f, 10.0f));
        sofa_cube_2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.7f, 13.7f, 19.3f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 0.1f, 0.7f));
        sofa_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(22.7f, 10.7f, 10.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 1.5f, 9.3f));
        sofa_cube_3.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(22.7f, 10.7f, 17.3f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 1.5f, -0.1f));
        sofa_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.7f, 12.2f, 10.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 1.5f, 9.2f));
        sofa_cube_3.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.7f, 12.2f, 19.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.5f, 0.1f));
        sofa_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //dinning
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(17.0f, 13.9005f, 42.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 1.0f, 4.0f));
        disctex.drawCylinder(lightingShaderWithTexture, dinning_texture, modelMatrixForContainer);

        //tea table
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(28.0f, 12.4005f, 13.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.5f, 1.0f, 2.5f));
        disctex.drawCylinder(lightingShaderWithTexture, tea_table_texture, modelMatrixForContainer);

        //chandeliers
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(28.0f, 25.0f, 13.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(15.0f), glm::vec3(0.0, 1.0, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 1.0f, 1.0f));
        torustex.drawTorus(lightingShaderWithTexture, chandeliers_1_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(28.0f, 24.0f, 13.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 1.0f, 1.0f));
        torustex1.drawTorus(lightingShaderWithTexture, chandeliers_1_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(28.0f, 23.2f, 13.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(15.0f), glm::vec3(0.0, 1.0, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 1.0f, 1.0f));
        torustex2.drawTorus(lightingShaderWithTexture, chandeliers_1_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(28.0f, 22.6f, 13.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 1.0f, 1.0f));
        torustex3.drawTorus(lightingShaderWithTexture, chandeliers_1_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(28.0f, 22.0f, 13.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.4f, 0.4f, 0.4f));
        spheretex.drawSphere(lightingShaderWithTexture, chandeliers_1_texture, modelMatrixForContainer);

        //main door
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 10.7f, 15.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 6.0f, 5.0f));
        door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.8f, 10.7f, 25.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 6.0f, -5.0f));
        door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //----------------------------First Floor Design end for Lighting with Texture Shader--------------------
        // 
        // 
        // 
        // 
        // 
        //----------------------------2nd Floor Design Start for Lighting with Texture Shader--------------------
        //floor
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 20.5f, 16.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.0f, 0.2f, 34.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
            //left
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 20.7f, 20.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.1f, 20.0f));
        master_bed_room_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
            //right
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 20.7f, 20.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.1f, 20.0f));
        master_bed_room_tile_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
            //left bathroom
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 20.7f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.1f, 10.0f));
        bathroom_floor_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
            //right bathroom
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 20.7f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.1f, 10.0f));
        bathroom_floor_3_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //corridor
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 20.7f, 16.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.0f, 0.1f, 4.0f));
        corridor_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        for (float i = 35.0f; i >= 10.0f; i -= 3.0f)
        {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(i, 20.7f, 16.0f));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 1.5f, 0.2f));
            ralling_bez.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);
        }
        for (float i = 1.0f; i < 16.0f; i += 3.0f)
        {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0, 20.7f, i));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 1.5f, 0.2f));
            ralling_bez.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);
        }

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(22.5f, 23.6f, 16.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(12.5f, 0.1f, 0.1f));
        cylindertex.drawCylinder(lightingShaderWithTexture, fountain_5_tex,modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 23.6f, 8.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(8.0f, 0.1f, 0.1f));
        cylindertex.drawCylinder(lightingShaderWithTexture, fountain_5_tex, modelMatrixForContainer);

        //walls
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 20.7f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 25.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 20.7f, 35.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 15.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 20.7f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 25.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 20.7f, 35.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 15.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 20.7f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.4f, 10.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(19.9f, 20.7f, 20.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 30.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 20.5f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(9.8f, 0.2f, 16.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 20.7f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 0.1f, 16.0f));
        kitchen_cabinet_tile_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 20.7f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(19.9f, 10.0f, -0.1f));
        Bathroom_wall_6_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 20.7f, 49.9f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 10.0f, -9.9f));
        Bathroom_wall_6_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(19.9f, 20.7f, 49.9f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.1f, 5.0f, -9.9f));
        Bathroom_wall_6_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 20.7f, 40.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.9f, 10.0f, -0.1f));
        Bathroom_wall_6_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 27.7f, 40.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 3.0f, -0.1f));
        Bathroom_wall_6_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //2
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.1f, 20.7f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(19.9f, 10.0f, -0.1f));
        bathroom_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.1f, 20.7f, 49.9f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 10.0f, -9.9f));
        bathroom_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0f, 20.7f, 49.9f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.1f, 5.0f, -9.9f));
        bathroom_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(25.1f, 20.7f, 40.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.9f, 10.0f, -0.1f));
        bathroom_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.1f, 27.7f, 40.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 3.0f, -0.1f));
        Bathroom_wall_5_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //bathroom cabinet left
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(28.0f, 20.8f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 3.0f, 2.0f));
        bathroom_cabinet_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //basin point
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(29.5f, 23.9f, 41.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1, 1.0, 0.1));
        disctex.drawCylinder(lightingShaderWithTexture, basin_point_texture ,modelMatrixForContainer);

        //bathroom floor 2
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0f, 20.8f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 0.05f, 5.0f));
        bathroom_floor_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //left room
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(25.0f, 20.7f, 39.9));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.9f, 10.0f, -0.1f));
        master_bed_room_wall_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.2f, 20.7f, 20.1));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.8f, 10.0f, 0.1f));
        master_bed_room_wall_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.1f, 27.7f, 39.9));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.9f, 3.0f, -0.1f));
        master_bed_room_wall_2_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0f, 27.7f, 20.1));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 3.0f, 0.1f));
        master_bed_room_wall_2_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.1, 20.7f, 20.1));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1f, 10.0f, 19.7f));
        master_bed_room_wall_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(27.5, 20.7f, 39.8));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 8.0f, -0.1f));
        leftroom_bed_back_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //door
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0, 20.7f, 20.1));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(-FreezeAngle), glm::vec3(0.0, 1.0f, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 7.0f, 0.1f));
        door_left_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //varanda
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0, 20.706f, 30.0));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0, 1.0f, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, 5.0f));
        halfdisctex.drawCylinder(lightingShaderWithTexture, master_bed_room_tile_1_texture,modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0, 20.705f, 30.0));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0, 1.0f, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, 5.0f));
        halfdisctex.drawCylinder(lightingShaderWithTexture, wall_texture_1_texture, modelMatrixForContainer);

        //left room bedside table
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(26.0f, 20.8f, 36.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 2.0f, -0.1f));
        leftroom_bedside_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //left room bed
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(29.5f, 20.7f, 39.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 5.0f, 0.2f));
        leftroom_bedBack_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(29.5f, 20.7f, 39.4f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 1.5f, -7.0f));
        guest_room_bed_low_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(29.5f, 22.2f, 39.4f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 0.5f, -7.0f));
        rsz_leftroom_bedsheet_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(31.2f, 23.005f, 37.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.2f, 0.3f, 0.6f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, cushion_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(34.0f, 23.005f, 37.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.2f, 0.3f, 0.6f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, cushion_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(32.5f, 20.805f, 30.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, 5.0f));
        disctex.drawCylinder(lightingShaderWithTexture, leftroom_mat_texture, modelMatrixForContainer);

        //left room bathroom door
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(25.0f, 20.8f, 39.9f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 7.0f, 0.1f));
        room_door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //right room
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(7.5, 20.7f, 39.8));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 8.0f, -0.1f));
        leftroom_bed_back_wall_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 20.7f, 39.9));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.9f, 10.0f, -0.1f));
        master_bed_room_wall_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 20.7f, 20.1));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.8f, 10.0f, 0.1f));
        master_bed_room_wall_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 27.7f, 39.9));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 3.0f, -0.1f));
        master_bed_room_wall_1_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 27.7f, 20.1));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 3.0f, 0.1f));
        master_bed_room_wall_1_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(19.9f, 20.7f, 20.1));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.1f, 10.0f, 19.7f));
        master_bed_room_wall_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //right room bathroom door
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 20.8f, 39.9f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(FreezeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 7.0f, 0.1f));
        room_door_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //right room bedside table
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(6.0f, 20.8f, 36.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 2.0f, -0.1f));
        leftroom_bedside_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //right room bed
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.5f, 20.7f, 39.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 5.0f, 0.2f));
        leftroom_bedBack_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.5f, 20.7f, 39.4f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 1.5f, -7.0f));
        guest_room_bed_low_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.5f, 22.2f, 39.4f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(6.0f, 0.5f, -7.0f));
        rsz_leftroom_bedsheet_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(11.2f, 23.005f, 37.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.2f, 0.3f, 0.6f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, cushion_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(14.0f, 23.005f, 37.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.2f, 0.3f, 0.6f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cylindertex.drawCylinder(lightingShaderWithTexture, cushion_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(12.5f, 20.805f, 30.8f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, 5.0f));
        disctex.drawCylinder(lightingShaderWithTexture, leftroom_mat_texture, modelMatrixForContainer);

        //bathroom cabinet right
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(8.0f, 20.8f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 3.0f, 2.0f));
        bathroom_cabinet_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //basin point right
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(9.5f, 23.9f, 41.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.1, 1.0, 0.1));
        disctex.drawCylinder(lightingShaderWithTexture, basin_point_texture, modelMatrixForContainer);

        //bathroom floor 2 right
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(15.0f, 20.8f, 40.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 0.05f, 5.0f));
        bathroom_floor_2_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //door
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0, 20.7f, 20.2));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(180.0f), glm::vec3(0.0, 1.0f, 0.0));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(FreezeAngle), glm::vec3(0.0, 1.0f, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 7.0f, 0.1f));
        door_left_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //varanda
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2, 20.706f, 30.0));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0, 1.0f, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, -5.0f));
        halfdisctex.drawCylinder(lightingShaderWithTexture, master_bed_room_tile_texture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2, 20.705f, 30.0));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0, 1.0f, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 1.0f, -5.0f));
        halfdisctex.drawCylinder(lightingShaderWithTexture, wall_texture_1_texture, modelMatrixForContainer);

        //corridor wall

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 27.7f, 20.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 3.0f, 0.1f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(5.0f, 20.7f, 20.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.9f, 10.0f, 0.1f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(35.0f, 27.7f, 20.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(5.0f, 3.0f, 0.1f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.1f, 20.7f, 20.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(14.9f, 10.0f, 0.1f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //stair
        for (float i = 21.7f, j = 16.0f; i < 30.7f; i += 1.0f, j -= 1.0f) {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, i, j));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 0.2f, -1.0f));
            Stair_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        }


        //----------------------------2nd Floor Design End for Lighting with Texture Shader-------------------- 




        //----------------------------Roof Design Start for Lighting with Texture Shader-------------------- 
        
        //table
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(13.0, 32.65f, 23.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.5f, 1.0f, 3.5f));
        disctex.drawCylinder(lightingShaderWithTexture,kitchen_cabinet_tile_texture ,modelMatrixForContainer);


        //floor
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 30.5, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 0.2f, 7.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 30.5, 16.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.0f, 0.2f, 34.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 30.5, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(30.0f, 0.2f, 16.1f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //extension
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 30.5, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.4f, 10.2f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 40.5, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 0.2f, 16.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        //roof grass
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 30.7, 16.1));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.0f, 0.1f, 33.9f));
        roof_grass_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.2f, 30.7, 0.0));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(29.8f, 0.1f, 16.1f));
        roof_grass1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //roof wall
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 30.7, 0.2f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 15.8f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2f, 30.7, 16.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 2.0f, 34.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 30.7, 16.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.2f, 10.0f, 0.1f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 30.7, 7.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.2f, 10.0f, 9.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 30.7, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(40.2f, 2.0f, 0.2f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(10.0f, 40.5, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 0.2f, 10.0f));
        wall_texture_1_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        for (float i = 0.0f; i < 40.0f; i += 4.0f) {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(i, 32.7, 50.0f));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 3.0f, 0.2f));
            roof_wall_triangle.drawPrism(lightingShaderWithTexture, modelMatrixForContainer);
        }

        for (float i = 16.1f; i < 50.0f; i += 4.0f) {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-0.2, 32.7, i));
            modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0f));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(4.0f, 3.0f, 0.2f));
            roof_wall_triangle.drawPrism(lightingShaderWithTexture, modelMatrixForContainer);
        }

        for (float i = 2.0f; i < 52.0f; i += 2.0f) {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0, 30.7, i));
            modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0f));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 1.5f, 0.2f));
            roof_wall_triangle.drawPrism(lightingShaderWithTexture, modelMatrixForContainer);
        }

        for (float i = 2.0f; i < 52.0f; i += 2.0f) {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(40.0, 32.2, i));
            modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0f));
            modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0f));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 1.5f, -0.2f));
            roof_wall_triangle.drawPrism(lightingShaderWithTexture, modelMatrixForContainer);
        }

        modelMatrixForContainer = glm::mat4(1.0f);
        glm::mat4 shearMatrix = createShearMatrix(1.75, 0, 0, 0, 0, 0);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 40.7, 0.0f)) ;
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0f)) * shearMatrix;
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 5.0f, 0.2f));
        roof_wall_triangle.drawPrism(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        shearMatrix = createShearMatrix(1.75, 0, 0, 0, 0, 0);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(30.0f, 40.7, 0.0f)) ;
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0f)) * shearMatrix;
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 5.0f, 0.2f));
        roof_wall_triangle.drawPrism(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        shearMatrix = createShearMatrix(1.75, 0, 0, 0, 0, 0);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(30.0f, 40.7, 10.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0f)) * shearMatrix;
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 5.0f, 0.2f));
        roof_wall_triangle.drawPrism(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        shearMatrix = createShearMatrix(1.75, 0, 0, 0, 0, 0);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.0f, 40.7, 10.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(270.0f), glm::vec3(0.0, 0.0, 1.0f)) * shearMatrix;
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(10.0f, 5.0f, 0.2f));
        roof_wall_triangle.drawPrism(lightingShaderWithTexture, modelMatrixForContainer);

        //roof tiles
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(12.5f, 30.8f, 4.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 0.1f, 2.0f));
        roof_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(16.5f, 30.8f, 4.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 0.1f, 2.0f));
        roof_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.5f, 30.8f, 4.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 0.1f, 2.0f));
        roof_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.5f, 30.8f, 9.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 0.1f, 2.0f));
        roof_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.5f, 30.8f, 14.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 0.1f, 2.0f));
        roof_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.5f, 30.8f, 19.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 0.1f, 2.0f));
        roof_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.5f, 30.8f, 24.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 0.1f, 2.0f));
        roof_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(20.5f, 30.8f, 29.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 0.1f, 2.0f));
        roof_tile_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //roof platform
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.0f, 30.8f, 47.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.5f, -15.0f));
        roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.0f, 31.3f, 32.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.5f, 8.0f, 0.5f));
        roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(13.0f, 31.3f, 32.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.5f, 8.0f, 0.5f));
        roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(23.0f, 31.3f, 47.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.5f, 8.0f, -0.5f));
        roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(23.0f, 31.3f, 39.5f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.5f, 8.0f, 0.5f));
        roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.0f, 39.3f, 32.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 0.5f, 0.5f));
        roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        for (float i = 23.0f; i > 3.0f; i -= 2)
        {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(i, 39.3f, 47.0f));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.5f, 0.5f, -14.5f));
            roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        }

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.0f, 39.3f, 47.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.5f, 0.5f, -14.5f));
        roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        for (float i = 21.0f; i > 3.0f; i -= 2)
        {
            modelMatrixForContainer = glm::mat4(1.0f);
            modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(i, 32.8f, 47.0f));
            modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(-0.5f, 7.0f, 0.2f));
            roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        }

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.0f, 32.8f, 47.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.5f, 7.0f, 0.2f));
        roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);



        //roof plant base
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 30.8f, 16.1f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 2.0f, 34.0f));
        roof_plant_base_cube_1.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(3.0f, 30.8f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(37.0f, 2.0f, -3.0f));
        roof_plant_base_cube_2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //roof plant
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 32.8f, 16.1f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.8f, 1.0f, 34.0f));
        roof_plant_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 33.8f, 16.1f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.8f, 0.01f, 34.0f));
        roof_plant_cube_1.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(2.0f, 32.8f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(38.0f, 1.0f, -2.8f));
        roof_plant_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(2.0f, 33.8f, 50.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(38.0f, 0.01f, -2.8f));
        roof_plant_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //swing

        swingAngleAmplitude = (swingVelocity / maxSwingVelocity) * maxSwingAngle;
        currenttime += swingVelocity;
        float swingAngle = glm::sin(currenttime) * swingAngleAmplitude;
        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(11.25f, 32.8f, 37.5f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(1.5f, 6.5f, 2.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(swingAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(-1.5f, -6.5f, -2.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(3.0f, 0.2f, 4.0f));
        roof_platform_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(12.75f, 32.8f, 39.5f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, 6.5f, 0.0f));
        modelMatrixForContainer = glm::rotate(modelMatrixForContainer, glm::radians(swingAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(0.0f, -6.5f, 0.0f));
        modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.05f, 6.5f, 0.05f));
        swing_cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //----------------------------Roof Design End for Lighting with Texture Shader-------------------- 
         
        
        //-----------------------texture shader end---------------------------------

        //-----------------------transparent shader------------------------------------

        TransparentShader.use();
        //glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 view2 = camera.GetViewMatrix();
        TransparentShader.setMat4("projection", projection);
        TransparentShader.setMat4("view", view);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        x_shift = 0.0;
        y_shift = 10.6;
        z_shift = 20.0;
        Glass_Lift(transparentVAO2, TransparentShader, x_shift, y_shift, z_shift);

        x_shift = 10.1;
        y_shift = 18.7;
        z_shift = -12.0;
        Glass_Clock_bg(transparentVAO, TransparentShader, x_shift, y_shift, z_shift);

        x_shift = 3.9;
        y_shift = 12.7;
        z_shift = -14.0;
        Glass_stair(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 180, 9.0, 2.3);

        x_shift = 5.9;
        y_shift = 0.8;
        z_shift = -15.0;
        Glass_stair(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 0, 10.0, 2.5);

        x_shift = 36.2;
        y_shift = 22.5;
        z_shift = -16.0;
        Glass_stair(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 180,12.0, 4.2);

        x_shift = 45.02;
        y_shift = 13.72;
        z_shift = -38.7;
        Glass_plant(transparentVAO, TransparentShader, x_shift, y_shift, z_shift);

        x_shift = 27.5;
        y_shift = 3.0;
        z_shift = -50.1;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, FreezeAngle ,0.0f);

        x_shift = 32.5;
        y_shift = 3.0;
        z_shift = -50.1;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, -FreezeAngle, 180.0f);

        //1st floor window
        x_shift = 15.0;
        y_shift = 13.2;
        z_shift = -50.1;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, FreezeAngle, 0.0f);

        x_shift = 20.0;
        y_shift = 13.2;
        z_shift = -50.1;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, -FreezeAngle, 180.0f);

        x_shift = -0.2;
        y_shift = 13.2;
        z_shift = -34.5;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, FreezeAngle, -90.0f);

        x_shift = -0.2;
        y_shift = 13.2;
        z_shift = -39.5;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, -FreezeAngle, -270.0f);

        x_shift = 40.2;
        y_shift = 3.0;
        z_shift = -37.5;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, -FreezeAngle, -90.0f);

        x_shift = 40.2;
        y_shift = 3.0;
        z_shift = -42.5;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, FreezeAngle, -270.0f);

        x_shift = -0.2;
        y_shift = 3.0;
        z_shift = -37.5;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, FreezeAngle, -90.0f);

        x_shift = -0.2;
        y_shift = 3.0;
        z_shift = -42.5;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, -FreezeAngle, -270.0f);

        x_shift = 18.75;
        y_shift = 3.1;
        z_shift = -30.0;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, FreezeAngle, 180.0f);

        x_shift = 13.75;
        y_shift = 3.1;
        z_shift = -30.0;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, -FreezeAngle, 0.0f);

        x_shift = 36.25;
        y_shift = 3.1;
        z_shift = -30.0;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, FreezeAngle, 180.0f);

        x_shift = 31.25;
        y_shift = 3.1;
        z_shift = -30.0;
        Glass_window(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, -FreezeAngle, 0.0f);

        if (glass_gateOpen) {
            if (sc >= 0.0f)
            {
                sc -= 0.01;
            }
        }
        else 
        {
            if (sc < 1.0 - 0.01f)
            {
                sc += 0.01f;
            }
            else
            {
                sc = 1.0f;
            }
        }
        x_shift = 10.0;
        y_shift = 0.6;
        z_shift = -24.0f;
        collapsible_gate_draw(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, sc);

        x_shift = 10.0;
        y_shift = -0.1;
        z_shift = 40.1f;
        collapsible_gate_draw_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, sc);

        //left bathroom
        x_shift = 34.995;
        y_shift = 20.8;
        z_shift = -43.0f;
        Glass_window_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 90.0f);

        x_shift = 34.995;
        y_shift = 20.8;
        z_shift = -50.0f;
        Glass_window_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 90.0f);

        x_shift = 37.0;
        y_shift = 20.8;
        z_shift = -45.005f;
        Glass_window_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 0.0f);

        //right bathroom
        x_shift = 14.995;
        y_shift = 20.8;
        z_shift = -43.0f;
        Glass_window_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 90.0f);

        x_shift = 14.995;
        y_shift = 20.8;
        z_shift = -50.0f;
        Glass_window_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 90.0f);

        x_shift = 17.0;
        y_shift = 20.8;
        z_shift = -45.005f;
        Glass_window_2(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 0.0f);

        // 2nd floor left varanda door
        x_shift = 40.1 ;
        y_shift = 20.7;
        z_shift = -35.0f - liftDoorPositionDown;
        Glass_varanda_door(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 90.0f);

        // 2nd floor right varanda door
        x_shift = -0.1;
        y_shift = 20.7;
        z_shift = -35.0f - liftDoorPositionDown;
        Glass_varanda_door(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 90.0f);

        // 1st floor guest room varanda door
        x_shift = 40.1;
        y_shift = 10.7;
        z_shift = -43.05f - liftDoorPositionDown;
        Glass_varanda_door(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 90.0f);

        // 1st floor varanda door
        x_shift = 15.0 - liftDoorPositionDown;
        y_shift = 10.7;
        z_shift = 0.1f;
        Glass_varanda_door(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 0.0f);

        x_shift = 25.0 + liftDoorPositionDown;
        y_shift = 10.7;
        z_shift = 0.1f;
        Glass_varanda_door(transparentVAO, TransparentShader, x_shift, y_shift, z_shift, 0.0f);

        glDisable(GL_BLEND);

        // Step 3: Render the mirror surface with the texture
        //mirrorPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Position at (0, 1, -5)

        renderMirrorSurface(texture, projection, mirrorShaderProgram, mirrorPosition, mirroredView);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

unsigned int createFramebuffer(unsigned int& texture, unsigned int width, unsigned int height) {
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create a texture to render to
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Create a renderbuffer object for depth and stencil attachment
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return framebuffer;
}


void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f, float shininess = 32.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", shininess);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Glass_window(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float freeze_angle, float rota)
{
    //variable declaration
    float local_x = 0.0f, local_y = 0.0f, local_z = 0.f;
    float len = 0.0f, width = 0.0f, height = 0.0f;
    float  rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;

    rot_x = 0.0f, rot_y = 0.0f - rotateAxis_Y_win_2_1, rot_z = 0.0f;
    trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;
    len = 2.5f, width = 1.0f, height = 5.0f;
    local_x = 0.0f, local_y = 0.0f, local_z = 0.0f;
    GenSheet_Transparent_for_window(TransparentVAO, transparentShader, len, width, height, local_x, local_y, local_z, rot_x, rot_y, rot_z, trans_x, trans_y, trans_z, x_shift, y_shift, z_shift, off_white, glass, -freeze_angle, rota);
    
}

void Glass_window_2(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float rot)
{
    //variable declaration
    float local_x = 0.0f, local_y = 0.0f, local_z = 0.f;
    float len = 0.0f, width = 0.0f, height = 0.0f;
    float  rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;

    rot_x = 0.0f, rot_y = rot - rotateAxis_Y_win_2_1, rot_z = 0.0f;
    trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;
    len = 3.0f, width = 1.0f, height = 8.0f;
    local_x = 0.0f, local_y = 0.0f, local_z = 0.0f;
    GenSheet_Transparent(TransparentVAO, transparentShader, len, width, height, local_x, local_y, local_z, rot_x, rot_y, rot_z, trans_x, trans_y, trans_z, x_shift, y_shift, z_shift, off_white, glass);
}

void Glass_varanda_door(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float rot)
{
    //variable declaration
    float local_x = 0.0f, local_y = 0.0f, local_z = 0.f;
    float len = 0.0f, width = 0.0f, height = 0.0f;
    float  rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;

    rot_x = 0.0f, rot_y = rot - rotateAxis_Y_win_2_1, rot_z = 0.0f;
    trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;
    len = 10.0f, width = 1.0f, height = 9.9f;
    local_x = 0.0f, local_y = 0.0f, local_z = 0.0f;
    GenSheet_Transparent(TransparentVAO, transparentShader, len, width, height, local_x, local_y, local_z, rot_x, rot_y, rot_z, trans_x, trans_y, trans_z, x_shift, y_shift, z_shift, off_white, glass2);
}

void Glass_plant(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift)
{
    //variable declaration
    float local_x = 0.0f, local_y = 0.0f, local_z = 0.f;
    float len = 0.0f, width = 0.0f, height = 0.0f;
    float  rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;

    rot_x = 0.0f, rot_y = 90.0f - rotateAxis_Y_win_2_1, rot_z = 0.0f;
    trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;
    len = 5.0f, width = 0.0f, height = -5.0f;
    local_x = 0.5f, local_y = 0.0f, local_z = 0.001f;
    GenSheet_Transparent(TransparentVAO, transparentShader, len, width, height, local_x, local_y, local_z, rot_x, rot_y, rot_z, trans_x, trans_y, trans_z, x_shift, y_shift, z_shift, off_white, hanging_plant);

}


void collapsible_gate_draw(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float sc)
{
    //variable declaration
    float local_x = 0.0f, local_y = 0.0f, local_z = 0.f;
    float len = 0.0f, width = 0.0f, height = 0.0f;
    float  rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;

    rot_x = 0.0f, rot_y = 90.0f - rotateAxis_Y_win_2_1, rot_z = 0.0f;
    trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;
    len = 7.0f, width = 0.0f, height = 7.5f;
    local_x = 0.0f, local_y = 0.0f, local_z = 0.0f;
    GenSheet_Transparent_collapsible_gate_draw(TransparentVAO, transparentShader, len, width, height, local_x, local_y, local_z, rot_x, rot_y, rot_z, trans_x, trans_y, trans_z, x_shift, y_shift, z_shift, off_white, collapsible_gate, sc);

}

void collapsible_gate_draw_2(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float sc)
{
    //variable declaration
    float local_x = 0.0f, local_y = 0.0f, local_z = 0.f;
    float len = 0.0f, width = 0.0f, height = 0.0f;
    float  rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;

    rot_x = 0.0f, rot_y = 0.0f - rotateAxis_Y_win_2_1, rot_z = 0.0f;
    trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;
    len = 20.0f, width = 0.0f, height = 7.0f;
    local_x = 0.0f, local_y = 0.0f, local_z = 0.0f;
    GenSheet_Transparent_collapsible_gate_draw_2(TransparentVAO, transparentShader, len, width, height, local_x, local_y, local_z, rot_x, rot_y, rot_z, trans_x, trans_y, trans_z, x_shift, y_shift, z_shift, off_white, collapsible_gate, sc);

}

void Glass_Clock_bg(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift)
{
    //variable declaration
    float local_x = 0.0f, local_y = 0.0f, local_z = 0.f;
    float len = 0.0f, width = 0.0f, height = 0.0f;
    float  rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;

    rot_x = 0.0f, rot_y = 90.0f - rotateAxis_Y_win_2_1, rot_z = 0.0f;
    trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;
    len = 7.0f, width = 0.0f, height = -7.0f;
    local_x = 0.5f, local_y = 0.0f, local_z = 0.001f;
    GenSheet_Transparent(TransparentVAO, transparentShader, len, width, height, local_x, local_y, local_z, rot_x, rot_y, rot_z, trans_x, trans_y, trans_z, x_shift, y_shift, z_shift, off_white, clock_bg);

}

void Glass_stair(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift, float rot, float length, float sh)
{
    //variable declaration
    float local_x = 0.0f, local_y = 0.0f, local_z = 0.f;
    float len = 0.0f, width = 0.0f, height = 0.0f;
    float  rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;

    //*********************************************************left glass***********************************************************
    //*****************************************************************************************************************************
    rot_x = -rot, rot_y = 90.0f - rotateAxis_Y_win_2_1, rot_z = 0.0f;
    trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;
    //left glasS--------------------------------------------
    len = length, width = 1.0f, height = 2.0f;
    local_x = 0.0f, local_y = 0.0f, local_z = 0.0f;
    GenSheet_Transparent_2(TransparentVAO, transparentShader, len, width, height, local_x, local_y, local_z, rot_x, rot_y, rot_z, trans_x, trans_y, trans_z, x_shift, y_shift, z_shift, off_white, glass, sh);
    //*********************************************************END of Left Glass*****************************************************
}
void Glass_Lift(unsigned int TransparentVAO, Shader transparentShader, float x_shift, float y_shift, float z_shift)
{
    // Variable declarations
    float local_x = 0.0f, local_y = 0.0f, local_z = 0.0f;
    float len = 0.0f, width = 0.0f, height = 0.0f;
    float rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f;

    // Define parameters for the half-cylinder
    float radius = 5.0f;  // Radius of the half-cylinder
    float height_cylinder = 20.0f; // Height of the half-cylinder
    int segments = 36;    // Number of segments for smoothness

    // Set rotation and transformation
    rot_x = 0.0f;
    rot_y = -90.0f; // Rotate to align with the required axis
    rot_z = 0.0f;
    trans_x = x_shift; // Apply x_shift for positioning
    trans_y = y_shift; // Apply y_shift for positioning
    trans_z = z_shift; // Apply z_shift for positioning

    // Generate and render the half-cylinder
    GenHalfCylinder_Transparent(
        TransparentVAO, transparentShader, radius, height_cylinder, segments,
        rot_x, rot_y, rot_z, trans_x, trans_y, trans_z, glass2
    );
}

void GenSheet_Transparent_for_window(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture, float freeze_angle, float rota)
{
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 local_translateMatrix, translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, uni_translate, rotateYMatrix_rota;
    uni_translate = glm::translate(identityMatrix, glm::vec3(x_shift, y_shift, -1.0f * (z_shift)));
    float x_sft = local_x, y_sft = local_y, z_sft = local_z;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    local_translateMatrix = glm::translate(identityMatrix, glm::vec3(x_sft, y_sft, -1.0f * z_sft));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(trans_x, trans_y, -1.0f * trans_z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(len, height, -1.0 * width));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix_rota = glm::rotate(identityMatrix, glm::radians(rota), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(freeze_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = uni_translate * rotateZMatrix * rotateYMatrix * rotateYMatrix_rota * rotateXMatrix * translateMatrix * local_translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GenSheet_Transparent(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture)
{
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 local_translateMatrix, translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, uni_translate;
    uni_translate = glm::translate(identityMatrix, glm::vec3(x_shift, y_shift, -1.0f * (z_shift)));
    float x_sft = local_x, y_sft = local_y, z_sft = local_z;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    local_translateMatrix = glm::translate(identityMatrix, glm::vec3(x_sft, y_sft, -1.0f * z_sft));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(trans_x, trans_y, -1.0f * trans_z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(len, height, -1.0 * width));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = uni_translate * rotateZMatrix * rotateYMatrix * rotateXMatrix * translateMatrix * local_translateMatrix * scaleMatrix ;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GenSheet_Transparent_collapsible_gate_draw(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture, float sc)
{
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 local_translateMatrix, translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, uni_translate;
    uni_translate = glm::translate(identityMatrix, glm::vec3(x_shift, y_shift, -1.0f * (z_shift)));
    float x_sft = local_x, y_sft = local_y, z_sft = local_z;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    local_translateMatrix = glm::translate(identityMatrix, glm::vec3(x_sft, y_sft, -1.0f * z_sft));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(trans_x, trans_y, -1.0f * trans_z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(len * sc, height, -1.0 * width));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = uni_translate * rotateZMatrix * rotateYMatrix * rotateXMatrix * translateMatrix * local_translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GenSheet_Transparent_collapsible_gate_draw_2(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture, float sc)
{
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 local_translateMatrix, translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, uni_translate;
    uni_translate = glm::translate(identityMatrix, glm::vec3(x_shift, y_shift, -1.0f * (z_shift)));
    float x_sft = local_x, y_sft = local_y, z_sft = local_z;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    local_translateMatrix = glm::translate(identityMatrix, glm::vec3(x_sft, y_sft, -1.0f * z_sft));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(trans_x, trans_y, -1.0f * trans_z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(len * sc, height, -1.0 * width));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = uni_translate * rotateZMatrix * rotateYMatrix * rotateXMatrix * translateMatrix * local_translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void GenSheet_Transparent_2(unsigned int VAO, Shader ourShader, float len, float width, float height, float local_x, float local_y, float local_z, float rot_x, float rot_y, float rot_z, float trans_x, float trans_y, float trans_z, float x_shift, float y_shift, float z_shift, glm::vec4 color, unsigned int texture, float sh)
{
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 local_translateMatrix, translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, uni_translate;
    uni_translate = glm::translate(identityMatrix, glm::vec3(x_shift, y_shift, -1.0f * (z_shift)));
    float x_sft = local_x, y_sft = local_y, z_sft = local_z;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);//bindtexture
    local_translateMatrix = glm::translate(identityMatrix, glm::vec3(x_sft, y_sft, -1.0f * z_sft));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(trans_x, trans_y, -1.0f * trans_z));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(len, height, -1.0 * width));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 shearMatrix = createShearMatrix(0, 0, sh, 0, 0, 0);
    model = uni_translate * rotateZMatrix * rotateYMatrix * rotateXMatrix * translateMatrix * local_translateMatrix * scaleMatrix * shearMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void GenHalfCylinder_Transparent(
    unsigned int VAO, Shader ourShader, float radius, float height, int segments,
    float rot_x, float rot_y, float rot_z,
    float trans_x, float trans_y, float trans_z,
    unsigned int texture)
{
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 local_translateMatrix, translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, uni_translate;

    uni_translate = glm::translate(identityMatrix, glm::vec3(trans_x, trans_y, trans_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = uni_translate * rotateZMatrix * rotateYMatrix * rotateXMatrix;
    ourShader.setMat4("model", model);

    // Generate Half-Cylinder Geometry
    std::vector<float> vertices;
    for (int i = 0; i <= segments; ++i)
    {
        float theta = glm::pi<float>() * i / segments; // Angle for half-circle
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        float y_top = height / 2.0f;
        float y_bottom = -height / 2.0f;

        // Add top and bottom vertices
        vertices.insert(vertices.end(), { x, y_top, z, (float)i / segments, 1.0f });  // Top vertex
        vertices.insert(vertices.end(), { x, y_bottom, z, (float)i / segments, 0.0f }); // Bottom vertex
    }

    // Generate VAO/VBO for the half-cylinder
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Texture Coords
    glEnableVertexAttribArray(1);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw the half-cylinder
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (segments + 1) * 2);

    // Cleanup
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
}



//Texture Loading function
void load_texture(unsigned int& texture, string image_name, GLenum format)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_name.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture " << image_name << endl;
    }
    stbi_image_free(data);
}


void floor(unsigned int& cubeVAO, Shader& lightingShader)
{
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    //glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.2, 10.0));
    translate = glm::translate(identityMatrix, glm::vec3(-5.0, -1.0, -5.0));
    glm::mat4 model = translate * scale ;
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

    scale = glm::scale(identityMatrix, glm::vec3(0.1, 2.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(-5.0, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1, 32.0);

    //wall
    scale = glm::scale(identityMatrix, glm::vec3(-5.0, 5.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(4.9, -0.8, 2.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);
}
void frontWall(unsigned int& cubeVAO, Shader& lightingShader, Cylinder& cylinder, HemiSphere& hemisphere, Cone& cone)
{
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 model = translate * scale;
    ////wall
    //scale = glm::scale(identityMatrix, glm::vec3(0.1, 5.0, 10.0));
    //translate = glm::translate(identityMatrix, glm::vec3(4.9, -0.8, -5.0));
    //model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);
    ////mirror
    //scale = glm::scale(identityMatrix, glm::vec3(0.1, 1.0, 1.0));
    //translate = glm::translate(identityMatrix, glm::vec3(4.8, 2.5, 0.0));
    //model = translate * scale;
    //cylinder.drawCylinder(lightingShader, model);

    ////table
    //scale = glm::scale(identityMatrix, glm::vec3(2.0, 1.2, 2.0));
    //translate = glm::translate(identityMatrix, glm::vec3(3.0, -0.8, -1.0));
    //model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.212, 0.067, 0.031, 32.0);

    //basin
    scale = glm::scale(identityMatrix, glm::vec3(1.2, 1.2, 1.2));
    translate = glm::translate(identityMatrix, glm::vec3(4.0, 1.6, 0.0));
    model = translate * scale;
    hemisphere.drawHemiSphere(lightingShader, model);

    //lamp holder
    scale = glm::scale(identityMatrix, glm::vec3(0.6, 0.05, 0.05));
    translate = glm::translate(identityMatrix, glm::vec3(4.4f, 2.9f, 1.475f));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

    translate = glm::translate(identityMatrix, glm::vec3(4.4f, 2.9f, -1.525f));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

}
void rightWall(unsigned int& cubeVAO, Shader& lightingShader, CutCone& cutcone)
{
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 model = translate * scale;
    //wall
    scale = glm::scale(identityMatrix, glm::vec3(10.0, 2.5, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(-5.0, 1.7, 4.9));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);

    //commode
    scale = glm::scale(identityMatrix, glm::vec3(1.0, 1.0, 1.5));
    translate = glm::translate(identityMatrix, glm::vec3(-3, -0.8, 3.5));
    model = translate * scale;
    cutcone.drawCutCone(lightingShader, model);
    //commode
    scale = glm::scale(identityMatrix, glm::vec3(1.0, 0.1, 1.5));
    translate = glm::translate(identityMatrix, glm::vec3(-3, -0.1, 3.5));
    model = translate * scale;
    cutcone.drawCutCone(lightingShader, model);

    scale = glm::scale(identityMatrix, glm::vec3(1.7, 1.5, 0.7));
    translate = glm::translate(identityMatrix, glm::vec3(-3.85, -0.1, 4.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.122, 0.361, 0.357, 32.0);


}

void axis(unsigned int& cubeVAO, Shader& lightingShader)
{
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    //glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    //x
    scale = glm::scale(identityMatrix, glm::vec3(2.0, 0.1, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    glm::mat4 model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0, 32.0);
    //y
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 2.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 1.0, 0.0, 32.0);
    //z
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 2.0));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 1.0, 32.0);
}

void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float baseHeight = 0.3;
    float width = 1;
    float length = 2;
    float pillowWidth = 0.3;
    float pillowLength = 0.15;
    float blanketWidth = 0.8;
    float blanketLength = 0.7;
    float headHeight = 0.6;

    //base
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075, 32.0);

    //foam
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight, 0));
    scale = glm::scale(model, glm::vec3(width, 0.06, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361, 32.0);

    //pillow 1
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3((width / 2) - (0.1 + pillowWidth / 2), baseHeight + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0, 32.0);

    //pillow 2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3((-width / 2) + (0.1 + pillowWidth / 2), baseHeight + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0, 32.0);

    //blanket
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight + 1 * 0.06, -(length / 2 - 0.025) + blanketLength / 2));
    scale = glm::scale(model, glm::vec3(blanketWidth, 0.015, blanketLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.541, 0.169, 0.886, 32.0);

    //head
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0, (length / 2 - 0.02 / 2) + 0.02));
    scale = glm::scale(model, glm::vec3(width, headHeight, 0.02));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075, 32.0);

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    //if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    //if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    //{
    //    rotateAngle_X += 0.1;
    //    rotateAxis_X = 1.0;
    //    rotateAxis_Y = 0.0;
    //    rotateAxis_Z = 0.0;
    //}
    //if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    //{
    //    rotateAngle_Y += 0.1;
    //    rotateAxis_X = 0.0;
    //    rotateAxis_Y = 1.0;
    //    rotateAxis_Z = 0.0;
    //}
    //if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    //{
    //    rotateAngle_Z += 0.1;
    //    rotateAxis_X = 0.0;
    //    rotateAxis_Y = 0.0;
    //    rotateAxis_Z = 1.0;
    //}

    //if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    //{
    //    eyeX += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    //{
    //    eyeX -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    //{
    //    eyeZ += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    //{
    //    eyeZ -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    //{
    //    eyeY += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    //{
    //    eyeY -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}

}
//void ambienton_off(Shader& lightingShader)
//{
//    double currentTime = glfwGetTime();
//    if (currentTime - lastKeyPressTime < keyPressDelay) return;
//    lightingShader.use();
//    if (AmbientON)
//    {
//        pointlight1.turnAmbientOff();
//        pointlight2.turnAmbientOff();
//        pointlight3.turnAmbientOff();
//        pointlight4.turnAmbientOff();
//        lightingShader.setVec3("directionalLight.ambient", 0.0f, 0.0f, 0.0f);
//        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
//        AmbientON = !AmbientON;
// //    double currentTime = glfwGetTime();
//    if (currentTime - lastKeyPressTime < keyPressDelay) return;
//        lastKeyPressTime = currentTime;
//    }
//    else
//    {
//        pointlight1.turnAmbientOn();
//        pointlight2.turnAmbientOn();
//        pointlight3.turnAmbientOn();
//        pointlight4.turnAmbientOn();
//        lightingShader.setVec3("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
//        lightingShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
//        AmbientON = !AmbientON;
//        lastKeyPressTime = currentTime;
//    }
//}
//void diffuse_on_off(Shader& lightingShader)
//{
//    double currentTime = glfwGetTime();
//    if (currentTime - lastKeyPressTime < keyPressDelay) return;
//    lightingShader.use();
//    if (DiffusionON)
//    {
//        pointlight1.turnDiffuseOff();
//        pointlight2.turnDiffuseOff();
//        pointlight3.turnDiffuseOff();
//        pointlight4.turnDiffuseOff();
//        lightingShader.setVec3("directionalLight.diffuse", 0.0f, 0.0f, 0.0f);
//        lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
//        DiffusionON = !DiffusionON;
//        lastKeyPressTime = currentTime;
//    }
//    else
//    {
//        pointlight1.turnDiffuseOn();
//        pointlight2.turnDiffuseOn();
//        pointlight3.turnDiffuseOn();
//        pointlight4.turnDiffuseOn();
//        lightingShader.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
//        lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
//        DiffusionON = !DiffusionON;
//        lastKeyPressTime = currentTime;
//    }
//}
//void specular_on_off(Shader& lightingShader)
//{
//    double currentTime = glfwGetTime();
//    if (currentTime - lastKeyPressTime < keyPressDelay) return;
//    lightingShader.use();
//    if (SpecularON)
//    {
//        pointlight1.turnSpecularOff();
//        pointlight2.turnSpecularOff();
//        pointlight3.turnSpecularOff();
//        pointlight4.turnSpecularOff();
//        lightingShader.setVec3("directionalLight.specular", 0.0f, 0.0f, 0.0f);
//        lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
//        SpecularON = !SpecularON;
//        lastKeyPressTime = currentTime;
//    }
//    else
//    {
//        pointlight1.turnSpecularOn();
//        pointlight2.turnSpecularOn();
//        pointlight3.turnSpecularOn();
//        pointlight4.turnSpecularOn();
//        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
//        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
//        SpecularON = !SpecularON;
//        lastKeyPressTime = currentTime;
//    }
//}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    //{
    //    if (pointLightOn)
    //    {
    //        pointlight1.turnOff();
    //        pointlight2.turnOff();
    //        pointlight3.turnOff();
    //        pointlight4.turnOff();
    //        pointLightOn = !pointLightOn;
    //    }
    //    else
    //    {
    //        pointlight1.turnOn();
    //        pointlight2.turnOn();
    //        pointlight3.turnOn();
    //        pointlight4.turnOn();
    //        pointLightOn = !pointLightOn;
    //    }
    //}

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointlight5.turnOff();
            pointlight6.turnOff();
            pointlight7.turnOff();
            pointlight8.turnOff();
            pointlight9.turnOff();
            pointlight10.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointlight5.turnOn();
            pointlight6.turnOn();
            pointlight7.turnOn();
            pointlight8.turnOn();
            pointlight9.turnOn();
            pointlight10.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        directionalLightOn = !directionalLightOn;
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (SpotLightOn)
        {
            spotlight1.turnOff();
            SpotLightOn = !SpotLightOn;
        }
        else
        {
            spotlight1.turnOn();
            SpotLightOn = !SpotLightOn;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void renderMirrorSurface(unsigned int mirrorTexture, const glm::mat4& projection, Shader& mirrorShaderProgram, const glm::vec3& mirrorPosition, glm::mat4& mirroredView) {
    // Define the mirror plane's vertices (a simple quad)
    float mirrorVertices[] = {
        // positions          // texture coordinates
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f
    };

    unsigned int mirrorIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    // VAO, VBO, and EBO static for reuse across multiple calls
    static unsigned int VAO = 0, VBO = 0, EBO = 0;

    if (VAO == 0) {
        // Generate and bind VAO, VBO, and EBO for the mirror plane
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Upload vertex data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mirrorVertices), mirrorVertices, GL_STATIC_DRAW);

        // Upload index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mirrorIndices), mirrorIndices, GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position attribute
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Texture coordinate attribute
        glEnableVertexAttribArray(1);

        glBindVertexArray(0); // Unbind VAO
    }

    // Activate the mirror shader program
    mirrorShaderProgram.use();

    // Bind the mirror texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mirrorTexture);
    mirrorShaderProgram.setInt("mirrorTexture", 0); // Bind texture unit to the uniform

    // Set model, view, and projection matrices
    glm::mat4 model = glm::translate(glm::mat4(1.0f), mirrorPosition);
    // Introduce rotation
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    float mirrorSize = 2.0f; // Adjust size as needed
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));// Position the mirror in the scene
    glm::mat4 view = camera.GetViewMatrix(); // Assuming you have a camera class
    glm::mat4 MVP = projection * view * model;
    mirrorShaderProgram.setMat4("MVP", MVP); // Send MVP matrix to the shader

    // Render the mirror plane
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind VAO
}

