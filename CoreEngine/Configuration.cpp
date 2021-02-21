#pragma once
#include "stdafx.h"
#include "Configuration.h"
#include <string>
#include <fstream>
#include <inipp.h>
#include <INIReader.h>
#include "log.h"

Configuration::Configuration():pool_size(0), chunk_size(0), log_level(0), full_screen(false), height(-1), width(-1), fps(30)
{
	memset(version, 0, sizeof(version));
	memset(app_name, 0, sizeof(app_name));
	memset(data_file, 0, sizeof(data_file));
	memset(asset_path, 0, sizeof(asset_path));
	memset(font_path, 0, sizeof(font_path));
	memset(shader_path, 0, sizeof(shader_path));
}

void Configuration::loadConfig(const char* file)
{
	//<Read config.ini
	inipp::Ini<char> ini;
	std::ifstream config_ifstream(file);
	_ASSERT(config_ifstream.is_open());
	ini.parse(config_ifstream);

	std::string str_return;

	inipp::extract(ini.sections["General"]["version"], str_return);
	memcpy_s(version, sizeof(version), str_return.data(), str_return.size());
	version[sizeof(version) - 1] = 0;

	inipp::extract(ini.sections["General"]["Application"], str_return);
	memcpy_s(app_name, sizeof(app_name), str_return.data(), str_return.size());
	app_name[sizeof(app_name) - 1] = 0;

	inipp::extract(ini.sections["Log"]["level"], log_level);

	inipp::extract(ini.sections["Graphic"]["fullscreen"], full_screen);

	inipp::extract(ini.sections["Graphic"]["heigh"], height);

	inipp::extract(ini.sections["Graphic"]["width"], width);

	inipp::extract(ini.sections["Memory"]["pool_size"], pool_size);
	inipp::extract(ini.sections["Memory"]["chunk_size"], chunk_size);

	inipp::extract(ini.sections["Texture"]["data_file"], str_return);
	memcpy_s(data_file, sizeof(data_file), str_return.data(), str_return.size());
	data_file[sizeof(data_file) - 1] = 0;

	inipp::extract(ini.sections["Texture"]["asset_path"], str_return);
	memcpy_s(asset_path, sizeof(asset_path), str_return.data(), str_return.size());
	asset_path[sizeof(asset_path) - 1] = 0;

	inipp::extract(ini.sections["Texture"]["font_path"], str_return);
	memcpy_s(font_path, sizeof(font_path), str_return.data(), str_return.size());
	font_path[sizeof(font_path) - 1] = 0;

	inipp::extract(ini.sections["Texture"]["shader_path"], str_return);
	memcpy_s(shader_path, sizeof(shader_path), str_return.data(), str_return.size());
	font_path[sizeof(shader_path) - 1] = 0;

	inipp::extract(ini.sections["Graphic"]["fps"], fps);

	inipp::extract(ini.sections["OpenGL"]["major_version"], major_verion);
	inipp::extract(ini.sections["OpenGL"]["minor_version"], minor_version);
	inipp::extract(ini.sections["OpenGL"]["r_size"], r_size);
	inipp::extract(ini.sections["OpenGL"]["b_size"], b_size);
	inipp::extract(ini.sections["OpenGL"]["g_size"], g_size);

	inipp::extract(ini.sections["OpenGL"]["depth_size"], depth_size);
	inipp::extract(ini.sections["OpenGL"]["double_buffer"], double_buffer);

	LOG_DEBUG("Version[%s]", version);
	LOG_DEBUG("Application[%s]", app_name);
	LOG_DEBUG("log level[%d]", log_level);
	LOG_DEBUG("Dimension[%dx%d]", width, height);
	LOG_DEBUG("Data file[%s]", data_file);
	LOG_DEBUG("Asset path[%s]", asset_path);
	LOG_DEBUG("Font path[%s]", font_path);
	LOG_DEBUG("Shader path[%s]", shader_path);

	LOG_DEBUG("Frame rate[%d]", fps);

	LOG_DEBUG("major_verion[%d]", major_verion);
	LOG_DEBUG("minor_version[%d]", minor_version);
	LOG_DEBUG("r_size[%d]", r_size);
	LOG_DEBUG("b_size[%d]", b_size);
	LOG_DEBUG("g_size[%d]", g_size);
	LOG_DEBUG("Depth size[%d]", depth_size);
	LOG_DEBUG("Double buffer[%d]", double_buffer);
}

Configuration::~Configuration() {}

