#include "ShaderBuilder.h"

#include <string>

void ShaderBuilder::enableMMatrix() {
	isMMatrixEnabled = true;
}

void ShaderBuilder::enableVMatrix() {
	isVMatrixEnabled = true;
}

void ShaderBuilder::enablePMatrix() {
	isPMatrixEnabled = true;
}

void ShaderBuilder::enableObserverPosition() {
	isObserverPositionEnabled = true;
}

void ShaderBuilder::setLightSourceCount(unsigned int lightSources) {
	lightSourcesCount = lightSources;
}

void ShaderBuilder::enableAmbientMaterial() {
	isAmbientMaterialEnabled = true;
}

void ShaderBuilder::enableDiffuseMaterial() {
	isDiffuseMaterialEnabled = true;
}

void ShaderBuilder::enableSpecularMaterial() {
	isSpecularMaterialEnabled = true;
}

void ShaderBuilder::enableTransparencyValue() {
	isTransparencyValueEnabled = true;
}

void ShaderBuilder::enableDiffuseMapTexturingUnit() {
	isDiffuseMapTexturingUnitEnabled = true;
}

void ShaderBuilder::enableSpecularMapTexturingUnit() {
	isSpecularMapTexturingUnitEnabled = true;
}

void ShaderBuilder::enableAlphaChannelMapTexturingUnit() {
	isAlphaChannelMapTexturingUnitEnabled = true;
}

void ShaderBuilder::enableNormalVector() {
	isNormalVectorEnabled = true;
}

void ShaderBuilder::enableTextureCoordinates() {
	areTextureCoordinatesEnabled = true;
}

void ShaderBuilder::enableAmbientShading() {
	if (isAmbientMaterialEnabled) {
		isAmbientShadingEnabled = true;
	}
	else {
		throw "It is not possible to enable ambient shading without ambient material!";
	}
}

void ShaderBuilder::enableColorDiffuseShading() {
	if (isTextureDiffuseShadingEnabled) {
		throw "It is not possible to use both color and texture diffuse shading!";
	}
	else if (!isDiffuseMaterialEnabled) {
		throw "It is not possible to enable color diffuse shading without diffuse material!";
	}
	else if (!isNormalVectorEnabled) {
		throw "It is not possible to enable color diffuse shading without normal vectors!";
	}
	else {
		isColorDiffuseShadingEnabled = true;
	}
}

void ShaderBuilder::enableTextureDiffuseShading() {
	if (isColorDiffuseShadingEnabled) {
		throw "It is not possible to use both color and texture diffuse shading!";
	}
	else if (!isNormalVectorEnabled) {
		throw "It is not possible to enable textured diffuse shading without normal vectors!";
	}
	else if (!isDiffuseMapTexturingUnitEnabled) {
		throw "It is not possible to enable textured diffuse shading without texturing unit!";
	}
	else {
		isTextureDiffuseShadingEnabled = true;
	}
}

void ShaderBuilder::enableColorSpecularShading() {
	if (isTextureSpecularShadingEnabled) {
		throw "It is not possible to use both color and texture specular shading!";
	}
	else if (!isSpecularMaterialEnabled) {
		throw "It is not possible to enable color specular shading without specular material!";
	}
	else if (!isObserverPositionEnabled) {
		throw "It is not possible to enable color specular shading without observer position!";
	}
	else if (!isNormalVectorEnabled) {
		throw "It is not possible to enable color specular shading without normal vectors!";
	}
	else {
		isColorSpecularShadingEnabled = true;
	}
}

void ShaderBuilder::enableTextureSpecularShading() {
	if (isColorSpecularShadingEnabled) {
		throw "It is not possible to use both color and texture specular shading!";
	}
	else if (!isObserverPositionEnabled) {
		throw "It is not possible to enable textured specular shading without observer position!";
	}
	else if (!isNormalVectorEnabled) {
		throw "It is not possible to enable textured specular shading without normal vectors!";
	}
	else if (!isSpecularMapTexturingUnitEnabled) {
		throw "It is not possible to enable textured specular shading without texturing unit!";
	}
	else {
		isTextureSpecularShadingEnabled = true;
	}
}

