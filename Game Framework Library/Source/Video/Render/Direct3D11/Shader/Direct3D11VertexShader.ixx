module;

#include "../Direct3D11.h"

export module Direct3D11VertexShader;

import std;
import Direct3D11Shader;
import Log;
import VertexFormat;

export namespace gfl
{
	class Direct3D11VertexShader : public Direct3D11Shader
	{
	public:
		Direct3D11VertexShader(std::string_view fileName, VertexFormat vertexFormat, ID3D11Device1* device, Log* log);
		void SetShader(ID3D11DeviceContext1* context) const override;
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	};
}