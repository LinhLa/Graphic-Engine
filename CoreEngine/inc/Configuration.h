#pragma once
#include "SingletonHolder.h"

class Configuration : public SingletonHolder<Configuration>
{
public:
	char version[8];
	char app_name[14];
	char data_file[256];
	char asset_path[256];
	char shader_path[256];
	char font_path[256];

	uint64_t pool_size;
	uint64_t chunk_size;

	int32_t log_level;

	bool    full_screen;
	int32_t height;
	int32_t width;

	int32_t fps;

	//<OpenGL config
	int32_t major_verion = 0;
	int32_t minor_version = 0;

	int32_t r_size = 8;
	int32_t b_size = 8;
	int32_t g_size = 8;
	int32_t depth_size = 32;
	int32_t	double_buffer = 1;

	void loadConfig(const char* file);
protected:
	friend class SingletonHolder<Configuration>;
	Configuration();
	~Configuration();
};
