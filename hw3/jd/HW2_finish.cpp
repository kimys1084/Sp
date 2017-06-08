#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <fstream> 
#include <iostream>
#include <string>

glm::mat4 projection_mat;
glm::mat4 projection_mat_point;
glm::mat4 depthMVI[13];



//SHADER
static GLuint program = 0, vertShader = 0, fragShader = 0;
static GLuint shadow_program = 0, shadow_vertShader = 0, shadow_fragShader = 0;

//For Camera_position
GLfloat camera_position[3] = {10,10,10};

//Camera_point
GLfloat point_camera[6][6] = {{1,0,0,0,1,0}, {-1,0,0,0,1,0}, {0,1,0,1,0,0}, {0,-1,0,1,0,0}, {0,0,1,0,1,0}, {0,0,-1,0,1,0}};

//For Light_position 0: Direactional light, 1: Point light, 2: Point light 

glm::vec3 light_on_;
GLfloat light_position[3][4] = {{10,10,-10,0}, {0,20,0,1}, {-10,10,10,1}};
GLfloat light_on[3] = {0,0,0};
static GLfloat lightKa[4] = {0.2, 0.2f, 0.2f, 0.2f};
static GLfloat lightKd[4] = {0.3f, 0.3f, 0.3f, 0.3f};
static GLfloat lightKs[4] = {0.7f, 0.7f, 0.7f, 0.7f};
static GLfloat matKs[4] = {1.f, 1.f, 1.f, 1.f};
static GLfloat matShininess = 50.f;

//For Original Scene
GLuint Fb0; 
GLuint Tx0;
GLfloat cube_angle = 0;
GLuint Rb0;


//For ShadowMap Scene
GLuint Fb1[13];
GLuint Tx1[13];
int showind_shadow_num = 0; //몇번째 shadow map이 나올 것인지 check 하는 것 

using namespace std;

void drawing_part(GLfloat* camera_position, int camera_or_light);

void solution_Original_scene() {
	glBindFramebuffer(GL_FRAMEBUFFER, Fb0);
		glUseProgram(program);
		{
			

			GLint projection_matID = glGetUniformLocation(program, "projection_mat");
			GLint projection_mat_pointID = glGetUniformLocation(program, "projection_mat_point");
			GLint depthMVIID = glGetUniformLocation(program, "depthMVI");

			glUniformMatrix4fv(depthMVIID, 13, GL_FALSE, &depthMVI[0][0][0]);
			glUniformMatrix4fv(projection_matID, 1, GL_FALSE, &projection_mat[0][0]);	
			glUniformMatrix4fv(projection_mat_pointID, 1, GL_FALSE, &projection_mat_point[0][0]);	

			for(int i = 0; i < 13; i++)
			{
				char buf[256];
				sprintf(buf, "shadow_map[%d]", i);
				//cout << buf << endl;
				GLint shadowmapID = glGetUniformLocation(program, buf);
				glActiveTexture(GL_TEXTURE1 + i);
				glBindTexture(GL_TEXTURE_2D, Tx1[i]);
				glUniform1i(shadowmapID, i+1);
			}
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);

			drawing_part(camera_position, 0);
		}
		glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void solution_Shadowmap(int n) {
	glBindFramebuffer(GL_FRAMEBUFFER, Fb1[n]);
		glUseProgram(shadow_program);
		{
				GLint shadowdepthID = glGetUniformLocation(shadow_program, "shadow_depth");
				//cout << "LIGHT" << light_on_[0] << endl;
				GLfloat shadow_depth = 50;
				glUniform1f(shadowdepthID, shadow_depth);
		}
		drawing_part(light_position[int((n + 5)/6)], n+1);
		glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void solution() {

	//====================================Making Texture map part ============================================
	for(int i = 0; i<13; i++)
		solution_Shadowmap(i);
	
	//========================================================================================================
	//ORIGINALSCENE
	solution_Original_scene();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 512, 512);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10, 10, -10, 10, 0, 20);
	glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0,0,-10,0,0,0,0,1,0);

		glPushMatrix();
			glColor3f(1.f, 1.f, 1.f);
			glBindTexture(GL_TEXTURE_2D, Tx0); //Original Rendering Part
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex3f(-10,-10,0);

				glTexCoord2f(1,0);
				glVertex3f(10,-10,0);

				glTexCoord2f(1,1);
				glVertex3f(10,10,0);

				glTexCoord2f(0,1);
				glVertex3f(-10,10,0);
			glEnd();
