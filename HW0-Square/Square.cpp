//HW 0 - Moving Square
//Starter code for the first homework assignment.
//This code assumes SDL2 and OpenGL are both properly installed on your system

#include "glad/glad.h"  //Include order can matter here
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cstdio>
#include <math.h>
#define PI acos(-1.0)
#include <iostream>
using namespace std;

//Globals to store the state of the square (position, width, and angle)
float g_pos_x = 0.0f;
float g_pos_y = 0.0f;
float g_size = 0.25f;
float g_angle = 0;
float a = 0.0f;
float b = 0.0f;
float sina;


float vertices[] = {  //These values should be updated to match the square's state when it changes
  0.25f,  0.25f,  // top right
  0.25f, -0.25f,  // bottom right
  -0.25f,  0.25f,  // top left
  -0.25f, -0.25f,  // bottom left
};

int screen_width = 800;
int screen_height = 800;

float g_mouse_down = false;

void mouseClicked(float mx, float my);
void mouseDragged(float mx, float my);

bool g_bTranslate = false;
bool g_bRotate = false;
bool g_bScale = false;


//////////////////////////
///  Begin your code here
/////////////////////////

//TODO: Account for rotation
void updateVertices(){
  if(fabs(g_angle) > 0)
  {
    float rtx,rty,rdx,rdy,ltx,lty,ldx,ldy;
    rtx = g_pos_x + g_size;  //Top right x
    rty = g_pos_y + g_size;  //Top right y

    rdx = g_pos_x + g_size;  //Bottom right x
    rdy = g_pos_y - g_size;  //Bottom right y

    ltx = g_pos_x - g_size;  //Top left x
    lty = g_pos_y + g_size;  //Top left y

    ldx = g_pos_x - g_size;  //Bottom left x
    ldy = g_pos_y - g_size;  //Bottom left y
    float cosaa = 0;
    float sinaa = 0;
    // if (sina < 0)
    // {
    //   g_angle = - g_angle;
    // }
    cosaa = cos(g_angle);
    sinaa = sin(g_angle);
       vertices[0] = ((rtx - g_pos_x) * cosaa) - ((rty - g_pos_y) * sinaa) +g_pos_x;
       vertices[1] = ((rtx - g_pos_x) * sinaa) + ((rty - g_pos_y) * cosaa) +g_pos_y;
       vertices[2] = ((rdx - g_pos_x) * cosaa) - ((rdy - g_pos_y) * sinaa) +g_pos_x;
       vertices[3] = ((rdx - g_pos_x) * sinaa) + ((rdy - g_pos_y) * cosaa) +g_pos_y;
       vertices[4] = ((ltx - g_pos_x) * cosaa) - ((lty - g_pos_y) * sinaa) +g_pos_x;
       vertices[5] = ((ltx - g_pos_x) * sinaa) + ((lty - g_pos_y) * cosaa) +g_pos_y;
       vertices[6] = ((ldx - g_pos_x) * cosaa) - ((ldy - g_pos_y) * sinaa) +g_pos_x;
       vertices[7] = ((ldx - g_pos_x) * sinaa) + ((ldy - g_pos_y) * cosaa) +g_pos_y;
        cout << "goujian" << endl;
 }
 else
 {
   vertices[0] = g_pos_x + g_size;  //Top right x
   vertices[1] = g_pos_y + g_size;  //Top right y

   vertices[2] = g_pos_x + g_size;  //Bottom right x
   vertices[3] = g_pos_y - g_size;  //Bottom right y

   vertices[4] = g_pos_x - g_size;  //Top left x
   vertices[5] = g_pos_y + g_size;  //Top left y

   vertices[6] = g_pos_x - g_size;  //Bottom left x
   vertices[7] = g_pos_y - g_size;  //Bottom left y
 }
}

