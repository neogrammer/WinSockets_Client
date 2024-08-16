#ifndef INTERRELATED_H__
#define INTERRELATED_H__
#include <vector>
struct ObjectRenderData
{
	int x{}, y{}, texRectPosX{}, texRectPosY{}, texRectW{}, texRectH{};
	int texId{}, entityId{};
};

struct WorldUpdate
{
	std::vector<ObjectRenderData> objectDataVec{};
};

struct ClientFrameInput
{
	int down{ 0 }, left{ 0 }, up{ 0 }, right{ 0 };
	int attack{ 0 }, run{ 0 }, start{ 0 };
};

#endif