std::string ShaderBuilder::buildVertexShader() {
	std::string shader = "#version 330\n\n";

	if (isMMatrixEnabled) {
		shader += "uniform mat4 M;\n";
	}

	if (isVMatrixEnabled) {
		shader += "uniform mat4 V;\n";
	}

	if (isPMatrixEnabled) {
		shader += "uniform mat4 P;\n";
	}

	if (isObserverPositionEnabled) {
		shader += "uniform vec4 observerPosition;\n";
	}

	for (int i = 0; i < lightSourcesCount; i++) {
		shader += "uniform vec4 lightPosition" + std::to_string(i);
		shader += ";\n";
	}

	shader += "\nin vec4 vertex;\n";
	
	if (isNormalVectorEnabled) {
		shader += "in vec4 normal;\n";
	}

	if (areTextureCoordinatesEnabled) {
		shader += "in vec2 textureCoordinates;\n";
	}

	shader += "\n";

	if (isNormalVectorEnabled) {
		shader += "out vec4 n;\n";
	}

	for (int i = 0; i < lightSourcesCount; i++) {
		shader += "out vec4 lp" + std::to_string(i);
		shader += ";\n";
	}

	if (isObserverPositionEnabled) {
		shader += "out vec4 op;\n";
	}

	if (areTextureCoordinatesEnabled) {
		shader += "out vec2 texCoords;\n";
	}

	shader += "\nvoid main(void) {\n";

	if (isNormalVectorEnabled) {
		shader += std::string("n = normalize( ") + (isMMatrixEnabled ? "M *" : "");
		shader += " normal );\n";
	}

	for (int i = 0; i < lightSourcesCount; i++) {
		shader += "lp" + std::to_string(i);
		shader += " = normalize( lightPosition" + std::to_string(i);
		shader += std::string(" - ") + (isMMatrixEnabled ? " M * " : "");
		shader += "vertex );\n";
	}

	if (isObserverPositionEnabled) {
		shader += std::string("op = normalize( observerPosition - ") + (isMMatrixEnabled ? "M * " : "");
		shader += "vertex );\n";
	}

	if (areTextureCoordinatesEnabled) {
		shader += "texCoords = textureCoordinates;\n";
	}

	shader += "gl_Position = ";

	if (isPMatrixEnabled) {
		shader += "P * ";
	}

	if (isVMatrixEnabled) {
		shader += "V * ";
	}

	if (isMMatrixEnabled) {
		shader += "M * ";
	}

	shader += "vertex;\n\n}\n";

	return shader;
}



