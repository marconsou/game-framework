module;

#include "../Direct3D11.h"

export module Direct3D11PixelShader;

import std;
import Direct3D11Shader;
import Log;

export namespace gfl
{
	class Direct3D11PixelShader : public Direct3D11Shader
	{
	public:
		Direct3D11PixelShader(std::string_view fileName, ID3D11Device1* device, Log* log);
		void SetShader(ID3D11DeviceContext1* context) const override;
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	};
}