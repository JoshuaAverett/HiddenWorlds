#include "pch.h"
#include "HWTextureAtlas.h"

HWTextureAtlas::HWTextureAtlas(S32 width, S32 height, S32 depth)
{
	texture = new HWTexture(width, height, depth);
	nodes.push_back(glm::ivec3(1, 1, width - 2));
}

HWTextureAtlas::~HWTextureAtlas()
{
	delete texture;
}

void HWTextureAtlas::Bind(GLuint texID)
{
	texture->Bind(texID, 0);
}

glm::ivec4 HWTextureAtlas::GetRegion(S32 width, S32 height)
{
    glm::ivec4 region = glm::ivec4(0,0,width,height);

    S32 best_height = INT_MAX;
    S32 best_index  = -1;
    S32 best_width = INT_MAX;

	for(S32 i = 0; i < nodes.size(); ++i)
	{
        S32 y = Fit(i, width, height);
		if(y >= 0)
		{
            glm::ivec3 node = nodes[i];

			if( ( (y + height) < best_height ) ||
                ( ((y + height) == best_height) && (node.z < best_width)) )
			{
				best_height = y + height;
				best_index = i;
				best_width = node.z;
				region.x = node.x;
				region.y = y;
			}
        }
    }
   
	if(best_index == -1)
    {
        region.x = -1;
        region.y = -1;
        region.z = 0;
        region.w = 0;

        return region;
    }

	glm::ivec3 node = glm::ivec3(region.x, region.y + height, width);
	nodes.insert(nodes.begin() + best_index, node);

    for(S32 i = best_index + 1; i < nodes.size(); ++i)
    {
        glm::ivec3 node = nodes[i];
        glm::ivec3 prev = nodes[i - 1];

        if(node.x < (prev.x + prev.z))
        {
            S32 shrink = prev.x + prev.z - node.x;
            node.x += shrink;
            node.z -= shrink;

			nodes[i] = node;

            if(node.z <= 0)
            {
				nodes.erase(nodes.begin() + i);
                --i;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    Merge();
    return region;
}

void HWTextureAtlas::SetRegion(S32 x, S32 y, S32 width, S32 height, const U8* data, S32 stride)
{
	S32 texDepth = texture->GetDepth() / 8;
    for(S32 i = 0; i < height; ++i)
    {
        memcpy(texture->GetBitmap() + ((y + i) * texture->GetWidth() + x) * texDepth, data + (i * stride), width * texDepth);
    }
}

S32 HWTextureAtlas::Fit(S32 index, S32 width, S32 height)
{
	glm::ivec3 node = nodes[index];
    S32 x = node.x;
	S32 y = node.y;

    S32 width_left = width;
	S32 i = index;

	if((x + width) > (texture->GetWidth() - 1))
    {
		return -1;
    }

	while(width_left > 0)
	{
		node = nodes[i];

        if(node.y > y)
        {
            y = node.y;
        }

		if((y + height) > (texture->GetHeight() - 1))
        {
			return -1;
        }

		width_left -= node.z;
		++i;
	}

	return y;
}

void HWTextureAtlas::Merge()
{
	for(S32 i = 0; i < nodes.size() - 1; ++i )
    {
        glm::ivec3 node = nodes[i];
		glm::ivec3 next = nodes[i + 1];
		if(node.y == next.y)
		{
			node.z += next.z;
			nodes.erase(nodes.begin() + i + 1);
			--i;
		}
    }
}

S32 HWTextureAtlas::GetWidth() const
{
	return texture->GetWidth();
}

S32 HWTextureAtlas::GetHeight() const
{
	return texture->GetHeight();
}

S32 HWTextureAtlas::GetDepth() const
{
	return texture->GetDepth();
}
