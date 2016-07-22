
#include "Effekseer.h"

namespace Effekseer
{
#pragma region EffekseerFileReader
	class EffekseerFileReader :
		public Effekseer::FileReader
	{
		std::vector<uint8_t>	data;
		int32_t					position;
	public:
		EffekseerFileReader(std::vector<uint8_t>& data)
		{
			this->data = data;
			position = 0;
		}

		virtual ~EffekseerFileReader() {}

		size_t Read(void* buffer, size_t size)
		{
			int32_t readable = size;
			if (data.size() - position < size) readable = data.size() - position;

			memcpy(buffer, &(data[position]), readable);
			position += readable;
			return readable;
		}

		void Seek(int position)
		{
			this->position = position;
			if (this->position < 0) this->position = 0;
			if (this->position > data.size()) this->position = data.size();
		}

		int GetPosition()
		{
			return position;
		}

		size_t GetLength()
		{
			return data.size();
		}
	};
#pragma endregion

#pragma region EffekseerFile
	EffekseerFile::EffekseerFile() {}
	EffekseerFile::~EffekseerFile() {}

	Effekseer::FileReader* EffekseerFile::OpenRead(const EFK_CHAR* path)
	{
		char path_[300];
		::Effekseer::ConvertUtf16ToUtf8((int8_t*)path_, 300, (const int16_t*)path);

		cocos2d::Data data_ = cocos2d::FileUtils::getInstance()->getDataFromFile(path_);

		if (data_.isNull())
		{
			return nullptr;
		}

		std::vector<uint8_t> data;
		data.resize(data_.getSize());
		memcpy(data.data(), data_.getBytes(), data.size());
		return new EffekseerFileReader(data);
	}

	Effekseer::FileWriter* EffekseerFile::OpenWrite(const EFK_CHAR* path)
	{
		return nullptr;
	}
#pragma endregion
}