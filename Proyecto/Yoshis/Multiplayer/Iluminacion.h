#pragma once
#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>


//Luces para cada momento del dia
		// Direccion de la luz
glm::vec3 direcciones[] = {
	glm::vec3(-0.3f, -0.8f,  0.2f),  // Noche
	glm::vec3(0.4f, -0.5f, -0.7f),  // Tarde
	glm::vec3(-0.2f, -1.0f, -0.3f)   // Dia
};

// Luz ambiental
glm::vec3 ambient[] = {
	glm::vec3(0.08f, 0.09f, 0.15f),
	glm::vec3(0.20f, 0.13f, 0.08f),
	glm::vec3(0.30f, 0.30f, 0.30f)
};

// Luz difusa
glm::vec3 diffuse[] = {
	glm::vec3(0.18f, 0.22f, 0.38f),
	glm::vec3(0.95f, 0.55f, 0.30f),
	glm::vec3(0.85f, 0.82f, 0.75f)
};

// Luz especular
glm::vec3 specular[] = {
	glm::vec3(0.25f, 0.28f, 0.40f),
	glm::vec3(0.75f, 0.45f, 0.25f),
	glm::vec3(0.90f, 0.90f, 0.85f)
};

void iluminacion(Shader lightingShader, GLfloat tiempoActual, 
	bool lamparasOn, bool spotOn, bool generalOn)
{


	glm::vec3 lightColor = lamparasOn ? colorLamparas : glm::vec3(0.0f);

	glm::vec3 amb = ambient[ambiente];
	glm::vec3 dif = diffuse[ambiente];
	glm::vec3 spe = specular[ambiente];

	if (generalOn)
	{
		amb = glm::vec3(0.45f);
		dif = glm::vec3(0.55f);
		spe = glm::vec3(0.5f);
	}

	glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), direcciones[ambiente].x, direcciones[ambiente].y, direcciones[ambiente].z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), amb.x, amb.y, amb.z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), dif.x, dif.y, dif.z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), spe.x, spe.y, spe.z);


	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), lightColor.x, lightColor.y, lightColor.z);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.007f);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.001f);



	// Point light 2
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), lightColor.x, lightColor.y, lightColor.z);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.007f);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.001f);

	// Point light 3
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), lightColor.x, lightColor.y, lightColor.z);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.007f);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.001f);

	// Point light 4
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
	glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

	// SpotLights de fiesta (3 postes)
	for (int i = 0; i < 3; i++)
	{
		std::string s = "spotLights[" + std::to_string(i) + "].";

		GLfloat ang = tiempoActual * fiesta.velocidadGiro + i * 2.094f;
		glm::vec3 punto = fiesta.objetivo + glm::vec3(cos(ang) * fiesta.radioBarrido, 0.0f,
			sin(ang) * fiesta.radioBarrido);
		glm::vec3 dirSpot = glm::normalize(punto - fiesta.postes[i]);

		glm::vec3 colorSpot = fiesta.colores[i] * fiesta.intensidad;
		if (!spotOn) colorSpot = glm::vec3(0.0f);

		dirsHaz[i] = dirSpot;
		largoHaz[i] = glm::length(punto - fiesta.postes[i]);

		glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "position").c_str()), fiesta.postes[i].x, fiesta.postes[i].y, fiesta.postes[i].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "direction").c_str()), dirSpot.x, dirSpot.y, dirSpot.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "ambient").c_str()), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "diffuse").c_str()), colorSpot.x, colorSpot.y, colorSpot.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "specular").c_str()), colorSpot.x, colorSpot.y, colorSpot.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "linear").c_str()), fiesta.linear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "quadratic").c_str()), fiesta.quadratic);
		glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "cutOff").c_str()), glm::cos(glm::radians(fiesta.apertura)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "outerCutOff").c_str()), glm::cos(glm::radians(fiesta.apertura + fiesta.suavizado)));
	}

	// Set material properties
	glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
}

void iluminacion2(Shader beamShader, glm::mat4 view, glm::mat4 projection, 
	GLuint coneVAO)
{
	const int SEG = 24;
	beamShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(beamShader.Program, "view"), 1, 
		GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(beamShader.Program, "projection"), 1, 
		GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);   // suma de luz
	glDepthMask(GL_FALSE);               // no tapa lo que está detrás

	glBindVertexArray(coneVAO);
	for (int i = 0; i < 3; i++)
	{
		// girar el cono (que apunta a -Y) hacia dirsHaz[i]
		glm::vec3 eje = glm::cross(glm::vec3(0, -1, 0), dirsHaz[i]);
		GLfloat ang = acos(glm::clamp(glm::dot(glm::vec3(0, -1, 0), dirsHaz[i]), -1.0f, 1.0f));

		GLfloat radio = largoHaz[i] * tan(glm::radians(fiesta.aperturaHaz));

		glm::mat4 m(1.0f);
		m = glm::translate(m, fiesta.postes[i]);
		if (glm::length(eje) > 0.0001f)
			m = glm::rotate(m, ang, glm::normalize(eje));
		m = glm::scale(m, glm::vec3(radio, largoHaz[i], radio));


		glUniformMatrix4fv(glGetUniformLocation(beamShader.Program, "model"), 1, GL_FALSE, 
			glm::value_ptr(m));
		glUniform3f(glGetUniformLocation(beamShader.Program, "beamColor"),
			fiesta.colores[i].x, fiesta.colores[i].y, fiesta.colores[i].z);
		glUniform1f(glGetUniformLocation(beamShader.Program, "opacidad"), fiesta.opacidadHaz);
		glDrawArrays(GL_TRIANGLES, 0, SEG * 3);

	}
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}