//TODO: Choose between translate rotate and scale based on where the user clicked
void mouseClicked(float m_x, float m_y){
   printf("Clicked at %f, %f\n",m_x,m_y);
   cout << g_angle << endl;
   cout << endl;
   if (g_angle != 0){
     float a,b;
     a = m_x;
     b = m_y;
     m_x = ((a - g_pos_x) * cos(-g_angle)) - ((b - g_pos_y) * sin(-g_angle)) +g_pos_x;
     m_y = ((a - g_pos_x) * sin(-g_angle)) + ((b - g_pos_y) * cos(-g_angle)) +g_pos_y;
   }
   if(((m_x < g_pos_x +0.9*g_size) &&(m_x > g_pos_x - 0.9*g_size))&&
   ((m_y < g_pos_y +0.9*g_size) &&(m_y > g_pos_y - 0.9*g_size) ))
   {

        if(m_x != g_pos_x && m_y != g_pos_y)
             {
               a = m_x;
               b = m_y;
                 g_bTranslate = true;
             }
             else if (m_x == g_pos_x && m_y == g_pos_y){
                g_bTranslate = true;
             }
   }
   else{
     g_bTranslate = false;
   }
  cout << m_x << "x" << m_y << "y" << endl;
   cout << (g_pos_x + 0.92*g_size) << endl;
   cout << "g_pos_x + g_size" << g_pos_x + g_size << endl;

    if(((m_x > g_pos_x + 0.92*g_size) &&(m_x < g_pos_x + g_size) &&
        (m_y > g_pos_y + 0.92*g_size) && (m_y < g_pos_y + g_size))||
       ((m_x > g_pos_x + 0.92*g_size) &&(m_x < g_pos_x + g_size) &&
        (m_y < g_pos_y - 0.92*g_size) && (m_y > g_pos_y - g_size))||
       ((m_x < g_pos_x - 0.92*g_size) &&(m_x > g_pos_x - g_size) &&
        (m_y > g_pos_y + 0.92*g_size) && (m_y < g_pos_y + g_size))||
       ((m_x < g_pos_x - 0.92*g_size) &&(m_x > g_pos_x - g_size) &&
        (m_y < g_pos_y - 0.92*g_size) && (m_y > g_pos_y - g_size)))
        {

             g_bRotate = true;
        }
        else
        {
          g_bRotate = false;
        }
    if((((m_x > g_pos_x + 0.92*g_size) &&(m_x < g_pos_x + g_size)&&
    (m_y > g_pos_y - g_size) &&(m_y < g_pos_y + g_size)) ||(
    (m_x < g_pos_x - 0.92*g_size) &&(m_x > g_pos_x - g_size)&&
    (m_y > g_pos_y - g_size) &&(m_y < g_pos_y + g_size)) || (
    (m_x > g_pos_x - g_size) &&(m_x < g_pos_x + g_size)&&
    (m_y > g_pos_y + 0.92*g_size) &&(m_y < g_pos_y + g_size)) ||(
    (m_x > g_pos_x - g_size) &&(m_x < g_pos_x + g_size)&&
    (m_y > g_pos_y - g_size) &&(m_y < g_pos_y - 0.92*g_size))) && !g_bRotate)
    {
       g_bScale = true;

    }
    else
    {
        g_bScale = false;
    }
   //Let's assume the user clicked in the middle of the square


}