// -->   right downside drawing part depth rendering
			///*
			glBindTexture(GL_TEXTURE_2D, Tx1[showind_shadow_num]); //Depth Rendering Part
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex3f(-10,-10,-5);

				glTexCoord2f(1,0);
				glVertex3f(-5,-10,-5);

				glTexCoord2f(1,1);
				glVertex3f(-5,-5,-5);

				glTexCoord2f(0,1);
				glVertex3f(-10,-5,-5);
			glEnd();//*/
		glPopMatrix();

	glFinish();
}


//For Shader====================================================================================
#define GLSL(version, shader)  "#version " #version "\n" #shader
//=SHADOWMAP SHADER=
const char shadow_vert[] = GLSL(120,
varying vec4 position;
void main() {
	position = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = ftransform();
});

const char shadow_frag[] = GLSL(120,
varying vec4 position;
uniform float shadow_depth;
void main() {
	gl_FragDepth = length(vec3(position)) / shadow_depth;
});

//==================




//VERTEX SHADER PART
const char vert[] = GLSL(120,
varying vec3 normal;
varying vec3 lightDir[3];
varying vec3 halfVector[3];
varying float x;

varying vec4 view_position;
uniform vec3 light_on;
void main() {
	normal = normalize(gl_NormalMatrix*gl_Normal);
	for(int i = 0; i < 3; i++)
	{
		lightDir[i] = normalize(gl_LightSource[i].position.xyz);
		halfVector[i] = normalize(gl_LightSource[i].halfVector.xyz);
	}
	x = gl_Vertex.x;
	gl_Position = ftransform();

	view_position = gl_ModelViewMatrix * gl_Vertex;
});

//FRAGMENT SHADER PART
const char frag[] = GLSL(120,
varying vec3 normal;
varying vec3 lightDir[3];
varying vec3 halfVector[3];
varying float x;
varying vec4 view_position;

uniform vec3 light_on;

//For shadow mapping
uniform mat4 depthMVI[13];
uniform mat4 projection_mat;
uniform mat4 projection_mat_point;
uniform sampler2D shadow_map[13];
//==================

void main() {
	vec3 n;
	vec3 h;
	float NdotL;
	float NdotH;

	vec4 color = vec4(0);// gl_LightModel.ambient*gl_FrontMaterial.ambient;
	
	vec4 light_view_position;
	vec4 light_texture_position;
	n = normalize(normal);
	for(int i=0; i < 3;i++)
	{
		vec4 tmp_color = vec4(0); 
		if(light_on[i] == 1){
			tmp_color = gl_FrontMaterial.ambient*gl_LightSource[i].ambient;
			NdotL = max(dot(n,lightDir[i]),0.0);
			
			if (NdotL > 0.0) {
				tmp_color += gl_FrontMaterial.diffuse*gl_LightSource[i].diffuse*NdotL;
				h = normalize(halfVector[i]);
				NdotH = max(dot(n,h),0.0);
				//tmp_color += gl_FrontMaterial.specular*gl_LightSource[i].specular*pow(NdotH, gl_FrontMaterial.shininess)*(gl_FrontMaterial.shininess+1.0)/200.0;
			}
			
			if(i==0){
				
				light_view_position = depthMVI[0]*view_position;
				//light_texture_position = projection_mat*light_view_position;
				light_texture_position = projection_mat*light_view_position;
				light_texture_position /= light_texture_position.w;
				light_view_position /= light_view_position.w;
			
				if(light_texture_position.x < 1 && light_texture_position.x > -1 && light_texture_position.y < 1 && light_texture_position.y > -1)
				{
					float shadow = texture2D(shadow_map[0], light_texture_position.xy/2.0 + 0.5).x;
						
					if((length(vec3(light_view_position))/50.0) > shadow + 0.001)
					{
						tmp_color = vec4(0);//tmp_color * 0.5;
					}
				}
				//if((length(vec3(tmp_position)) / 50.0)>) 
			}
			else
			{
				for(int j = 0; j<6; j++)
				{
					light_view_position = depthMVI[(i - 1) * 6+j + 1]*view_position;
					//light_texture_position = projection_mat*light_view_position;
					light_texture_position = projection_mat_point*light_view_position;
					light_texture_position /= light_texture_position.w;
					light_view_position /= light_view_position.w;
				
					if(light_texture_position.x < 1 && light_texture_position.x > -1 && light_texture_position.y < 1 && light_texture_position.y > -1)
					{
						float shadow = texture2D(shadow_map[(i - 1) * 6+j + 1], light_texture_position.xy/2.0 + 0.5).x;
						
						//tmp_color = vec4(shadow);
						//tmp_color = vec4(length(vec3(light_view_position))/50.0);
						
						if((length(vec3(light_view_position))/50.0) > shadow + 0.001)
						{
							tmp_color = vec4(0);//tmp_color * 0.5;
						}
					}
				}
			}

	
		}
		color += tmp_color;
	}
	gl_FragColor = color;
});
//==========================================================================================

