#pragma once

// BT - STEAM

class FileHash
{

public:
	ZString m_filename;
	ZString m_hash;

	FileHash()
	{
	}

	FileHash(ZString filename, ZString hash)
		: m_filename(filename),
		m_hash(hash)
	{
	}

	inline bool operator == (const FileHash& fileHash) const
	{
		return m_filename == fileHash.m_filename;
	}

	inline  bool operator > (const FileHash& fileHash) const { return m_filename > fileHash.m_filename; }
};