//TODO: Update the position, rotation, or scale based on the mouse movement
//TODO: My version of translate "jumps" when you click in the square, try to make it move smoothly
void mouseDragged(float m_x, float m_y){
   printf("Dragging through %f, %f\n",m_x,m_y);
   if (g_bTranslate && a != m_x && b != m_y) {
    //  g_pos_x = g_pos_x + m_x - a;
    //  g_pos_y = g_pos_y + m_y - b;
    //  a = m_x;
    //  b = m_y;


     g_pos_x = m_x;
     g_pos_y = m_y;

   }
   if(g_bScale){
     if(( m_x - g_pos_x  > 0.0f || m_x - g_pos_x  < 0.0f)&&
      ((fabs(m_y - g_pos_y) < (fabs(m_x - g_pos_x))))){
      g_size = fabs(m_x - g_pos_x);

    }
    if(( m_y - g_pos_y  > 0.0f || m_y - g_pos_y  < 0.0f)&&
     ((fabs(m_x - g_pos_x) < (fabs(m_y - g_pos_y))))){
     g_size = fabs (m_y - g_pos_y);
   }
   }
   if(g_bRotate)
   {
    float rtx,rty,rdx,rdy,ltx,lty,ldx,ldy;
    rtx = g_pos_x + g_size;  //Top right x
    rty = g_pos_y + g_size;  //Top right y

    rdx = g_pos_x + g_size;  //Bottom right x
    rdy = g_pos_y - g_size;  //Bottom right y

    ltx = g_pos_x - g_size;  //Top left x
    lty = g_pos_y + g_size;  //Top left y

    ldx = g_pos_x - g_size;  //Bottom left x
    ldy = g_pos_y - g_size;  //Bottom left y
    float c = 0;
    float d = 0;
    float cosa;

    c = sqrt((pow((m_x - g_pos_x),2))+pow((m_y - g_pos_y),2));
    d = sqrt((pow((rtx - g_pos_x),2))+pow((rty - g_pos_y),2));
      // cout << g_angle << endl;
  //  cout << vertices[0] << "     " <<g_pos_y <<endl;
    cosa = (((m_x - g_pos_x) * (rtx-g_pos_x)) + ((m_y - g_pos_y) * (rty-g_pos_y)))/ (c * d);
    sina = (((m_y - g_pos_y) * (rtx-g_pos_x)) - ((rty-g_pos_y) * (m_x - g_pos_x))) / (c * d);
    // cout <<acos (cosa)<< "   jiao  "<< sina <<endl;
    g_angle =  acos (cosa);

    if (sina < 0)
    {
      g_angle = - g_angle;
    }
    // cosa = cos(g_angle);
    // sina = sin(g_angle);
    //    vertices[0] = ((rtx - g_pos_x) * cosa) - ((rty - g_pos_y) * sina) +g_pos_x;
    //    vertices[1] = ((rtx - g_pos_x) * sina) + ((rty - g_pos_y) * cosa) +g_pos_y;
    //    vertices[2] = ((rdx - g_pos_x) * cosa) - ((rdy - g_pos_y) * sina) +g_pos_x;
    //    vertices[3] = ((rdx - g_pos_x) * sina) + ((rdy - g_pos_y) * cosa) +g_pos_y;
    //    vertices[4] = ((ltx - g_pos_x) * cosa) - ((lty - g_pos_y) * sina) +g_pos_x;
    //    vertices[5] = ((ltx - g_pos_x) * sina) + ((lty - g_pos_y) * cosa) +g_pos_y;
    //    vertices[6] = ((ldx - g_pos_x) * cosa) - ((ldy - g_pos_y) * sina) +g_pos_x;
    //    vertices[7] = ((ldx - g_pos_x) * sina) + ((ldy - g_pos_y) * cosa) +g_pos_y;
      //  cout  << g_size << endl;

        //  printf("asdasdas\n\n\n\n\n\n");
      // return;
//
// return;
  }
          updateVertices();

}

/////////////////////////////
/// ... you can ignore any code below (for now)
////////////////////////////

// Shader sources
const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec2 position;"
    "void main() {"
    "   gl_Position = vec4(position, 0.0, 1.0);"
    "}";

const GLchar* fragmentSource =
    "#version 150 core\n"
    "uniform vec3 triangleColor;"
    "out vec4 outColor;"
    "void main() {"
    "   outColor = vec4(triangleColor, 1.0);"
    "}";

