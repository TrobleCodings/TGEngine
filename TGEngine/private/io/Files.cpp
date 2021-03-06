#include "../../public/io/Files.hpp"

namespace tge {
	namespace nio {

		using namespace std;

		char current_working_dir[FILENAME_MAX];

		void initFileSystem() {
			GetCurrentDir(current_working_dir, sizeof(current_working_dir));
		}

		File open(const char* name, const char* mode) {
			ASSERT_NONE_NULL_DB(name, "Filename is null!", TG_ERR_DB_NULLPTR)
				ASSERT_NONE_NULL_DB((*name != 0), "Filename is empty!", TG_ERR_DB_EMPTY_STRING)
				ASSERT_NONE_NULL_DB(mode, "Mode is null [" << name << "] !", TG_ERR_DB_NULLPTR)
				ASSERT_NONE_NULL_DB((*mode != 0), "Mode is empty [" << name << "] !", TG_ERR_DB_EMPTY_STRING)
			File file = fopen(name, mode);
			ASSERT_NONE_NULL_DB(!file, "Can not open file [" << name << "]!", TG_ERR_DB_FILE_NOT_FOUND)
			return file;
		}

		File readFileSize(const char* name, const char* mode, long* fileLength) {
			ASSERT_NONE_NULL_DB(fileLength, "File length pointer is null [" << name << "] !", TG_ERR_DB_NULLPTR)
				File file = open(name, mode);

			fseek(file, 0, SEEK_END);
			*fileLength = ftell(file);
			ASSERT_NONE_NULL_DB((*fileLength != 0), "File empty [" << name << "] !", TG_ERR_DB_EMPTY_STRING)
				fseek(file, 0, SEEK_SET);
			return file;
		}

		uint8_t* readAll(const char* name, long* size) {
			if(size == nullptr)
				size = new long[1];
			File file = readFileSize(name, "rb", size);
			uint8_t* data = new uint8_t[*size];
			fread(data, 1, *size, file);
			return data;
		}
	}
}