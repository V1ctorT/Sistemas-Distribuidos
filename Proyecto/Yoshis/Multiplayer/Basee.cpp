/* 
-----------------------------------------------------------------------------------------
	OpenGl 3.3 Proyecto Final Sistemas Distribuidos
	###########################Beisbol con multijugador#####################################
	Funciones principales
		Mover a Yoshi por el escenario
		Conectar de 2 a 4 jugadores
			Estos jugadores se moveran en el escenario
		Opcional -> Que haya una pelota por jugador y la pueda lanzar y todos la vean
		Opcional -> Poner mas de 4 jugadores (maximo 8?)

	///////////////////////////PROGRAMA SERVIDOR////////////////////////////////////////////
	Comunicacion UDP
		Direccion Servidor 192.168.10.1
		Direccion Cliente 192.168.10.2-n

	Creado por:
	Guadalupe Isabela De la Fuente Flores (Iluminacion y Carga de Modelos)
	Victor Manuel Torres Martinez (Programacion y Conexion Multijugador)
-----------------------------------------------------------------------------------------
*/
//Librerias para Sockets-----------------------------------------------------------------
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
//Libreria de Cliente-Servidor-----------------------------------------------------------
#include "Server.h"
#include "Client.h"
#include <glm/glm.hpp>
//Librerias de OpenGL y GLFW ------------------------------------------------------------
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// Other Libs
#include "stb_image.h"
#include <chrono>
#include <thread>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//Load Models
#include "SOIL2/SOIL2.h"
glm::vec3 pelotaPosicion(0.0f, 0.0f, 0.0f);

//Librerias para Carga de modelos y camara-----------------------------------------------
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
//Animaciones de publico y corredores----------------------------------------------------
#include "Publico.h"
#include "Corredores.h"
Publico publico;
Corredores corredoresA, corredoresB;
Corredores* ajuste = &corredoresA;   // el que se mueve con el teclado
int noCorrA = 3;
int noCorrB = 3;
int estado = 0; //0 - Caminar, 1 - Bateo, 2 - Atrapar 3 - Lanzar
int corredor = -1;


// Function prototypes-------------------------------------------------------------------
//Entradas del teclado y mouse
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);

//Funciones para de camara y movimientos
void DoMovement();
void ambientacion();