bool fullscreen = false;

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_VIDEO);  //Initialize Graphics (for OpenGL)

    //Ask SDL to get a recent version of OpenGL (3.2 or greater)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//Create a window (offsetx, offsety, width, height, flags)
	SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 100, 100, screen_width, screen_height, SDL_WINDOW_OPENGL);

	//The above window cannot be resized which makes some code slightly easier.
	//Below show how to make a full screen window or allow resizing
	//SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 0, 0, screen_width, screen_height, SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);
	//SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 100, 100, screen_width, screen_height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	//SDL_Window* window = SDL_CreateWindow("My OpenGL Program",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,0,0,SDL_WINDOW_FULLSCREEN_DESKTOP|SDL_WINDOW_OPENGL); //Boarderless window "fake" full screen

    float aspect = screen_width/(float)screen_height; //aspect ratio (needs to be updated if the window is resized)

	updateVertices(); //set initial position of the square to match it's state

	//Create a context to draw in
	SDL_GLContext context = SDL_GL_CreateContext(window);

	//OpenGL functions using glad library
    if (gladLoadGLLoader(SDL_GL_GetProcAddress)){
       printf("OpenGL loaded\n");
       printf("Vendor:   %s\n", glGetString(GL_VENDOR));
       printf("Renderer: %s\n", glGetString(GL_RENDERER));
       printf("Version:  %s\n", glGetString(GL_VERSION));
    }
    else {
        printf("ERROR: Failed to initialize OpenGL context.\n");
        return -1;
    }

	//Build a Vertex Array Object. This stores the VBO and attribute mappings in one object
	GLuint vao;
	glGenVertexArrays(1, &vao); //Create a VAO
	glBindVertexArray(vao); //Bind the above created VAO to the current context


	//Allocate memory on the graphics card to store geometry (vertex buffer object)
	GLuint vbo;
	glGenBuffers(1, &vbo);  //Create 1 buffer called vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //Set the vbo as the active array buffer (Only one buffer can be active at a time)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); //upload vertices to vbo
	//GL_STATIC_DRAW means we won't change the geometry, GL_DYNAMIC_DRAW = geometry changes infrequently
	//GL_STREAM_DRAW = geom. changes frequently.  This effects which types of GPU memory is used


	//Load the vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	//Let's double check the shader compiled
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (!status){
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf("Vertex Shader Compile Failed. Info:\n\n%s\n",buffer);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//Double check the shader compiled
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (!status){
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf("Fragment Shader Compile Failed. Info:\n\n%s\n",buffer);
	}

	//Join the vertex and fragment shaders together into one program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor"); // set output
	glLinkProgram(shaderProgram); //run the linker

	glUseProgram(shaderProgram); //Set the active shader (only one can be used at a time)


	//Tell OpenGL how to set fragment shader input
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	  //Attribute, vals/attrib., type, normalized?, stride, offset
	  //Binds to VBO current GL_ARRAY_BUFFER
	glEnableVertexAttribArray(posAttrib);

	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");


	//Event Loop (Loop forever processing each event as fast as possible)
	SDL_Event windowEvent;
	bool done = false;
  float color2 = 0.0f;
	while (!done){
      while (SDL_PollEvent(&windowEvent)){  //Process input events (e.g., mouse & keyboard)

        if (windowEvent.type == SDL_QUIT) done = true;
        //List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
        //Scancode referes to a keyboard position, keycode referes to the letter (e.g., EU keyboards)
        if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE)
          done = true; //Exit event loop
        if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_f) //If "f" is pressed
          fullscreen = !fullscreen;
          if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_q)
          done = true;
          SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); //Set to full screen
          if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_c)
        {
            color2 = 0.9f;

        }
      }


      int mx, my;
      if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)) { //Is the mouse down?
         if (g_mouse_down == false){
            mouseClicked(2*mx/(float)screen_width - 1, 1-2*my/(float)screen_height);
         }
         else{
            mouseDragged(2*mx/(float)screen_width-1, 1-2*my/(float)screen_height);
         }
         g_mouse_down = true;
      }
      else{
        g_mouse_down = false;
      }

      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); //upload vertices to vbo


      // Clear the screen to black
      glClearColor(.2f, 0.9f, 0.8f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);


      glUniform3f(uniColor, 1.0f, color2, 0.3f);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      SDL_GL_SwapWindow(window); //Double buffering

	}

	glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);


	//Clean Up
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}
