#pragma once
#include "resource_management/shadermanager.h"
class PostProcessing
{
public:
	PostProcessing();
	void init(ShaderManager* sdrMan);
	void sharpen(bool f);
	void grayscale(bool f);
	void negate(bool f);

	/*
	* getters
	*/
	bool getSharpen() const;
	bool getGrayscale() const;
	bool getNegate() const;
private:
	ShaderManager* m_sdrMan;
	bool m_sharpen, m_grayscale, m_negate;

};