std::string ShaderBuilder::buildFragmentShader() {
	std::string shader = "#version 330\n\n";

	if (isAmbientMaterialEnabled) {
		shader += "uniform vec4 Ma;\n";
	}

	if (isDiffuseMaterialEnabled) {
		shader += "uniform vec4 Md;\n";
	}

	if (isSpecularMaterialEnabled) {
		shader += "uniform vec4 Ms;\n";
	}

	if (isSpecularMaterialEnabled || isTextureSpecularShadingEnabled) {
		shader += "uniform float Ns;\n";
	}

	for (int i = 0; i < lightSourcesCount; i++) {
		shader += "uniform vec4 La" + std::to_string(i) + ";\n";
		shader += "uniform vec4 Ld" + std::to_string(i) + ";\n";
		shader += "uniform vec4 Ls" + std::to_string(i) + ";\n";
	}

	if (isTextureDiffuseShadingEnabled) {
		shader += "uniform sampler2D texture_map_Kd;\n";
	}

	if (isTextureSpecularShadingEnabled) {
		shader += "uniform sampler2D texture_map_Ks;\n";
	}

	if (isTransparencyValueEnabled) {
		shader += "uniform float d;\n";
	}

	if (isNormalVectorEnabled) {
		shader += "in vec4 n;\n";
	}

	if (areTextureCoordinatesEnabled) {
		shader += "in vec2 texCoords;\n";
	}

	if (isObserverPositionEnabled) {
		shader += "in vec4 op;\n";
	}

	for (int i = 0; i < lightSourcesCount; i++) {
		shader += "in vec4 lp" + std::to_string(i) + ";\n";
	}

	shader += "\nout vec4 pixelColor;\n";

	shader += "\nvoid main(void) {\n";

	if (isNormalVectorEnabled) {
		shader += "vec4 myN = normalize(n);\n";
	}

	for (int i = 0; i < lightSourcesCount; i++) {
		shader += "vec4 myL" + std::to_string(i) + " = normalize(lp" + std::to_string(i) + ");\n";
	}

	if (isObserverPositionEnabled) {
		shader += "vec4 myOP = normalize(op);\n";
	}

	if (isColorDiffuseShadingEnabled || isTextureDiffuseShadingEnabled) {
		for (int i = 0; i < lightSourcesCount; i++) {
			shader += "float nl" + std::to_string(i) + " = max(dot(myL" + std::to_string(i) + ", myN), 0);\n";
		}
	}

	if (isColorSpecularShadingEnabled || isTextureSpecularShadingEnabled) {
		for (int i = 0; i < lightSourcesCount; i++) {
			shader += "vec4 myR" + std::to_string(i) + " = normalize(-reflect(myL" + std::to_string(i) + ", myN));\n";
		}
		shader += "\n";
		for (int i = 0; i < lightSourcesCount; i++) {
			shader += "float rv" + std::to_string(i) + " = pow(max(dot(myR" + std::to_string(i) + ", myOP), 0), Ns);\n";
		}
	}

	shader += "pixelColor = ";

	if (isAmbientShadingEnabled && lightSourcesCount > 0) {
		shader += "(La0";
		for (int i = 1; i < lightSourcesCount; i++) {
			shader += " + La" + std::to_string(i);
		}
		shader += ") * Ma + \n";
	}

	if (isColorDiffuseShadingEnabled) {
		for (int i = 0; i < lightSourcesCount; i++) {
			shader += "Ld" + std::to_string(i) + " * Md * vec4(nl" + std::to_string(i) + ", nl" + std::to_string(i) + ", nl" + std::to_string(i);
			if (isTransparencyValueEnabled) {
				shader += ", d) + ";
			}
			else {
				shader += ", 1) + ";
			}
		}
	}
	else if (isTextureDiffuseShadingEnabled) {
		for (int i = 0; i < lightSourcesCount; i++) {
			shader += "Ld" + std::to_string(i) + " * texture2D(texture_map_Kd, texCoords) * vec4(nl" + std::to_string(i) + ", nl" + std::to_string(i) + ", nl" + std::to_string(i);
			if (isTransparencyValueEnabled) {
				shader += ", d) + ";
			}
			else {
				shader += ", 1) + ";
			}

		}
	}

	if (isColorSpecularShadingEnabled) {
		for (int i = 0; i < lightSourcesCount; i++) {
			shader += "Ls" + std::to_string(i) + " * Ms * vec4(rv" + std::to_string(i) + ", rv" + std::to_string(i) + ", rv" + std::to_string(i) + ", 0) + ";
		}
	}
	else if (isTextureSpecularShadingEnabled) {
		for (int i = 0; i < lightSourcesCount; i++) {
			shader += "Ls" + std::to_string(i) + " * texture2D(texture_map_Ks, texCoords) * vec4(rv" + std::to_string(i) + ", rv" + std::to_string(i) + ", rv" + std::to_string(i) + ", 0) + ";
		}
	}

	shader += "vec4(0,0,0,0);\n}\n";

	return shader;
}