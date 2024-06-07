module;

#include <directxtk/VertexTypes.h>

module Direct3D11VertexShader;

import File;
import WindowsApi;

namespace gfl
{
	Direct3D11VertexShader::Direct3D11VertexShader(std::string_view fileName, VertexFormat vertexFormat, ID3D11Device1* device, Log* log)
	{
		const auto bytecode{File::ExtractData(fileName)};
		if (WindowsApi::Failed(device->CreateVertexShader(bytecode.data(), bytecode.size(), nullptr, this->vertexShader.ReleaseAndGetAddressOf())))
			log->Error("CreateVertexShader");

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc;
		switch (vertexFormat)
		{
		case VertexFormat::PositionColor:
			for (const auto& item : DirectX::VertexPositionColor::InputElements)
				inputElementDesc.push_back(item);
			break;

		case VertexFormat::PositionColorTexture:
			for (const auto& item : DirectX::VertexPositionColorTexture::InputElements)
				inputElementDesc.push_back(item);
			break;

		default:
			break;
		}

		if (WindowsApi::Failed(device->CreateInputLayout(&inputElementDesc[0], static_cast<UINT>(std::size(inputElementDesc)), bytecode.data(), bytecode.size(), this->inputLayout.ReleaseAndGetAddressOf())))
			log->Error("CreateInputLayout");
	}

	void Direct3D11VertexShader::SetShader(ID3D11DeviceContext1* context) const
	{
		context->IASetInputLayout(this->inputLayout.Get());
		context->VSSetShader(this->vertexShader.Get(), nullptr, 0);
	}
}