//For SHADING PART!!!!!!!!!!!!!!!!!!!!
void printShaderInfoLog(GLuint obj) {
	int len = 0, charsWritten = 0;
	char* infoLog;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {                                                                                                   
                                          
		infoLog = (char*)malloc(len);
		glGetShaderInfoLog(obj, len, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

void printProgramInfoLog(GLuint obj) {
	int len = 0, charsWritten = 0;
	char* infoLog;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		infoLog = (char*)malloc(len);
		glGetProgramInfoLog(obj, len, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

GLuint createShader(const char* src, GLenum type) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	printShaderInfoLog(shader);
	return shader;
}


//For Drawing
void createProgram() {
	vertShader = createShader(vert, GL_VERTEX_SHADER);
	fragShader = createShader(frag, GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	printProgramInfoLog(program);
}

//For Drawing_shadowMap
void createProgram_shadow() {
	shadow_vertShader = createShader(shadow_vert, GL_VERTEX_SHADER);
	shadow_fragShader = createShader(shadow_frag, GL_FRAGMENT_SHADER);
	shadow_program = glCreateProgram();
	glAttachShader(shadow_program, shadow_vertShader);
	glAttachShader(shadow_program, shadow_fragShader);
	glLinkProgram(shadow_program);
	printProgramInfoLog(shadow_program);
}


void initGL() {
	if (glewInit() != GLEW_OK) {
		printf("GLEW init failed.\n");
		exit(EXIT_FAILURE);
	}
	if (!GLEW_VERSION_2_0) {
		printf("OpenGL 2.0 not supported.\n");
		exit(EXIT_FAILURE);
	}
	createProgram();
	createProgram_shadow();
}

//====================================





//OBJ loading 하는 부분

void OBJ_loader(){

}



//이 부분에만 집중 : 렌더링 하는 부분
void drawing_part(GLfloat* camera_position, int camera_or_light) { //Drawing Part
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 512, 512);


	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(camera_or_light==0 || camera_or_light ==1 )
			glOrtho(-15.f, 15.f, -15.f, 15.f, 0.f, 50.f);
		else
			gluPerspective(90,1,0.1, 50);
	glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if(camera_or_light == 0 || camera_or_light == 1)
			gluLookAt(camera_position[0], camera_position[1], camera_position[2], 0, 0, 0, 0, 1, 0);
		else
			gluLookAt(camera_position[0], camera_position[1], camera_position[2], camera_position[0] + point_camera[(camera_or_light-2)%6][0], camera_position[1]+ point_camera[(camera_or_light-2)%6][1], camera_position[2]+ point_camera[(camera_or_light-2)%6][2], point_camera[(camera_or_light-2)%6][3], point_camera[(camera_or_light-2)%6][4], point_camera[(camera_or_light-2)%6][5]);
			
		glPushMatrix();
			if(camera_or_light == 0)
			{
				GLint shadowmapID = glGetUniformLocation(program, "light_on");
				light_on_[0] = light_on[0];
				light_on_[1] = light_on[1];
				light_on_[2] = light_on[2];
				//cout << "LIGHT" << light_on_[0] << endl;
				glUniform3fv(shadowmapID, 1, &light_on_[0]);
			}
			//=====LIGHT=====
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
			glEnable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT, GL_SPECULAR, matKs);
			glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);
			
			for(int i = 0; i < 3; i++){
				if(light_on[i]){
					glEnable(GL_LIGHT0 + i);
					glLightfv(GL_LIGHT0 + i, GL_AMBIENT, lightKa);
					glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lightKd);
					glLightfv(GL_LIGHT0 + i, GL_SPECULAR, lightKs);
					glLightfv(GL_LIGHT0 + i, GL_POSITION, light_position[i]);
				}
				else
					glDisable(GL_LIGHT0 + i);
			
			}
			//===============

			//Creating Floor
			glColor3f(0.88,0.88,0);
			glPushMatrix();
				glTranslatef(0,-0.5, 0);
				glScalef(20,1,20);
				glutSolidCube(1);
			glPopMatrix();

			glColor3f(0.4,0.4,0.4);
			glPushMatrix();
				glTranslatef(0,3,0);
				glPushMatrix();
					glScalef(0.5,6,2);
					glutSolidCube(1);
				glPopMatrix();
				glTranslatef(-4,0,0);
				glutSolidSphere(1,50,5);
			glPopMatrix();

			glDisable(GL_LIGHTING);
		glPopMatrix();
	glFinish();
}

void display() {
	//cube_angle += 0.05;
	//=====================Get Light Matrix ================
	
	projection_mat = glm::ortho<float> (-15,15,-15,15,0,50);
	projection_mat_point = glm::perspective<float> (glm::radians(90.f), 1, 0.1, 50);
	//glm::mat4 depthProjectionMatrix  = glm::ortho<float> (-20,20,-20,20,0,30);
	glm::mat4 inv_viewM = glm::inverse(glm::lookAt(glm::vec3(camera_position[0],camera_position[1],camera_position[2]), glm::vec3(0,0,0), glm::vec3(0,1,0)));
	glm::vec3 light_pos = glm::vec3(light_position[0][0], light_position[0][1], light_position[0][2]);
	depthMVI[0] = glm::lookAt(light_pos, glm::vec3(0,0,0), glm::vec3(0,1,0)) * inv_viewM;
	for(int i = 1; i<13; i++){
		glm::vec3 light_pos = glm::vec3(light_position[int((i-1)/6) + 1][0], light_position[int((i-1)/6) + 1][1], light_position[int((i-1)/6) + 1][2]);
		depthMVI[i] = glm::lookAt(light_pos, 
			glm::vec3(light_position[int((i-1)/6) + 1][0] + point_camera[(i-1)%6][0], light_position[int((i-1)/6) + 1][1] + point_camera[(i-1)%6][1] , light_position[int((i-1)/6) + 1][2]+ point_camera[(i-1)%6][2]), 
				glm::vec3(point_camera[(i-1)%6][3],point_camera[(i-1)%6][4],point_camera[(i-1)%6][5]))
		 * inv_viewM;
	}
	//======================================================
	solution();
}
void Homework2_end(){
	glDeleteFramebuffers(1, &Fb0);
	glDeleteTextures(1, &Tx0);
	glDeleteRenderbuffers(1, &Rb0);
	
	glDeleteFramebuffers(13, &Fb1[0]);
	glDeleteTextures(13, &Tx1[0]);

}
void Homework2_init() {
	//Color 

	//Generating
	glGenFramebuffers(1, &Fb0);
	glGenTextures(1, &Tx0);
	glGenRenderbuffers(1, &Rb0);

	//Binding
	glBindTexture(GL_TEXTURE_2D, Tx0);
	glBindFramebuffer(GL_FRAMEBUFFER, Fb0);
	glBindRenderbuffer(GL_RENDERBUFFER, Rb0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Rb0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,  Tx0, 0);

	//Depth

	//Generating
	glGenFramebuffers(13, &Fb1[0]);
	glGenTextures(13, &Tx1[0]);


	for(int i = 0; i<13; i++){
		glBindTexture(GL_TEXTURE_2D, Tx1[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, Fb1[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 512, 512, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Tx1[i], 0);
	}

	//Init
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void key(unsigned char k, int, int) {
	switch (k) {
		case '1':
			light_on[0] = !light_on[0];
			cout << light_on[0] << endl;
			break;
		case '2':
			light_on[1] = !light_on[1];
			cout << light_on[1] << endl;
			break;
		case '3':
			light_on[2] = !light_on[2];
			cout << light_on[2] << endl;
			break;
	
		case 'p':
		case 'P':
			showind_shadow_num++;
			showind_shadow_num = showind_shadow_num % 13;
			cout << "SHADOW NUM " << showind_shadow_num<< endl;
			break;
		case 'q':
			exit(0);
			break;
	/*	case '+':
			freq += 1.f;
			break;
		case '-':
			if (freq > 2.f)
				freq -= 1.f;
			break;
		case 's':
			matShininess *= 0.5f;
			break;
		case 'S':
			matShininess *= 2.0f;
			break;
		case 't':
		case 'T':
			enableStripe = !enableStripe;
			break;
			*/
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Homework3");
	
	glewInit();
	initGL();

	glEnable(GL_TEXTURE_2D);
	Homework2_init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(key);

	glutMainLoop();
	
	glEnable(GL_TEXTURE_2D);
	Homework2_end();
	return EXIT_SUCCESS;
}
