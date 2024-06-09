module;

#include "../Direct3D11.h"

module Direct3D11VertexBuffer;

import WindowsApi;

namespace gfl
{
	Direct3D11VertexBuffer::Direct3D11VertexBuffer(UINT size, ID3D11Device1* device, Log* log) :
		size{size},
		log{log}
	{
		const D3D11_BUFFER_DESC bufferDesc
		{
			.ByteWidth = this->size,
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
			.MiscFlags = 0,
			.StructureByteStride = 0
		};

		if (WindowsApi::Failed(device->CreateBuffer(&bufferDesc, nullptr, this->buffer.ReleaseAndGetAddressOf())))
			this->log->Error("CreateBuffer");
	}

	void Direct3D11VertexBuffer::SetVertexBuffer(UINT strides, ID3D11DeviceContext1* context) const
	{
		const UINT offsets{0};
		context->IASetVertexBuffers(0, 1, this->buffer.GetAddressOf(), &strides, &offsets);
	}
}