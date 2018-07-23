#pragma once

#include <iostream>

class ShaderBuilder {
private:
	bool isMMatrixEnabled = false;
	bool isVMatrixEnabled = false;
	bool isPMatrixEnabled = false;
	bool isObserverPositionEnabled = false;
	unsigned int lightSourcesCount = 0;
	bool isAmbientMaterialEnabled = false;
	bool isDiffuseMaterialEnabled = false;
	bool isSpecularMaterialEnabled = false;
	bool isTransparencyValueEnabled = false;
	bool isDiffuseMapTexturingUnitEnabled = false;
	bool isSpecularMapTexturingUnitEnabled = false;
	bool isAlphaChannelMapTexturingUnitEnabled = false;
	bool isNormalVectorEnabled = false;
	bool areTextureCoordinatesEnabled = false;
	bool isAmbientShadingEnabled = false;
	bool isColorDiffuseShadingEnabled = false;
	bool isTextureDiffuseShadingEnabled = false;
	bool isColorSpecularShadingEnabled = false;
	bool isTextureSpecularShadingEnabled = false;
public:
	void enableMMatrix();
	void enableVMatrix();
	void enablePMatrix();
	void enableObserverPosition();
	void setLightSourceCount(unsigned int lightSources);
	void enableAmbientMaterial();
	void enableDiffuseMaterial();
	void enableSpecularMaterial();
	void enableTransparencyValue();
	void enableDiffuseMapTexturingUnit();
	void enableSpecularMapTexturingUnit();
	void enableAlphaChannelMapTexturingUnit();
	void enableNormalVector();
	void enableTextureCoordinates();
	void enableAmbientShading();
	void enableColorDiffuseShading();
	void enableTextureDiffuseShading();
	void enableColorSpecularShading();
	void enableTextureSpecularShading();
	std::string buildVertexShader();
	std::string buildFragmentShader();
};