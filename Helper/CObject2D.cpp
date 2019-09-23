#include "CObject2D.h"
#include "CDirectX.h"

void CObject2D::Create(const vector<SVertex2D>& vVertices, const vector<SIndex>& vIndices, bool IsDynamic)
{
	assert(!m_VertexBuffer);
	assert(!m_IndexBuffer);

	m_Vertices = vVertices;
	m_Indices = vIndices;

	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = (IsDynamic) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(SVertex2D) * m_Vertices.size());
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = (IsDynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
		buffer_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_Vertices[0];

		assert(SUCCEEDED(m_DirectX->m_Device->CreateBuffer(&buffer_desc, &subresource_data, &m_VertexBuffer)));
	}

	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(SIndex) * m_Indices.size());
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_Indices[0];

		assert(SUCCEEDED(m_DirectX->m_Device->CreateBuffer(&buffer_desc, &subresource_data, &m_IndexBuffer)));
	}
}

void CObject2D::CreateRectangle(const XMFLOAT2& RectangleSize, bool IsDynamic)
{
	assert(!m_VertexBuffer);
	assert(!m_IndexBuffer);

	float half_width{ RectangleSize.x / 2 };
	float half_height{ RectangleSize.y / 2 };

	vector<SVertex2D> vertices
	{
		SVertex2D(XMFLOAT2(-half_width, +half_height), XMFLOAT2(0.0f, 0.0f)),
		SVertex2D(XMFLOAT2(+half_width, +half_height), XMFLOAT2(1.0f, 0.0f)),
		SVertex2D(XMFLOAT2(-half_width, -half_height), XMFLOAT2(0.0f, 1.0f)),
		SVertex2D(XMFLOAT2(+half_width, -half_height), XMFLOAT2(1.0f, 1.0f)),
	};

	vector<SIndex> indices
	{
		SIndex(0, 1, 2),
		SIndex(1, 3, 2),
	};

	Create(vertices, indices, IsDynamic);
}

void CObject2D::SetRectangleUVRange(const XMFLOAT2& OffsetInTexture, const XMFLOAT2& SizeInTexture, ERenderFlipOption Flip)
{
	auto texture_size{ m_PtrSharedTexture->GetTextureSize() };
	XMFLOAT2 OffsetUV{}, SizeUV{};
	OffsetUV.x = OffsetInTexture.x / texture_size.x;
	OffsetUV.y = OffsetInTexture.y / texture_size.y;
	SizeUV.x = SizeInTexture.x / texture_size.x;
	SizeUV.y = SizeInTexture.y / texture_size.y;

	m_Vertices[3].TexCoord = m_Vertices[2].TexCoord =
		m_Vertices[1].TexCoord = m_Vertices[0].TexCoord = OffsetUV;

	m_Vertices[1].TexCoord.x += SizeUV.x;
	m_Vertices[2].TexCoord.y += SizeUV.y;
	m_Vertices[3].TexCoord.x += SizeUV.x;
	m_Vertices[3].TexCoord.y += SizeUV.y;

	if (Flip == ERenderFlipOption::Horizontal || Flip == ERenderFlipOption::HorizontalAndVertical)
	{
		std::swap(m_Vertices[0].TexCoord.x, m_Vertices[1].TexCoord.x);
		std::swap(m_Vertices[2].TexCoord.x, m_Vertices[3].TexCoord.x);
	}

	if (Flip == ERenderFlipOption::Vertical || Flip == ERenderFlipOption::HorizontalAndVertical)
	{
		std::swap(m_Vertices[0].TexCoord.y, m_Vertices[2].TexCoord.y);
		std::swap(m_Vertices[1].TexCoord.y, m_Vertices[3].TexCoord.y);
	}

	UpdateVertexBuffer();
}

void CObject2D::SetRectangleUVRangeAndSize(const XMFLOAT2& OffsetInTexture, const XMFLOAT2& SizeInTexture, ERenderFlipOption Flip)
{
	m_Vertices[0].Position.x = -SizeInTexture.x / 2;
	m_Vertices[0].Position.y = +SizeInTexture.y / 2;
	m_Vertices[1].Position.x = +SizeInTexture.x / 2;
	m_Vertices[1].Position.y = +SizeInTexture.y / 2;
	m_Vertices[2].Position.x = -SizeInTexture.x / 2;
	m_Vertices[2].Position.y = -SizeInTexture.y / 2;
	m_Vertices[3].Position.x = +SizeInTexture.x / 2;
	m_Vertices[3].Position.y = -SizeInTexture.y / 2;

	SetRectangleUVRange(OffsetInTexture, SizeInTexture, Flip);
}

void CObject2D::UpdateVertexBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	if (SUCCEEDED(m_DirectX->m_DeviceContext->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
	{
		memcpy(mapped_subresource.pData, &m_Vertices[0], sizeof(SVertex2D) * m_Vertices.size());
		m_DirectX->m_DeviceContext->Unmap(m_VertexBuffer.Get(), 0);
	}
}

void CObject2D::SetTexture(CTexture* SharedTexture)
{
	m_PtrSharedTexture = SharedTexture;
}

void CObject2D::Draw()
{
	if (m_PtrSharedTexture)
	{
		m_DirectX->UseSampler(Sampler);

		m_PtrSharedTexture->Use();
	}

	XMMATRIX Translation{ XMMatrixTranspose(XMMatrixTranslation(WorldPosition.x, WorldPosition.y, 0.0f)) };
	XMMATRIX Rotation{ XMMatrixTranspose(XMMatrixRotationZ(RotationAngle)) };
	XMMATRIX Scaling{ XMMatrixTranspose(XMMatrixScaling(Scalar.x, Scalar.y, 1.0f)) };
	m_MatrixWorld = Translation * Rotation * Scaling;

	m_DirectX->m_CBWorld->Update(&m_MatrixWorld);

	m_DirectX->m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VertexBufferStride, &m_VertexBufferOffset);
	m_DirectX->m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_DirectX->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_DirectX->m_DeviceContext->DrawIndexed(static_cast<UINT>(m_Indices.size() * 3), 0, 0);
}