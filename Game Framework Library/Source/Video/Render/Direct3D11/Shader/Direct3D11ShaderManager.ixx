export module Direct3D11ShaderManager;

import Direct3D11Video;
import Log;
import ShaderManager;

export namespace gfl
{
	class Direct3D11ShaderManager : public ShaderManager
	{
	public:
		Direct3D11ShaderManager(Direct3D11Video* direct3D11Video, Log* log);
		void LoadVertexShader(std::string_view resourceName, std::string_view fileName, VertexFormat vertexFormat) override;
		void LoadPixelShader(std::string_view resourceName, std::string_view fileName) override;
		const Shader* GetShader(std::string_view resourceName) const override;
	private:
		Direct3D11Video* direct3D11Video{};
		Log* log{};
	};
}