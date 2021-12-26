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
	char shader_bin[256];
	char font_path[256];

	uint64_t pool_size = 0U;
	uint64_t chunk_size = 0U;

	int32_t log_level = 0;

	//<Graphic config
	bool    full_screen = false;
	bool	vsync = false;
	bool	msaa = false;

	int32_t height;
	int32_t width;

	int32_t fps = 60;

	//<OpenGL config
	int32_t major_verion = 3;
	int32_t minor_version = 3;

	int32_t r_size = 8;
	int32_t b_size = 8;
	int32_t g_size = 8;
	int32_t a_size = 8;
	int32_t depth_size = 24;
	bool	double_buffer = false;
	int32_t stencil_size = 8;
	int32_t msaa_buffer = 1;
	int32_t msaa_sample = 2;

	void loadConfig(const char* file);
protected:
	friend class SingletonHolder<Configuration>;
	Configuration();
	~Configuration();
};
