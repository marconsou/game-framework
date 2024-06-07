export module ShaderManager;

import std;
import Shader;
import VertexFormat;

export namespace gfl
{
	class ShaderManager
	{
	public:
		ShaderManager() = default;
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager(ShaderManager&&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;
		ShaderManager& operator=(ShaderManager&&) = delete;
		virtual ~ShaderManager() = default;
		virtual void LoadVertexShader(std::string_view resourceName, std::string_view fileName, VertexFormat vertexFormat) = 0;
		virtual void LoadPixelShader(std::string_view resourceName, std::string_view fileName) = 0;
		virtual const Shader* GetShader(std::string_view resourceName) const = 0;
	};
}