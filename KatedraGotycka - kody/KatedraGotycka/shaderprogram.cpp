#include "shaderprogram.h"

//Procedura wczytuje plik do tablicy znaków.
char* ShaderProgram::readFile(char* fileName) {
	int filesize;
	FILE *plik;
	char* result;

	fopen_s(&plik, fileName,"r");
	fseek(plik,0,SEEK_END);
	filesize=ftell(plik);
	fseek(plik,0,SEEK_SET);
	result=new char[filesize+1];
	fread(result,1,filesize,plik);
	result[filesize]=0;
	fclose(plik);

	return result;
}

//Metoda wczytuje i kompiluje shader, a nastêpnie zwraca jego uchwyt
GLuint ShaderProgram::loadShader(GLenum shaderType,char* fileName) {
	//Wygeneruj uchwyt na shader
	GLuint shader=glCreateShader(shaderType);//shaderType to GL_VERTEX_SHADER, GL_GEOMETRY_SHADER lub GL_FRAGMENT_SHADER
	//Wczytaj plik ze Ÿród³em shadera do tablicy znaków
	const GLchar* shaderSource=readFile(fileName);
	//Powi¹¿ Ÿród³o z uchwytem shadera
	glShaderSource(shader,1,&shaderSource,NULL);
	//Skompiluj Ÿród³o
	glCompileShader(shader);
	//Usuñ Ÿród³o shadera z pamiêci (nie bêdzie ju¿ potrzebne)
	delete []shaderSource;
	
	//Pobierz log b³êdów kompilacji i wyœwietl
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		delete []infoLog;
	}
	
	//Zwróæ uchwyt wygenerowanego shadera
	return shader;
}

GLuint ShaderProgram::loadShader(GLenum shaderType, const char* data) {
	GLuint shader = glCreateShader(shaderType);
	const GLchar* shaderSource = data;
	glShaderSource(shader, 1, &shaderSource, NULL);
	//Skompiluj Ÿród³o
	glCompileShader(shader);

	//Pobierz log b³êdów kompilacji i wyœwietl
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}

	//Zwróæ uchwyt wygenerowanego shadera
	return shader;
}

ShaderProgram::ShaderProgram(const char* vShaderCode, const char* fShaderCode) {
	//Wczytaj vertex shader
	printf("Loading vertex shader...\n");

	vertexShader = loadShader(GL_VERTEX_SHADER, vShaderCode);
	
	geometryShader = 0;

	//Wczytaj fragment shader
	printf("Loading fragment shader...\n");
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderCode);

	//Wygeneruj uchwyt programu cieniuj¹cego
	shaderProgram = glCreateProgram();

	//Pod³¹cz do niego shadery i zlinkuj program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Pobierz log b³êdów linkowania i wyœwietl
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}

	printf("Shader program created \n");
}

ShaderProgram::~ShaderProgram() {
	//Od³¹cz shadery od programu
	glDetachShader(shaderProgram, vertexShader);
	if (geometryShader!=0) glDetachShader(shaderProgram, geometryShader);
	glDetachShader(shaderProgram, fragmentShader);

	//Wykasuj shadery
	glDeleteShader(vertexShader);
	if (geometryShader!=0) glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	//Wykasuj program
	glDeleteProgram(shaderProgram);
}


//W³¹cz u¿ywanie programu cieniuj¹cego reprezentowanego przez aktualny obiekt
void ShaderProgram::use() {
	glUseProgram(shaderProgram);
}

//Pobierz numer slotu odpowiadaj¹cego zmiennej jednorodnej o nazwie variableName
GLuint ShaderProgram::getUniformLocation(char* variableName) {
	return glGetUniformLocation(shaderProgram,variableName);
}

//Pobierz numer slotu odpowiadaj¹cego atrybutowi o nazwie variableName
GLuint ShaderProgram::getAttribLocation(char* variableName) {
	return glGetAttribLocation(shaderProgram,variableName);
}

void ShaderProgram::passUniformSceneData(SceneData* sceneData) {
	glUniformMatrix4fv(getUniformLocation("P"), 1, false, glm::value_ptr(sceneData->P));
	glUniformMatrix4fv(getUniformLocation("V"), 1, false, glm::value_ptr(sceneData->V));
	glUniformMatrix4fv(getUniformLocation("M"), 1, false, glm::value_ptr(sceneData->M));
	glUniform4f(getUniformLocation("lightPosition0"), sceneData->lightSourcesPositions[0].x, sceneData->lightSourcesPositions[0].y, sceneData->lightSourcesPositions[0].z, 1);
	glUniform4f(getUniformLocation("lightPosition1"), sceneData->lightSourcesPositions[1].x, sceneData->lightSourcesPositions[1].y, sceneData->lightSourcesPositions[1].z, 1);
	glUniform4f(getUniformLocation("observerPosition"), sceneData->cameraPosition.x, sceneData->cameraPosition.y, sceneData->cameraPosition.z, 1);
}