//Reajustar espacio de Renderizado
void resize(GLFWwindow *window, int width, int height);
void imprimirPosicion(glm::vec3 posiciones[], int size);
//---------------------------------------------------------------------------------------
//Camara---------------------------------------------------------------------------------
//Limite de movimiento de la camara
void limiteCamara();
int modoC = 0; //0 yoshi, 1 tercera persona, 2 huesitos lanzador, 3 pelota
GLfloat cameraDistance = 5.0f;
GLfloat cameraHeight = 2.0f;
Camera  camera(glm::vec3(0.0f, 0.0f, 0.0f));
//---------------------------------------------------------------------------------------
//Ventana y mouse dimensiones------------------------------------------------------------
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH = WIDTH, SCREEN_HEIGHT = HEIGHT;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
const double FPS = 60.0f;
const double limitFPS = 1.0 / FPS;
//---------------------------------------------------------------------------------------
//Rutinas--------------------------------------------------------------------------------
//Sin implementar aun
void rutLanzar();
bool lanzar = false;
uint8_t obtenerTeclas(GLFWwindow* window);
//---------------------------------------------------------------------------------------
//Luces e iluminacion--------------------------------------------------------------------
// Posiciones de Las Luces
glm::vec3 pointLightPositions[] = {
	//Lampara1
	glm::vec3(40.7534f, 46.5365f, 52.7199f),

	glm::vec3(-52.2116f,46.4115 ,-40.2842f ),

	glm::vec3(59.7414f, 40.5053f, -108.932f),


	glm::vec3(73.534f, 46.536f, 20.756f),
	glm::vec3(73.534f, 43.771f, 20.756f),

	glm::vec3(71.347f, 49.302f, 22.178f),
	glm::vec3(71.347f, 46.536f, 22.178f),
	glm::vec3(71.347f, 43.771f, 22.178f),

	glm::vec3(69.144f, 49.302f, 23.600f),
	glm::vec3(69.144f, 46.536f, 23.600f),
	glm::vec3(69.144f, 43.771f, 23.600f),

	glm::vec3(67.441f, 49.302f, 25.163f),
	glm::vec3(67.441f, 46.536f, 25.163f),
	glm::vec3(67.441f, 43.771f, 25.163f),

	glm::vec3(65.753f, 49.302f, 27.100f),
	glm::vec3(65.753f, 46.536f, 27.100f),
	glm::vec3(65.753f, 43.771f, 27.100f),

	glm::vec3(64.050f, 49.302f, 29.053f),
	glm::vec3(64.050f, 46.536f, 29.053f),
	glm::vec3(64.050f, 43.771f, 29.053f),

	glm::vec3(62.378f, 49.302f, 30.991f),
	glm::vec3(62.378f, 46.536f, 30.991f),
	glm::vec3(62.378f, 43.771f, 30.991f),

	glm::vec3(61.066f, 49.302f, 32.944f),
	glm::vec3(61.066f, 46.536f, 32.944f),
	glm::vec3(61.066f, 43.771f, 32.944f),

	glm::vec3(59.987f, 49.302f, 35.288f),
	glm::vec3(59.987f, 46.536f, 35.288f),
	glm::vec3(59.987f, 43.771f, 35.288f),

	glm::vec3(58.909f, 49.302f, 37.647f),
	glm::vec3(58.909f, 46.536f, 37.647f),
	glm::vec3(58.909f, 43.771f, 37.647f)
};
//Variables de Iluminacion
int estadoLuces = 0;   // 0 apagado, 1 lámparas, 2 spotlights, 3 luz general
int ambiente = 2;
glm::vec3 colorLamparas(1.0f, 1.0f, 0.0f);
struct ConfigFiesta
{
	glm::vec3 postes[3] = {
	glm::vec3(70.768f,  7.6772f, -112.43f),
	glm::vec3(88.346f,  7.6772f, -100.52f),
	glm::vec3(106.8f,   7.6772f, -88.04f)
	};
	glm::vec3 colores[3] = {
		glm::vec3(1.0f, 0.1f, 0.1f),   // poste 1: rojo
		glm::vec3(0.1f, 1.0f, 0.2f),   // poste 2: verde
		glm::vec3(0.2f, 0.3f, 1.0f)    // poste 3: azul
	};
	glm::vec3 objetivo = glm::vec3(23.0f, 2.911f, -17.0f); // centro del campo
	GLfloat radioBarrido = 20.0f;
	GLfloat velocidadGiro = 1.0f;
	GLfloat intensidad = 1.5f;
	GLfloat suavizado = 6.0f;
	GLfloat apertura = 10.0f;
	GLfloat linear = 0.002f;
	GLfloat quadratic = 0.00002f;
	GLfloat opacidadHaz = 0.3f;
	GLfloat aperturaHaz = 3.0f;
} fiesta;
glm::vec3 dirsHaz[3];
GLfloat largoHaz[3];
#include "Iluminacion.h"
//---------------------------------------------------------------------------------------
//Cubo Unitario para los modelos
float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
//---------------------------------------------------------------------------------------
//Posicion del Personajes----------------------------------------------------------------
glm::vec3 posicionPersonaje(0.0f, 0.5f, 0.0f);
glm::vec3 posicionHuesitos(15.547f, 3.47702f, -16.07f);
glm::vec3 posicionToad(3.0f, 0.43065f, -3.0f);
glm::vec3 posicionJugador1(-2.0f,0.5f,0.0f);
glm::vec3 posicionJugador2(2.0f, 0.5f, 0.0f);
float yawJugador1 = 0.0f;
float yawJugador2 = 0.0f;
//---------------------------------------------------------------------------------------
//Rutinas y Utilerias--------------------------------------------------------------------
#include "Limites.h"
GLfloat rotPierna = 0.0f;
GLfloat rotacionBrazo = 0.0f;
GLfloat rotCabeza = 0.0f;
float rotPel = 0.0f;
#include "Bateo.h"
//Bate Actual
int noBate = -1;
//Caminar
GLfloat fasePaso = 0.0f;          // avanza solo mientras caminas
GLfloat intensidadCorrer = 0.0f;  // 0 = quieto, 1 = corriendo (transición suave)
//---------------------------------------------------------------------------------------
// Cliente-Servidor----------------------------------------------------------------------
bool esServidor = true;

