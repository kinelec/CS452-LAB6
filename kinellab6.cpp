#include "initShaders.h"
#include <cstdlib>
using namespace std;

void rotate(GLuint locate);

GLuint vaoID,vboID[2],eboID;
GLuint prog;

GLfloat pit,yaw,scalar=1;
glm::vec3 cubeTran;

GLfloat normal=1.0f/sqrt(3.0f);

GLfloat vertices[]={
	-10.0f,10.0f,10.0f,
	-10.0f,-10.0f,10.0f,
	10.0f,-10.0f,10.0f,
	10.0f,10.0f,10.0f,	
	-10.0f,10.0f,-10.0f,
	-10.0f,-10.0f,-10.0f,
	10.0f,-10.0f,-10.0f,
	10.0f,10.0f,-10.0f
					};

GLfloat colors[]={
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f,
	1.0f,0.0f,0.0f,1.0f						
	              		};

GLfloat norms[]={
	-normal,normal,normal,
	-normal,-normal,normal,
	normal,-normal,normal,
	normal,normal,normal,
	-normal,normal,-normal,
	-normal,-normal,-normal,
	normal,-normal,-normal,
	normal,normal,-normal
};
											
GLubyte elems[]={
	7,6,5,4,
	7,3,0,4,
	7,6,2,3,
	7,4,0,1,
	5,4,5,2,
	1,5,6,2,
	1,5,6,2,
	3,0,1,3,
	2,1,0,3
			};

void init(){
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH); 
	glEnable(GL_NORMALIZE);
	glViewport(0, 0, 600, 600);	

	glGenVertexArrays(1,&vaoID);
	glBindVertexArray(vaoID);
	
	glGenBuffers(3, vboID);
	glBindBuffer(GL_ARRAY_BUFFER,vboID[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(norms),norms,GL_STATIC_DRAW);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  
	glGenBuffers(1,&eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elems),elems,GL_STATIC_DRAW);

	Shader s[]={
  	{ GL_VERTEX_SHADER , "vertexshader.glsl"},
  	{ GL_FRAGMENT_SHADER , "fragmentshader.glsl"}, 
  	{ GL_NONE , NULL} 
  	};
		
  	prog=initShaders(s);
  
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);  
}


void display(SDL_Window* screen){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 trans;
	trans = glm::translate(trans,cubeTran);
  	trans = glm::rotate(trans,pit,glm::vec3(1,0,0));
  	trans = glm::rotate(trans,yaw,glm::vec3(0,1,0));
  	trans = glm::scale(trans,glm::vec3(scalar));
	GLfloat amb[]={0.7f,0.7f,0.7f,1.0f};
	GLfloat light1d[] = {-10.0f, 30.0f, 20.0f};
  	GLfloat light1c[] = {0.1f, 0.1f, 0.1f};
    

  	GLint tmpLocation = glGetUniformLocation(prog,"modelMatrix");
	glUniformMatrix4fv(tmpLocation,1,GL_FALSE,&trans[0][0]);
	GLint lightLocation = glGetUniformLocation(prog,"Ambient");
	glUniform4fv(lightLocation,1,amb);
	lightLocation = glGetUniformLocation(prog,"LightD1");
	glUniform3fv(lightLocation,1,light1d);

	lightLocation = glGetUniformLocation(prog,"LightC1");
	glUniform3fv(lightLocation,1,light1c);

	lightLocation = glGetUniformLocation(prog,"Half1");
	glUniform3fv(lightLocation,1,light1d);

	
	glDrawElements(GL_TRIANGLE_STRIP,36,GL_UNSIGNED_BYTE,NULL);
	glFlush();
	SDL_GL_SwapWindow(screen);
}

void input(SDL_Window* screen){
	
	//keyboard events
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type){
			case SDL_QUIT:exit(0);break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:exit(0);
					case SDLK_w:cubeTran.y+=2;break;
					case SDLK_s:cubeTran.y-=2;break;
					case SDLK_a:cubeTran.x-=2;break;
					case SDLK_d:cubeTran.x+=2;break;
					case SDLK_e:scalar+=.1f;break;
					case SDLK_q:scalar-=.1f;break;
					case SDLK_i:pit+=2;break;
					case SDLK_k:pit-=2;break;
					case SDLK_j:yaw+=2;break;
					case SDLK_l:yaw-=2;break;
				}
		}
	}
}


int main(int argc, char **argv){
	SDL_Window *window;
	
	//initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
    	SDL_Quit();
    	exit(1);
	}

	window = SDL_CreateWindow("Kinel Lab 6",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,SDL_WINDOW_OPENGL);
	
	if(window==NULL){
		fprintf(stderr,"Unable to create window: %s\n",SDL_GetError());
	}

	SDL_GLContext glcontext=SDL_GL_CreateContext(window);
	
	//initialize glew
  	glewExperimental=GL_TRUE;
  	if(glewInit()){
    	fprintf(stderr, "Unable to initalize GLEW");
    	exit(EXIT_FAILURE);
  	}
  
	init();
	
	while(true){
		input(window);
		display(window);
	}

	SDL_GL_DeleteContext(glcontext);
  	SDL_DestroyWindow(window);
  	SDL_Quit();
 
  	return 0;
}
