module Direct3D11PixelShader;

import File;
import WindowsApi;

namespace gfl
{
	Direct3D11PixelShader::Direct3D11PixelShader(std::string_view fileName, ID3D11Device1* device, Log* log)
	{
		const auto bytecode{File::ExtractData(fileName)};
		if (WindowsApi::Failed(device->CreatePixelShader(bytecode.data(), bytecode.size(), nullptr, this->pixelShader.ReleaseAndGetAddressOf())))
			log->Error("CreatePixelShader");
	}

	void Direct3D11PixelShader::SetShader(ID3D11DeviceContext1* context) const
	{
		context->PSSetShader(this->pixelShader.Get(), nullptr, 0);
	}
}