//---------------------------------------------------------------------------------------
//Main Function--------------------------------------------------------------------------
int main()
{
	std::cout << "=== INICIO DEL PROGRAMA ===" << std::endl;
	std::cout << "Entrando a main()" << std::endl;
	//Creacion del contexto de OpenGL y GLFW---------------------------------------------
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	// Create a GLFWwindow object that we can use for GLFW's functions
	if (!glfwInit())
	{
		std::cerr << "Error: no se pudo inicializar GLFW" << std::endl;
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(
		WIDTH, HEIGHT, "Beisbol Proyecto", nullptr, nullptr
	);

	if (window == nullptr)
	{
		std::cerr << "Error: no se pudo crear la ventana" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwSetFramebufferSizeCallback(window, resize);
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	// GLFW Options
	//Ocultar el cursor del mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set this to true so GLEW knows to use a modern approach to retrieving 
	// function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// Desactiva V-Sync para controlar el tiempo manualmente
	glfwSwapInterval(0);
	double lastTime = glfwGetTime();
	std::cout << "Prueba de consola" << std::endl;
	// Cliente-Servidor----------------------------------------------------------------------
	Server servidor;
	Client cliente;
	std::cout << "Valor esServidor: " << esServidor << std::endl;
	if(esServidor)
	{
		if (!servidor.iniciarServidor(3000)) {
			std::cout << "No se pudo inicializar el servidor\n";
			return -1;
		}
		std::cout << "Servidor UDP iniciado\n";
		std::cout << "Escuchando en puerto: 3000\n";
	}
	else {
		if (!cliente.conectarServidor("192.168.1.11", 3000)) {
			std::cout << "No se pudo conectar al servidor\n";
			return -1;
		}
		std::cout << "Cliente UDP iniciado\n";
		std::cout << "Enviando en puerto: 3000\n";
	}
	//-----------------------------------------------------------------------------------
	//Carga de Shaders ------------------------------------------------------------------
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader beamShader("Shader/beam.vs", "Shader/beam.frag");
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//Carga de Modelos
	Model estadio((char*)"Modelos/Stadium/SMestadio.obj");
	//Personajes-------------------------------------------------------------------------
	Model yoshis((char*)"Modelos/YoshiOBJ/yoshi.obj"); //Completo
	Model huesittos2((char*)"Modelos/YoshiOBJ/Red Dry Bones/bloody_karon.obj"); //Completo
	//Pelotas----------------------------------------------------------------------------
	Model pelotaa((char*)"Modelos/Pelotas/pelota.obj");
	//Gorras-----------------------------------------------------------------------------
	Model quico((char*)"Modelos/Publico/Gorras/Quico_Gorra.obj");
	Model donRamon((char*)"Modelos/Publico/Gorras/Don_Ramon_Gorra.obj");
	Model referee((char*)"Modelos/Publico/Gorras/Referee.obj");
	Model amarillo((char*)"Modelos/Publico/Gorras/CascoAmarillo.obj");
	Model azul((char*)"Modelos/Publico/Gorras/CascoAzul.obj");
	Gorras gorras = {&quico,&donRamon,&referee, &amarillo, &azul};
	//Yoshi------------------------------------------------------------------------------
	Model yoshiH((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCabeza.obj");
	Model yoshiCuerpo((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCuerpo.obj");
	Model yoL_hand((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoL_Hand.obj");
	Model yoBrazo_L((char*)"Modelos/YoshiOBJ/yoshi_Partes/YL_Brazo.obj");
	Model yoBrazo_R((char*)"Modelos/YoshiOBJ/yoshi_Partes/YD_Brazo.obj");
	Model yoD_hand((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoD_Hand.obj");
	Model yoPierna_L((char*)"Modelos/YoshiOBJ/yoshi_Partes/Y_LegI.obj");
	Model yoPierna_R((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoLegDer.obj");
	PartesYoshi partes = { &yoshiCuerpo, &yoshiH, &yoBrazo_L, &yoL_hand,
					   &yoBrazo_R, &yoD_hand, &yoPierna_L, &yoPierna_R, &pelotaa };
	//Huesitos---------------------------------------------------------------------------
	Model hCabeza((char*)"Modelos/YoshiOBJ/huesitos_Partes/HCabeza.obj");
	Model hCuerpo((char*)"Modelos/YoshiOBJ/huesitos_Partes/HCuerpo.obj");
	Model hManoD((char*)"Modelos/YoshiOBJ/huesitos_Partes/HManoD.obj");
	Model hBrazoI((char*)"Modelos/YoshiOBJ/huesitos_Partes/HBrazoI.obj");
	Model hBrazoD((char*)"Modelos/YoshiOBJ/huesitos_Partes/HBrazoD.obj");
	Model hManoI((char*)"Modelos/YoshiOBJ/huesitos_Partes/HManoI.obj");
	Model hPataI((char*)"Modelos/YoshiOBJ/huesitos_Partes/HPataI.obj");
	Model hPataD((char*)"Modelos/YoshiOBJ/huesitos_Partes/HPataD.obj");
	PartesYoshi huesitos = {&hCuerpo,&hCabeza,&hBrazoI,&hManoI,&hBrazoD,
		&hManoD,&hPataI,& hPataD,& pelotaa };
	//Toad-------------------------------------------------------------------------------
	Model tBody((char*)"Modelos/Publico/Toad/Toad_Body.obj");
	Model tCabeza((char*)"Modelos/Publico/Toad/Toad_Head.obj");
	Model tManoD((char*)"Modelos/Publico/Toad/Toad_BArm.obj");
	Model tManoI((char*)"Modelos/Publico/Toad/Toad_LArm.obj");
	Model tPataD((char*)"Modelos/Publico/Toad/Toad_DLeg.obj");
	Model tPataI((char*)"Modelos/Publico/Toad/Toad_ILeg.obj");
	
	//Modelos de ambientacion y utileria-------------------------------------------------
	//Bates
	Model bateStel((char*)"Modelos/Bates/StellesBat/bate.obj");
	Model bateAl((char*)"Modelos/Bates/Aluminio/bateAl.obj");
	Model bateMadera((char*)"Modelos/Bates/Madera/MaderaBate.obj");
	Model bateHmmm((char*)"Modelos/Bates/bate/bateHmm.obj");
	Bates bates = { &bateStel ,&bateAl ,&bateMadera ,&bateHmmm };
	Model pelota((char*)"Modelos/Pelotas/pelota.obj");
	//-----------------------------------------------------------------------------------
	//Bloques de Publico-----------------------------------------------------------------
	 // piso 1 (4.83f para el piso 2)
	const GLfloat YC = 1.0f;           // altura del Yoshi corriendo sobre el piso 1
	publico.init(partes);

	// ---- BLOQUE 1 ----
	publico.bloque4(glm::vec3(36.2f, 2.911f, 31.77f),
		glm::vec3(55.0f, 2.911f, 27.0f),25.0f);
	// ---- BLOQUE 2 ----
	publico.bloque4(glm::vec3(58.739f, 2.911f, 25.083f),
		glm::vec3(75.16f, 2.911f, 12.428f),35.0f);
	// ---- BLOQUE 3 ----
	publico.bloque4(glm::vec3(85.908f, 2.911f, -9.8168f),
		glm::vec3(90.346f, 2.911f, -28.475f),80.0f);
	// ---- BLOQUE 4 ----
	publico.bloque4(glm::vec3(90.3434f, 2.911f, -32.85f),
		glm::vec3(84.047f, 2.911f, -57.923f),100.0f);
	// ---- BLOQUE 5 ----
	publico.bloque4(glm::vec3(42.786f, 2.911f, -89.643f),
		glm::vec3(2.7034f, 2.911f, -82.71f),200.0f);
	// ---- BLOQUE 6 ----
	publico.bloque4(glm::vec3(-4.93f, 2.911f, -78.154f),
		glm::vec3(-29.234f, 2.911f, -45.706f),230.0f);
	// ---- BLOQUE 7 ----
	publico.bloque4(glm::vec3(-32.327f, 2.911f, -9.921f),
		glm::vec3(-21.9118f, 2.911f, 19.766f),300.0f);
	// ---- BLOQUE 8 ----
	publico.bloque4(glm::vec3(-18.508f, 2.911f, 22.85f),
		glm::vec3(10.161f, 2.911f, 33.105f),0.0f);

	// Banca 1 (frente a z ≈ 30, recorrido a lo largo de X)
	corredoresA.init(partes);
	corredoresA.centro = glm::vec3(22.5f, YC, 19.0f);
	corredoresA.rotPista = 0.0f;
	corredoresA.radioX = 8.0f;
	corredoresA.radioZ = 1.5f;
	corredoresA.exponente = 4.0f;          // 2 = elipse, 4-8 = más cuadrado
	corredoresA.agregar(noCorrA);

	// Banca 2 (frente a x ≈ -29.3, recorrido a lo largo de Z)
	noCorrB = 3;
	corredoresB.init(huesitos);
	corredoresB.centro = glm::vec3(-18.0f, YC, -22.5f);
	corredoresB.rotPista = 90.0f;
	corredoresB.radioX = 8.0f;
	corredoresB.radioZ = 1.5f;
	corredoresB.exponente = 4.0f;
	corredoresB.agregar(noCorrB);
	//-----------------------------------------------------------------------------------
	//First, set the container's VAO (and VBO)------------------------------------------- 
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//-----------------------------------------------------------------------------------
	//Configuracion de luces tipo spootlight---------------------------------------------
	// Cono para los haces: punta en (0,0,0), base en y = -1 con radio 1
	std::vector<float> cono;
	const int SEG = 24;
	for (int i = 0; i < SEG; i++)
	{
		float a0 = glm::two_pi<float>() * i / SEG;
		float a1 = glm::two_pi<float>() * (i + 1) / SEG;
		cono.insert(cono.end(), { 0.0f, 0.0f, 0.0f,
								  cosf(a0), -1.0f, sinf(a0),
								  cosf(a1), -1.0f, sinf(a1) });
	}
	GLuint coneVAO, coneVBO;
	glGenVertexArrays(1, &coneVAO);
	glGenBuffers(1, &coneVBO);
	glBindVertexArray(coneVAO);
	glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
	glBufferData(GL_ARRAY_BUFFER, cono.size() * sizeof(float), cono.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	//-----------------------------------------------------------------------------------
	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//Intervalos de Tiempo
	GLfloat tiempoAnterior = glfwGetTime();
	const double intervalo = 0.016666f;
	lastFrame = tiempoAnterior;
	//-----------------------------------------------------------------------------------
	// Game loop-------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		
		//Intervalos de tiempo-----------------------------------------------------------
		GLfloat tiempoActual = glfwGetTime();
		if (tiempoActual - tiempoAnterior >= intervalo)
		{
			deltaTime = tiempoActual - lastFrame;
			lastFrame = tiempoAnterior;
			tiempoAnterior = tiempoActual;

			// Check if any events have been activiated and call corresponding response functions
			glfwPollEvents();
			DoMovement();
			ambientacion();
			//0 yoshi, 1 tercera persona
			//3 pelota 2 huesitos lanzador 
			switch (modoC)
			{
			case 0://Primera Persona
				camera.PrimeraPersona(posicionJugador1);
				break;
			case 1://Yoshi tercera persona
				camera.UpdatePivote(posicionJugador1, 5.0f, 1.5f);
				break;
			case 2: //Pelota
				camera.UpdatePivote(pelotaPosicion, 5.0f, 1.5f);
				break;
			case 3: //Huesitos lanzador
				camera.UpdatePivote(posicionHuesitos, 5.0f, 1.5f);
				break;
			default: // Por si toma un valor inesperado
				camera.PrimeraPersona(posicionPersonaje);
				break;
			}

			//Leer teclas
			uint8_t teclas = obtenerTeclas(window);
			glm::vec3 direccion = camera.GetFront();
			direccion.y = 0.0f;
			direccion = glm::normalize(direccion);
			glm::vec3 derecha = glm::normalize(glm::cross(direccion, glm::vec3(0.0f, 1.0f, 0.0f)));
			//Servidor-------------------------------------------------------------------
			if (esServidor) 
			{
				yawJugador1 = -camera.GetYaw() + 290.0f;
				//Recibir mensajes
				servidor.recibirMensajes();
				//Actualizar jugadores
				servidor.actualizar(deltaTime, teclas, direccion,derecha);
				//Enviar posiciones
				servidor.enviarPosiciones();
				posicionJugador1 = glm::vec3(
					servidor.getPlayer1X(),
					servidor.getPlayer1Y(),
					servidor.getPlayer1Z());
				
				posicionJugador2 = glm::vec3(
					servidor.getPlayer2X(),
					servidor.getPlayer2Y(),
					servidor.getPlayer2Z());
				
				yawJugador1 = -camera.GetYaw() + 290.0f;
				yawJugador2 = servidor.getPlayer2Yaw();
			}
			else //Cliente---------------------------------------------------------------
			{
				float yawLocal = -camera.GetYaw() + 290.0f;
				// Enviar teclas al servidor
				cliente.enviarTeclas(teclas, yawLocal);
				// Recibir posiciones
				cliente.recibirPosiciones();
				//Recibir posiciones
				ServerMessage state = cliente.getGameState();
				posicionJugador1 = glm::vec3(
					state.player1X,state.player1Y,state.player1Z);
				posicionJugador2 = glm::vec3(
					state.player2X,state.player2Y,state.player2Z);
				float yawJugador1 = state.player1Yaw;
				float yawJugador2 = state.player2Yaw;
			}

			glm::mat4 projection = glm::perspective(camera.GetZoom(),
				(GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 500.0f);


			// Clear the colorbuffer
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// OpenGL options
			glEnable(GL_DEPTH_TEST);

			// Use cooresponding shader when setting uniforms/drawing objects
			lightingShader.Use();
			glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
			//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

			GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
			glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


			// Luces light-------------------------------------------------------------------
			bool lamparasOn = (estadoLuces == 1);
			bool spotOn = (estadoLuces == 2);
			bool generalOn = (estadoLuces == 3);

			iluminacion(lightingShader, tiempoActual, lamparasOn, spotOn, generalOn);


			//Camera transformations---------------------------------------------------------
			glm::mat4 view;
			view = camera.GetViewMatrix();

			// Get the uniform locations
			GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
			GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
			GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

			// Pass the matrices to the shader
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


			glm::mat4 model(1);
			glm::mat4 modelTemp(1);
			glm::mat4 modelCuerpo(1);

			//Carga de escena----------------------------------------------------------------------
			view = camera.GetViewMatrix();

			//Escenario Principal------------------------------------------------------------------
			model = glm::mat4(1);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			estadio.Draw(lightingShader);

			publico.actualizar(deltaTime);
			publico.dibujar(modelLoc, lightingShader, camera.GetPosition(), camera.GetFront(),
				tiempoActual, gorras);


			//Yoshi, personaje principal-----------------------------------------------------------
			// Correr solo cuando se presiona una tecla de movimiento
			bool moviendo = keys[GLFW_KEY_W] || keys[GLFW_KEY_A] || keys[GLFW_KEY_S] || keys[GLFW_KEY_D]
				|| keys[GLFW_KEY_UP] || keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT];
			GLfloat objetivoCorrer = moviendo ? 1.0f : 0.0f;
			intensidadCorrer += (objetivoCorrer - intensidadCorrer) * glm::min(1.0f, 10.0f * deltaTime);
			if (moviendo) fasePaso += deltaTime * 8.0f;

			GLfloat rotacionBrazo = sin(fasePaso) * 25.0f * intensidadCorrer;
			rotPierna = rotacionBrazo * 3.0f;
			
			glm::mat4 baseJugador(1.0f);
			baseJugador = glm::translate(baseJugador, posicionJugador1 + glm::vec3(0.0f, 1.159f, 0.0f));
			baseJugador = glm::rotate(baseJugador, glm::radians(yawJugador1), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 baseJugador2(1.0f);
			baseJugador2 = glm::translate(baseJugador2, posicionJugador2 + glm::vec3(0.0f, 1.159f, 0.0f));
			baseJugador2 = glm::rotate(baseJugador2, glm::radians(yawJugador2), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 modelManoYoshi(1.0f);
			glm::mat4 modelManoHuesitos(1.0f);
			corredoresA.agregar(noCorrA);
			corredoresB.agregar(noCorrB);

			corredoresA.dibujar(modelLoc, lightingShader, tiempoActual, baseJugador, 1, gorras);
			corredoresB.dibujar(modelLoc, lightingShader, tiempoActual, baseJugador, 2, gorras);
			glm::mat4 huesitoss(1.0f);
			huesitoss = glm::translate(huesitoss, posicionHuesitos);

			//---------------------------------------------------------------------------
			//Dibujo de personajes-------------------------------------------------------
			dibujarYoshi(
				modelLoc, lightingShader,
				partes, baseJugador,
				rotacionBrazo, 0.0f,
				rotacionBrazo, rotPierna,
				modelManoYoshi, gorras
			);
			dibujarYoshi(
				modelLoc, lightingShader,
				partes, baseJugador2,
				rotacionBrazo, 0.0f,
				rotacionBrazo, rotPierna,
				modelManoYoshi, gorras
			);

			dibujarBates(modelLoc, lightingShader, modelManoYoshi, noBate, bates);

			if (lanzar) {

			}

			glBindVertexArray(0);

			if (spotOn) //Animacion de luz spotlight-----------------------------------------
			{
				iluminacion2(beamShader, view, projection, coneVAO);
			}

			// Swap the screen buffers
			glfwSwapBuffers(window);

			// Calcular tiempo transcurrido y esperar lo restante

		}
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

void ambientacion() {
	//Ambiente
	if (keys[GLFW_KEY_1])
	{
		ambiente = 0;
	}
	if (keys[GLFW_KEY_2])
	{
		ambiente = 1;
	}
	if (keys[GLFW_KEY_3])
	{
		ambiente = 2;
	}
}

void resize(GLFWwindow* window, int width, int height)
{
	SCREEN_HEIGHT = height;
	SCREEN_WIDTH = width;
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void imprimirPosicion(glm::vec3 posiciones[], int size)
{
	system("cls");
	for (int i = 0; i < size; i++)
	{
		std::cout << "Posicion Camara" << camera.GetPosition().x << ", " << camera.GetPosition().y << ", " << camera.GetPosition().z << ", " << camera.GetYaw() << "\n" << std::endl;
		std::cout << "Posicion Personaje" << posicionPersonaje.x << ", " << posicionPersonaje.y << ", " << posicionPersonaje.z <<"\n" << std::endl;
	}
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls--------------------------------------------------------------

	glm::vec3 direccion = camera.GetFront();
	direccion.y = 0.0f;
	direccion = glm::normalize(direccion);
	glm::vec3 derecha = glm::normalize(glm::cross(direccion, glm::vec3(0.0f, 1.0f, 0.0f)));
	GLfloat velocity = 5.0f * deltaTime;

	moverPersonaje(direccion,derecha);


	if(keys[GLFW_KEY_R])
	{
		camera.ProcessKeyboard(RESET, deltaTime);
	}
	


	if (keys[GLFW_KEY_T] )
	{

	}

	if (keys[GLFW_KEY_Y] )
	{

	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// Encender y apagar luz con SPACE---------------------------------------------------
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		estadoLuces = (estadoLuces + 1) % 4;
	}
	//Cambiar de modo de camara con C----------------------------------------------------
	if (keys[GLFW_KEY_C] && action == GLFW_PRESS)
	{
		modoC = (modoC + 1) % 4;
	}
	//Accionar el publico----------------------------------------------------------------
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		publico.alternar();
	}
	//Ajuste de Corredores---------------------------------------------------------------
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		ajuste = (ajuste == &corredoresA) ? &corredoresB : &corredoresA;
	//Imprime centro corredores----------------------------------------------------------
	if (key == GLFW_KEY_F)
	{
		
	}
	//Lanzamiento de la pelota-----------------------------------------------------------
	if(key == GLFW_KEY_G)
	{
		
	}

	if (key == GLFW_KEY_Q)
	{
		
	}
	
	//Zonas------------------------------------------------------------------------------
	if (keys[GLFW_KEY_ENTER] && action == GLFW_PRESS)
	{
		//Zona Para Lanzar
		if (posicionPersonaje.x >= 11.47f && posicionPersonaje.x <= 15.94f
			&& posicionPersonaje.z >= -15.758f && posicionPersonaje.z <= -11.374f)
		{
			lanzar = true;
		}
		corredor = dentroCorredores(
			glm::vec2(posicionPersonaje.x,posicionPersonaje.z));
		if (corredor == 1) {
			noCorrA += 1;
		}
		else if (corredor == 2)
		{
			noCorrB += 1;
		}
	}
	//Agarrar Bate-----------------------------------------------------------------------
	if (keys[GLFW_KEY_E] && action == GLFW_PRESS)
	{
		//camera.ProcessKeyboard(UP, deltaTime);
		if (noBate == -1)
		{
			int bateCercano = detectarBate(posicionPersonaje);

			if (bateCercano != -1)
			{
				noBate = bateCercano;

				printf("Agarraste el bate %d\n", noBate);
			}
		}
		else {
			soltarBate(noBate);
		}
	}
	if (keys[GLFW_KEY_0] && action == GLFW_PRESS) 
	{
		estado = 0;
		soltarBate(noBate);
		noBate = -1;
	}
	//Animacion de Bateo
	
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset, pelotaPosicion,true, modoC);
}

uint8_t obtenerTeclas(GLFWwindow* window) {

	uint8_t keys = KEY_NONE;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		keys |= KEY_W;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		keys |= KEY_A;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		keys |= KEY_S;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		keys |= KEY_D;

	return keys;
}