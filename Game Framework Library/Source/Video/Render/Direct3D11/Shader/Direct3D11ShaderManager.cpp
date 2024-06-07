module Direct3D11ShaderManager;

import Direct3D11PixelShader;
import Direct3D11VertexShader;

namespace gfl
{
	Direct3D11ShaderManager::Direct3D11ShaderManager(Direct3D11Video* direct3D11Video, Log* log) :
		direct3D11Video{direct3D11Video},
		log{log}
	{

	}

	void Direct3D11ShaderManager::LoadVertexShader(std::string_view resourceName, std::string_view fileName, VertexFormat vertexFormat)
	{
		this->direct3D11Video->AddShader(resourceName, std::make_unique<Direct3D11VertexShader>(fileName, vertexFormat, this->direct3D11Video->GetDevice(), this->log));
	}

	void Direct3D11ShaderManager::LoadPixelShader(std::string_view resourceName, std::string_view fileName)
	{
		this->direct3D11Video->AddShader(resourceName, std::make_unique<Direct3D11PixelShader>(fileName, this->direct3D11Video->GetDevice(), this->log));
	}

	const Shader* Direct3D11ShaderManager::GetShader(std::string_view resourceName) const
	{
		return this->direct3D11Video->GetShader(resourceName);
	}
}