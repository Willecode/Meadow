#pragma once
#include <string>
namespace ShaderUniformNames {
	/*
	* PBR material uniforms
	*/
	const std::string MATERIAL_ALBEDO = "material.albedo";
	const std::string MATERIAL_METALLIC = "material.metallic";
	const std::string MATERIAL_ROUGHNESS = "material.roughness";
	const std::string MATERIAL_ALBEDOMAP = "material.albedoMap";
	const std::string MATERIAL_NORMALMAP = "material.normalMap";
	const std::string MATERIAL_METALLICMAP = "material.metallicMap";
	const std::string MATERIAL_ROUGHNESSMAP = "material.roughnessMap";
	const std::string MATERIAL_AOMAP = "material.aoMap";
	const std::string MATERIAL_OPACITYMAP = "material.opacityMap";
	const std::string MATERIAL_HAS_ALBEDOMAP = "material.hasAlbedoMap";
	const std::string MATERIAL_HAS_NORMALMAP = "material.hasNormalMap";
	const std::string MATERIAL_HAS_METALLICMAP = "material.hasMetallicMap";
	const std::string MATERIAL_HAS_ROUGHNESSMAP = "material.hasRoughnessMap";
	const std::string MATERIAL_HAS_AOMAP = "material.hasAoMap";
	const std::string MATERIAL_HAS_OPACITYMAP = "material.hasOpacityMap";
}